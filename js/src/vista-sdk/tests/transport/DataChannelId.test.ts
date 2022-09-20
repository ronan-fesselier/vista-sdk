import { DataChannelId } from "../../lib/transport/domain";

describe("DataChannelId", () => {
    const validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/meta/qty-temperature/cnt-cooling.water/pos-inlet";
    const invalidLocalIdStr =
        "/invalid/dnv-v2/vis-3-4a/411.1/C101.63/S206/meta/qty-temperature/cnt-cooling.water/pos-inlet";
    const otherString = "myVessel";

    test("ParseValid LocalId", async () => {
        const dataChannelId = await DataChannelId.parseAsync(validLocalIdStr);

        expect(dataChannelId.localId).toBeTruthy();
        expect(dataChannelId.localId!.toString()).toEqual(validLocalIdStr);
    });

    test("ParseInvalidValid LocalId", async () => {
        const dataChannelId = await DataChannelId.parseAsync(invalidLocalIdStr);

        expect(dataChannelId.localId).toBeUndefined();
        expect(dataChannelId.shortId).toBeTruthy();
        expect(dataChannelId.shortId).toEqual(invalidLocalIdStr);
    });

    test("ParseValid Other", async () => {
        const dataChannelId = await DataChannelId.parseAsync(otherString);

        expect(dataChannelId.shortId).toBeTruthy();
        expect(dataChannelId.shortId).toEqual(otherString);
    });
});
