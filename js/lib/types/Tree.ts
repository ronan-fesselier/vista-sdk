import { GmodPath } from "..";

export type TreeNode = {
    key: string;
    path: GmodPath;
    parent?: TreeNode;
    children: TreeNode[];
    mergedNode?: TreeNode;
};

export type TreeHandler = (node: TreeNode) => void;

export type TreeHandlerWithState<T> = (node: TreeNode, state: T) => void;
