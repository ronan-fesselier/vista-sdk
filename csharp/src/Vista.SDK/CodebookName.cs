namespace Vista.SDK;

public enum CodebookName
{
    Position = 1,
    Quantity = 2,
    Calculation = 3,
    State = 4,
    Content = 5,
    Command = 6,
    Type = 7,
    Detail = 8
}

public static class CodebookNames
{
    public static CodebookName FromString(string name) =>
        name switch
        {
            "positions" => CodebookName.Position,
            "calculations" => CodebookName.Calculation,
            "quantities" => CodebookName.Quantity,
            "states" => CodebookName.State,
            "contents" => CodebookName.Content,
            "commands" => CodebookName.Command,
            "types" => CodebookName.Type,
            "detail" => CodebookName.Detail,
            _ => throw new ArgumentException("Unknown metadata tag: " + name, nameof(name)),
        };

    public static CodebookName FromPrefix(string? prefix) =>
        prefix switch
        {
            "pos" => CodebookName.Position,
            "qty" => CodebookName.Quantity,
            "calc" => CodebookName.Calculation,
            "state" => CodebookName.State,
            "cnt" => CodebookName.Content,
            "cmd" => CodebookName.Command,
            "type" => CodebookName.Type,
            "detail" => CodebookName.Detail,
            null => throw new ArgumentException(nameof(prefix)),
            _ => throw new ArgumentException("unknown prefix: " + prefix, nameof(prefix)),
        };
}
