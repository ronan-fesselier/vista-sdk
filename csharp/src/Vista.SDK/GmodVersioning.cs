using System.Diagnostics.CodeAnalysis;

using Vista.SDK.Internal;

namespace Vista.SDK;

public sealed class GmodVersioning
{
    private readonly Dictionary<string, GmodVersioningNode> _versioningsMap = new();
    private readonly Func<VisVersion, Gmod> _gmod;

    internal GmodVersioning(GmodVersioningDto dto, Func<VisVersion, Gmod> gmod)
    {
        foreach (var versioningDto in dto.Items)
        {
            var visVersion = versioningDto.Key;
            var gmodVersioningNode = new GmodVersioningNode(versioningDto.Value);
            _versioningsMap.Add(visVersion, gmodVersioningNode);
        }
        _gmod = gmod;
    }

    public GmodNode ConvertNode(
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

        var gmod = _gmod(targetVersion);

        if (!gmod.TryGetNode(nextCode, out var targetNode))
            throw new ArgumentException("Couldn't get target node with code: " + nextCode);
        return targetNode;
    }

    public GmodPath ConvertPath(
        VisVersion sourceVersion,
        GmodPath sourcePath,
        VisVersion targetVersion
    )
    {
        // { "323.51/H362.1", "323.61/H362.1" }
        //Invalid gmod path - H362.1 not child of 323.61
        var endNode = ConvertNode(sourceVersion, sourcePath.Node, targetVersion);
        static bool OnlyOnePath(GmodNode node) =>
            node.Parents.Count == 1 && OnlyOnePath(node.Parents[0]);

        if (OnlyOnePath(endNode))
            return new GmodPath(
                endNode.Parents,
                ConvertNode(sourceVersion, endNode, targetVersion)
            );

        var targetNodes = new List<GmodNode>();

        // finds the first leaf node
        var (depth, leafNode) = sourcePath.GetFullPath().FirstOrDefault(n => n.Node.IsLeafNode);
        // . Edge case: There can be nodes between Leaf Node and endNode

        var targetNode = ConvertNode(sourceVersion, leafNode, targetVersion);

        var gmod = _gmod(targetVersion);
        gmod.Traverse(
            targetNodes,
            rootNode: targetNode,
            handler: (targetNodes, parents, node) =>
            {
                if (node.Code != endNode.Code)
                    return TraversalHandlerResult.Continue;

                targetNodes.AddRange(parents.Where(p => p.Code != targetNode.Code).ToList());

                while (targetNode.Parents.Count == 1)
                {
                    // Traversing upwards;
                    targetNodes.Insert(0, targetNode);
                    targetNode = targetNode.Parents[0];
                }

                // if (targetNode.Parents.Count > 1)
                // {
                //     var current = targetNode.Parents.FirstOrDefault(
                //         n => !n.IsProductGroupLevel && !n.Code.EndsWith("99")
                //     );
                //     if (current is null)
                //         throw new InvalidOperationException("Failed to get parent");
                //     targetNodes.Insert(0, current);
                //     targetNode = current.Parents[0];
                //     return TraversalHandlerResult.Continue;
                // }

                targetNodes.Insert(0, gmod.RootNode);

                return TraversalHandlerResult.Stop;
            }
        );

        return new GmodPath(targetNodes, endNode);

        // var current = leafNode.Location is not null ? targetNode with {Location = sourcePath[depth].Location} : targetNode;
        // var current = targetNode;
        // targetNodes.Add(current);
        // targetNodesTemp.Add(current);
        // while (current.Code != "VE")
        // {
        //     if (current.Parents.Count == 1)
        //     {
        //         current = current.Parents[0];
        //         targetNodes.Add(current);
        //         targetNodesTemp.Add(current);
        //         continue;
        //     }
        //
        //     var parentNode = current.Parents.FirstOrDefault(n => !n.IsProductGroupLevel);
        //     targetNodesTemp.Add(parentNode!);
        //
        //     for (int i = 0; i < current.Parents.Count; i++)
        //     {
        //         var parent = current.Parents[i];
        //         if (parent.IsProductGroupLevel)
        //             continue;
        //         current = parent;
        //         targetNodes.Add(current);
        //     }
        // }
        //
        // targetNodes.Reverse();
        // targetNodesTemp.Reverse();
        // var tempPath = new GmodPath(targetNodesTemp, endNode);
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
