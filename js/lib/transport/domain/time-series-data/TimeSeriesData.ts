import { ShipId, DataChannelId } from "..";

export namespace TimeSeries {
    export type TimeSeriesDataPackage = {
        package: Package;
    };

    export type Package = {
        header?: Header;
        timeSeriesData: TimeSeriesData[];
    };

    export type Header = {
        shipId: ShipId;
        timeSpan?: TimeSpan;
        dateCreated?: Date;
        dateModified?: Date;
        author?: string;
        systemConfiguration?: ConfigurationReference[];
        customHeaders?: { [k: string]: object };
    };

    export type ConfigurationReference = {
        id: string;
        timeStamp: Date;
    };

    export type TimeSpan = { start: Date; end: Date };

    export type TimeSeriesData = {
        dataConfiguration?: ConfigurationReference;
        tabularData?: TabularData[];
        eventData?: EventData;
        customProperties?: { [k: string]: object };
    };

    export type TabularData = {
        numberOfDataSet?: string;
        numberOfDataChannel?: string;
        dataChannelId?: DataChannelId[];
        dataSet?: TabularDataSet[];
    };

    export type TabularDataSet = {
        timeStamp: Date;
        value: string[];
        quality?: string[];
    };

    export type EventData = {
        numberOfDataSet?: string;
        dataSet?: EventDataSet[];
    };

    export type EventDataSet = {
        timeStamp: Date;
        dataChannelId: DataChannelId;
        value: string;
        quality?: string;
    };
}
