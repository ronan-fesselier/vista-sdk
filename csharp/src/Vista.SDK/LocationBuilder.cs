using System.ComponentModel.DataAnnotations;
using System.Text;
using Vista.SDK;

public enum LocationGroup
{
    Number,
    Side,
    Vertical,
    Transverse,
    Longitudinal
}

public record LocationBuilder
{
    private int? _number;
    private char? _side;
    private char? _vertical;
    private char? _transverse;
    private char? _longitudinal;

    private Locations _locations;

    public VisVersion VisVersion { get; }

    private IReadOnlyDictionary<char, LocationGroup> _reversedGroups;
    public IReadOnlyDictionary<LocationGroup, IReadOnlyList<char>> GroupedCodes { get; }

    internal LocationBuilder(VisVersion version, Locations locations)
    {
        VisVersion = version;
        _locations = locations;

        var groups = new Dictionary<LocationGroup, List<char>>();
        var reversedGroups = new Dictionary<char, LocationGroup>();

        foreach (var item in locations.RelativeLocations)
        {
            var key = item.Code switch
            {
                'N' => LocationGroup.Number,
                'P' or 'C' or 'S' => LocationGroup.Side,
                'U' or 'M' or 'L' => LocationGroup.Vertical,
                'I' or 'O' => LocationGroup.Transverse,
                'F' or 'A' => LocationGroup.Longitudinal,
                _ => throw new Exception($"Unsupported code: {item.Code}")
            };

            if (!groups.ContainsKey(key))
                groups.Add(key, new());
            if (key == LocationGroup.Number)
                continue;

            reversedGroups.Add(item.Code, key);
            groups.GetValueOrDefault(key)?.Add(item.Code);
        }

        _reversedGroups = reversedGroups;
        GroupedCodes = groups.ToDictionary(g => g.Key, g => g.Value as IReadOnlyList<char>);
    }

    public LocationBuilder WithNumber(int number) => WithValue(LocationGroup.Number, number.ToString());

    public LocationBuilder WithSide(string side) => WithValue(LocationGroup.Side, side);

    public LocationBuilder WithVertical(string vertical) => WithValue(LocationGroup.Vertical, vertical);

    public LocationBuilder WithTransverse(string transverse) => WithValue(LocationGroup.Transverse, transverse);

    public LocationBuilder WithLongitudinal(string longitudinal) => WithValue(LocationGroup.Longitudinal, longitudinal);

    public LocationBuilder WithValue(string value)
    {
        if (int.TryParse(value, out _))
            return WithValue(LocationGroup.Number, value);
        if (!char.TryParse(value, out var val))
            throw new ValidationException("Value should be a character");
        if (!_reversedGroups.TryGetValue(val, out var key))
            throw new ValidationException($"The value {value} is an invalid Locations value");
        return WithValue(key, value);
    }

    public LocationBuilder WithValue(LocationGroup group, string value)
    {
        if (group == LocationGroup.Number)
        {
            if (!int.TryParse(value, out var number))
                throw new ValidationException("Value should be number");
            return this with { _number = number };
        }

        if (char.TryParse(value, out var val))
            throw new ValidationException("Value should be a character");

        if (!_reversedGroups.ContainsKey(val))
            throw new ValidationException($"The value {value} is an invalid Locations value");

        switch (group)
        {
            case LocationGroup.Side:
                return this with { _side = val };
            case LocationGroup.Vertical:
                return this with { _vertical = val };
            case LocationGroup.Transverse:
                return this with { _transverse = val };
            case LocationGroup.Longitudinal:
                return this with { _longitudinal = val };
            default:
                throw new Exception($"Unsupported code: {group}");
        }
    }

    public Location Build()
    {
        return new Location(ToString());
    }

    public override string ToString()
    {
        var items = new char?[] { _side, _vertical, _transverse, _longitudinal };

        var str = "";

        foreach (var item in items)
        {
            if (item is null)
                continue;

            str += item.ToString();
        }

        var parts = str.ToString().OrderBy(c => c).Select(s => s.ToString());

        if (_number is not null)
            parts.Prepend(_number.ToString());

        return string.Concat(parts);
    }
}
