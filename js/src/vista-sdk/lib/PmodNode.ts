import { GmodNode } from ".";

export class PmodNode {
    private _node: GmodNode;
    private _depth: number;

    public constructor(node: GmodNode, depth: number) {
        this._node = node;
        this._depth = depth;
    }

    public get node() {
        return this._node;
    }
    public get depth() {
        return this._depth;
    }

    public get code() {
        return this._node.code;
    }

    public addChild(child: PmodNode) {
        this._node.addChild(child.node);
    }

    public addParent(parent: PmodNode) {
        this._node.addParent(parent.node);
    }

    public withEmptyRelations() {
        return new PmodNode(this._node.withEmptyRelations(), this._depth);
    }

    public toString() {
        return this._node.toString();
    }
}
