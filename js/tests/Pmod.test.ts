import {
    Gmod,
    ImoNumber,
    LocalId,
    Pmod,
    PmodNode,
    VIS,
    VisVersion,
} from "../lib";
import * as testData from "../../testdata/PmodData.json";
import { TraversalHandlerResult } from "../lib/types/Gmod";

describe("Pmod", () => {
    const vis = VIS.instance;
    const version = VisVersion.v3_4a;
    const gmodPromise = vis.getGmod(version);
    const codebooksPromise = vis.getCodebooks(version);
    const locationsPromise = vis.getLocations(version);

    test("From LocalIds", async () => {
        const gmod = await gmodPromise;
        const codeBooks = await codebooksPromise;
        const locations = await locationsPromise;

        const localIds = testData.localIds.map((localIdStr) =>
            LocalId.parse(localIdStr, gmod, codeBooks, locations)
        );

        const maxDepth = Math.max(
            ...localIds
                .flatMap((l) => [l.primaryItem, l.secondaryItem])
                .map((p) => (p ? p.getFullPath().length - 1 : 0))
        );

        const pmod = Pmod.createFromLocalIds(VisVersion.v3_4a, localIds, {
            imoNumber: ImoNumber.create(1234567),
        });

        const cs1Instances = pmod.getNodesByCode("CS1");

        for (let i = 0; i < cs1Instances.length - 1; i++) {
            const currInstance = cs1Instances[i];
            const nextInstance = cs1Instances[i + 1];
            expect(currInstance).not.toBe(nextInstance);
        }

        expect(pmod.info?.imoNumber?.value).toEqual(1234567);
        expect(pmod.rootNode.toString()).toEqual("VE");
        expect(pmod.rootNode.node.children.length).toBeGreaterThan(0);
        expect(pmod.maxDepth).toEqual(maxDepth);
        expect(pmod.isValid).toBeTruthy();
    });

    // Traversal test to map to tree
    test("Traverse with skip", async () => {
        const gmod = await gmodPromise;
        const codeBooks = await codebooksPromise;
        const locations = await locationsPromise;

        const localIds = testData.localIds.map((localIdStr) =>
            LocalId.parse(localIdStr, gmod, codeBooks, locations)
        );

        const pmod = Pmod.createFromLocalIds(VisVersion.v3_4a, localIds, {
            imoNumber: ImoNumber.create(1234567),
        });

        type SomeOtherType = {
            key: string;
            code: string;
            children: SomeOtherType[];
            depth: number;
            skip: boolean;
            merge: boolean;
        };

        const context: {
            nodes: SomeOtherType[];
            parent?: SomeOtherType;
            depth: number;
        } = { nodes: [], depth: 0 };

        pmod.traverse(
            (parents, node, context) => {
                const key = parents
                    .concat(node)
                    .map((n) => n.toString())
                    .join("/");
                let parent =
                    parents.length > 0
                        ? parents[parents.length - 1]
                        : undefined;

                const newNode: SomeOtherType = {
                    key,
                    children: [],
                    depth: node.depth,
                    code: node.code,
                    merge: false,
                    skip: false,
                };

                // Add parent to tree
                if (!parent) {
                    context.nodes.push(newNode);
                    context.parent = newNode;
                    return TraversalHandlerResult.Continue;
                }
                if (!context.parent)
                    throw new Error("Unexpected state: missing parent");

                const skip = Gmod.isProductSelectionAssignment(
                    parent.node,
                    node.node
                );

                const merge = Gmod.isProductTypeAssignment(
                    parent.node,
                    node.node
                );

                newNode.skip = skip;
                newNode.merge = merge;

                if (newNode.skip || newNode.merge)
                    return TraversalHandlerResult.Continue;

                context.parent.children.push(newNode);
                context.parent = newNode;

                return TraversalHandlerResult.Continue;
            },
            {
                state: context,
            }
        );
        // const print = (n: SomeOtherType): string => {
        //     if (n.code === "VE") return n.children.map(print).join("\n");
        //     const indents = Array.from(Array(n.depth).keys())
        //         .map(() => "\t")
        //         .join();
        //     const merge = n.merge ? " m " : "";
        //     const skip = n.skip ? " s " : "";

        //     return (
        //         indents +
        //         n.code +
        //         merge +
        //         skip +
        //         "\n" +
        //         n.children.map(print).join("\n")
        //     );
        // };
        // console.log(context.nodes.map(print).join("\n"));
        expect(context.nodes.length).toEqual(1);
    });
});
