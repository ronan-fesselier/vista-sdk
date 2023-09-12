import { GmodPath } from "./GmodPath";
import { MetadataTag } from "./MetadataTag";
import { VisVersion } from "./VisVersion";

export interface ILocalId {
    visVersion: VisVersion;
    verboseMode: boolean;
    primaryItem: GmodPath;
    secondaryItem: GmodPath | undefined;
    hasCustomTag: boolean;
    metadataTags: MetadataTag[];

    toString(): string;
}

export interface ILocalIdGeneric<T extends ILocalIdGeneric<T>>
    extends ILocalId {}
