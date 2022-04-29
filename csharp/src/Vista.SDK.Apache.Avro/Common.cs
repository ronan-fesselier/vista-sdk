namespace Vista.SDK.Transport;

public static class Common
{
    public static IDictionary<string, object> CopyProperties(
        this IReadOnlyDictionary<string, object> props
    ) => props.ToDictionary(kvp => kvp.Key, kvp => kvp.Value);

    public static IReadOnlyDictionary<string, object> CopyProperties(
        this IDictionary<string, object> props
    ) => props.ToDictionary(kvp => kvp.Key, kvp => kvp.Value);
}
