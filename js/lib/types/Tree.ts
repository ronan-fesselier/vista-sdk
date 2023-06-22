import { GmodPath } from "..";
import { TraversalHandlerResult } from "./Gmod";

export type TreeNode<T = {}> = T &
    Record<"children", TreeNode<T>[]> & {
        parent?: TreeNode<T>;
        mergedNode?: TreeNode<T>;
        key: string;
        path: GmodPath;
    };

export type TreeHandler<TNode> = (
    node: TreeNode<TNode>
) => TraversalHandlerResult;

export type TreeHandlerWithState<TState, TNode> = (
    node: TreeNode<TNode>,
    state: TState
) => TraversalHandlerResult;

export type FormatNode<TNode> = (node: TreeNode<{}>) => TNode;
