using Vista.SDK.Internal;

namespace Vista.SDK;

public sealed partial record class UniversalIdBuilder : IUniversalIdBuilder
{
    public static readonly string NamingEntity = "data.dnv.com";
    private LocalIdBuilder? _localId;
    public ImoNumber? ImoNumber { get; private init; }

    public LocalIdBuilder? LocalId => _localId;

    public bool IsValid => ImoNumber is not null && _localId is not null && _localId.IsValid;

    public static UniversalIdBuilder Create(VisVersion version) =>
        new UniversalIdBuilder().WithLocalId(LocalIdBuilder.Create(version));

    public bool Equals(UniversalIdBuilder? other)
    {
        if (other is null)
            return false;
        return ImoNumber == other.ImoNumber && LocalId == other.LocalId;
    }

    public UniversalId Build()
    {
        return new UniversalId(this);
    }

    public UniversalIdBuilder WithLocalId(in LocalIdBuilder localId) =>
        this with
        {
            _localId = localId
        };

    public UniversalIdBuilder WithoutLocalId() => this with { _localId = null };

    public bool TryWithLocalId(
        in LocalIdBuilder? localId,
        out UniversalIdBuilder universalIdBuilder
    )
    {
        if (localId == null)
        {
            universalIdBuilder = this;
            return false;
        }
        universalIdBuilder = this with { _localId = localId };
        return true;
    }

    public UniversalIdBuilder TryWithLocalId(in LocalIdBuilder? localId)
    {
        if (localId is null)
            return this;
        return this with { _localId = localId };
    }

    public UniversalIdBuilder WithImoNumber(in ImoNumber imoNumber) =>
        this with
        {
            ImoNumber = imoNumber
        };

    public UniversalIdBuilder WithoutImoNumber() => this with { ImoNumber = null };

    public UniversalIdBuilder TryWithImoNumber(in ImoNumber? imoNumber)
    {
        if (imoNumber is null)
            return this;

        return this with
        {
            ImoNumber = imoNumber
        };
    }

    public bool TryWithImoNumber(in ImoNumber? imoNumber, out UniversalIdBuilder universalIdBuilder)
    {
        if (imoNumber is null)
        {
            universalIdBuilder = this;
            return false;
        }
        universalIdBuilder = this with { ImoNumber = imoNumber };
        return true;
    }

    public sealed override int GetHashCode()
    {
        var hashCode = new HashCode();
        hashCode.Add(ImoNumber);
        hashCode.Add(LocalId);
        return hashCode.ToHashCode();
    }

    public override string ToString()
    {
        if (ImoNumber is null)
            throw new InvalidOperationException("Invalid Universal Id state: Missing IMO Number");
        if (LocalId is null)
            throw new InvalidOperationException("Invalid Universal Id state: Missing LocalId");

        string namingEntity = $"{NamingEntity}";
        using var lease = StringBuilderPool.Get();

        var builder = lease.Builder;

        builder.Append(namingEntity);
        builder.Append("/");
        builder.Append(ImoNumber.ToString());

        LocalId.ToString(builder);

        return lease.ToString();
    }
}
