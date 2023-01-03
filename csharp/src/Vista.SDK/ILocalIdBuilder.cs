namespace Vista.SDK;

public interface ILocalIdBuilder
{
    VisVersion? VisVersion { get; }

    bool VerboseMode { get; }

    GmodPath? PrimaryItem { get; }

    GmodPath? SecondaryItem { get; }

    MetadataTag? Quantity { get; }

    MetadataTag? Content { get; }

    MetadataTag? Calculation { get; }

    MetadataTag? State { get; }

    MetadataTag? Command { get; }

    MetadataTag? Type { get; }

    MetadataTag? Position { get; }

    MetadataTag? Detail { get; }

    bool HasCustomTag { get; }

    LocalIdBuilder WithVisVersion(string visVersion);

    LocalIdBuilder WithVisVersion(in VisVersion version);
    bool TryWithVisVersion(string? visVersionStr, out VisVersion visVersion);
    LocalIdBuilder WithoutVisVersion();

    LocalIdBuilder WithVerboseMode(in bool verboseMode);
    LocalIdBuilder WithoutVerboseMode();

    LocalIdBuilder WithPrimaryItem(in GmodPath item);
    LocalIdBuilder TryWithPrimaryItem(in GmodPath? item);
    LocalIdBuilder WithoutPrimaryItem();

    LocalIdBuilder WithSecondaryItem(in GmodPath item);
    LocalIdBuilder TryWithSecondaryItem(in GmodPath? item);
    LocalIdBuilder WithoutSecondaryItem();

    LocalIdBuilder WithMetadataTag(in MetadataTag metadataTag);
    LocalIdBuilder TryWithMetadataTag(in MetadataTag? metadataTag);
    LocalIdBuilder WithoutMedatadaTag(in MetadataTag metadataTag);

    LocalId Build();

    bool IsValid { get; }

    bool IsEmpty { get; }

    string ToString();
}
