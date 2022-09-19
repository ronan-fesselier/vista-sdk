import { VIS, VisVersion } from "../../lib";
import { DataChannelId } from "../../lib/transport/domain";

describe("DataChannelId", () => {
    const vis = VIS.instance;
    const visVersion = VisVersion.v3_4a;
    const gmodPromise = vis.getGmod(visVersion);
    const codebooksPromise = vis.getCodebooks(visVersion);

    const validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/meta/qty-temperature/cnt-cooling.water/pos-inlet";
    const invalidLocalIdStr =
        "/invalid/dnv-v2/vis-3-4a/411.1/C101.63/S206/meta/qty-temperature/cnt-cooling.water/pos-inlet";
    const otherString = "myVessel";

    test("ParseValid LocalId", async () => {
        const gmod = await gmodPromise;
        const codebooks = await codebooksPromise;

        const dataChannelId = DataChannelId.parse(
            validLocalIdStr,
            gmod,
            codebooks
        );

        expect(dataChannelId.localId).toBeTruthy();
        expect(dataChannelId.localId!.toString()).toEqual(validLocalIdStr);
    });

    test("ParseInvalidValid LocalId", async () => {
        const gmod = await gmodPromise;
        const codebooks = await codebooksPromise;

        const dataChannelId = DataChannelId.parse(
            invalidLocalIdStr,
            gmod,
            codebooks
        );

        expect(dataChannelId.localId).toBeUndefined();
        expect(dataChannelId.shortId).toBeTruthy();
        expect(dataChannelId.shortId).toEqual(invalidLocalIdStr);
    });

    test("ParseValid Other", async () => {
        const gmod = await gmodPromise;
        const codebooks = await codebooksPromise;

        const dataChannelId = DataChannelId.parse(otherString, gmod, codebooks);

        expect(dataChannelId.shortId).toBeTruthy();
        expect(dataChannelId.shortId).toEqual(otherString);
    });
});
