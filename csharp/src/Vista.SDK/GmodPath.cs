using System.Collections;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Text;
using Vista.SDK.Internal;

namespace Vista.SDK;

public sealed record GmodInstantiableSet
{
    private readonly List<GmodNode> _nodes;

    public IReadOnlyList<GmodNode> Nodes => _nodes;

    internal GmodInstantiableSet(List<GmodNode> nodes)
    {
        if (nodes.Count == 0)
            throw new Exception("GmodInstantiableSet cant be empty");
        if (nodes.Any(n => !n.IsInstantiatable))
            throw new Exception("GmodInstantiableSet nodes must be instantiatable");
        if (nodes.Select(n => n.Location).Distinct().Count() != 1)
            throw new Exception();

        _nodes = nodes;
    }

    public Location? Location
    {
        get => _nodes[0].Location;
        set
        {
            for (int i = 0; i < _nodes.Count; i++)
            {
                if (value is null)
                    _nodes[i] = _nodes[i].WithoutLocation();
                else
                    _nodes[i] = _nodes[i].WithLocation(value.Value);
            }
        }
    }

    public override string ToString() => string.Join("/", _nodes.Where(n => n.IsLeafNode).Select(n => n.ToString()));
}

public sealed record GmodPath
{
    public readonly List<GmodNode> _parents;
    public IReadOnlyList<GmodNode> Parents => _parents;

    public GmodNode Node { get; private set; }

    public int Length => _parents.Count + 1;

    public bool IsMappable => Node.IsMappable;

    public GmodNode this[int depth]
    {
        get
        {
            if (depth < 0)
                throw new IndexOutOfRangeException("Index out of range for GmodPath indexer");
            if (depth > _parents.Count)
                throw new IndexOutOfRangeException("Index out of range for GmodPath indexer");

            return depth < _parents.Count ? _parents[depth] : Node;
        }
    }

    // public IReadOnlyList<GmodInstantiableSet> InstantiatiableSets
    // {
    //     get
    //     {
    //         var result = new List<GmodInstantiableSet>();
    //         IterateGmodInstantiableSets(result);
    //         return result;
    //     }
    // }

    private void IterateGmodInstantiableSets(List<GmodInstantiableSet>? result = null)
    {
        var currentParentStart = -1;
        for (int i = 0; i < Length; i++)
        {
            var node = this[i];
            var isParent = Gmod.PotentialParentScopeTypes.Contains(node.Metadata.Type);
            if (currentParentStart == -1)
            {
                if (isParent)
                    currentParentStart = i;
                if (node.IsInstantiatable && result is not null)
                    result.Add(new GmodInstantiableSet(new List<GmodNode> { node }));
            }
            else
            {
                if (isParent)
                {
                    var nodes = result is not null ? new List<GmodNode>() : null;
                    if (currentParentStart + 1 == i)
                    {
                        if (node.IsInstantiatable)
                            nodes?.Add(node);
                    }
                    else
                    {
                        GmodNode? locationNode = null;
                        for (var j = currentParentStart + 1; j <= i; j++)
                        {
                            var setNode = this[j];
                            if (!setNode.IsInstantiatable)
                                continue;

                            if (locationNode is null)
                            {
                                if (setNode.Location is not null)
                                    locationNode = setNode;
                            }
                            else
                            {
                                Debug.Assert(
                                    locationNode.Location is not null,
                                    "We only set locationNode for this case"
                                );
                                if (setNode.Location is not null && setNode.Location != locationNode.Location)
                                {
                                    throw new Exception(
                                        $"Tried to propagate location, but location was different: {locationNode}, {setNode}"
                                    );
                                }
                                if (setNode.Location != locationNode.Location)
                                {
                                    setNode = setNode.WithLocation(locationNode!.Location!.Value);
                                    if (j < _parents.Count)
                                        _parents[j] = setNode;
                                    else
                                        Node = setNode;
                                }
                            }
                            nodes?.Add(setNode);
                        }
                    }
                    currentParentStart = i;
                    if (result is not null && nodes is not null && nodes.Count > 0)
                        result.Add(new GmodInstantiableSet(nodes));
                }
            }
        }
    }

    internal GmodPath(List<GmodNode> parents, GmodNode node, bool skipVerify = true)
    {
        if (!skipVerify)
        {
            if (parents.Count == 0)
                throw new ArgumentException($"Invalid gmod path - no parents, and {node.Code} is not the root of gmod");
            if (parents.Count > 0 && !parents[0].IsRoot)
                throw new ArgumentException(
                    $"Invalid gmod path - first parent should be root of gmod (VE), but was {parents[0].Code}"
                );

#if NETCOREAPP3_1_OR_GREATER
            var set = new HashSet<string>(parents.Count);
#else
            var set = new HashSet<string>();
#endif
            set.Add("VE");

            for (int i = 0; i < parents.Count; i++)
            {
                var parent = parents[i];
                var nextIndex = i + 1;
                var child = nextIndex < parents.Count ? parents[nextIndex] : node;
                if (!parent.IsChild(child))
                    throw new ArgumentException($"Invalid gmod path - {child.Code} not child of {parent.Code}");

                if (!set.Add(child.Code))
                    throw new ArgumentException($"Recursion in gmod path argument for code: {child.Code}");
            }
        }

        _parents = parents;
        Node = node;
    }

    public static bool IsValid(IReadOnlyList<GmodNode> parents, GmodNode node) => IsValid(parents, node, out _);

    internal static bool IsValid(IReadOnlyList<GmodNode> parents, GmodNode node, out int missingLinkAt)
    {
        missingLinkAt = -1;

        if (parents.Count == 0)
            return false;

        if (parents.Count > 0 && !parents[0].IsRoot)
            return false;

#if NETCOREAPP3_1_OR_GREATER
        var set = new HashSet<string>(parents.Count);
#else
        var set = new HashSet<string>();
#endif
        set.Add("VE");

        for (int i = 0; i < parents.Count; i++)
        {
            var parent = parents[i];
            var nextIndex = i + 1;
            var child = nextIndex < parents.Count ? parents[nextIndex] : node;
            if (!parent.IsChild(child))
            {
                missingLinkAt = i;
                return false;
            }

            if (!set.Add(child.Code))
                return false;
        }
        return true;
    }

    public GmodPath(List<GmodNode> parents, GmodNode node)
        : this(parents, node, false) { }

    public GmodPath WithoutLocations() =>
        new GmodPath(_parents.Select(n => n.WithoutLocation()).ToList(), Node.WithoutLocation());

    public override string ToString()
    {
        using var lease = StringBuilderPool.Get();
        ToString(lease.Builder);
        return lease.ToString();
    }

    public void ToString(StringBuilder builder, char separator = '/')
    {
        foreach (var parent in _parents)
        {
            if (!Gmod.IsLeafNode(parent.Metadata))
                continue;

            parent.ToString(builder);
            builder.Append(separator);
        }

        Node.ToString(builder);
    }

    public string ToFullPathString()
    {
        using var lease = StringBuilderPool.Get();
        ToFullPathString(lease.Builder);
        return lease.ToString();
    }

    public void ToFullPathString(StringBuilder builder)
    {
        foreach (var (depth, pathNode) in GetFullPath())
        {
            pathNode.ToString(builder);

            if (depth != (Length - 1))
                builder.Append('/');
        }
    }

    public string ToStringDump()
    {
        using var lease = StringBuilderPool.Get();
        ToStringDump(lease.Builder);
        return lease.ToString();
    }

    public void ToStringDump(StringBuilder builder)
    {
        foreach (var (depth, pathNode) in GetFullPath())
        {
            if (depth == 0)
                continue;

            if (depth != 1)
                builder.Append(" | ");

            builder.Append(pathNode.Code);

            if (!string.IsNullOrWhiteSpace(pathNode.Metadata.Name))
            {
                builder.Append("/N:");
                builder.Append(pathNode.Metadata.Name);
            }

            if (!string.IsNullOrWhiteSpace(pathNode.Metadata.CommonName))
            {
                builder.Append("/CN:");
                builder.Append(pathNode.Metadata.CommonName);
            }

            var normalAssignmentName = GetNormalAssignmentName(depth);
            if (!string.IsNullOrWhiteSpace(normalAssignmentName))
            {
                builder.Append("/NAN:");
                builder.Append(normalAssignmentName);
            }
        }
    }

    public bool Equals(GmodPath? other)
    {
        if (other is null)
            return false;

        if (_parents.Count != other._parents.Count)
            return false;

        for (int i = 0; i < _parents.Count; i++)
        {
            if (_parents[i] != other._parents[i])
                return false;
        }

        return Node == other.Node;
    }

    public override int GetHashCode()
    {
        var hashCode = new HashCode();
        for (int i = 0; i < _parents.Count; i++)
            hashCode.Add(_parents[i]);

        hashCode.Add(Node);
        return hashCode.ToHashCode();
    }

    public Enumerator GetFullPath() => new Enumerator(this);

    public Enumerator GetFullPathFrom(int fromDepth) => new Enumerator(this, fromDepth);

    public struct Enumerator : IEnumerable<(int Depth, GmodNode Node)>, IEnumerator<(int Depth, GmodNode Node)>
    {
        private readonly GmodPath _path;

        public void Reset() { }

        object IEnumerator.Current => Current;

        public (int Depth, GmodNode Node) Current { get; private set; }

        public Enumerator(GmodPath path, int? fromDepth = null)
        {
            _path = path;
            Current = (-1, null!);
            if (fromDepth is not null)
            {
                if (fromDepth < 0 || fromDepth > _path._parents.Count)
                    throw new ArgumentOutOfRangeException(nameof(fromDepth));

                Current = (fromDepth.Value - 1, fromDepth == 0 ? null! : _path[fromDepth.Value - 1]);
            }
        }

        public bool MoveNext()
        {
            if (Current.Depth < _path._parents.Count)
            {
                Current =
                    Current.Depth == _path._parents.Count - 1
                        ? (Current.Depth + 1, _path.Node)
                        : (Current.Depth + 1, _path._parents[Current.Depth + 1]);
                return true;
            }

            return false;
        }

        IEnumerator IEnumerable.GetEnumerator() => this;

        public IEnumerator<(int Depth, GmodNode Node)> GetEnumerator() => this;

        public void Dispose() { }
    }

    public string? GetNormalAssignmentName(int nodeDepth)
    {
        var node = this[nodeDepth];
        var normalAssignmentNames = node.Metadata.NormalAssignmentNames;
        if (normalAssignmentNames is null || normalAssignmentNames.Count == 0)
            return null;

        for (int i = Length - 1; i >= 0; i--)
        {
            var child = this[i];
            if (normalAssignmentNames.TryGetValue(child.Code, out var name))
                return name;
        }

        return null;
    }

    public IEnumerable<(int Depth, string Name)> GetCommonNames()
    {
        foreach (var (depth, node) in this.GetFullPath())
        {
            var isTarget = depth == _parents.Count;
            if (!(node.IsLeafNode || isTarget) || !node.IsFunctionNode)
                continue;

            var name = node.Metadata.CommonName ?? node.Metadata.Name;
            var normalAssignmentNames = node.Metadata.NormalAssignmentNames;

            if (normalAssignmentNames is not null)
            {
                {
                    if (normalAssignmentNames.TryGetValue(Node.Code, out var assignment))
                        name = assignment;
                }
                for (int i = _parents.Count - 1; i >= depth; i--)
                {
                    if (!normalAssignmentNames.TryGetValue(_parents[i].Code, out var assignment))
                        continue;

                    name = assignment;
                }
            }

            yield return (depth, name);
        }
    }

    private readonly record struct PathNode(string Code, Location? Location = null);

    private sealed record ParseContext(Queue<PathNode> Parts)
    {
        public PathNode ToFind;
        public Dictionary<string, Location>? Locations;
        public GmodPath? Path;
    }

    public static GmodPath Parse(string item, VisVersion visVersion)
    {
        if (!TryParse(item, visVersion, out var path))
            throw new ArgumentException("Couldnt parse path");

        return path;
    }

    public static bool TryParse(string? item, VisVersion visVersion, [NotNullWhen(true)] out GmodPath? path)
    {
        var gmod = VIS.Instance.GetGmod(visVersion);
        var locations = VIS.Instance.GetLocations(visVersion);
        return TryParse(item, gmod, locations, out path);
    }

    public static GmodPath Parse(string item, Gmod gmod, Locations locations)
    {
        if (!TryParse(item, gmod, locations, out var path))
            throw new ArgumentException("Couldnt parse path");

        return path;
    }

    public static bool TryParse(string? item, Gmod gmod, Locations locations, [NotNullWhen(true)] out GmodPath? path)
    {
        if (gmod.VisVersion != locations.VisVersion)
            throw new ArgumentException("Got different VIS versions for Gmod and Locations arguments");

        path = null;
        if (string.IsNullOrWhiteSpace(item))
            return false;

        item = item!.Trim().TrimStart('/');

        var parts = new Queue<PathNode>();
        foreach (var partStr in item.Split('/'))
        {
            if (partStr.Contains('-'))
            {
                var split = partStr.Split('-');
                if (!locations.TryParse(split[1], out var location))
                    return false;
                parts.Enqueue(new PathNode(split[0], location));
            }
            else
            {
                parts.Enqueue(new PathNode(partStr));
            }
        }

        if (parts.Count == 0)
            return false;
        if (parts.Any(p => string.IsNullOrWhiteSpace(p.Code)))
            return false;

        var toFind = parts.Dequeue();
        if (!gmod.TryGetNode(toFind.Code, out var baseNode))
            return false;

        var context = new ParseContext(parts) { ToFind = toFind };

        gmod.Traverse(
            context,
            baseNode,
            (context, parents, current) =>
            {
                ref var toFind = ref context.ToFind;
                var found = current.Code == toFind.Code;

                if (!found && Gmod.IsLeafNode(current.Metadata))
                    return TraversalHandlerResult.SkipSubtree;

                if (!found)
                    return TraversalHandlerResult.Continue;

                if (toFind.Location is not null)
                {
                    context.Locations ??= new();
                    context.Locations.Add(toFind.Code, toFind.Location.Value);
                }

                if (context.Parts.Count > 0)
                {
                    toFind = context.Parts.Dequeue();
                    return TraversalHandlerResult.Continue;
                }

                var pathParents = new List<GmodNode>(parents.Count + 1);
                foreach (var parent in parents)
                {
                    if (context.Locations?.TryGetValue(parent.Code, out var location) ?? false)
                        pathParents.Add(parent.WithLocation(location));
                    else
                        pathParents.Add(parent);
                }
                var endNode = toFind.Location is not null ? current.WithLocation(toFind.Location) : current;

                var startNode =
                    pathParents.Count > 0 && pathParents[0].Parents.Count == 1
                        ? pathParents[0].Parents[0]
                        : endNode.Parents.Count == 1
                            ? endNode.Parents[0]
                            : null;

                if (startNode is null || startNode.Parents.Count > 1)
                    return TraversalHandlerResult.Stop;

                while (startNode.Parents.Count == 1)
                {
                    pathParents.Insert(0, startNode);
                    startNode = startNode.Parents[0];
                    if (startNode.Parents.Count > 1)
                        return TraversalHandlerResult.Stop;
                }

                pathParents.Insert(0, gmod.RootNode);

                context.Path = new GmodPath(pathParents, endNode);
                return TraversalHandlerResult.Stop;
            }
        );

        if (context.Path is null)
            return false;

        path = context.Path;
        return true;
    }

    public static GmodPath ParseFullPath(string pathStr, VisVersion visVersion)
    {
        if (!TryParseFullPath(pathStr, visVersion, out var path))
            throw new ArgumentException("Couldnt parse path");

        return path;
    }

    public static bool TryParseFullPath(
        string? pathStr,
        VisVersion visVersion,
        [MaybeNullWhen(false)] out GmodPath path
    )
    {
        var vis = VIS.Instance;
        var gmod = vis.GetGmod(visVersion);
        var locations = vis.GetLocations(visVersion);
        return TryParseFullPathInternal(pathStr.AsSpan(), gmod, locations, out path);
    }

    public static bool TryParseFullPath(
        ReadOnlySpan<char> pathStr,
        VisVersion visVersion,
        [MaybeNullWhen(false)] out GmodPath path
    )
    {
        var vis = VIS.Instance;
        var gmod = vis.GetGmod(visVersion);
        var locations = vis.GetLocations(visVersion);
        return TryParseFullPathInternal(pathStr, gmod, locations, out path);
    }

    public static bool TryParseFullPath(
        string? pathStr,
        Gmod gmod,
        Locations locations,
        [MaybeNullWhen(false)] out GmodPath path
    ) => TryParseFullPathInternal(pathStr.AsSpan(), gmod, locations, out path);

    public static bool TryParseFullPath(
        ReadOnlySpan<char> pathStr,
        Gmod gmod,
        Locations locations,
        [MaybeNullWhen(false)] out GmodPath path
    ) => TryParseFullPathInternal(pathStr, gmod, locations, out path);

    private static bool TryParseFullPathInternal(
        ReadOnlySpan<char> span,
        Gmod gmod,
        Locations locations,
        [MaybeNullWhen(false)] out GmodPath path
    )
    {
        Debug.Assert(gmod.VisVersion == locations.VisVersion);

        path = default;
        if (span.IsEmpty || span.IsWhiteSpace())
            return false;

        if (!span.StartsWith(gmod.RootNode.Code.AsSpan(), StringComparison.Ordinal))
            return false;

        var nodes = new List<GmodNode>(span.Length / 3);
        foreach (ReadOnlySpan<char> nodeStr in span.Split('/'))
        {
            var dashIndex = nodeStr.IndexOf('-');

            GmodNode? node;
            if (dashIndex == -1)
            {
                if (!gmod.TryGetNode(nodeStr, out node))
                    return false;
            }
            else
            {
                if (!gmod.TryGetNode(nodeStr.Slice(0, dashIndex), out node))
                    return false;

                var locationStr = nodeStr.Slice(dashIndex + 1);
                if (!locations.TryParse(locationStr, out var location))
                    return false;

                node = node.WithLocation(location);
            }

            nodes.Add(node);
        }

        if (nodes.Count == 0)
            return false;

        var endNode = nodes[nodes.Count - 1];
        nodes.RemoveAt(nodes.Count - 1);
        if (!IsValid(nodes, endNode))
            return false;

        path = new GmodPath(nodes, endNode, skipVerify: true);
        return true;
    }
}
