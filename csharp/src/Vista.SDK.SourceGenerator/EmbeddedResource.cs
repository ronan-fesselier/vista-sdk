// Copyright @kzu
// License MIT
// copied from https://github.com/devlooped/ThisAssembly/blob/main/src/EmbeddedResource.cs

using System.IO.Compression;
using System.Reflection;
using System.Text.Json;

namespace Vista.SDK;

internal static class EmbeddedResource
{
    private static string GetDecompressedContent(string baseName, string manifestResourceName)
    {
        using var stream = Assembly
            .GetExecutingAssembly()
            .GetManifestResourceStream(manifestResourceName);

        if (stream == null)
            throw new InvalidOperationException(
                $"Did not find required resource '{manifestResourceName}' in assembly '{baseName}'."
            );

        using var decompressionStream = new GZipStream(stream, CompressionMode.Decompress);

        using var reader = new StreamReader(decompressionStream);
        return reader.ReadToEnd();
    }

    public static List<string> GetGmodVisVersion()
    {
        var baseName = Assembly.GetExecutingAssembly().GetName().Name;
        if (baseName is null)
            throw new InvalidOperationException(
                "Couldnt get name of assembly while loading vis versions"
            );

        var manifestResourceNames = Assembly
            .GetExecutingAssembly()
            .GetManifestResourceNames()
            .Where(x => x.EndsWith(".gz"))
            .ToList();

        if (!manifestResourceNames.Any())
            throw new InvalidOperationException(
                $"Did not find required resources in assembly '{baseName}'."
            );

        var visVersions = new List<string>();

        foreach (var manifestResourceName in manifestResourceNames)
        {
            if (manifestResourceName.Contains("gmod"))
            {
                var gmodJson = GetDecompressedContent(baseName, manifestResourceName);
                var gmod =
                    JsonSerializer.Deserialize<GmodDto>(gmodJson)
                    ?? throw new InvalidOperationException(
                        $"Could not deserialize Gmod {nameof(gmodJson)}"
                    );
                visVersions.Add(gmod.VisVersion);
            }
        }
        return visVersions;
    }

    private static GZipStream GetDecompressedStream(Assembly assembly, string resourceName)
    {
        var stream = assembly.GetManifestResourceStream(resourceName);

        if (stream == null)
            throw new InvalidOperationException(
                $"Did not find required resource '{resourceName}' in assembly '{assembly.FullName}'."
            );

        return new GZipStream(stream, CompressionMode.Decompress, leaveOpen: false);
    }

    public static async ValueTask<GmodDto?> GetGmod(
        string visVersion,
        CancellationToken cancellationToken
    )
    {
        var assembly = Assembly.GetExecutingAssembly();
        var baseName = assembly.GetName().Name;

        var gmodResourceName = Assembly
            .GetExecutingAssembly()
            .GetManifestResourceNames()
            .Where(x => x.Contains("gmod") && x.EndsWith(".gz"))
            .Where(x => x.Contains(visVersion))
            .SingleOrDefault();

        if (gmodResourceName is null)
            return null;

        using var stream = GetDecompressedStream(assembly, gmodResourceName);

        return await JsonSerializer.DeserializeAsync<GmodDto>(
            stream,
            cancellationToken: cancellationToken
        );
    }

    public static async ValueTask<CodebooksDto?> GetCodebooks(
        string visVersion,
        CancellationToken cancellationToken
    )
    {
        var assembly = Assembly.GetExecutingAssembly();
        var baseName = assembly.GetName().Name;

        var codebooksResourceName = assembly
            .GetManifestResourceNames()
            .Where(x => x.Contains("codebooks") && x.EndsWith(".gz"))
            .Where(x => x.Contains(visVersion))
            .SingleOrDefault();

        if (codebooksResourceName is null)
            return null;

        using var stream = GetDecompressedStream(assembly, codebooksResourceName);

        return await JsonSerializer.DeserializeAsync<CodebooksDto>(
            stream,
            cancellationToken: cancellationToken
        );
    }
}
