import { ImoNumber, LocalIdBuilder, Pmod, VIS, VisVersion } from "../lib";

describe("Pmod", () => {
    const vis = VIS.instance;
    const version = VisVersion.v3_4a;
    const gmodPromise = vis.getGmod(version);
    const codebooksPromise = vis.getCodebooks(version);

    const testData = [
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta",
        "/dnv-v2/vis-3-4a/1031/meta/cnt-refrigerant/state-leaking",
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/1021.1i-6P/H123/meta/qty-volume/cnt-cargo/pos~percentage",
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/~propulsion.engine/~cooling.system/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-1P/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1/C101.313-4/C469/meta/qty-temperature/state-high/pos-intake.side",
    ];

    test("From LocalIds", async () => {
        const gmod = await gmodPromise;
        const codeBooks = await codebooksPromise;

        const localIds = testData.map((localIdStr) =>
            LocalIdBuilder.parse(localIdStr, gmod, codeBooks)
        );

        const pmod = Pmod.createFromLocalIds(localIds, gmod, {
            imoNumber: new ImoNumber(1234567),
        });

        expect(pmod.info?.imoNumber?.value).toEqual(1234567);
        expect(pmod.model.getNode("C469")).toBeTruthy();
        expect(pmod.model.getNode("1031")).toBeTruthy();
        expect(pmod.model.getNode("S90.3")).toBeTruthy();
        expect(pmod.model.getNode("C101.31-1P").location).toEqual("1P");
        expect(pmod.model.getNode("C101.31-5").location).toEqual("5");
    });
});
