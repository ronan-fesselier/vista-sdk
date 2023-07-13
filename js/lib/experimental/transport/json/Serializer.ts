import { TimeSeriesDto } from "./time-series-data/TimeSeriesData";

export class Serializer {
    public static deserializeTimeSeriesData(
        payload: string
    ): TimeSeriesDto.TimeSeriesDataPackage {
        return JSON.parse(payload.toString());
    }

    public static serializeTimeSeriesData(
        timeSeriesData: TimeSeriesDto.TimeSeriesDataPackage
    ): string {
        return JSON.stringify(timeSeriesData);
    }
}
