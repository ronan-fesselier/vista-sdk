using System.Text;
using Vista.SDK.Internal;

namespace Vista.SDK;

public partial record class LocalIdBuilder : ILocalIdBuilder
{
    public static readonly string NamingRule = "dnv-v2";

    public VisVersion? VisVersion { get; private init; }

    public bool VerboseMode { get; private init; } = false;

    internal LocalIdItems Items { get; private init; }

    public GmodPath? PrimaryItem => Items.PrimaryItem;

    public GmodPath? SecondaryItem => Items.SecondaryItem;

    public MetadataTag? Quantity { get; private init; }

    public MetadataTag? Content { get; private init; }

    public MetadataTag? Calculation { get; private init; }

    public MetadataTag? State { get; private init; }

    public MetadataTag? Command { get; private init; }

    public MetadataTag? Type { get; private init; }

    public MetadataTag? Position { get; private init; }

    public MetadataTag? Detail { get; private init; }

    public LocalIdBuilder WithVisVersion(in string visVersion) =>
        this with
        {
            VisVersion = VisVersions.Parse(visVersion)
        };

    public LocalIdBuilder WithVisVersion(in VisVersion version) =>
        this with
        {
            VisVersion = version
        };

    public LocalIdBuilder TryWithVisVersion(in VisVersion? visVersion)
    {
        if (visVersion == null)
            return this;

        return WithVisVersion(visVersion.Value);
    }

    public bool TryWithVisVersion(in string? visVersionStr, out LocalIdBuilder localIdBuilder)
    {
        if (VisVersions.TryParse(visVersionStr, out VisVersion v) == true)
        {
            localIdBuilder = this with { VisVersion = v };
            return true;
        }
        localIdBuilder = this;
        return false;
    }

    public LocalIdBuilder WithoutVisVersion() => this with { VisVersion = null };

    public LocalIdBuilder WithVerboseMode(in bool verboseMode) =>
        this with
        {
            VerboseMode = verboseMode
        };

    public LocalIdBuilder WithPrimaryItem(in GmodPath item) =>
        this with
        {
            Items = this.Items with { PrimaryItem = item }
        };

    public LocalIdBuilder WithoutPrimaryItem() =>
        this with
        {
            Items = this.Items with { PrimaryItem = null }
        };

    public LocalIdBuilder TryWithPrimaryItem(in GmodPath? item)
    {
        if (item is null)
            return this;
        return WithPrimaryItem(item);
    }

    public bool TryWithPrimaryItem(in GmodPath? item, out LocalIdBuilder localIdBuilder)
    {
        if (item is null)
        {
            localIdBuilder = this;
            return false;
        }

        localIdBuilder = WithPrimaryItem(item);
        return true;
    }

    public LocalIdBuilder WithSecondaryItem(in GmodPath item) =>
        this with
        {
            Items = this.Items with { SecondaryItem = item }
        };

    public LocalIdBuilder WithoutSecondaryItem() =>
        this with
        {
            Items = this.Items with { SecondaryItem = null }
        };

    public LocalIdBuilder TryWithSecondaryItem(in GmodPath? item)
    {
        if (item is null)
            return this;
        return WithSecondaryItem(item);
    }

    public bool TryWithSecondaryItem(in GmodPath? item, out LocalIdBuilder localIdBuilder)
    {
        if (item is null)
        {
            localIdBuilder = this;
            return false;
        }

        localIdBuilder = WithSecondaryItem(item);
        return true;
    }

    public LocalIdBuilder WithMetadataTag(in MetadataTag metadataTag) =>
        metadataTag.Name switch
        {
            CodebookName.Quantity => WithQuantity(in metadataTag),
            CodebookName.Content => WithContent(in metadataTag),
            CodebookName.Calculation => WithCalculation(in metadataTag),
            CodebookName.State => WithState(in metadataTag),
            CodebookName.Command => WithCommand(in metadataTag),
            CodebookName.Type => WithType(in metadataTag),
            CodebookName.Position => WithPosition(in metadataTag),
            CodebookName.Detail => WithDetail(in metadataTag),
            _ => throw new ArgumentException("Invalid metadata tag: " + metadataTag),
        };

    public LocalIdBuilder WithoutMetadataTag(in CodebookName name) =>
        name switch
        {
            CodebookName.Quantity => WithoutQuantity(),
            CodebookName.Content => WithoutContent(),
            CodebookName.Calculation => WithoutCalculcation(),
            CodebookName.State => WithoutState(),
            CodebookName.Command => WithoutCommand(),
            CodebookName.Type => WithoutType(),
            CodebookName.Position => WithoutPosition(),
            CodebookName.Detail => WithoutDetail(),
            _ => throw new ArgumentException("invalid metadata codebook name: " + name),
        };

    public LocalIdBuilder TryWithMetadataTag(in MetadataTag? metadataTag)
    {
        if (metadataTag is null)
            return this;

        try
        {
            return WithMetadataTag(metadataTag.Value);
        }
        catch (ArgumentException)
        {
            return this;
        }
    }

    public bool TryWithMetadataTag(in MetadataTag? metadataTag, out LocalIdBuilder localIdBuilder)
    {
        if (metadataTag is null)
        {
            localIdBuilder = this;
            return false;
        }

        try
        {
            localIdBuilder = WithMetadataTag(metadataTag.Value);
            return true;
        }
        catch (ArgumentException)
        {
            localIdBuilder = this;
            return false;
        }
    }

    public LocalIdBuilder WithoutQuantity() => this with { Quantity = null };

    public LocalIdBuilder WithoutContent() => this with { Content = null };

    public LocalIdBuilder WithoutCalculcation() => this with { Calculation = null };

    public LocalIdBuilder WithoutState() => this with { State = null };

    public LocalIdBuilder WithoutCommand() => this with { Command = null };

    public LocalIdBuilder WithoutType() => this with { Type = null };

    public LocalIdBuilder WithoutPosition() => this with { Position = null };

    public LocalIdBuilder WithoutDetail() => this with { Detail = null };

    private LocalIdBuilder WithQuantity(in MetadataTag quantity) =>
        this with
        {
            Quantity = quantity,
        };

    private LocalIdBuilder WithContent(in MetadataTag content) => this with { Content = content, };

    private LocalIdBuilder WithCalculation(in MetadataTag calculation) =>
        this with
        {
            Calculation = calculation,
        };

    private LocalIdBuilder WithState(in MetadataTag state) => this with { State = state, };

    private LocalIdBuilder WithCommand(in MetadataTag command) => this with { Command = command, };

    private LocalIdBuilder WithType(in MetadataTag type) => this with { Type = type, };

    private LocalIdBuilder WithPosition(in MetadataTag position) =>
        this with
        {
            Position = position,
        };

    private LocalIdBuilder WithDetail(in MetadataTag detail) => this with { Detail = detail, };

    public static LocalIdBuilder Create(VisVersion version) =>
        new LocalIdBuilder().WithVisVersion(version);

    public LocalId Build()
    {
        if (IsEmpty)
            throw new InvalidOperationException("Cant build to LocalId from empty LocalIdBuilder");
        if (!IsValid)
            throw new InvalidOperationException(
                "Cant build to LocalId from invalid LocalIdBuilder"
            );

        return new LocalId(this);
    }

    public bool HasCustomTag =>
        (Quantity?.IsCustom ?? false)
        || (Calculation?.IsCustom ?? false)
        || (Content?.IsCustom ?? false)
        || (Position?.IsCustom ?? false)
        || (State?.IsCustom ?? false)
        || (Command?.IsCustom ?? false)
        || (Type?.IsCustom ?? false)
        || (Detail?.IsCustom ?? false);

    public bool IsValid =>
        VisVersion is not null
        && Items.PrimaryItem is not null
        && (
            Quantity is not null
            || Calculation is not null
            || Content is not null
            || Position is not null
            || State is not null
            || Command is not null
            || Type is not null
            || Detail is not null
        );

    public bool IsEmpty =>
        Items.PrimaryItem is null
        && Items.SecondaryItem is null
        && (
            Quantity is null
            && Calculation is null
            && Content is null
            && Position is null
            && State is null
            && Command is null
            && Type is null
            && Detail is null
        );

    public bool IsEmptyMetadata =>
        Quantity is null
        && Calculation is null
        && Content is null
        && Position is null
        && State is null
        && Command is null
        && Type is null
        && Detail is null;

    public bool Equals(LocalIdBuilder? other)
    {
        if (other is null)
            return false;

        if (VisVersion != other.VisVersion)
            throw new InvalidOperationException(
                "Cant compare local IDs from different VIS versions"
            );

        return PrimaryItem == other.PrimaryItem
            && SecondaryItem == other.SecondaryItem
            && Quantity == other.Quantity
            && Calculation == other.Calculation
            && Content == other.Content
            && Position == other.Position
            && State == other.State
            && Command == other.Command
            && Type == other.Type
            && Detail == other.Detail;
    }

    public sealed override int GetHashCode()
    {
        var hashCode = new HashCode();
        hashCode.Add(PrimaryItem);
        hashCode.Add(SecondaryItem);
        hashCode.Add(Quantity);
        hashCode.Add(Calculation);
        hashCode.Add(Content);
        hashCode.Add(Position);
        hashCode.Add(State);
        hashCode.Add(Command);
        hashCode.Add(Type);
        hashCode.Add(Detail);
        return hashCode.ToHashCode();
    }

    public void ToString(StringBuilder builder)
    {
        if (VisVersion is null)
            throw new InvalidOperationException("No VisVersion configured on LocalId");

        string namingRule = $"/{NamingRule}/";

        builder.Append(namingRule);

        builder.Append("vis-");
        VisVersion.Value.ToVersionString(builder);
        builder.Append('/');

        Items.Append(builder, VerboseMode);

        builder.Append("meta/");

        // NOTE: order of metadatatags matter,
        // should not be changed unless changed in the naming rule/standard
        builder.AppendMeta(Quantity);
        builder.AppendMeta(Content);
        builder.AppendMeta(Calculation);
        builder.AppendMeta(State);
        builder.AppendMeta(Command);
        builder.AppendMeta(Type);
        builder.AppendMeta(Position);
        builder.AppendMeta(Detail);

        if (builder[builder.Length - 1] == '/')
            builder.Remove(builder.Length - 1, 1);
    }

    public override string ToString()
    {
        using var lease = StringBuilderPool.Get();
        var builder = lease.Builder;

        ToString(builder);

        return lease.ToString();
    }
}
