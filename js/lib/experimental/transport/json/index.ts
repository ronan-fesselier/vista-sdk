import { Extensions as TimeSeriesExtensions } from "./time-series-data/Extensions";
import { TimeSeriesDto } from "./time-series-data/TimeSeriesData";

class JSONExtensions {
    static TimeSeries = TimeSeriesExtensions;
}

export { JSONExtensions };

export { TimeSeriesDto };
export { Serializer as VistaJSONSerializer } from "./Serializer";
