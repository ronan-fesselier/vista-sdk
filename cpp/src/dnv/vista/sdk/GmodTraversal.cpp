#include "pch.h"

#include "dnv/vista/sdk/Config.h"
#include "dnv/vista/sdk/GmodTraversal.h"

namespace dnv::vista::sdk
{
	//=====================================================================
	// Traversal algorithms
	//=====================================================================

	namespace GmodTraversal
	{
		namespace detail
		{
			//----------------------------------------------
			// Path analysis context
			//----------------------------------------------

			struct PathExistsContext
			{
				const GmodNode& to;
				std::vector<const GmodNode*> remainingParents;
				std::vector<const GmodNode*> fromPath;

				PathExistsContext( const GmodNode& toNode, const std::vector<const GmodNode*>& fromPathList )
					: to{ toNode }, fromPath{ fromPathList }
				{
					remainingParents.reserve( 32 );
				}

				PathExistsContext() = delete;
				PathExistsContext( const PathExistsContext& ) = delete;
				PathExistsContext( PathExistsContext&& ) noexcept = delete;
				~PathExistsContext() = default;
				PathExistsContext& operator=( const PathExistsContext& ) = delete;
				PathExistsContext& operator=( PathExistsContext&& ) noexcept = delete;
			};
		}

		//=====================================================================
		// Public API
		//=====================================================================

		bool traverse( const Gmod& gmodInstance, TraverseHandler handler, const TraversalOptions& options )
		{
			TraverseHandler capturedHandler = handler;
			TraverseHandlerWithState<TraverseHandler> wrapperHandler =
				[]( TraverseHandler& h, const std::vector<const GmodNode*>& parents, const GmodNode& node )
				-> TraversalHandlerResult { return h( parents, node ); };

			return traverse( capturedHandler, gmodInstance, wrapperHandler, options );
		}

		bool traverse( const GmodNode& rootNode, TraverseHandler handler, const TraversalOptions& options )
		{
			TraverseHandler capturedHandler = handler;
			TraverseHandlerWithState<TraverseHandler> wrapperHandler =
				[]( TraverseHandler& h, const std::vector<const GmodNode*>& parents, const GmodNode& node )
				-> TraversalHandlerResult { return h( parents, node ); };

			return traverse( capturedHandler, rootNode, wrapperHandler, options );
		}

		bool pathExistsBetween(
			const Gmod& gmodInstance,
			const std::vector<const GmodNode*>& fromPath,
			const GmodNode& to,
			std::vector<const GmodNode*>& remainingParents )
		{
			remainingParents.clear();

			const GmodNode* lastAssetFunction = nullptr;
			for ( auto it = fromPath.rbegin(); it != fromPath.rend(); ++it )
			{
				if ( *it && ( *it )->metadata().category() == GMODNODE_CATEGORY_ASSET_FUNCTION )
				{
					lastAssetFunction = *it;
					break;
				}
			}

			detail::PathExistsContext context( to, fromPath );

			TraverseHandlerWithState<detail::PathExistsContext> handler =
				[]( detail::PathExistsContext& state, const std::vector<const GmodNode*>& parents, const GmodNode& node ) -> TraversalHandlerResult {
				if ( node.code() != state.to.code() )
					return TraversalHandlerResult::Continue;

				std::vector<const GmodNode*> actualParents;
				actualParents.reserve( parents.size() + 10 );

				if ( !parents.empty() && !parents[0]->isRoot() )
				{
					std::vector<const GmodNode*> pathToRoot;
					const GmodNode* current = parents[0];

					while ( current && !current->isRoot() )
					{
						if ( current->parents().size() != 1 )
						{
							throw std::runtime_error( "Invalid state - expected one parent" );
						}

						current = current->parents()[0];
						if ( current )
						{
							pathToRoot.push_back( current );
						}
					}

					std::reverse( pathToRoot.begin(), pathToRoot.end() );
					actualParents.insert( actualParents.end(), pathToRoot.begin(), pathToRoot.end() );
					actualParents.insert( actualParents.end(), parents.begin(), parents.end() );
				}
				else
				{
					actualParents = parents;
				}

				if ( actualParents.size() < state.fromPath.size() )
				{
					return TraversalHandlerResult::Continue;
				}

				bool match = true;
				for ( size_t i = 0; i < state.fromPath.size(); ++i )
				{
					if ( actualParents[i]->code() != state.fromPath[i]->code() )
					{
						match = false;
						break;
					}
				}

				if ( match )
				{
					state.remainingParents.clear();
					for ( const auto* p : actualParents )
					{
						bool found = false;
						for ( const auto* fp : state.fromPath )
						{
							if ( fp->code() == p->code() )
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
