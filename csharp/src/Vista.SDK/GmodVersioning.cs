using System.Diagnostics;
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

    public bool HasChangedForNext(VisVersion version, GmodNode node)
    {
        if (!TryGetVersioningNode(version.ToVersionString(), out var versioningNode))
            throw new ArgumentException(
                "Couldn't get versioning node with VIS version" + version.ToVersionString()
            );

        return versioningNode.TryGetCodeChanges(node.Code, out _);
    }

    private GmodVersioningNodeChanges? GetNodeChange(VisVersion version, GmodNode node)
    {
        if (!TryGetVersioningNode(version.ToVersionString(), out var versioningNode))
            throw new ArgumentException(
                "Couldn't get versioning node with VIS version" + version.ToVersionString()
            );

        return versioningNode.TryGetCodeChanges(node.Code, out var nodeChanges)
          ? nodeChanges
          : null;
    }

    public GmodPath ConvertPath(
        VisVersion sourceVersion,
        GmodPath sourcePath,
        VisVersion targetVersion
    )
    {
        // { "323.51/H362.1", "323.61/H362.1" }
        //Invalid gmod path - H362.1 not child of 323.61
        var targetEndNode = ConvertNode(sourceVersion, sourcePath.Node, targetVersion);

        static bool OnlyOnePathToRoot(GmodNode node) =>
            node.IsRoot || (node.Parents.Count == 1 && OnlyOnePathToRoot(node.Parents[0]));

        // If there is only 1 path to root from the converted node, we can just exit here.
        if (OnlyOnePathToRoot(targetEndNode))
        {
            static void GetPath(GmodNode parent, List<GmodNode> path)
            {
                path.Insert(0, parent); // Really inefficient..
                if (parent.IsRoot)
                    return;
                GetPath(parent.Parents[0], path);
            }
            var parents = new List<GmodNode>();
            GetPath(targetEndNode.Parents[0], parents);
            return new GmodPath(parents, targetEndNode);
        }

        var sourceToTargetMapping = sourcePath
            .GetFullPath()
            .Select(
                t =>
                    (
                        SourceNode: t.Node,
                        TargetNode: ConvertNode(sourceVersion, t.Node, targetVersion)
                    )
            )
            .ToDictionary(t => t.SourceNode, t => t.TargetNode);

        if (GmodPath.IsValid(sourceToTargetMapping.Values))
            return new GmodPath(sourceToTargetMapping.Values);

        var targetToSourceMapping = sourceToTargetMapping.ToDictionary(
            kvp => kvp.Value,
            kvp => kvp.Key
        );

        var sourceGmod = _gmod(sourceVersion);
        var targetGmod = _gmod(targetVersion);

#if DEBUG
        var allSourceNodes = sourcePath.GetFullPath().Select(t => t.Node).ToArray();
        var allTargetNodes = sourcePath
            .GetFullPath()
            .Select(t => ConvertNode(sourceVersion, t.Node, targetVersion))
            .ToArray();
#endif

        var targetFormerParents = allTargetNodes
            .Select(
                n =>
                {
                    var nodeChanges = GetNodeChange(targetVersion, n);
                    if (nodeChanges?.FormerParent is null)
                        return null;

                    if (
                        !sourceToTargetMapping.TryGetValue(
                            sourceGmod[nodeChanges.FormerParent],
                            out var targetFormerParent
                        )
                    )
                        return null;

                    return targetFormerParent;
                }
            )
            .Where(n => n is not null)
            .ToArray();
        var nonChangedNodes = sourcePath
            .GetFullPath()
            .Where(n => !HasChangedForNext(sourceVersion, n.Node))
            .ToArray();
        var changedNodes = targetToSourceMapping
            .Where(kvp => kvp.Key.Code != kvp.Value.Code)
            .Select(kvp => kvp.Key)
            .ToArray();

        var possiblePaths = new List<GmodPath>();

        // finds the first leaf node
        // we want the leaf node since that is the first part of the stringified path.
        var (_, sourceBaseNode) = sourcePath.GetFullPath().FirstOrDefault(n => n.Node.IsLeafNode);
        // Edge case: there can be multiple paths from base node (first leaf) to end node.

        var targetBaseNode = ConvertNode(sourceVersion, sourceBaseNode, targetVersion);
        // Edge case: source base node converted to target node may not be a base (first leaf) node anymore.

        Debug.Assert(sourceBaseNode.Parents.Count == 1);
        var olderSourceNode = sourceBaseNode.Parents[0];

        // First, find the topmost node that has only 1 path to root.
        while (!OnlyOnePathToRoot(targetBaseNode))
        {
            var nodeChanges = GetNodeChange(targetVersion, targetBaseNode);
            if (
                nodeChanges?.FormerParent is not null
                && sourceToTargetMapping.TryGetValue(
                    sourceGmod[nodeChanges.FormerParent],
                    out var targetFormerParent
                )
                && targetGmod.PathExistsBetween(targetFormerParent, targetBaseNode)
            )
            {
                targetBaseNode = targetFormerParent;
                continue;
            }

            targetBaseNode = sourceToTargetMapping[olderSourceNode];
            Debug.Assert(olderSourceNode.Parents.Count == 1);
            olderSourceNode = olderSourceNode.Parents[0]; // Should only be 1 here.
        }

        // Now find the topmost node that is a leaf (which will be the base from which we traverse to find the target end node)
        var tmpTargetBaseNode = targetBaseNode;
        while (!targetBaseNode.IsLeafNode)
        {
            if (targetBaseNode.IsRoot)
            {
                targetBaseNode = tmpTargetBaseNode;
                break;
            }
            Debug.Assert(targetBaseNode.Parents.Count == 1);
            targetBaseNode = targetBaseNode.Parents[0]; // Should only be 1 here now for target too.
        }

        // We didn't find the base node upwards, so lets go downwards 1 level.
        // TODO: is this enough? What if the target base node is 2 levels deep?
        while (!targetBaseNode.IsLeafNode)
        {
            var possibleTargetLeafs = new List<GmodNode>();
            foreach (var childTargetNode in targetBaseNode.Children)
            {
                if (!targetGmod.PathExistsBetween(childTargetNode, targetEndNode))
                    continue;

                if (childTargetNode.IsLeafNode)
                    possibleTargetLeafs.Add(childTargetNode);
            }

            if (possibleTargetLeafs.Count > 0)
            {
                Debug.Assert(possibleTargetLeafs.Count == 1);
                targetBaseNode = possibleTargetLeafs[0];
                break;
            }
        }

        targetBaseNode = targetFormerParents[0]!;

        // Target base node is now the topmost leaf, with only 1 path to VE root.
        // So lets find the possible paths from the base node to the target end node.
        targetGmod.Traverse(
            possiblePaths,
            rootNode: targetBaseNode,
            handler: (possiblePaths, parents, node) =>
            {
                if (!targetFormerParents.All(p => parents.Any(p2 => p2.Code == p!.Code)))
                    return TraversalHandlerResult.Continue;

                if (node.Code != targetEndNode.Code)
                    return TraversalHandlerResult.Continue;

                var targetParents = new List<GmodNode>(parents.Count);
                targetParents.AddRange(parents.Where(p => p.Code != targetBaseNode.Code).ToList());

                var currentTargetBaseNode = targetBaseNode;
                while (currentTargetBaseNode.Parents.Count == 1)
                {
                    // Traversing upwards to get to VE, since we until now have traversed from first leaf node.
                    targetParents.Insert(0, currentTargetBaseNode);
                    currentTargetBaseNode = currentTargetBaseNode.Parents[0];
                }

                targetParents.Insert(0, targetGmod.RootNode);

                possiblePaths.Add(new GmodPath(targetParents, node));
                return TraversalHandlerResult.Continue;
            }
        );

        // We now have a list of nodes from base to end.
        // We need to rank them somehow/choose the correct one.
        // TODO: expect only 1 top ranked candidate?
        // TODO: is this ranking always accurate? Can there be a deeper, but wrong path that gets more points?
        var rankedPossiblePaths = possiblePaths
            .Select(
                p =>
                    (
                        Score: p.GetFullPath()
                            .Sum(n => (targetToSourceMapping.ContainsKey(n.Node) ? 1 : 0)),
                        Path: p
                    )
            )
            .OrderByDescending(t => t.Score)
            .Where(
                t => changedNodes.All(n => t.Path.GetFullPath().Any(t2 => t2.Node.Code == n.Code))
            )
            .ToArray();

        Debug.Assert(rankedPossiblePaths.Length > 0, "We should atleast have 1 possible path");
        var top = rankedPossiblePaths[0];
        Debug.Assert(
            rankedPossiblePaths.Count(p => p.Score == top.Score) == 1,
            "Multiple possible paths with the same score"
        );

        return top.Path;
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
