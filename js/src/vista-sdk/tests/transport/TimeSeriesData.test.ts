import { VIS, VisVersion } from "../../lib";
import { DataChannelId, ShipId, TimeSeries } from "../../lib/transport/domain";

describe("TimeSeriesDataPackage", () => {
    const vis = VIS.instance;
    const visVersion = VisVersion.v3_4a;
    const gmodPromise = vis.getGmod(visVersion);
    const codebooksPromise = vis.getCodebooks(visVersion);

    const validLocalIdStr =
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/meta/qty-temperature/cnt-cooling.water/pos-inlet";
    test("Model", async () => {
        const gmod = await gmodPromise;
        const codebooks = await codebooksPromise;

        // Values
        const dataChannelId = DataChannelId.parse(
            validLocalIdStr,
            gmod,
            codebooks
        );
        const shipId = ShipId.parse("IMO1234567");
        const value = "1";
        const quality = "quality";
        const now = new Date();
        const idStr = "test-id";

        const timeSeriesDataPackage: TimeSeries.DataPackage = {
            package: {
                timeSeriesData: [
                    {
                        eventData: {
                            dataSet: {
                                dataChannelId: dataChannelId,
                                timeStamp: now,
                                value: value,
                                quality: quality,
                            },
                            numberOfDataSet: "1",
                        },
                        tabularData: {
                            dataChannelId: dataChannelId,
                            dataSet: {
                                timeStamp: now,
                                value: [value],
                                quality: [quality],
                            },
                            numberOfDataChannel: "1",
                            numberOfDataSet: "1",
                        },
                        dataConfiguration: {
                            id: idStr,
                            timeStamp: now,
                        },
                    },
                ],
                header: {
                    shipId: shipId,
                    dateCreated: now,
                    dateModified: now,
                    systemConfiguration: [{ timeStamp: now, id: idStr }],
                    author: "author",
                    timeSpan: { end: now, start: now },
                },
            },
        };

        // Header
        expect(timeSeriesDataPackage.package.header!.dateCreated).toBe(now);
        expect(timeSeriesDataPackage.package.header!.shipId).toBe(shipId);

        // Data
        expect(timeSeriesDataPackage.package.timeSeriesData.length).toBe(1);
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.dataConfiguration!
                .id
        ).toEqual(idStr);

        // EventData
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.eventData!
                .numberOfDataSet
        ).toEqual("1");
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.eventData!.dataSet!
                .value
        ).toEqual(value);
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.eventData!.dataSet!
                .dataChannelId
        ).toBe(dataChannelId);

        // TabularData
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.tabularData!
                .numberOfDataSet
        ).toEqual("1");
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.tabularData!
                .numberOfDataChannel
        ).toEqual("1");
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.tabularData!
                .dataChannelId
        ).toBe(dataChannelId);
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.tabularData!
                .dataSet!.value.length
        ).toEqual(1);
        expect(
            timeSeriesDataPackage.package.timeSeriesData[0]!.tabularData!
                .dataSet!.value[0]
        ).toEqual(value);
    });
});
