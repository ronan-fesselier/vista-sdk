using System.Text.Json.Serialization;

namespace Vista.SDK;

public sealed record GmodVersioningNodeChangesDto(
    [property: JsonPropertyName("nextVisVersion")] string NextVisVersion,
    [property: JsonPropertyName("nextCode")] string NextCode,
    [property: JsonPropertyName("previousVisVersion")] string PreviousVisVersion,
    [property: JsonPropertyName("previousCode")] string PreviousCode,
    [property: JsonPropertyName("formerParent")] string FormerParent
);

public sealed record GmodVersioningDto(
    [property: JsonPropertyName("items")]
        IReadOnlyDictionary<string, IReadOnlyDictionary<string, GmodVersioningNodeChangesDto>> Items
);
