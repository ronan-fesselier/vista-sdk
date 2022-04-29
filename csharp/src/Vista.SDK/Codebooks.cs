namespace Vista.SDK;

public sealed class Codebooks
{
    public VisVersion VisVersion { get; }

    private readonly Dictionary<CodebookName, Codebook> _codebooks;

    internal Codebooks(VisVersion version, CodebooksDto dto)
    {
        VisVersion = version;

        _codebooks = new Dictionary<CodebookName, Codebook>(dto.Items.Length);

        foreach (var typeDto in dto.Items)
        {
            var type = new Codebook(typeDto);
            _codebooks.Add(type.Name, type);
        }

        var detailsCodebook = new Codebook(
            new CodebookDto("detail", new Dictionary<string, string[]>())
        );
        _codebooks.Add(detailsCodebook.Name, detailsCodebook);
    }

    public Codebook this[CodebookName name]
    {
        get
        {
            if (!_codebooks.TryGetValue(name, out var codebook))
                throw new ArgumentException("Invalid codebook name: " + name);

            return codebook;
        }
    }

    public MetadataTag? TryCreateTag(CodebookName name, string? value) =>
        _codebooks[name].TryCreateTag(value);

    public MetadataTag CreateTag(CodebookName name, string value) =>
        _codebooks[name].CreateTag(value);

    public Codebook GetCodebook(CodebookName name)
    {
        if (!_codebooks.TryGetValue(name, out var codebook))
            throw new ArgumentException("Invalid codebook name: " + name);

        return codebook;
    }
}
