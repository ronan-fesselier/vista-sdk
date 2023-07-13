export namespace TimeSeriesDto {
    /**
     * Generated from https://transform.tools/json-schema-to-typescript
     *
     * A TimeSeriesData package for ISO19848
     *
     *
     * Make sure to
     *      replace [k: string]: unknown; with AdditionalProperties
     *      replace string to Date where needed
     */
    export interface TimeSeriesDataPackage {
        Package: Package;
    }
    export interface Package {
        Header?: Header;
        TimeSeriesData: TimeSeriesData[];
    }
    export interface Header {
        AssetID: string;
        TimeSpan?: TimeSpan;
        DateCreated?: Date;
        DateModified?: Date;
        Author?: string;
        SystemConfiguration?: ConfigurationReference[];
        AdditionalProperties?: {
            [k: string]: object;
        };
    }
    export interface TimeSpan {
        Start: Date;
        End: Date;
    }
    export interface ConfigurationReference {
        ID: string;
        TimeStamp: Date;
    }
    export interface TimeSeriesData {
        DataConfiguration?: ConfigurationReference;
        TabularData?: TabularData[];
        EventData?: EventData;
        AdditionalProperties?: {
            [k: string]: object;
        };
    }
    export interface TabularData {
        NumberOfDataSet?: string;
        NumberOfDataChannel?: string;
        DataId?: string[];
        DataSet?: TabularDataSet[];
    }
    export interface TabularDataSet {
        TimeStamp: Date;
        Value: string[];
        Quality?: string[];
    }
    export interface EventData {
        NumberOfDataSet?: string;
        DataSet?: EventDataSet[];
    }
    export interface EventDataSet {
        TimeStamp: Date;
        DataId: string;
        Value: string;
        Quality?: string;
    }
}
