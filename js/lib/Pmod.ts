import { GmodNode, GmodPath, VisVersion } from ".";
import { LocalId } from "./LocalId";
import { LocalIdBuilder } from "./LocalId.Builder";
import { PmodNode } from "./PmodNode";
import { TraversalHandlerResult } from "./types/Gmod";
import {
    Parents,
    PmodInfo,
    TraversalContext,
    TraversalHandler,
    TraversalHandlerWithState,
} from "./types/Pmod";
import { isNullOrWhiteSpace } from "./util/util";

export class Pmod {
    public visVersion: VisVersion;
    private _info?: PmodInfo;
    private _rootNode: PmodNode;
    private _nodeMap: Map<string, PmodNode>;

    private constructor(
        visVersion: VisVersion,
        rootNode: PmodNode,
        nodeMap: Map<string, PmodNode>,
        info?: PmodInfo
    ) {
        this.visVersion = visVersion;
        this._info = info;
        this._rootNode = rootNode;
        this._nodeMap = nodeMap;
    }

    public static createFromPaths(
        visVersion: VisVersion,
        paths: GmodPath[],
        info?: PmodInfo
    ) {
        const nodeMap = new Map<string, PmodNode>();

        const addToNodeMap = (key: string, node: GmodNode, depth: number) => {
            const pmodNode = new PmodNode(node, depth)
                .withEmptyRelations()
                .withId(key);
            nodeMap.set(key, pmodNode);
        };

        for (const path of paths) {
            const fullPath = path.getFullPath();

            for (let i = 0; i < fullPath.length; i++) {
                const pathNode = fullPath[i].withEmptyRelations();
                if (i === 0 && pathNode.code !== "VE")
                    throw new Error("Root node is not VE");

                if (pathNode.code === "VE") {
                    if (!nodeMap.has("VE")) addToNodeMap("VE", pathNode, 0);
                    continue;
                }

                const nodeId = fullPath
                    .slice(0, i + 1)
                    .map((p) => p.toString())
                    .join("/");

                const parentId = fullPath
                    .slice(0, i)
                    .map((p) => p.toString())
                    .join("/");

                if (isNullOrWhiteSpace(parentId) || isNullOrWhiteSpace(nodeId))
                    throw new Error(
                        "Invalid generated pathId for node:" +
                            pathNode.toString()
                    );

                if (nodeMap.has(nodeId)) continue;

                addToNodeMap(nodeId, pathNode, i);

                const childNode = nodeMap.get(nodeId);
                const parentNode = nodeMap.get(parentId);

                if (!childNode)
                    throw new Error(
                        "Unable to get node: " + pathNode.toString()
                    );

                if (!parentNode)
                    throw new Error(
                        "Unable to get parent node for node: " +
                            pathNode.toString()
                    );

                childNode.addParent(parentNode);
                parentNode.addChild(childNode);
            }
        }

        const rootNode = nodeMap.get("VE");
        if (!rootNode) throw Error("Failed to get rootNode 'VE'");

        return new Pmod(visVersion, rootNode, nodeMap, info);
    }

    public static createFromLocalIds(
        visVersion: VisVersion,
        localIds: LocalId[] | LocalIdBuilder[],
        info?: PmodInfo
    ) {
        const paths = localIds
            .flatMap((localId) => [localId.primaryItem, localId.secondaryItem])
            .filter((l) => l) as GmodPath[];

        return this.createFromPaths(visVersion, paths, info);
    }

    public get info() {
        return this._info;
    }

    public get rootNode() {
        return this._rootNode;
    }

    public get isValid() {
        return !Array.from(this._nodeMap.values()).find((n) => !n.isValid);
    }

    public get maxDepth() {
        return Math.max(
            ...Array.from(this._nodeMap.values()).map((n) => n.depth)
        );
    }

    public get numNodes() {
        return this._nodeMap.size;
    }

    public getNodeByPath(path: GmodPath) {
        return this._nodeMap.get(path.toFullPathString());
    }

    public getNodesByCode(code: string) {
        return Array.from(this._nodeMap.values()).filter(
            (n) => n.node.code === code
        );
    }

    // Traversal
    public traverse<T>(
        handler: TraversalHandler | TraversalHandlerWithState<T>,
        params?: {
            fromPath?: GmodPath;
            state?: T;
        }
    ): boolean {
        const { fromPath = new GmodPath([], this._rootNode.node), state } =
            params || {};

        if (!state) {
            return this.traverseFromNodeWithState(
                handler as TraversalHandler,
                fromPath,
                (parents, node, handler) => handler(parents, node)
            );
        }

        return this.traverseFromNodeWithState(
            state,
            fromPath,
            handler as TraversalHandlerWithState<T>
        );
    }

    public traverseFromNodeWithState<T>(
        state: T,
        fromPath: GmodPath,
        handler: TraversalHandlerWithState<T>
    ): boolean {
        const parents = new Parents(fromPath);
        const context: TraversalContext<T> = {
            parents,
            handler,
            state,
        };

        const pmodNode = this.getNodeByPath(fromPath);
        if (!pmodNode) throw new Error("Start path not found in Pmod");
        return (
            this.traverseNode<T>(context, pmodNode) ===
            TraversalHandlerResult.Continue
        );
    }

    private traverseNode<T>(
        context: TraversalContext<T>,
        node: PmodNode
    ): TraversalHandlerResult {
        if (context.parents.has(node)) return TraversalHandlerResult.Continue;

        let result = context.handler(
            context.parents.asList,
            node,
            context.state
        );

        if (
            result === TraversalHandlerResult.Stop ||
            result === TraversalHandlerResult.SkipSubtree
        )
            return result;

        context.parents.push(node);

        for (const child of node.children) {
            result = this.traverseNode(context, child);
            if (result === TraversalHandlerResult.Stop) return result;
            else if (result === TraversalHandlerResult.SkipSubtree) continue;
        }

        context.parents.pop();
        return TraversalHandlerResult.Continue;
    }
}
