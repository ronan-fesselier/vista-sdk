namespace Vista.SDK;

public sealed record LocalIdQueryBuilder
{
    private GmodPathQuery? _primaryItem;
    private GmodPathQuery? _secondaryItem;
    private readonly Dictionary<CodebookName, MetadataTag> _tags = new();

    public IReadOnlyList<MetadataTag> Tags => _tags.Values.ToList();

    public LocalIdQuery Build() => new(this);

    public static LocalIdQueryBuilder Empty() => new();

    public static LocalIdQueryBuilder From(LocalId localId)
    {
        var builder = new LocalIdQueryBuilder().WithPrimaryItem(GmodPathQueryBuilder.From(localId.PrimaryItem).Build());
        if (localId.SecondaryItem != null)
            builder = builder.WithSecondaryItem(GmodPathQueryBuilder.From(localId.SecondaryItem).Build());
        foreach (var tag in localId.MetadataTags)
            builder = builder.WithTag(tag.Name, tag.Value);
        return builder;
    }

    public LocalIdQueryBuilder WithPrimaryItem(
        GmodPath primaryItem,
        Func<GmodPathQueryBuilder.Path, GmodPathQuery> configure
    )
    {
        var builder = GmodPathQueryBuilder.From(primaryItem);
        return WithPrimaryItem(configure(builder));
    }

    public LocalIdQueryBuilder WithPrimaryItem(GmodPath primaryItem)
    {
        return WithPrimaryItem(GmodPathQueryBuilder.From(primaryItem).Build());
    }

    public LocalIdQueryBuilder WithPrimaryItem(GmodPathQuery primaryItem)
    {
        return this with { _primaryItem = primaryItem };
    }

    public LocalIdQueryBuilder WithSecondaryItem(
        GmodPath secondaryItem,
        Func<GmodPathQueryBuilder.Path, GmodPathQuery> configure
    )
    {
        var builder = GmodPathQueryBuilder.From(secondaryItem);
        return WithSecondaryItem(configure(builder));
    }

    public LocalIdQueryBuilder WithSecondaryItem(GmodPath secondaryItem)
    {
        return WithSecondaryItem(GmodPathQueryBuilder.From(secondaryItem).Build());
    }

    public LocalIdQueryBuilder WithSecondaryItem(GmodPathQuery secondaryItem)
    {
        return this with { _secondaryItem = secondaryItem };
    }

    public LocalIdQueryBuilder WithTag(MetadataTag tag)
    {
        _tags.Add(tag.Name, tag);
        // Not sure if this is necessary
        return this with { };
    }

    public LocalIdQueryBuilder WithTag(CodebookName name, string value)
    {
        return WithTag(new MetadataTag(name, value));
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

        if (_primaryItem != null && _primaryItem.Match(localId.PrimaryItem) == false)
            return false;
        if (_secondaryItem != null && _secondaryItem.Match(localId.SecondaryItem) == false)
            return false;
        var metadataTags = other.MetadataTags.ToDictionary(t => t.Name);
        if (_tags.Count > 0)
        {
            foreach (var tag in _tags.Values)
            {
                if (!metadataTags.TryGetValue(tag.Name, out var otherTag))
                    return false;
                if (tag.Equals(otherTag) == false)
                    return false;
            }
        }

        return true;
    }
}

internal sealed record PathItem(GmodPath Path, bool Invidivualized);
