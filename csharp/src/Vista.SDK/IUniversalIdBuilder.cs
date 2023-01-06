namespace Vista.SDK;

public interface IUniversalIdBuilder
{
    bool IsValid { get; }

    ImoNumber? ImoNumber { get; }
    LocalIdBuilder? LocalId { get; }

    UniversalIdBuilder WithLocalId(in LocalIdBuilder localId);
    UniversalIdBuilder WithoutLocalId();
    UniversalIdBuilder TryWithLocalId(in LocalIdBuilder? localId);
    UniversalIdBuilder TryWithLocalId(in LocalIdBuilder? localId, out bool succeeded);

    UniversalIdBuilder WithImoNumber(in ImoNumber imoNumber);
    UniversalIdBuilder TryWithImoNumber(in ImoNumber? imoNumber);
    UniversalIdBuilder TryWithImoNumber(in ImoNumber? imoNumber, out bool succeeded);
    UniversalIdBuilder WithoutImoNumber();

    string ToString();
}
