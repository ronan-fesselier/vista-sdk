using System.Globalization;
using Vista.SDK.Internal;

namespace Vista.SDK;

public enum LocationGroup
{
    Number,
    Side,
    Vertical,
    Transverse,
    Longitudinal
}

public readonly record struct Location
{
    public readonly string Value { get; }

    internal Location(string value)
    {
        Value = value;
    }

    public override readonly string ToString() => Value;

    public static implicit operator string(Location n) => n.Value;
}

public sealed class Locations
{
    private readonly char[] _locationCodes;
    private readonly List<RelativeLocation> _relativeLocations;

    public VisVersion VisVersion { get; }

    // This is if we need Code, Name, Definition in Frontend UI
    public IReadOnlyList<RelativeLocation> RelativeLocations => _relativeLocations;

    public IReadOnlyDictionary<LocationGroup, IReadOnlyList<RelativeLocation>> Groups => GroupLocations();

    internal Locations(VisVersion version, LocationsDto dto)
    {
        VisVersion = version;

        _locationCodes = dto.Items.Select(d => d.Code).ToArray();

        _relativeLocations = new List<RelativeLocation>(dto.Items.Length);
        foreach (var relativeLocationsDto in dto.Items)
        {
            _relativeLocations.Add(
                new RelativeLocation(
                    relativeLocationsDto.Code,
                    relativeLocationsDto.Name,
                    new Location(relativeLocationsDto.Code.ToString()),
                    relativeLocationsDto.Definition
                )
            );
        }
    }

    public Location Parse(string locationStr)
    {
        var errorBuilder = LocationParsingErrorBuilder.Empty;

        var span = locationStr is null ? ReadOnlySpan<char>.Empty : locationStr.AsSpan();
        if (!TryParseInternal(span, locationStr, out var location, ref errorBuilder))
            throw new ArgumentException($"Invalid value for location: {locationStr}, errors: {errorBuilder.Build()}");

        return location;
    }

    public Location Parse(ReadOnlySpan<char> locationStr)
    {
        var errorBuilder = LocationParsingErrorBuilder.Empty;

        if (!TryParseInternal(locationStr, null, out var location, ref errorBuilder))
            throw new ArgumentException(
                $"Invalid value for location: {locationStr.ToString()}, errors: {errorBuilder.Build()}"
            );

        return location;
    }

    public bool TryParse(string? value, out Location location)
    {
        var span = value is null ? ReadOnlySpan<char>.Empty : value.AsSpan();
        var errorBuilder = LocationParsingErrorBuilder.Empty;
        return TryParseInternal(span, value, out location, ref errorBuilder);
    }

    public bool TryParse(string? value, out Location location, out ParsingErrors errors)
    {
        var span = value is null ? ReadOnlySpan<char>.Empty : value.AsSpan();
        var errorBuilder = LocationParsingErrorBuilder.Empty;
        var result = TryParseInternal(span, value, out location, ref errorBuilder);
        errors = errorBuilder.Build();
        return result;
    }

    public bool TryParse(ReadOnlySpan<char> value, out Location location)
    {
        var errorBuilder = LocationParsingErrorBuilder.Empty;
        return TryParseInternal(value, null, out location, ref errorBuilder);
    }

    public bool TryParse(ReadOnlySpan<char> value, out Location location, out ParsingErrors errors)
    {
        var errorBuilder = LocationParsingErrorBuilder.Empty;
        var result = TryParseInternal(value, null, out location, ref errorBuilder);
        errors = errorBuilder.Build();
        return result;
    }

    private bool TryParseInternal(
        ReadOnlySpan<char> span,
        string? originalStr,
        out Location location,
        ref LocationParsingErrorBuilder errorBuilder
    )
    {
        location = default;

        if (span.IsEmpty)
        {
            AddError(
                ref errorBuilder,
                LocationValidationResult.NullOrWhiteSpace,
                "Invalid location: contains only whitespace"
            );
            return false;
        }

        if (span.IsWhiteSpace())
        {
            AddError(
                ref errorBuilder,
                LocationValidationResult.NullOrWhiteSpace,
                "Invalid location: contains only whitespace"
            );
            return false;
        }

        var originalSpan = span;

        int? digitStartIndex = null;
        int? lastLetterIndex = null;
        int? charsStartIndex = null;
        int? n = null;

        for (int i = 0; i < span.Length; i++)
        {
            ref readonly var ch = ref span[i];

            if (char.IsDigit(ch))
            {
                if (digitStartIndex is null)
                {
                    digitStartIndex = i;
                    if (lastLetterIndex is not null)
                    {
                        AddError(
                            ref errorBuilder,
                            LocationValidationResult.Invalid,
                            $"Invalid location: numeric location should start before location code(s) in location: '{originalStr ?? originalSpan.ToString()}'"
                        );
                        return false;
                    }
                }
                else
                {
                    if (lastLetterIndex is not null)
                    {
                        if (lastLetterIndex < digitStartIndex)
                        {
                            AddError(
                                ref errorBuilder,
                                LocationValidationResult.Invalid,
                                $"Invalid location: numeric location should start before location code(s) in location: '{originalStr ?? originalSpan.ToString()}'"
                            );
                            return false;
                        }
                        else if (lastLetterIndex > digitStartIndex && lastLetterIndex < i)
                        {
                            AddError(
                                ref errorBuilder,
                                LocationValidationResult.Invalid,
                                $"Invalid location: cannot have multiple separated digits in location: '{originalStr ?? originalSpan.ToString()}'"
                            );
                            return false;
                        }
                    }

#if NETCOREAPP3_1_OR_GREATER
                    n = int.Parse(
                        span.Slice(digitStartIndex.Value, i - digitStartIndex.Value),
                        NumberStyles.None,
                        CultureInfo.InvariantCulture
                    );
#else
                    n = int.Parse(
                        span.Slice(digitStartIndex.Value, i - digitStartIndex.Value).ToString(),
                        NumberStyles.None,
                        CultureInfo.InvariantCulture
                    );
#endif

                    if (n.Value < 0)
                    {
                        AddError(
                            ref errorBuilder,
                            LocationValidationResult.Invalid,
                            $"Invalid location: negative numeric location is not allowed: '{originalStr ?? originalSpan.ToString()}'"
                        );
                        return false;
                    }
                }
            }
            else
            {
                if (ch == 'N' || !_locationCodes.Contains(ch))
                {
                    var invalidChars = string.Join(
                        ",",
                        (originalStr ?? originalSpan.ToString())
                            .Where(c => !char.IsDigit(c) && (c == 'N' || !_locationCodes.Contains(c)))
                            .Select(c => $"'{c}'")
                    );
                    AddError(
                        ref errorBuilder,
                        LocationValidationResult.InvalidCode,
                        $"Invalid location code: '{originalStr ?? originalSpan.ToString()}' with invalid location code(s): {invalidChars}"
                    );
                    return false;
                }

                if (charsStartIndex is null)
                {
                    charsStartIndex = i;
                }
                else if (i > 0)
                {
                    ref readonly var prevCh = ref span[i - 1];
                    if (ch.CompareTo(prevCh) < 0)
                    {
                        AddError(
                            ref errorBuilder,
                            LocationValidationResult.InvalidOrder,
                            $"Invalid location: '{originalStr ?? originalSpan.ToString()}' not alphabetically sorted"
                        );
                        return false;
                    }
                }

                lastLetterIndex = i;
            }
        }

        location = new Location(originalStr ?? originalSpan.ToString());
        return true;
    }

    static void AddError(ref LocationParsingErrorBuilder errorBuilder, LocationValidationResult name, string message)
    {
        if (!errorBuilder.HasError)
            errorBuilder = LocationParsingErrorBuilder.Create();
        errorBuilder.AddError(name, message);
    }

    private IReadOnlyDictionary<LocationGroup, IReadOnlyList<RelativeLocation>> GroupLocations()
    {
        var groups = new Dictionary<LocationGroup, List<RelativeLocation>>();

        foreach (var location in _relativeLocations)
        {
            var key = char.ToLower(location.Code) switch
            {
                'n' => LocationGroup.Number,
                'p' or 'c' or 's' => LocationGroup.Side,
                'u' or 'm' or 'l' => LocationGroup.Vertical,
                'i' or 'o' => LocationGroup.Transverse,
                'f' or 'a' or 's' => LocationGroup.Longitudinal,
                _ => throw new Exception($"Unsupported code: {location.Code}")
            };
            if (!groups.ContainsKey(key))
                groups.Add(key, new());
            groups.GetValueOrDefault(key)?.Add(location);
        }
        return groups.ToDictionary(g => g.Key, g => g.Value.ToArray() as IReadOnlyList<RelativeLocation>);
    }
}

public sealed record RelativeLocation
{
    public char Code { get; }
    public string Name { get; }
    public string? Definition { get; }

    public Location Location { get; }

    internal RelativeLocation(char code, string name, Location location, string? definition)
    {
        Code = code;
        Name = name;
        Definition = definition;
        Location = location;
    }

    public override int GetHashCode() => Code.GetHashCode();

    public bool Equals(RelativeLocation? other) => Code == other?.Code;
}

public enum LocationValidationResult
{
    Invalid,
    InvalidCode,
    InvalidOrder,
    NullOrWhiteSpace,
    Valid
}
