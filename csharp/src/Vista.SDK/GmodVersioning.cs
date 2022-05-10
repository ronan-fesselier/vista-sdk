using System.Diagnostics.CodeAnalysis;

using Vista.SDK.Internal;

namespace Vista.SDK;

public sealed class GmodVersioning
{
    private readonly Dictionary<string, GmodVersioningNode> _versioningsMap = new();
    private readonly Func<VisVersion, ValueTask<Gmod>> _gmod;

    internal GmodVersioning(GmodVersioningDto dto, Func<VisVersion, ValueTask<Gmod>> gmod)
    {
        foreach (var versioningDto in dto.Items)
        {
            var visVersion = versioningDto.Key;
            var gmodVersioningNode = new GmodVersioningNode(versioningDto.Value);
            _versioningsMap.Add(visVersion, gmodVersioningNode);
        }
        _gmod = gmod;
    }

    public async ValueTask<GmodNode> ConvertNode(
        VisVersion sourceVersion,
        GmodNode sourceNode,
        VisVersion targetVersion
    )
    {
        ValidateSourceAndTargetVersions(sourceVersion, targetVersion);

        if (!TryGetVersioningNode(sourceVersion.ToVersionString(), out var versioningNode))
            throw new ArgumentException(
                "Couldn't get versioning node with VIS version" + sourceVersion.ToVersionString()
            );

        var nextCode = versioningNode.TryGetCodeChanges(sourceNode.Code, out var nodeChanges)
          ? nodeChanges.NextCode
          : sourceNode.Code;

        var gmod = await _gmod(targetVersion);
        if (!gmod.TryGetNode(nextCode is null ? sourceNode.Code : nextCode, out var targetNode))
            throw new ArgumentException("Couldn't get target node with code: " + nextCode);
        return targetNode;
    }

    public async ValueTask<GmodPath> ConvertPath(
        VisVersion sourceVersion,
        GmodPath sourcePath,
        VisVersion targetVersion
    )
    {
        var newPathIndex = 0;
        var newParents = new GmodNode[sourcePath.Length - 1];
        GmodNode? newNode = null;
        foreach (var (depth, node) in sourcePath.GetFullPath())
        {
            var nextNode = await ConvertNode(sourceVersion, node, targetVersion);
            var targetNode = node.Location is not null
                ? nextNode with
                  {
                      Location = sourcePath[depth].Location
                  }
                : nextNode;

            if (depth < sourcePath.Length - 1)
                newParents[newPathIndex++] = targetNode;
            else
                newNode = targetNode;
        }

        if (newNode is null)
            throw new InvalidOperationException("Invalid conversion");

        return new GmodPath(newParents, newNode);
    }

    public GmodVersioningNode this[string key] => _versioningsMap[key];

    public bool TryGetVersioningNode(string visVersion, out GmodVersioningNode versioningNode) =>
        _versioningsMap.TryGetValue(visVersion, out versioningNode);

    private void ValidateSourceAndTargetVersions(VisVersion sourceVersion, VisVersion targetVersion)
    {
        if (string.IsNullOrWhiteSpace(sourceVersion.ToVersionString()))
            throw new ArgumentException(
                "Invalid source VIS Version: " + sourceVersion.ToVersionString()
            );

        if (string.IsNullOrWhiteSpace(targetVersion.ToVersionString()))
            throw new ArgumentException(
                "Invalid target VISVersion: " + targetVersion.ToVersionString()
            );

        if (!_versioningsMap.ContainsKey(sourceVersion.ToVersionString()))
            throw new ArgumentException(
                "Source VIS Version does not exist in versionings: "
                    + sourceVersion.ToVersionString()
            );

        if (!_versioningsMap.ContainsKey(targetVersion.ToVersionString()))
            throw new ArgumentException(
                "Target VIS Version does not exist in versionings: "
                    + targetVersion.ToVersionString()
            );
    }
}

public readonly record struct GmodVersioningNode
{
    private readonly Dictionary<string, GmodVersioningNodeChanges> _versioningNodeChanges = new();

    internal GmodVersioningNode(IReadOnlyDictionary<string, GmodVersioningNodeChangesDto> dto)
    {
        foreach (var versioningNodeDto in dto)
        {
            var code = versioningNodeDto.Key;
            var versioningNodeChanges = new GmodVersioningNodeChanges(
                versioningNodeDto.Value.NextVisVersion,
                versioningNodeDto.Value.NextCode,
                versioningNodeDto.Value.PreviousVisVersion,
                versioningNodeDto.Value.PreviousCode
            );
            _versioningNodeChanges.Add(code, versioningNodeChanges);
        }
    }

    public bool TryGetCodeChanges(
        string code,
        [MaybeNullWhen(false)] out GmodVersioningNodeChanges nodeChanges
    ) => _versioningNodeChanges.TryGetValue(code, out nodeChanges);
}

public sealed record class GmodVersioningNodeChanges(
    string NextVisVersion,
    string NextCode,
    string PreviousVisVersion,
    string PreviousCode
);
