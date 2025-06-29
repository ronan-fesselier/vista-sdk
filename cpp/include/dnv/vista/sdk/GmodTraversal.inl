/**
 * @file GmodTraversal.inl
 * @brief Inline implementations for performance-critical GmodTraversal operations
 */

#include "Config.h"
#include "utils/StringUtils.h"

namespace dnv::vista::sdk
{
	namespace GmodTraversal
	{
		namespace detail
		{
			//----------------------------------------------
			// Parents class
			//----------------------------------------------

			inline Parents::Parents()
			{
				m_parents.reserve( 64 );
				m_occurrences.reserve( 4 );
			}

			inline void Parents::push( const GmodNode& parent )
			{
				m_parents.push_back( parent );

				if ( auto it = m_occurrences.find( parent.code() ); it != m_occurrences.end() )
				{
					++it->second;
				}
				else
				{
					m_occurrences.emplace( std::string( parent.code() ), 1 );
				}
			}

			inline void Parents::pop()
			{
				if ( m_parents.empty() )
				{
					return;
				}

				const GmodNode& parent = m_parents.back();
				m_parents.pop_back();

				if ( auto it = m_occurrences.find( parent.code() ); it != m_occurrences.end() )
				{
					if ( it->second == 1 )
					{
						m_occurrences.erase( it );
					}
					else
					{
						--it->second;
					}
				}
			}

			inline int Parents::occurrences( const GmodNode& node ) const noexcept
			{
				if ( auto it = m_occurrences.find( node.code() ); it != m_occurrences.end() )
				{
					return it->second;
				}

				return 0;
			}

			inline const GmodNode* Parents::lastOrDefault() const noexcept
			{
				return m_parents.empty() ? nullptr : &m_parents.back();
			}

			inline const std::vector<GmodNode>& Parents::asList() const noexcept
			{
				return m_parents;
			}

			//----------------------------------------------
			// TraversalContext struct
			//----------------------------------------------

			template <typename TState>
			inline TraversalContext<TState>::TraversalContext( Parents& p, TraverseHandlerWithState<TState> h, TState& s, int maxOcc )
				: parents{ p },
				  handler{ h },
				  state{ s },
				  maxTraversalOccurrence{ maxOcc }
			{
			}

			//----------------------------------------------
			// PathExistsContext struct
			//----------------------------------------------

			inline PathExistsContext::PathExistsContext( const GmodNode& toNode, const std::vector<GmodNode>& fromPathList )
				: to{ toNode },
				  fromPath{ fromPathList }
			{
				remainingParents.reserve( 32 );
			}

			//----------------------------------------------
			// Core algorithm
			//----------------------------------------------

			template <typename TState>
			inline TraversalHandlerResult traverseNode( TraversalContext<TState>& context, const GmodNode& node )
			{
				if ( node.metadata().installSubstructure().has_value() && !node.metadata().installSubstructure().value() )
				{
					return TraversalHandlerResult::Continue;
				}

				TraversalHandlerResult result = context.handler( context.state, context.parents.asList(), node );
				if ( result == TraversalHandlerResult::Stop || result == TraversalHandlerResult::SkipSubtree )
				{
					return result;
				}

				bool skipOccurrenceCheck = Gmod::isProductSelectionAssignment( context.parents.lastOrDefault(), &node );

				if ( !skipOccurrenceCheck )
				{
					int occ = context.parents.occurrences( node );
					if ( occ == context.maxTraversalOccurrence )
					{
						return TraversalHandlerResult::SkipSubtree;
					}
					if ( occ > context.maxTraversalOccurrence )
					{
						throw std::runtime_error( "Invalid state - node occurred more than expected" );
					}
				}

				context.parents.push( node );

				const auto& children = node.children();
				for ( const auto& child : children )
				{
					result = traverseNode( context, child );
					if ( result == TraversalHandlerResult::Stop )
					{
						return TraversalHandlerResult::Stop;
					}
					else if ( result == TraversalHandlerResult::SkipSubtree )
					{
						continue;
					}
				}

				context.parents.pop();

				return TraversalHandlerResult::Continue;
			}
		}

		//=====================================================================
		// Public API
		//=====================================================================

		//----------------------------------------------
		// Non-template functions
		//----------------------------------------------

		inline bool traverse( const Gmod& gmodInstance, TraverseHandler handler, const TraversalOptions& options )
		{
			TraverseHandler capturedHandler = handler;
			TraverseHandlerWithState<TraverseHandler> wrapperHandler =
				[]( TraverseHandler& h, const std::vector<GmodNode>& parents, const GmodNode& node )
				-> TraversalHandlerResult {
				return h( parents, node );
			};

			return traverse( capturedHandler, gmodInstance, wrapperHandler, options );
		}

		inline bool traverse( const GmodNode& rootNode, TraverseHandler handler, const TraversalOptions& options )
		{
			TraverseHandler capturedHandler = handler;
			TraverseHandlerWithState<TraverseHandler> wrapperHandler =
				[]( TraverseHandler& h, const std::vector<GmodNode>& parents, const GmodNode& node )
				-> TraversalHandlerResult {
				return h( parents, node );
			};

			return traverse( capturedHandler, rootNode, wrapperHandler, options );
		}

		//----------------------------------------------
		// Template functions
		//----------------------------------------------

		template <typename TState>
		inline bool traverse( TState& state, const Gmod& gmodInstance, TraverseHandlerWithState<TState> handler, const TraversalOptions& options )
		{
			detail::Parents parentsStack;
			detail::TraversalContext<TState> context( parentsStack, handler, state, options.maxTraversalOccurrence );

			return detail::traverseNode( context, gmodInstance.rootNode() ) == TraversalHandlerResult::Continue;
		}

		template <typename TState>
		inline bool traverse( TState& state, const GmodNode& rootNode, TraverseHandlerWithState<TState> handler, const TraversalOptions& options )
		{
			detail::Parents parentsStack;
			detail::TraversalContext<TState> context( parentsStack, handler, state, options.maxTraversalOccurrence );

			return detail::traverseNode( context, rootNode ) == TraversalHandlerResult::Continue;
		}
	}
}
