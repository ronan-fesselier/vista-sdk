import { VisVersion } from "..";
import { VIS } from "../VIS";

describe("Gmod", () => {
    const vis = VIS.instance;
    const version = VisVersion.v3_4a;
    const gmodPromise = vis.getGmod(version);

    test("Gmod loads", async () => {
        const gmod = await gmodPromise;

        expect(gmod).toBeTruthy();
        expect(gmod.tryGetNode("400a")).toBeTruthy();
    });

    test("Gmod node equality", async () => {
        const gmod = await gmodPromise;

        const node1 = gmod.getNode("400a");
        const node2 = gmod.getNode("400a");

        expect(node1).toEqual(node2);
        expect(node1).toBe(node2);

        const node3 = node2.withLocation("1");
        expect(node1).not.toEqual(node3);
        expect(node1).not.toBe(node3);

        const node4 = node2.clone();
        expect(node1).toEqual(node4);
        expect(node1).not.toBe(node4);
    });

    test("Gmod rootnode children", async () => {
        const gmod = await gmodPromise;

        const node = gmod.rootNode;

        expect(node.children).not.toHaveLength(0);
    });
});
