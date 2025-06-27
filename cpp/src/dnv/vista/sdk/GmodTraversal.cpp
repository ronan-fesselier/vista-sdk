#include "pch.h"

#include "dnv/vista/sdk/Config.h"
#include "dnv/vista/sdk/GmodTraversal.h"
#include "dnv/vista/sdk/utils/StringUtils.h"

namespace dnv::vista::sdk
{
	namespace GmodTraversal
	{
		//=====================================================================
		// Public API
		//=====================================================================

		bool traverse( const Gmod& gmodInstance, TraverseHandler handler, const TraversalOptions& options )
		{
			TraverseHandler capturedHandler = handler;
			TraverseHandlerWithState<TraverseHandler> wrapperHandler =
				[]( TraverseHandler& h, const std::vector<GmodNode>& parents, const GmodNode& node )
				-> TraversalHandlerResult {
				return h( parents, node );
			};

			return traverse( capturedHandler, gmodInstance, wrapperHandler, options );
		}

		bool traverse( const GmodNode& rootNode, TraverseHandler handler, const TraversalOptions& options )
		{
			TraverseHandler capturedHandler = handler;
			TraverseHandlerWithState<TraverseHandler> wrapperHandler =
				[]( TraverseHandler& h, const std::vector<GmodNode>& parents, const GmodNode& node )
				-> TraversalHandlerResult {
				return h( parents, node );
			};

			return traverse( capturedHandler, rootNode, wrapperHandler, options );
		}

		bool pathExistsBetween(
			const Gmod& gmodInstance,
			const std::vector<GmodNode>& fromPath,
			const GmodNode& to,
			std::vector<GmodNode>& remainingParents )
		{
			const GmodNode* lastAssetFunction = nullptr;
			for ( auto it = fromPath.rbegin(); it != fromPath.rend(); ++it )
			{
				if ( Gmod::isAssetFunctionNode( it->metadata() ) )
				{
					lastAssetFunction = &( *it );
					break;
				}
			}

			remainingParents.clear();

			detail::PathExistsContext context( to, fromPath );

			TraverseHandlerWithState<detail::PathExistsContext> handler =
				[]( detail::PathExistsContext& state, const std::vector<GmodNode>& parents, const GmodNode& node ) -> TraversalHandlerResult {
				if ( !equals( node.code(), state.to.code() ) )
				{
					return TraversalHandlerResult::Continue;
				}

				std::vector<GmodNode> actualParents;

				if ( !parents.empty() && !parents[0].isRoot() )
				{
					// Build complete parent chain by traversing up from first parent
					// Use pointer-based traversal to avoid unnecessary copying
					std::vector<const GmodNode*> parentChain;
					
					const GmodNode* current = &parents[0];
					while ( !current->isRoot() )
					{
						if ( current->parents().size() != 1 )
						{
							throw std::runtime_error( "Invalid state - expected one parent" );
						}
						
						current = current->parents()[0];
						parentChain.push_back( current );
					}
					
					// Reserve space and build actualParents from root down
					actualParents.reserve( parentChain.size() + parents.size() );
					
					// Add parents from root down (reverse order of parentChain)
					for ( auto it = parentChain.rbegin(); it != parentChain.rend(); ++it )
					{
						actualParents.emplace_back( **it );
					}
					
					// Add the existing parents
					actualParents.insert( actualParents.end(), parents.begin(), parents.end() );
				}
				else
				{
					actualParents.assign( parents.begin(), parents.end() );
				}

				if ( actualParents.size() < state.fromPath.size() )
				{
					return TraversalHandlerResult::Continue;
				}

				bool match = true;
				for ( size_t i = 0; i < state.fromPath.size(); ++i )
				{
					if ( !equals( actualParents[i].code(), state.fromPath[i].code() ) )
					{
						match = false;
						break;
					}
				}

				if ( match )
				{
					state.remainingParents.clear();
					
					// Optimize path matching using code-based comparison with O(n) performance
					// Create a set of codes from fromPath for fast lookup
					StringSet fromPathCodes;
					fromPathCodes.reserve( state.fromPath.size() );
					for ( const auto& fp : state.fromPath )
					{
						fromPathCodes.insert( std::string( fp.code() ) );
					}
					
					// Find remaining parents not in fromPath
					for ( const auto& p : actualParents )
					{
						if ( fromPathCodes.find( std::string( p.code() ) ) == fromPathCodes.end() )
						{
							state.remainingParents.push_back( p );
						}
					}
					
					return TraversalHandlerResult::Stop;
				}

				return TraversalHandlerResult::Continue;
			};

			bool reachedEnd = traverse(
				context,
				lastAssetFunction ? *lastAssetFunction : gmodInstance.rootNode(),
				handler );

			remainingParents = std::move( context.remainingParents );

			return !reachedEnd;
		}
	}
}
