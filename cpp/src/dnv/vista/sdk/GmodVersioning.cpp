#include "pch.h"

#include "dnv/vista/sdk/GmodVersioning.h"

#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/GmodNode.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VisVersion.h"

namespace dnv::vista::sdk
{
	GmodVersioning::ConversionType GmodVersioning::ParseConversionType( const std::string& type )
	{
		if ( type == "ChangeCode" )
			return ConversionType::ChangeCode;
		if ( type == "Merge" )
			return ConversionType::Merge;
		if ( type == "Move" )
			return ConversionType::Move;
		if ( type == "AssignmentChange" )
			return ConversionType::AssignmentChange;
		if ( type == "AssignmentDelete" )
			return ConversionType::AssignmentDelete;

		SPDLOG_ERROR( "Unknown conversion type: {}", type );
		throw std::invalid_argument( "Unknown conversion type: " + type );
	}

	GmodVersioning::GmodVersioningNode::GmodVersioningNode(
		VisVersion visVersion,
		const std::unordered_map<std::string, GmodNodeConversionDto>& dto )
		: m_visVersion( visVersion )
	{
		for ( const auto& [code, dtoNode] : dto )
		{
			GmodNodeConversion conversion;
			conversion.source = dtoNode.source;
			conversion.target = dtoNode.target;
			conversion.oldAssignment = dtoNode.oldAssignment;
			conversion.newAssignment = dtoNode.newAssignment;
			conversion.deleteAssignment = dtoNode.deleteAssignment;

			if ( !dtoNode.operations.empty() )
			{
				for ( const auto& type : dtoNode.operations )
				{
					conversion.operations.insert( ParseConversionType( type ) );
				}
			}

			m_versioningNodeChanges.emplace( code, conversion );
		}
	}

	VisVersion GmodVersioning::GmodVersioningNode::GetVisVersion() const
	{
		return m_visVersion;
	}

	bool GmodVersioning::GmodVersioningNode::TryGetCodeChanges(
		const std::string& code, GmodNodeConversion& nodeChanges ) const
	{
		auto it = m_versioningNodeChanges.find( code );
		if ( it != m_versioningNodeChanges.end() )
		{
			nodeChanges = it->second;
			return true;
		}
		return false;
	}

	GmodVersioning::GmodVersioning( const std::unordered_map<std::string, GmodVersioningDto>& dto )
	{
		for ( const auto& [versionStr, versioningDto] : dto )
		{
			VisVersion version = VisVersionExtensions::Parse( versionStr );

			if ( !versioningDto.items.empty() )
			{
				m_versioningsMap.emplace( version, GmodVersioningNode( version, versioningDto.items ) );
			}
		}
	}

	void GmodVersioning::ValidateSourceAndTargetVersions( VisVersion sourceVersion, VisVersion targetVersion ) const
	{
		if ( !VisVersionExtensions::IsValid( sourceVersion ) )
		{
			SPDLOG_ERROR( "Invalid source version: {}", static_cast<int>( sourceVersion ) );
			throw std::invalid_argument( "Invalid source version" );
		}

		if ( !VisVersionExtensions::IsValid( targetVersion ) )
		{
			SPDLOG_ERROR( "Invalid target version: {}", static_cast<int>( targetVersion ) );
			throw std::invalid_argument( "Invalid target version" );
		}

		if ( sourceVersion >= targetVersion )
		{
			SPDLOG_ERROR( "Source version {} must be earlier than target version {}", static_cast<int>( sourceVersion ),
				static_cast<int>( targetVersion ) );
			throw std::invalid_argument( "Source version must be earlier than target version" );
		}
	}

	void GmodVersioning::ValidateSourceAndTargetVersionPair( VisVersion sourceVersion, VisVersion targetVersion ) const
	{
		ValidateSourceAndTargetVersions( sourceVersion, targetVersion );

		if ( sourceVersion == targetVersion )
		{
			SPDLOG_ERROR( "Source and target versions are the same: {} = {}", static_cast<int>( sourceVersion ),
				static_cast<int>( targetVersion ) );
			throw std::invalid_argument( "Source and target versions are the same" );
		}

		if ( static_cast<int>( targetVersion ) - static_cast<int>( sourceVersion ) != 1 )
		{
			SPDLOG_ERROR( "Target version {} must be one version higher than source version {}", static_cast<int>( targetVersion ),
				static_cast<int>( sourceVersion ) );
			throw std::invalid_argument( "Target version must be one version higher than source version" );
		}
	}

	bool GmodVersioning::TryGetVersioningNode( VisVersion visVersion, GmodVersioningNode& versioningNode ) const
	{
		auto it = m_versioningsMap.find( visVersion );
		if ( it != m_versioningsMap.end() )
		{
			versioningNode = it->second;
			return true;
		}
		return false;
	}

	GmodNode GmodVersioning::ConvertNodeInternal(
		VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) const
	{
		auto& vis = VIS::Instance();
		const auto& targetGmod = vis.GetGmod( targetVersion );

		std::string nextCode = sourceNode.GetCode();

		auto it = m_versioningsMap.find( targetVersion );
		if ( it != m_versioningsMap.end() )
		{
			GmodNodeConversion change;
			if ( it->second.TryGetCodeChanges( sourceNode.GetCode(), change ) && change.target.has_value() )
			{
				nextCode = *change.target;
			}
		}

		GmodNode targetNode;
		if ( !targetGmod.TryGetNode( nextCode, targetNode ) )
		{
			return GmodNode();
		}

		GmodNode result = targetNode;
		if ( sourceNode.GetLocation().has_value() &&
			 ( !result.GetLocation().has_value() || result.GetLocation() != sourceNode.GetLocation() ) )
		{
			if ( result.IsIndividualizable( false, true ) )
			{
				result = result.WithLocation( *sourceNode.GetLocation() );
			}
		}

		return result;
	}

	std::optional<GmodNode> GmodVersioning::ConvertNode(
		VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) const
	{
		if ( sourceVersion == targetVersion )
			return sourceNode;

		if ( sourceNode.GetCode().empty() )
			return std::nullopt;

		ValidateSourceAndTargetVersions( sourceVersion, targetVersion );

		if ( sourceVersion < targetVersion )
		{
			VisVersion source = sourceVersion;
			GmodNode node = sourceNode;

			while ( source < targetVersion )
			{
				VisVersion next = static_cast<VisVersion>( static_cast<int>( source ) + 1 );
				node = ConvertNodeInternal( source, node, next );

				if ( node.GetCode().empty() )
					return std::nullopt;

				source = next;
			}

			return node;
		}
		else
		{
			VisVersion source = sourceVersion;
			GmodNode node = sourceNode;

			while ( source > targetVersion )
			{
				VisVersion prev = static_cast<VisVersion>( static_cast<int>( source ) - 1 );
				node = ConvertNodeInternal( source, node, prev );

				if ( node.GetCode().empty() )
					return std::nullopt;

				source = prev;
			}

			return node;
		}
	}

	std::optional<LocalIdBuilder> GmodVersioning::ConvertLocalId(
		const LocalIdBuilder& sourceLocalId, VisVersion targetVersion ) const
	{
		if ( !sourceLocalId.GetVisVersion().has_value() )
		{
			SPDLOG_ERROR( "Cannot convert local ID without a specific VIS version" );
			throw std::invalid_argument( "Cannot convert local ID without a specific VIS version" );
		}

		std::optional<GmodPath> primaryItem;
		if ( sourceLocalId.GetPrimaryItem().has_value() )
		{
			auto convertedPath = ConvertPath(
				*sourceLocalId.GetVisVersion(),
				sourceLocalId.GetPrimaryItem().value(),
				targetVersion );

			if ( convertedPath )
			{
				primaryItem = std::move( convertedPath );
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<GmodPath> secondaryItem;
		if ( sourceLocalId.GetSecondaryItem().has_value() )
		{
			auto convertedPath = ConvertPath(
				*sourceLocalId.GetVisVersion(),
				sourceLocalId.GetSecondaryItem().value(),
				targetVersion );

			if ( convertedPath )
			{
				secondaryItem = std::move( convertedPath );
			}
			else
			{
				return std::nullopt;
			}
		}

		return LocalIdBuilder::Create( targetVersion )
			.TryWithPrimaryItem( primaryItem )
			.TryWithSecondaryItem( secondaryItem )
			.WithVerboseMode( sourceLocalId.GetVerboseMode() )
			.TryWithMetadataTag( sourceLocalId.GetQuantity() )
			.TryWithMetadataTag( sourceLocalId.GetContent() )
			.TryWithMetadataTag( sourceLocalId.GetCalculation() )
			.TryWithMetadataTag( sourceLocalId.GetState() )
			.TryWithMetadataTag( sourceLocalId.GetCommand() )
			.TryWithMetadataTag( sourceLocalId.GetType() )
			.TryWithMetadataTag( sourceLocalId.GetPosition() )
			.TryWithMetadataTag( sourceLocalId.GetDetail() );
	}

	std::optional<LocalId> GmodVersioning::ConvertLocalId(
		const LocalId& sourceLocalId, VisVersion targetVersion ) const
	{
		auto builder = ConvertLocalId( sourceLocalId.GetBuilder(), targetVersion );
		if ( !builder.has_value() )
			return std::nullopt;

		return builder->Build();
	}

	std::optional<GmodPath> GmodVersioning::ConvertPath(
		VisVersion sourceVersion, const GmodPath& sourcePath, VisVersion targetVersion ) const
	{
		ValidateSourceAndTargetVersions( sourceVersion, targetVersion );

		auto targetEndNode = ConvertNode( sourceVersion, sourcePath.GetNode(), targetVersion );
		if ( !targetEndNode.has_value() )
			return std::nullopt;

		if ( targetEndNode->IsRoot() )
			return GmodPath( {}, *targetEndNode, true );

		const auto& sourceGmod = VIS::Instance().GetGmod( sourceVersion );
		const auto& targetGmod = VIS::Instance().GetGmod( targetVersion );

		std::vector<std::pair<GmodNode, GmodNode>> qualifyingNodes;
		for ( const auto& pathNode : sourcePath.GetFullPath() )
		{
			auto convertedNode = ConvertNode( sourceVersion, pathNode.second, targetVersion );
			if ( !convertedNode.has_value() )
				return std::nullopt;

			qualifyingNodes.emplace_back( pathNode.second, *convertedNode );
		}

		if ( std::any_of( qualifyingNodes.begin(), qualifyingNodes.end(),
				 []( const auto& pair ) { return pair.second.GetCode().empty(); } ) )
		{
			SPDLOG_ERROR( "Failed to convert node forward" );
			throw std::runtime_error( "Failed to convert node forward" );
		}

		std::vector<GmodNode> potentialParents;
		for ( size_t i = 0; i < qualifyingNodes.size() - 1; ++i )
		{
			potentialParents.push_back( qualifyingNodes[i].second );
		}

		if ( GmodPath::IsValid( potentialParents, *targetEndNode ) )
			return GmodPath( potentialParents, *targetEndNode, true );

		auto addToPath = []( std::vector<GmodNode>& path, const GmodNode& node ) {
			if ( !path.empty() )
			{
				const GmodNode& prev = path.back();
				if ( !prev.IsChild( node ) )
				{
					for ( const auto* parent : node.GetParents() )
					{
						if ( parent->GetCode() == prev.GetCode() )
						{
							return true;
						}
					}
					return false;
				}
			}
			path.push_back( node );
			return true;
		};

		std::vector<GmodNode> path;
		for ( size_t i = 0; i < qualifyingNodes.size(); ++i )
		{
			const auto& qualifyingNode = qualifyingNodes[i];
			const auto& sourceNode = qualifyingNode.first;
			const auto& targetNode = qualifyingNode.second;

			if ( i > 0 && targetNode.GetCode() == qualifyingNodes[i - 1].second.GetCode() )
				continue;

			bool codeChanged = sourceNode.GetCode() != targetNode.GetCode();

			const GmodNode* sourceNormalAssignment = sourceNode.ProductType();
			const GmodNode* targetNormalAssignment = targetNode.ProductType();

			bool normalAssignmentChanged = false;
			bool wasDeleted = false;

			if ( ( sourceNormalAssignment != nullptr ) != ( targetNormalAssignment != nullptr ) )
			{
				normalAssignmentChanged = true;
				wasDeleted = targetNormalAssignment == nullptr;
			}
			else if ( sourceNormalAssignment != nullptr && targetNormalAssignment != nullptr )
			{
				auto convertedSourceNormalAssignment =
					ConvertNode( sourceVersion, *sourceNormalAssignment, targetVersion );

				normalAssignmentChanged =
					!convertedSourceNormalAssignment.has_value() ||
					convertedSourceNormalAssignment->GetCode() != targetNormalAssignment->GetCode();
			}

			bool selectionChanged = false;
			if ( sourceNode.IsProductSelection() != targetNode.IsProductSelection() )
			{
				selectionChanged = true;
			}
			else if ( sourceNode.IsProductSelection() && targetNode.IsProductSelection() )
			{
				selectionChanged = sourceNode.GetCode() != targetNode.GetCode();
			}

			if ( codeChanged )
			{
				if ( !addToPath( path, targetNode ) )
				{
					SPDLOG_ERROR( "Failed to add node to path: parent-child relationship broken" );
					throw std::runtime_error( "Failed to add node to path: parent-child relationship broken" );
				}
			}
			else if ( normalAssignmentChanged )
			{
				// AC || AN || AD

				if ( !codeChanged && !path.empty() && path.back().GetCode() == targetNode.GetCode() )
				{
					continue;
				}

				if ( wasDeleted )
				{
					if ( targetNode.GetCode() == targetEndNode->GetCode() )
					{
						bool skipNode = false;
						if ( i + 1 < qualifyingNodes.size() )
						{
							const auto& next = qualifyingNodes[i + 1];
							if ( next.second.GetCode() != qualifyingNode.second.GetCode() )
							{
								skipNode = true;
							}
						}
						if ( !skipNode )
						{
							if ( !addToPath( path, targetNode ) )
							{
								SPDLOG_ERROR( "Failed to add node to path: parent-child relationship broken" );
								throw std::runtime_error( "Failed to add node to path: parent-child relationship broken" );
							}
						}
					}
				}
				else if ( targetNode.GetCode() != targetEndNode->GetCode() )
				{
					if ( !addToPath( path, targetNode ) )
					{
						SPDLOG_ERROR( "Failed to add node to path: parent-child relationship broken" );
						throw std::runtime_error( "Failed to add node to path: parent-child relationship broken" );
					}
				}
			}
			else if ( selectionChanged )
			{
				// TODO SC || SN || SD
			}
			else if ( !codeChanged && !normalAssignmentChanged )
			{
				if ( !addToPath( path, targetNode ) )
				{
					SPDLOG_ERROR( "Failed to add node to path: parent-child relationship broken" );
					throw std::runtime_error( "Failed to add node to path: parent-child relationship broken" );
				}
			}

			if ( !path.empty() && path.back().GetCode() == targetEndNode->GetCode() )
			{
				break;
			}
		}

		if ( path.empty() )
		{
			SPDLOG_ERROR( "Failed to build path: no nodes added" );
			throw std::runtime_error( "Failed to build path: no nodes added" );
		}

		potentialParents = std::vector<GmodNode>( path.begin(), path.end() - 1 );
		targetEndNode = path.back();

		int missingLinkAt = -1;
		if ( !GmodPath::IsValid( potentialParents, *targetEndNode, missingLinkAt ) )
		{
			SPDLOG_ERROR( "Failed to create a valid path. Missing link at: {}", missingLinkAt );
			throw std::runtime_error( "Failed to create a valid path. Missing link at: " +
									  std::to_string( missingLinkAt ) );
		}

		return GmodPath( potentialParents, *targetEndNode );
	}
}
