import {
    AssetIdentifier,
    DataId,
    JSONExtensions,
    TimeSeries,
    TimeSeriesDto,
    VistaJSONSerializer,
} from "../../../lib/experimental";

describe("Transport JSON", () => {
    test("TimeSeries Extensions", async () => {
        const p: TimeSeries.TimeSeriesDataPackage = {
            package: {
                header: {
                    assetId: await AssetIdentifier.parse("IMO1234567"),
                },
                timeSeriesData: [
                    {
                        dataConfiguration: {
                            id: "DataChannelList.xml",
                            timeStamp: new Date(),
                        },
                        tabularData: [
                            {
                                dataId: [await DataId.parseAsync("0010")],
                                dataSet: [
                                    { timeStamp: new Date(), value: ["00"] },
                                ],
                            },
                        ],
                        eventData: {
                            dataSet: [
                                {
                                    dataId: await DataId.parseAsync("0010"),
                                    timeStamp: new Date(),
                                    value: "00",
                                },
                            ],
                        },
                    },
                ],
            },
        };

        const dto = JSONExtensions.TimeSeries.toJsonDto(p);
        const jsonStr = VistaJSONSerializer.serializeTimeSeriesData(dto);
        const json = VistaJSONSerializer.deserializeTimeSeriesData(jsonStr);
        const domain = await JSONExtensions.TimeSeries.toDomainModel(json);

        expect(domain.package.header!.assetId.imoNumber!.value).toEqual(
            1234567
        );
        expect(dto.Package.Header!.AssetID).toEqual("IMO1234567");
    });
});
