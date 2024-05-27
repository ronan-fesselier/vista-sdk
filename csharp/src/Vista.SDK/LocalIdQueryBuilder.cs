namespace Vista.SDK;

public sealed record LocalIdQueryBuilder
{
    private PathItem? _primaryItem;
    private PathItem? _secondaryItem;
    private readonly Dictionary<CodebookName, MetadataTag> _tags = new();

    public IReadOnlyList<MetadataTag> Tags => _tags.Values.ToList();
    public GmodPath? PrimaryItem => _primaryItem?.Path;
    public GmodPath? SecondaryItem => _secondaryItem?.Path;

    public LocalIdQuery Build() => new(this);

    public static LocalIdQueryBuilder New() => new();

    public static LocalIdQueryBuilder From(LocalId localId)
    {
        var builder = new LocalIdQueryBuilder().WithPrimaryItem(localId.PrimaryItem, true);
        if (localId.SecondaryItem != null)
            builder = builder.WithSecondaryItem(localId.SecondaryItem, true);
        foreach (var tag in localId.MetadataTags)
            builder = builder.WithTag(tag.Name, tag.Value);
        return builder;
    }

    public LocalIdQueryBuilder WithPrimaryItem(GmodPath primaryItem, bool individualized)
    {
        var p =
            primaryItem.VisVersion == VIS.LatestVisVersion
                ? primaryItem
                : VIS.Instance.ConvertPath(primaryItem, VIS.LatestVisVersion);
        if (p is null)
            throw new Exception("Failed to convert path");
        return this with { _primaryItem = new(p, individualized) };
    }

    public LocalIdQueryBuilder WithSecondaryItem(GmodPath secondaryItem, bool individualized)
    {
        var p =
            secondaryItem.VisVersion == VIS.LatestVisVersion
                ? secondaryItem
                : VIS.Instance.ConvertPath(secondaryItem, VIS.LatestVisVersion);
        if (p is null)
            throw new Exception("Failed to convert path");
        return this with { _secondaryItem = new(p, individualized) };
    }

    public LocalIdQueryBuilder WithTag(CodebookName name, string value)
    {
        _tags.Add(name, new MetadataTag(name, value));
        // Not sure if this is necessary
        return this with { };
    }

    internal bool Match(string other) => Match(LocalId.Parse(other));

    internal bool Match(LocalId other)
    {
        LocalId localId = other;
        if (other.VisVersion <= VIS.LatestVisVersion)
        {
            var converted = VIS.Instance.ConvertLocalId(other, VIS.LatestVisVersion);
            if (converted is null)
                throw new Exception("Failed to convert local id");
            localId = converted;
        }

        if (_primaryItem != null && MatchPath(_primaryItem, localId.PrimaryItem) == false)
            return false;
        if (_secondaryItem != null && MatchPath(_secondaryItem, localId.SecondaryItem) == false)
            return false;

        foreach (var tag in localId.MetadataTags)
        {
            if (_tags.TryGetValue(tag.Name, out var expectedTag))
                if (expectedTag.Value != tag.Value)
                    return false;
        }

        return true;
    }

    private static bool MatchPath(PathItem item, GmodPath? other)
    {
        var source = item.Path;
        var target = other;
        if (item.Invidivualized == false)
        {
            source = item.Path.WithoutLocations();
            target = item.Path.WithoutLocations();
        }
        return source == target;
    }
}

internal sealed record PathItem(GmodPath Path, bool Invidivualized);
