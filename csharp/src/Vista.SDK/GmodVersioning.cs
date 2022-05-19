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

        if (!gmod.TryGetNode(nextCode, out var targetNode))
            throw new ArgumentException("Couldn't get target node with code: " + nextCode);
        return targetNode;
    }

    public async ValueTask<GmodPath> ConvertPath(
        VisVersion sourceVersion,
        GmodPath sourcePath,
        VisVersion targetVersion
    )
    {
        // "511.331/C221", --- > "C121.31/C221"
        var endNode = await ConvertNode(sourceVersion, sourcePath.Node, targetVersion); // sourcePath[^1] last node
        static bool OnlyOnePath(GmodNode node) =>
            node.Parents.Count == 1 && OnlyOnePath(node.Parents[0]);

        if (OnlyOnePath(endNode))
            return new GmodPath(
                endNode.Parents,
                await ConvertNode(sourceVersion, endNode, targetVersion)
            );

        var targetNodes = new List<GmodNode>();

        // var current = endNode;
        var parentToEndNode = sourcePath.Parents[sourcePath.Parents.Count - 1];
        var newParentToEndNode = await ConvertNode(sourceVersion, parentToEndNode, targetVersion);
        var current = newParentToEndNode;
        targetNodes.Add(current);
        while (current.Code != "VE")
        {
            if (current.Parents.Count == 1)
            {
                current = current.Parents[0];
                targetNodes.Add(current);
                continue;
            }
            for (int i = 0; i < current.Parents.Count; i++)
            {
                var currentParent = current.Parents[i]; // 511.31
                // The number 2 within targetNodes[targetNodes.Count - 2] is used to get C121.3,
                // but for other test cases one will get
                // System.ArgumentOutOfRangeException
                // Needs a fix to generalize it / make edge cases
                var previousTargetNode = targetNodes[targetNodes.Count - 2]; // C121.3
                var parentToCurrentParent = currentParent.Parents[0]; // 511.3i
                if (
                    !TryGetVersioningNode(
                        targetVersion.ToVersionString(),
                        out var nextVersioningNode
                    )
                )
                    throw new Exception("Invalid versioning");
                if (
                    !nextVersioningNode.TryGetCodeChanges(
                        previousTargetNode.Code,
                        out var nodeChanges
                    )
                )
                    continue;
                if (nodeChanges?.FormerParent is null)
                    continue;

                var formerParentCode = nodeChanges.FormerParent; // 511.3i (string)
                if (parentToCurrentParent.Code != formerParentCode)
                    continue;
                // Maybe uncomment these lines to double-check parent-child relationship?
                // var gmod = await _gmod(targetVersion);
                //var targetParentNode = gmod[formerParentCode];
                // if (!targetParentNode.IsChild(currentParent))
                //     continue;
                targetNodes.Add(currentParent);
                //targetNodes.Add(targetParentNode); // 511.3i
                current = currentParent;
                break;
            }
            // var formerParentNode = sourcePath.Parents[^1];
            // var newParentNode = await ConvertNode(sourceVersion, formerParentNode, targetVersion);
            // targetNodes.Add(newParentNode);
            // current = newParentNode;

        }

        targetNodes.Reverse();

        return new GmodPath(targetNodes, endNode);

        // for (int i = sourcePath.Length - 1; i >= 0; i--)
        // {
        //     var sourceNode = sourcePath[i];
        //     if (sourceNode.Parents.Count == i)
        //     {
        //         var nextNode = await ConvertNode(sourceVersion, sourceNode, targetVersion);
        //         targetNodes.Add(nextNode);
        //         continue;
        //     }
        //
        //     if (i == sourcePath.Length - 1)
        //     {
        //         endNode = await ConvertNode(sourceVersion, sourceNode, targetVersion);
        //         continue;
        //     }
        //     // 511.331 -> C121.31
        //     var nextParentNode = await ConvertNode(sourceVersion, sourceNode, targetVersion);
        //     var targetParentNode = sourceNode.Location is not null
        //         ? nextParentNode with
        //           {
        //               Location = sourcePath[i].Location
        //           }
        //         : nextParentNode;
        //     if (!TryGetVersioningNode(targetVersion.ToVersionString(), out var nextVersioningNode))
        //         throw new ArgumentException(
        //             "Couldn't get target versioning node with VIS version"
        //                 + targetVersion.ToVersionString()
        //         );
        //     if (
        //         nextVersioningNode.TryGetCodeChanges(nextParentNode.Code, out var nodeChanges)
        //         && nodeChanges?.FormerParent is not null
        //     )
        //     {
        //         targetNodes.Add(targetParentNode);
        //          THIS DOES NOT WORK
        //         if (sourceNode.Parents.Select(n => n.Code).Contains(nodeChanges?.FormerParent))
        //         {
        //             var parent = targetParentNode.Parents[0];
        //             targetNodes.Add(parent);
        //         }
        //     }
        //     else
        //     {
        //         targetNodes.Add(targetParentNode);
        //     }
        // }
        //

        // var newPath = new GmodPath(targetNodes, endNode);

        // return new GmodPath(targetNodes, endNode);

        // var newPathIndex = 0;
        // var newParents = new GmodNode[sourcePath.Length - 1];
        // GmodNode? newNode = null;
        // foreach (var (depth, node) in sourcePath.GetFullPath())
        // {
        //     var nextNode = await ConvertNode(sourceVersion, node, targetVersion);
        //     var targetNode = node.Location is not null
        //         ? nextNode with
        //           {
        //               Location = sourcePath[depth].Location
        //           }
        //         : nextNode;
        //
        //     if (depth < sourcePath.Length - 1)
        //         newParents[newPathIndex++] = targetNode;
        //     else
        //         newNode = targetNode;
        // }
        //
        // if (newNode is null)
        //     throw new InvalidOperationException("Invalid conversion");
        // return new GmodPath(newParents, newNode);
    }

    public GmodVersioningNode this[string key] => _versioningsMap[key];

    public bool TryGetVersioningNode(
        string visVersion,
        [MaybeNullWhen(false)] out GmodVersioningNode versioningNode
    ) => _versioningsMap.TryGetValue(visVersion, out versioningNode);

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
                versioningNodeDto.Value.PreviousCode,
                versioningNodeDto.Value.FormerParent
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
    string PreviousCode,
    string FormerParent
);
