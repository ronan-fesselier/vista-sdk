export namespace DataChannelListDto {
    /**
     * Generated from https://transform.tools/json-schema-to-typescript
     *
     * A DataChannelList package for ISO19848
     *
     *      Make sure to
     *      replace [k: string]: unknown; with AdditionalProperties
     *      replace string to Date where needed
     */
    export interface DataChannelListPackage {
        Package: Package;
    }
    export interface Package {
        Header: Header;
        DataChannelList: DataChannelList;
    }
    export interface Header {
        ShipID: string;
        DataChannelListID: ConfigurationReference;
        VersionInformation?: VersionInformation;
        Author?: string;
        DateCreated?: Date;
        AdditionalProperties?: {
            [k: string]: any;
        };
    }
    export interface ConfigurationReference {
        ID: string;
        Version?: string;
        TimeStamp: Date;
    }
    export interface VersionInformation {
        NamingRule: string;
        NamingSchemeVersion: string;
        ReferenceURL?: string;
    }
    export interface DataChannelList {
        DataChannel: DataChannel[];
    }
    export interface DataChannel {
        DataChannelID: DataChannelID;
        Property: Property;
    }
    export interface DataChannelID {
        LocalID: string;
        ShortID?: string;
        NameObject?: NameObject;
    }
    export interface NameObject {
        NamingRule: string;
        AdditionalProperties?: {
            [k: string]: any;
        };
    }
    export interface Property {
        DataChannelType: DataChannelType;
        Format: Format;
        Range?: Range;
        Unit?: Unit;
        QualityCoding?: string;
        AlertPriority?: string;
        Name?: string;
        Remarks?: string;
        AdditionalProperties?: {
            [k: string]: any;
        };
    }
    export interface DataChannelType {
        Type: string;
        UpdateCycle?: string;
        CalculationPeriod?: string;
    }
    export interface Format {
        Type: string;
        Restriction?: Restriction;
    }
    export interface Restriction {
        Enumeration?: string[];
        FractionDigits?: string;
        Length?: string;
        MaxExclusive?: string;
        MaxInclusive?: string;
        MaxLength?: string;
        MinExclusive?: string;
        MinInclusive?: string;
        MinLength?: string;
        Pattern?: string;
        TotalDigits?: string;
        WhiteSpace?: RestrictionWhiteSpace;
    }
    export interface Range {
        High: string;
        Low: string;
    }
    export interface Unit {
        UnitSymbol: string;
        QuantityName?: string;
        AdditionalProperties?: {
            [k: string]: any;
        };
    }

    export enum RestrictionWhiteSpace {
        Preserve = 0,
        Replace = 1,
        Collapse = 2,
    }
}
