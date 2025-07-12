/**
 * @file GmodVersioning.cpp
 * @brief Implementation of the GmodVersioning class for converting GMOD objects between VIS versions.
 */

#include "pch.h"

#include "dnv/vista/sdk/GmodVersioning.h"

#include "dnv/vista/sdk/Gmod.h"
#include "dnv/vista/sdk/GmodNode.h"
#include "dnv/vista/sdk/GmodPath.h"
#include "dnv/vista/sdk/GmodTraversal.h"
#include "dnv/vista/sdk/LocalId.h"
#include "dnv/vista/sdk/LocalIdBuilder.h"
#include "dnv/vista/sdk/MetadataTag.h"
#include "dnv/vista/sdk/VIS.h"
#include "dnv/vista/sdk/VISVersion.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// GmodVersioning class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	GmodVersioning::GmodVersioning( const std::unordered_map<std::string, GmodVersioningDto>& dto )
	{
		for ( const auto& [versionStr, versioningDto] : dto )
		{
			VisVersion version = VisVersionExtensions::parse( versionStr );

			m_versioningsMap.emplace( version, GmodVersioningNode( version, versioningDto.items() ) );
		}
	}

	//----------------------------------------------
	// Conversion
	//----------------------------------------------

	//----------------------------
	// Node
	//----------------------------

	std::optional<GmodNode> GmodVersioning::convertNode(
		VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) const
	{
		if ( sourceNode.code().empty() )
			return std::nullopt;

		validateSourceAndTargetVersions( sourceVersion, targetVersion );

		std::optional<GmodNode> node = sourceNode;
		VisVersion source = sourceVersion;

		while ( static_cast<int>( source ) <= static_cast<int>( targetVersion ) - 100 )
		{
			if ( !node.has_value() )
			{
				break;
			}

			VisVersion target = static_cast<VisVersion>( static_cast<int>( source ) + 100 );

			node = convertNodeInternal( source, *node, target );

			source = static_cast<VisVersion>( static_cast<int>( source ) + 100 );
		}

		return node;
	}

	//----------------------------
	// Path
	//----------------------------

	std::optional<GmodPath> GmodVersioning::convertPath(
		VisVersion sourceVersion, const GmodPath& sourcePath, VisVersion targetVersion ) const
	{
		std::optional<GmodNode> targetEndNode = convertNode( sourceVersion, sourcePath.node(), targetVersion );
		if ( !targetEndNode.has_value() )
		{
			return std::nullopt;
		}

		auto& vis = VIS::instance();
		const auto& targetGmod = vis.gmod( targetVersion );

		if ( targetEndNode->isRoot() )
		{
			const GmodNode* rootNodeInGmodPtr = nullptr;
			if ( !targetGmod.tryGetNode( targetEndNode->code(), rootNodeInGmodPtr ) || !rootNodeInGmodPtr )
			{
				throw std::runtime_error( "Failed to get root node from targetGmod during root path conversion" );
			}

			return GmodPath( targetGmod, *rootNodeInGmodPtr, {} );
		}

		std::vector<std::pair<const GmodNode*, GmodNode>> qualifyingNodes;
		qualifyingNodes.reserve( 32 );

		auto enumerator = sourcePath.fullPath();
		while ( enumerator.next() )
		{
			const auto& [depth, originalNodeInPath] = enumerator.current();

			std::optional<GmodNode> convertedNodeOpt = convertNode( sourceVersion, *originalNodeInPath, targetVersion );
			if ( !convertedNodeOpt.has_value() )
			{
				throw std::runtime_error( "Could not convert node forward" );
			}

			qualifyingNodes.emplace_back( originalNodeInPath, std::move( *convertedNodeOpt ) );
		}
		std::vector<GmodNode> potentialParents;
		potentialParents.reserve( qualifyingNodes.size() );

		for ( size_t i = 0; i < qualifyingNodes.size() - 1; ++i )
		{
			potentialParents.push_back( qualifyingNodes[i].second );
		}

		std::vector<GmodNode*> potentialParentPtrs;
		potentialParentPtrs.reserve( potentialParents.size() );
		for ( auto& parent : potentialParents )
		{
			potentialParentPtrs.push_back( &parent );
		}

		if ( GmodPath::isValid( potentialParentPtrs, *targetEndNode ) )
		{
			return GmodPath( targetGmod, *targetEndNode, std::move( potentialParents ) );
		}

		std::vector<GmodNode> path;
		path.reserve( 32 );

		auto addToPath = []( const Gmod& gmod, std::vector<GmodNode>& pathRef, const GmodNode& node ) {
			if ( !pathRef.empty() )
			{
				const GmodNode& prev = pathRef.back();
				if ( !prev.isChild( node ) )
				{
					for ( int j = static_cast<int>( pathRef.size() ) - 1; j >= 0; --j )
					{
						const GmodNode& parent = pathRef[static_cast<size_t>( j )];

						std::vector<const GmodNode*> currentParentPtrs;
						currentParentPtrs.reserve( static_cast<size_t>( j + 1 ) );
						for ( int k = 0; k <= j; ++k )
						{
							currentParentPtrs.push_back( &pathRef[static_cast<size_t>( k )] );
						}

						std::vector<const GmodNode*> remaining;
						if ( !GmodTraversal::pathExistsBetween( gmod, currentParentPtrs, node, remaining ) )
						{
							bool hasOtherAssetFunction = false;
							if ( parent.isAssetFunctionNode() )
							{
								for ( int k = 0; k <= j; ++k )
								{
									const auto& pathNode = pathRef[static_cast<size_t>( k )];
									if ( pathNode.isAssetFunctionNode() && pathNode.code() != parent.code() )
									{
										hasOtherAssetFunction = true;
										break;
									}
								}
								if ( !hasOtherAssetFunction )
								{
									throw std::runtime_error( "Tried to remove last asset function node" );
								}
							}
							pathRef.erase( pathRef.begin() + static_cast<std::ptrdiff_t>( j ) );
						}
						else
						{
							if ( node.location().has_value() )
							{
								const auto& nodeLocation = *node.location();
								for ( const GmodNode* n : remaining )
								{
									if ( !n->isIndividualizable( false, true ) )
									{
										pathRef.emplace_back( *n );
									}
									else
									{
										pathRef.emplace_back( n->tryWithLocation( nodeLocation ) );
									}
								}
							}
							else
							{
								for ( const GmodNode* n : remaining )
								{
									pathRef.emplace_back( *n );
								}
							}
							break;
						}
					}
				}
			}
			pathRef.emplace_back( node );
		};

		for ( size_t i = 0; i < qualifyingNodes.size(); ++i )
		{
			const auto& qualifyingNode = qualifyingNodes[i];

			if ( i > 0 && qualifyingNode.second.code() == qualifyingNodes[i - 1].second.code() )
			{
				continue;
			}

			bool codeChanged = qualifyingNode.first->code() != qualifyingNode.second.code();

			const GmodNode* sourceNormalAssignment = qualifyingNode.first->productType();
			const GmodNode* targetNormalAssignment = qualifyingNode.second.productType();

			bool normalAssignmentChanged =
				( sourceNormalAssignment == nullptr ) != ( targetNormalAssignment == nullptr ) ||
				( sourceNormalAssignment != nullptr && targetNormalAssignment != nullptr &&
					sourceNormalAssignment->code() != targetNormalAssignment->code() );

			bool selectionChanged = false;

			if ( codeChanged )
			{
				addToPath( targetGmod, path, qualifyingNode.second );
			}
			else if ( normalAssignmentChanged )
			{
				bool wasDeleted = sourceNormalAssignment != nullptr && targetNormalAssignment == nullptr;

				if ( !codeChanged )
				{
					addToPath( targetGmod, path, qualifyingNode.second );
				}

				if ( wasDeleted )
				{
					if ( qualifyingNode.second.code() == targetEndNode->code() )
					{
						if ( i + 1 < qualifyingNodes.size() )
						{
							const auto& next = qualifyingNodes[i + 1];
							if ( next.second.code() != qualifyingNode.second.code() )
							{
								throw std::runtime_error( "Normal assignment end node was deleted" );
							}
						}
					}
					continue;
				}
				else if ( qualifyingNode.second.code() != targetEndNode->code() )
				{
					if ( targetNormalAssignment != nullptr )
					{
						GmodNode targetNormalAssignmentVal = *targetNormalAssignment;
						if ( qualifyingNode.second.location().has_value() && targetNormalAssignment->isIndividualizable( false, true ) )
						{
							targetNormalAssignmentVal = targetNormalAssignment->tryWithLocation( *qualifyingNode.second.location() );
						}
						addToPath( targetGmod, path, targetNormalAssignmentVal );
						++i;
					}
				}
			}

			if ( selectionChanged )
			{
				/* SC || SN || SD */
			}

			if ( !codeChanged && !normalAssignmentChanged )
			{
				addToPath( targetGmod, path, qualifyingNode.second );
			}

			if ( !path.empty() && path.back().code() == targetEndNode->code() )
			{
				break;
			}
		}

		if ( path.empty() || path.size() < 1 )
		{
			throw std::runtime_error( "Path reconstruction resulted in an empty path" );
		}

		std::vector<GmodNode> potentialParentsFromPath;
		potentialParentsFromPath.reserve( path.size() );
		for ( size_t k = 0; k < path.size() - 1; ++k )
		{
			potentialParentsFromPath.push_back( path[k] );
		}

		GmodNode& targetEndNodeFromPath = path.back();

		std::vector<GmodNode*> potentialParentPtrsFromPath;
		potentialParentPtrsFromPath.reserve( potentialParentsFromPath.size() );
		for ( auto& parent : potentialParentsFromPath )
		{
			potentialParentPtrsFromPath.push_back( &parent );
		}

		int missingLinkAt;
		if ( !GmodPath::isValid( potentialParentPtrsFromPath, targetEndNodeFromPath, missingLinkAt ) )
		{
			throw std::runtime_error( "Did not end up with a valid path" );
		}

		return GmodPath( targetGmod, std::move( targetEndNodeFromPath ), std::move( potentialParentsFromPath ) );
	}

	//----------------------------
	// Local Id
	//----------------------------

	std::optional<LocalIdBuilder> GmodVersioning::convertLocalId(
		const LocalIdBuilder& sourceLocalId, VisVersion targetVersion ) const
	{
		if ( !sourceLocalId.visVersion().has_value() )
		{
			throw std::invalid_argument( "Cannot convert local ID without a specific VIS version" );
		}

		LocalIdBuilder targetLocalId = LocalIdBuilder::create( targetVersion );

		if ( sourceLocalId.primaryItem().has_value() )
		{
			auto targetPrimaryItem = convertPath(
				*sourceLocalId.visVersion(),
				sourceLocalId.primaryItem().value(),
				targetVersion );

			if ( !targetPrimaryItem.has_value() )
			{
				return std::nullopt;
			}

			targetLocalId = targetLocalId.withPrimaryItem( std::move( *targetPrimaryItem ) );
		}

		if ( sourceLocalId.secondaryItem().has_value() )
		{
			auto targetSecondaryItem = convertPath(
				*sourceLocalId.visVersion(),
				sourceLocalId.secondaryItem().value(),
				targetVersion );

			if ( !targetSecondaryItem.has_value() )
			{
				return std::nullopt;
			}

			targetLocalId = targetLocalId.withSecondaryItem( std::move( *targetSecondaryItem ) );
		}

		return targetLocalId
			.withVerboseMode( sourceLocalId.isVerboseMode() )
			.tryWithMetadataTag( sourceLocalId.quantity() )
			.tryWithMetadataTag( sourceLocalId.content() )
			.tryWithMetadataTag( sourceLocalId.calculation() )
			.tryWithMetadataTag( sourceLocalId.state() )
			.tryWithMetadataTag( sourceLocalId.command() )
			.tryWithMetadataTag( sourceLocalId.type() )
			.tryWithMetadataTag( sourceLocalId.position() )
			.tryWithMetadataTag( sourceLocalId.detail() );
	}

	std::optional<LocalId> GmodVersioning::convertLocalId(
		const LocalId& sourceLocalId, VisVersion targetVersion ) const
	{
		auto builder = convertLocalId( sourceLocalId.builder(), targetVersion );

		return builder.has_value()
				   ? std::make_optional( builder->build() )
				   : std::nullopt;
	}

	//----------------------------------------------
	// GmodVersioningNode class
	//----------------------------------------------

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	GmodVersioning::GmodVersioningNode::GmodVersioningNode(
		VisVersion visVersion,
		const std::unordered_map<std::string, GmodNodeConversionDto>& dto )
		: m_visVersion{ visVersion }
	{
		for ( const auto& [code, dtoNode] : dto )
		{
			GmodNodeConversion conversion;
			conversion.source = dtoNode.source();
			if ( dtoNode.target().empty() )
			{
				conversion.target = std::nullopt;
			}
			else
			{
				conversion.target = dtoNode.target();
			}
			conversion.oldAssignment = dtoNode.oldAssignment();
			conversion.newAssignment = dtoNode.newAssignment();
			conversion.deleteAssignment = dtoNode.deleteAssignment();

			if ( !dtoNode.operations().empty() )
			{
				for ( const auto& type : dtoNode.operations() )
				{
					conversion.operations.insert( parseConversionType( type ) );
				}
			}

			m_versioningNodeChanges.emplace( code, conversion );
		}
	}

	bool GmodVersioning::GmodVersioningNode::tryGetCodeChanges(
		std::string_view code, GmodNodeConversion& nodeChanges ) const
	{
		auto it = m_versioningNodeChanges.find( code );
		if ( it != m_versioningNodeChanges.end() )
		{
			nodeChanges = it->second;

			return true;
		}

		return false;
	}

	//----------------------------------------------
	// Private helper methods
	//----------------------------------------------

	std::optional<GmodNode> GmodVersioning::convertNodeInternal(
		[[maybe_unused]] VisVersion sourceVersion, const GmodNode& sourceNode, VisVersion targetVersion ) const
	{
		validateSourceAndTargetVersionPair( sourceNode.visVersion(), targetVersion );

		std::string nextCode{ sourceNode.code() };

		auto versioningIt = m_versioningsMap.find( targetVersion );
		if ( versioningIt != m_versioningsMap.end() )
		{
			const auto& versioningNode = versioningIt->second;

			GmodNodeConversion change{};
			if ( versioningNode.tryGetCodeChanges( nextCode, change ) && change.target.has_value() )
			{
				nextCode = change.target.value();
			}
		}

		const auto& targetGmod = VIS::instance().gmod( targetVersion );

		const GmodNode* targetNodePtr = nullptr;
		if ( !targetGmod.tryGetNode( nextCode, targetNodePtr ) )
		{
			return std::nullopt;
		}

		auto resultNode = targetNodePtr->tryWithLocation( sourceNode.location() );
		if ( sourceNode.location().has_value() && resultNode.location() != sourceNode.location() )
		{
			throw std::runtime_error( "Failed to set location" );
		}

		return resultNode;
	}

	bool GmodVersioning::tryGetVersioningNode(
		VisVersion visVersion,
		GmodVersioningNode& versioningNode ) const
	{
		auto it = m_versioningsMap.find( visVersion );
		if ( it != m_versioningsMap.end() )
		{
			versioningNode = it->second;

			return true;
		}

		return false;
	}

	//----------------------------------------------
	// Private validation methods
	//----------------------------------------------

	void GmodVersioning::validateSourceAndTargetVersions(
		VisVersion sourceVersion, VisVersion targetVersion ) const
	{
		if ( sourceVersion == VisVersion::Unknown )
		{
			throw std::invalid_argument( "Invalid source VIS Version: Unknown" );
		}

		if ( targetVersion == VisVersion::Unknown )
		{
			throw std::invalid_argument( "Invalid target VIS Version: Unknown" );
		}

		if ( sourceVersion >= targetVersion )
		{
			throw std::invalid_argument( "Source version must be earlier than target version" );
		}
	}

	void GmodVersioning::validateSourceAndTargetVersionPair(
		VisVersion sourceVersion, VisVersion targetVersion ) const
	{
		if ( sourceVersion >= targetVersion )
		{
			throw std::invalid_argument( "Source version must be less than target version" );
		}

		if ( static_cast<int>( targetVersion ) - static_cast<int>( sourceVersion ) != 100 )
		{
			throw std::invalid_argument( "Target version must be exactly one version higher than source version" );
		}
	}

	//----------------------------------------------
	// Private static utility methods
	//----------------------------------------------

	GmodVersioning::ConversionType GmodVersioning::parseConversionType( std::string_view type )
	{
		if ( type == "changeCode" )
		{
			return ConversionType::ChangeCode;
		}
		if ( type == "merge" )
		{
			return ConversionType::Merge;
		}
		if ( type == "move" )
		{
			return ConversionType::Move;
		}
		if ( type == "assignmentChange" )
		{
			return ConversionType::AssignmentChange;
		}
		if ( type == "assignmentDelete" )
		{
			return ConversionType::AssignmentDelete;
		}

		throw std::invalid_argument( "Invalid conversion type: " + std::string{ type } );
	}
}
