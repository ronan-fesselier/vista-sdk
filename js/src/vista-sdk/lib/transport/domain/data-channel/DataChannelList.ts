import { ShipId } from "..";
import { LocalId } from "../../..";
import { Version } from "./Version";

export namespace DataChannelList {
    export type DataChannelListPackage = {
        package: Package;
    };

    export type Package = {
        header: Header;
        dataChannelList: DataChannelList;
    };

    export type Header = {
        shipId: ShipId;
        dataChannelListId: ConfigurationReference;
        versionInformation?: VersionInformation;
        author?: string;
        dateCreated?: Date;
        customHeaders?: { [k: string]: object };
    };

    export type ConfigurationReference = {
        id: string;
        version?: Version;
        timestamp: Date;
    };

    export type VersionInformation = {
        namingRule: string;
        namingSchemeVersion: string;
        referenceUrl?: string;
    };

    export type DataChannelList = {
        dataChannel: DataChannel[];
    };

    export type DataChannel = {
        dataChannelId: DataChannelId;
        property: Property;
    };

    export type DataChannelId = {
        localId: LocalId;
        shortId?: string;
        nameObject?: NameObject;
    };

    export type NameObject = {
        namingRule: string;
        customProperties?: { [k: string]: object };
    };

    export type Property = {
        dataChannelType: DataChannelType;
        format: Format;
        range?: Range;
        unit?: Unit;
        qualityCoding?: string;
        alertPriority?: string;
        name?: string;
        remarks?: string;
        customProperties?: { [k: string]: object };
    };

    export type Format = {
        type: string;
        restriction?: Restriction;
    };

    export type Range = {
        high: string;
        low: string;
    };

    export type Unit = {
        unitSymbol: string;
        quantityName?: string;
        customProperties?: { [k: string]: object };
    };

    export type DataChannelType = {
        type: string;
        updateCycle?: string;
        calculationPeriod?: string;
    };

    export type Restriction = {
        enumeration?: string[];
        fractionDigits?: string;
        length?: string;
        maxExclusive?: string;
        maxInclusive?: string;
        maxLength?: string;
        minExclusive?: string;
        minInclusive?: string;
        minLength?: string;
        pattern?: string;
        totalDigits?: string;
        whiteSpace?: WhiteSpace;
    };

    export enum WhiteSpace {
        Preserve = 0,
        Replace = 1,
        Collapse = 2,
    }
}
