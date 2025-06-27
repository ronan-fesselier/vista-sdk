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
					actualParents = parents;

					GmodNode current = parents[0];
					while ( !current.isRoot() )
					{
						if ( current.parents().size() != 1 )
						{
							throw std::runtime_error( "Invalid state - expected one parent" );
						}

						current = *current.parents()[0];
						actualParents.insert( actualParents.begin(), current );
					}
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
					for ( const auto& p : actualParents )
					{
						bool found = false;
						for ( const auto& fp : state.fromPath )
						{
							if ( equals( fp.code(), p.code() ) )
							{
								found = true;
								break;
							}
						}
						if ( !found )
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
