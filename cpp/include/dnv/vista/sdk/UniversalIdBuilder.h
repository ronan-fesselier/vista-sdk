#pragma once

namespace dnv::vista::sdk
{
	class IUniversalIdBuilder;
	class ImoNumber;
	class LocalIdBuilder;
	class ParsingErrors;
	class UniversalId;

	class LocalIdParsingErrorBuilder;
	enum class LocalIdParsingState;
	class UniversalIdBuilder : public IUniversalIdBuilder
	{
	public:
		const std::string NamingEntity = "data.dnv.com";

	private:
		std::optional<LocalIdBuilder> m_localId;
		std::optional<ImoNumber> m_imoNumber;

	public:
		UniversalIdBuilder() = default;

		std::optional<ImoNumber> GetImoNumber() const override;
		std::optional<LocalIdBuilder> GetLocalId() const override;
		bool IsValid() const override;

		static UniversalIdBuilder Create( VisVersion version );
		UniversalId Build() const;

		UniversalIdBuilder WithLocalId( const LocalIdBuilder& localId ) override;
		UniversalIdBuilder WithoutLocalId() override;
		UniversalIdBuilder TryWithLocalId( const std::optional<LocalIdBuilder>& localId ) override;
		UniversalIdBuilder TryWithLocalId( const std::optional<LocalIdBuilder>& localId, bool& succeeded ) override;

		UniversalIdBuilder WithImoNumber( const ImoNumber& imoNumber ) override;
		UniversalIdBuilder TryWithImoNumber( const std::optional<ImoNumber>& imoNumber ) override;
		UniversalIdBuilder TryWithImoNumber( const std::optional<ImoNumber>& imoNumber, bool& succeeded ) override;
		UniversalIdBuilder WithoutImoNumber() override;

		std::string ToString() const override;
		bool Equals( const UniversalIdBuilder& other ) const;
		size_t GetHashCode() const;

		static UniversalIdBuilder Parse( const std::string& universalIdStr );
		static bool TryParse( const std::string& universalIdStr, ParsingErrors& errors, std::shared_ptr<UniversalIdBuilder>& builder );

	private:
		static void AddError( LocalIdParsingErrorBuilder& errorBuilder, LocalIdParsingState state, const std::string& message );
	};
}
