import { CodebookName } from "./CodebookName";
import { GmodPath } from "./GmodPath";
import { ILocalId } from "./ILocalId";
import { MetadataTag } from "./MetadataTag";
import { VisVersion } from "./VisVersion";

export interface ILocalIdBuilder<
    TBuilder extends ILocalIdBuilder<TBuilder, TResult>,
    TResult extends ILocalId<TResult>
> {
    visVersion?: VisVersion;
    verboseMode: boolean;
    primaryItem?: GmodPath;
    secondaryItem?: GmodPath;
    hasCustomTag: boolean;

    metadataTags: MetadataTag[];

    withVisVersion(visVersion: string): TBuilder;
    withVisVersion(visVersion: VisVersion): TBuilder;
    tryWithVisVersion(visVersion?: string): TBuilder;
    tryWithVisVersion(visVersion?: VisVersion): TBuilder;
    withoutVisVersion(): TBuilder;

    withVerboseMode(verbodeMode: boolean): TBuilder;

    withPrimaryItem(item: GmodPath): TBuilder;
    tryWithPrimaryItem(item?: GmodPath): TBuilder;
    withoutPrimaryItem(): TBuilder;

    withSecondaryItem(item: GmodPath): TBuilder;
    tryWithSecondaryItem(item?: GmodPath): TBuilder;
    withoutSecondaryItem(): TBuilder;

    withMetadataTag(metadataTag: MetadataTag): TBuilder;
    tryWithMetadataTag(metadataTag?: MetadataTag): TBuilder;
    withoutMetadataTag(name: CodebookName): TBuilder;

    build(): TResult;

    isValid: boolean;
    isEmpty: boolean;

    toString(): string;
}
