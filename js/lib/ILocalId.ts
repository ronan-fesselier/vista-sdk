import { GmodPath } from "./GmodPath";
import { MetadataTag } from "./MetadataTag";
import { VisVersion } from "./VisVersion";

export interface ILocalId<T extends ILocalId<T>> {
    visVersion: VisVersion;
    verboseMode: boolean;
    primaryItem: GmodPath;
    secondaryItem: GmodPath | undefined;
    hasCustomTag: boolean;
    metadataTags: MetadataTag[];

    toString(): string;
}
