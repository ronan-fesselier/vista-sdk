import * as testData from "../../testdata/PmodData.json";
import {
    Gmod,
    ImoNumber,
    LocalId,
    Pmod,
    PmodNode,
    TreeNode,
    VIS,
    VisVersion,
} from "../lib";
import { TraversalHandlerResult } from "../lib/types/Gmod";

// Used for testing
type VmodNode = {
    key: string;
    children: VmodNode[];
    depth: number;
    skip: boolean;
    merge: boolean;
    code: string;
};

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

    test("Traverse pmod from node", async () => {
        const gmod = await gmodPromise;
        const codeBooks = await codebooksPromise;
        const locations = await locationsPromise;

        const localIds = testData.localIds.map((localIdStr) =>
            LocalId.parse(localIdStr, gmod, codeBooks, locations)
        );

        const pmod = Pmod.createFromLocalIds(VisVersion.v3_4a, localIds, {
            imoNumber: ImoNumber.create(1234567),
        });

        const rootNodeCode = "411.1";

        const rootNode = pmod.getNodesByCode(rootNodeCode);
        const rootPath = gmod.tryParseFromFullPath(rootNode[0].id, locations);

        const context = {
            nodes: [] as VmodNode[],
            nodeMap: new Map<string, VmodNode>(
                rootNode[0].parents
                    .reverse()
                    .reduce<[string, VmodNode][]>((c, p, index, arr) => {
                        if (index === 0) {
                            c.push([
                                p.id,
                                {
                                    key: p.id,
                                    children: [],
                                    code: p.code,
                                    depth: 0,
                                    merge: false,
                                    skip: false,
                                },
                            ]);
                            return c;
                        }

                        c.unshift([
                            p.id,
                            {
                                key: p.id,
                                children: [c[index - 1][1]],
                                code: p.code,
                                depth: index,
                                merge: false,
                                skip: false,
                            },
                        ]);
                        return c;
                    }, [])
                    .reverse()
            ),
            iter: 0,
        };

        const createKey = (parents: PmodNode[], node: PmodNode) =>
            parents
                .concat(node)
                .map((n) => n.toString())
                .join("/");

        pmod.traverse(
            (parents, node, context) => {
                const key = createKey(parents, node);

                if (context.nodeMap.has(key))
                    return TraversalHandlerResult.Continue;

                const vmodNode: VmodNode = {
                    key,
                    children: [],
                    depth: node.depth,
                    code: node.code,
                    merge: false,
                    skip: false,
                };

                context.nodeMap.set(key, vmodNode);
                if (context.nodes.length === 0) {
                    context.nodes.push(vmodNode);
                    return TraversalHandlerResult.Continue;
                }

                let parentKey = createKey(
                    parents,
                    parents.splice(parents.length - 1, 1)[0]
                );

                let vmodParent = context.nodeMap.get(parentKey);
                if (!vmodParent)
                    throw new Error("Unexpected state: should find parent");

                vmodParent.children.push(vmodNode);
                return TraversalHandlerResult.Continue;
            },
            { state: context, fromPath: rootPath }
        );

        expect(context.nodes[0].code).toEqual(rootNodeCode);
    });

    test("Tree", async () => {
        const gmod = await gmodPromise;
        const codeBooks = await codebooksPromise;
        const locations = await locationsPromise;

        const localIds = testData.localIds.map((localIdStr) =>
            LocalId.parse(localIdStr, gmod, codeBooks, locations)
        );

        const pmod = Pmod.createFromLocalIds(VisVersion.v3_4a, localIds, {
            imoNumber: ImoNumber.create(1234567),
        });

        const rootNode = pmod.getNodeByPath(
            gmod.parseFromFullPath(
                "VE/400a/410/411/411i/411.1/CS1/C101",
                locations
            )
        );

        const rootPath = rootNode!.path;

        const nodes = pmod.getVisualizableTreeNodes(
            (node, _) => {
                if (node.path.node.code === "C101.322") {
                    expect(node.parent).not.toBeFalsy();
                    expect(node.parent?.path.node.code).toBe("C101.32");
                }
                return TraversalHandlerResult.Continue;
            },
            {
                fromPath: rootPath,
            }
        );

        // const resolveMergedNodes = (n: TreeNode) => {
        //     const items = [n.path.node.toString()];

        //     if (n.mergedNode) items.unshift(n.mergedNode.path.node.toString());

        //     return items.join(" | ");
        // };

        // const print = (n: TreeNode, depth = 0): string => {
        //     const indents = Array.from(Array(depth).keys())
        //         .map(() => "\t")
        //         .join();

        //     return (
        //         indents +
        //         resolveMergedNodes(n) +
        //         "\n" +
        //         n.children.map((n) => print(n, depth + 1)).join("\n")
        //     );
        // };

        // console.log(nodes.map(print).join("\n"));
    });
});
