namespace Vista.SDK;

public interface IUniversalIdBuilder
{
    bool IsValid { get; }

    ImoNumber? ImoNumber { get; }
    LocalIdBuilder? LocalId { get; }

    UniversalIdBuilder WithLocalId(in LocalIdBuilder localId);
    UniversalIdBuilder WithoutLocalId();
    UniversalIdBuilder TryWithLocalId(in LocalIdBuilder? localId);
    bool TryWithLocalId(in LocalIdBuilder? localId, out UniversalIdBuilder universalIdBuilder);

    UniversalIdBuilder WithImoNumber(in ImoNumber imoNumber);
    UniversalIdBuilder WithoutImoNumber();
    UniversalIdBuilder TryWithImoNumber(in ImoNumber? imoNumber);
    bool TryWithImoNumber(in ImoNumber? imoNumber, out UniversalIdBuilder universalIdBuilder);

    string ToString();
}
