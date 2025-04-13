#pragma once

#include "ChdDictionary.h"
#include "GmodNode.h"

namespace dnv::vista::sdk
{
	enum class VisVersion;
	struct GmodDto;
	class GmodPath;

	class Gmod
	{
	public:
		enum class TraversalHandlerResult
		{
			Stop,
			SkipSubtree,
			Continue
		};

		struct TraversalOptions
		{
			static constexpr int DEFAULT_MAX_TRAVERSAL_OCCURRENCE = 3;
			int MaxDepth = 100;
			int MaxOccurrence = DEFAULT_MAX_TRAVERSAL_OCCURRENCE;
			size_t MaxNodes = 100000;

			TraversalOptions() : MaxOccurrence( DEFAULT_MAX_TRAVERSAL_OCCURRENCE ) {}
			explicit TraversalOptions( int maxOccurrence ) : MaxOccurrence( maxOccurrence ) {}
		};

		using TraverseHandler = std::function<TraversalHandlerResult( const std::vector<GmodNode>&, const GmodNode& )>;

	public:
		Gmod() = default;
		Gmod( VisVersion version, const GmodDto& dto );
		Gmod( VisVersion version, const std::unordered_map<std::string, GmodNode>& nodeMap );

		VisVersion GetVisVersion() const;
		const GmodNode& GetRootNode() const;
		const GmodNode& operator[]( const std::string& key ) const;
		bool TryGetNode( const std::string& code, GmodNode& node ) const;
		bool TryGetNode( std::string_view code, GmodNode& node ) const;
		GmodPath ParsePath( const std::string& item ) const;
		bool TryParsePath( const std::string& item, std::optional<GmodPath>& path ) const;
		GmodPath ParseFromFullPath( const std::string& item ) const;
		bool TryParseFromFullPath( const std::string& item, std::optional<GmodPath>& path ) const;
		bool Traverse( const TraverseHandler& handler, const TraversalOptions& options = {} ) const;

		template <typename TState>
		bool Traverse(
			TState& state,
			const std::function<TraversalHandlerResult( TState&, const std::vector<GmodNode>&, const GmodNode& )>& handler,
			const TraversalOptions& options = {} ) const
		{
			return false;
		}

	public:
		static bool IsLeafNode( const std::string& fullType );
		static bool IsLeafNode( const GmodNodeMetadata& metadata );
		static bool IsFunctionNode( const std::string& category );
		static bool IsFunctionNode( const GmodNodeMetadata& metadata );
		static bool IsProductSelection( const GmodNodeMetadata& metadata );
		static bool IsProductType( const GmodNodeMetadata& metadata );
		static bool IsAsset( const GmodNodeMetadata& metadata );
		static bool IsAssetFunctionNode( const GmodNodeMetadata& metadata );
		static bool IsProductTypeAssignment( const GmodNode* parent, const GmodNode* child );
		static bool IsProductSelectionAssignment( const GmodNode* parent, const GmodNode* child );

	private:
		VisVersion m_visVersion;
		GmodNode m_rootNode;
		ChdDictionary<GmodNode> m_nodeMap;

		static const inline std::vector<std::string> s_leafTypes = { "ASSET FUNCTION LEAF", "PRODUCT FUNCTION LEAF" };

	public:
		class Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = GmodNode;
			using difference_type = std::ptrdiff_t;
			using pointer = const GmodNode*;
			using reference = const GmodNode&;

			Iterator() = default;
			Iterator( ChdDictionary<GmodNode>::Iterator innerIt );

			reference operator*() const;
			pointer operator->() const;
			Iterator& operator++();
			Iterator operator++( int );
			bool operator==( const Iterator& other ) const;
			bool operator!=( const Iterator& other ) const;

		private:
			ChdDictionary<GmodNode>::Iterator m_innerIt;
		};

		Iterator begin() const;
		Iterator end() const;
	};
}
