namespace Vista.SDK;

public interface IUniversalIdBuilder
{
    bool IsValid { get; }

    ImoNumber? ImoNumber { get; }
    LocalIdBuilder? LocalId { get; }

    UniversalIdBuilder WithLocalId(LocalIdBuilder localId);
    UniversalIdBuilder WithoutLocalId();
    bool TryWithLocalId(LocalIdBuilder? localId, out UniversalIdBuilder universalIdBuilder);

    UniversalIdBuilder WithImoNumber(ImoNumber imoNumber);
    UniversalIdBuilder WithoutImoNumber();
    UniversalIdBuilder TryWithImoNumber(ImoNumber? imoNumber);
    bool TryWithImoNumber(ImoNumber? imoNumber, out UniversalIdBuilder universalIdBuilder);

    string ToString();
}
