/**
 * @file GmodTraversal.inl
 * @brief Inline implementations for performance-critical GmodTraversal operations
 */

namespace dnv::vista::sdk
{
	namespace GmodTraversal
	{
		namespace detail
		{
			/**
			 * @brief Initialize parent stack with optimized capacity
			 */
			inline Parents::Parents( size_t maxOccurrence )
				: m_maxTraversalOccurrence{ maxOccurrence }
			{
				m_parents.reserve( 64 );

				if ( m_maxTraversalOccurrence > 1 )
				{
					m_occurrences.reserve( 8 );
				}
			}

			/**
			 * @brief Push parent node and update occurrence tracking
			 */
			inline void Parents::push( const GmodNode* parent )
			{
				m_parents.emplace_back( parent );

				const std::string_view key = parent->code();
				if ( const size_t* count = m_occurrences.tryGetValue( key ) )
				{
					++( const_cast<size_t&>( *count ) );
				}
				else
				{
					m_occurrences.insertOrAssign( std::string{ key }, 1 );
				}
			}

			/**
			 * @brief Pop parent node and update occurrence tracking
			 */
			inline void Parents::pop()
			{
				if ( m_parents.empty() )
				{
					return;
				}

				const GmodNode* parent = m_parents.back();
				m_parents.pop_back();

				const std::string_view key = parent->code();
				if ( const size_t* count = m_occurrences.tryGetValue( key ) )
				{
					if ( *count == 1 )
					{
						m_occurrences.erase( key );
					}
					else
					{
						--( const_cast<size_t&>( *count ) );
					}
				}
			}

			/**
			 * @brief Get occurrence count for specific node
			 */
			inline size_t Parents::occurrences( const GmodNode& node ) const noexcept
			{
				const std::string_view key = node.code();
				if ( const size_t* count = m_occurrences.tryGetValue( key ) )
				{
					return *count;
				}

				return 0;
			}

			/**
			 * @brief Get last parent node or nullptr if empty
			 */
			inline const GmodNode* Parents::lastOrDefault() const noexcept
			{
				return m_parents.empty() ? nullptr : m_parents.back();
			}

			/**
			 * @brief Get complete parent chain as vector
			 */
			inline const std::vector<const GmodNode*>& Parents::asList() const noexcept
			{
				return m_parents;
			}

			/**
			 * @brief Core recursive traversal implementation with cycle detection
			 */
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
					size_t occ = context.parents.occurrences( node );

					if ( occ == context.maxTraversalOccurrence )
					{
						return TraversalHandlerResult::SkipSubtree;
					}

					if ( occ > context.maxTraversalOccurrence )
					{
						throw std::runtime_error( "Invalid state - node occurred more than expected" );
					}
				}

				context.parents.push( &node );

				const auto& children = node.children();
				for ( size_t i = 0; i < children.size(); ++i )
				{
					const GmodNode& child = *children[i];
					result = traverseNode( context, child );
					if ( result == TraversalHandlerResult::Stop )
					{
						context.parents.pop();
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

		/**
		 * @brief Traverse GMOD from root with stateful handler
		 */
		template <typename TState>
		VISTA_SDK_CPP_FORCE_INLINE bool traverse( TState& state, const Gmod& gmodInstance, TraverseHandlerWithState<TState> handler, const TraversalOptions& options )
		{
			detail::Parents parentsStack( options.maxTraversalOccurrence );
			detail::TraversalContext<TState> context( parentsStack, handler, state, options.maxTraversalOccurrence );

			return detail::traverseNode( context, gmodInstance.rootNode() ) == TraversalHandlerResult::Continue;
		}

		template <typename TState>
		VISTA_SDK_CPP_FORCE_INLINE bool traverse( TState& state, const GmodNode& rootNode, TraverseHandlerWithState<TState> handler, const TraversalOptions& options )
		{
			detail::Parents parentsStack( options.maxTraversalOccurrence );
			detail::TraversalContext<TState> context( parentsStack, handler, state, options.maxTraversalOccurrence );

			return detail::traverseNode( context, rootNode ) == TraversalHandlerResult::Continue;
		}
	}
}
