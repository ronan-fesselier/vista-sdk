import * as fs from "fs-extra";
import { Extensions as DataChannelListExtensions } from "../../lib/transport/json/data-channel";
import { Extensions as TimeSeriesExtensions } from "../../lib/transport/json/time-series-data";
import { Serializer } from "../../lib/transport/json";

describe("Transport JSON", () => {
    test("DataChannelList Extensions", async () => {
        const testDataPath =
            "../../../schemas/json/DataChannelList.sample.json";
        const sample = await fs
            .readFile(testDataPath)
            .then((res) => res.toString());

        const initDto = Serializer.deserializeDataChannelList(sample);

        const domain = await DataChannelListExtensions.toDomainModel(initDto);

        const dto = DataChannelListExtensions.toJsonDto(domain);

        expect(domain.package.header.shipId.imoNumber!.value).toEqual(1234567);
        expect(dto.Package.Header.ShipID).toEqual("IMO1234567");
    });

    test("TimeSeries Extensions", async () => {
        const testDataPath = "../../../schemas/json/TimeSeriesData.sample.json";
        const sample = await fs
            .readFile(testDataPath)
            .then((res) => res.toString());

        const initDto = Serializer.deserializeTimeSeriesData(sample);

        const domain = await TimeSeriesExtensions.toDomainModel(initDto);

        const dto = TimeSeriesExtensions.toJsonDto(domain);

        expect(domain.package.header!.shipId.imoNumber!.value).toEqual(1234567);
        expect(dto.Package.Header!.ShipID).toEqual("IMO1234567");
    });
});
