import { GmodPath } from "..";
import { TraversalHandlerResult } from "./Gmod";

export type TreeNode = {
    key: string;
    path: GmodPath;
    parent?: TreeNode;
    children: TreeNode[];
    mergedNode?: TreeNode;
};

export type TreeHandler = (node: TreeNode) => TraversalHandlerResult;

export type TreeHandlerWithState<T> = (
    node: TreeNode,
    state: T
) => TraversalHandlerResult;
