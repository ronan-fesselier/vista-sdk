import { LocalId, LocalIdBuilder, VIS, VisVersion } from "../../lib";
import { ShipId, DataChannel } from "../../lib/transport/domain";
import { Version } from "../../lib/transport/domain/data-channel/Version";

describe("DataChannel", () => {
    const vis = VIS.instance;
    const visVersion = VisVersion.v3_4a;
    const gmodPromise = vis.getGmod(visVersion);
    const codebooksPromise = vis.getCodebooks(visVersion);

    const validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/meta/qty-temperature/cnt-cooling.water/pos-inlet";

    test("Model", async () => {
        const gmod = await gmodPromise;
        const codebooks = await codebooksPromise;

        const localId = LocalId.parse(validLocalIdStr, gmod, codebooks);

        const shipId = ShipId.parse("IMO1234567");
        const now = new Date();
        const idStr = "test-id";
        const namingRule = LocalIdBuilder.namingRule;
        const version = Version.parse(VisVersion.v3_5a);

        console.log(DataChannel.WhiteSpace.Collapse);

        const dataChannelListPackage: DataChannel.ListPackage = {
            package: {
                header: {
                    dataChannelListId: {
                        id: idStr,
                        timestamp: now,
                        version: version,
                    },
                    dateCreated: now,
                    shipId: shipId,
                    author: "author",
                    versionInformation: {
                        namingRule: namingRule,
                        namingSchemeVersion: "naming-scheme-version",
                        referenceUrl: "www.referenceurl.test",
                    },
                },
                dataChannelList: {
                    dataChannel: [
                        {
                            dataChannelId: {
                                localId: localId,
                                shortId: "unique-id",
                                nameObject: { namingRule: namingRule },
                            },
                            property: {
                                dataChannelType: {
                                    type: "Inst",
                                    calculationPeriod: undefined,
                                    updateCycle: "1",
                                },
                                format: {
                                    type: "formatType",
                                    restriction: {
                                        enumeration: ["0", "1"],
                                        fractionDigits: "1",
                                        length: "1",
                                        maxExclusive: "maxExclusive",
                                        minExclusive: "minExclusive",
                                        maxLength: "maxLength",
                                        maxInclusive: "maxInclusive",
                                        minInclusive: "minInclusive",
                                        minLength: "minLength",
                                        pattern: "pattern",
                                        totalDigits: "totalDigits",
                                        whiteSpace:
                                            DataChannel.WhiteSpace.Collapse,
                                    },
                                },
                                alertPriority: "alertPriority",
                                name: "Cooling system / Propulsion Engine",
                                qualityCoding: "qualityCoding",
                                range: { high: "100", low: "0" },
                                remarks: "This is a remark",
                                unit: {
                                    quantityName: "celcius",
                                    unitSymbol: "C",
                                },
                            },
                        },
                    ],
                },
            },
        };

        // Header
        expect(
            dataChannelListPackage.package.header.dataChannelListId.id
        ).toEqual(idStr);
        expect(dataChannelListPackage.package.header.shipId).toBe(shipId);
        expect(
            dataChannelListPackage.package.header.dataChannelListId.version
        ).toBe(version);
        expect(
            dataChannelListPackage.package.header.dataChannelListId.version!.toString()
        ).toEqual(VisVersion.v3_5a);

        // DataChannels
        expect(
            dataChannelListPackage.package.dataChannelList.dataChannel
        ).toHaveLength(1);
        expect(
            dataChannelListPackage.package.dataChannelList.dataChannel[0]!
                .dataChannelId.localId
        ).toBe(localId);
        expect(
            dataChannelListPackage.package.dataChannelList.dataChannel[0]!
                .dataChannelId.nameObject!.namingRule
        ).toEqual(namingRule);
        expect(
            dataChannelListPackage.package.dataChannelList.dataChannel[0]!
                .property.format
        ).toBeTruthy();
        expect(
            dataChannelListPackage.package.dataChannelList.dataChannel[0]!
                .property.dataChannelType.type
        ).toEqual("Inst");
    });
});
