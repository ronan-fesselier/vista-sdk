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

    LocalIdBuilder WithVisVersion(in string visVersion);
    LocalIdBuilder WithVisVersion(in VisVersion version);
    LocalIdBuilder TryWithVisVersion(in VisVersion? version);
    bool TryWithVisVersion(in string? visVersionStr, out LocalIdBuilder visVersion);
    LocalIdBuilder WithoutVisVersion();

    LocalIdBuilder WithVerboseMode(in bool verboseMode);

    LocalIdBuilder WithPrimaryItem(in GmodPath item);
    LocalIdBuilder TryWithPrimaryItem(in GmodPath? item);
    bool TryWithPrimaryItem(in GmodPath? item, out LocalIdBuilder localIdbuilder);
    LocalIdBuilder WithoutPrimaryItem();

    LocalIdBuilder WithSecondaryItem(in GmodPath item);
    LocalIdBuilder TryWithSecondaryItem(in GmodPath? item);
    bool TryWithSecondaryItem(in GmodPath? item, out LocalIdBuilder localIdBuilder);
    LocalIdBuilder WithoutSecondaryItem();

    LocalIdBuilder WithMetadataTag(in MetadataTag metadataTag);
    LocalIdBuilder TryWithMetadataTag(in MetadataTag? metadataTag);
    bool TryWithMetadataTag(in MetadataTag? metadataTag, out LocalIdBuilder localIdBuilder);
    LocalIdBuilder WithoutMetadataTag(in CodebookName name);

    LocalId Build();

    bool IsValid { get; }

    bool IsEmpty { get; }

    string ToString();
}
