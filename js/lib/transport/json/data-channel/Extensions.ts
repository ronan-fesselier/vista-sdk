import { LocalId } from "../../..";
import { DataChannelList, ShipId } from "../../domain";
import { Version } from "../../domain/data-channel/Version";
import { DataChannelListDto } from "./DataChannelList";

abstract class IExtension {
    public static toJsonDto: <Domain, Dto>(domain: Domain) => Dto;
    public static toDomainModel: <Domain, Dto>(dto: Dto) => Domain;
}

export class Extensions implements IExtension {
    public static toJsonDto(
        domain: DataChannelList.DataChannelListPackage
    ): DataChannelListDto.DataChannelListPackage {
        const p = domain.package;
        const h = domain.package.header;
        return {
            Package: {
                Header: {
                    Author: h.author,
                    DataChannelListID: {
                        ID: h.dataChannelListId.id,
                        TimeStamp: h.dataChannelListId.timestamp,
                        Version: h.dataChannelListId.version?.toString(),
                    },
                    ShipID: h.shipId.toString(),
                    DateCreated: h.dateCreated,
                    VersionInformation: h.versionInformation
                        ? {
                              NamingRule: h.versionInformation.namingRule,
                              NamingSchemeVersion:
                                  h.versionInformation.namingSchemeVersion,
                              ReferenceURL: h.versionInformation.referenceUrl,
                          }
                        : undefined,
                    AdditionalProperties: h.customHeaders,
                },
                DataChannelList: {
                    DataChannel: p.dataChannelList.dataChannel.map(
                        DataChannelExtension.toJsonDto
                    ),
                },
            },
        };
    }

    public static async toDomainModel(
        dto: DataChannelListDto.DataChannelListPackage
    ): Promise<DataChannelList.DataChannelListPackage> {
        const p = dto.Package;
        const h = dto.Package.Header;

        const dataChannels: DataChannelList.DataChannel[] = [];

        for (let c of p.DataChannelList.DataChannel) {
            const dc = await DataChannelExtension.toDomainModel(c);
            dataChannels.push(dc);
        }

        return {
            package: {
                header: {
                    dataChannelListId: {
                        id: h.DataChannelListID.ID,
                        timestamp: h.DataChannelListID.TimeStamp,
                        version: Version.parse(h.DataChannelListID.Version),
                    },
                    dateCreated: h.DateCreated,
                    shipId: ShipId.parse(h.ShipID),
                    author: h.Author,
                    versionInformation: h.VersionInformation
                        ? {
                              namingRule: h.VersionInformation.NamingRule,
                              namingSchemeVersion:
                                  h.VersionInformation.NamingSchemeVersion,
                              referenceUrl: h.VersionInformation.ReferenceURL,
                          }
                        : undefined,
                    customHeaders: h.AdditionalProperties,
                },
                dataChannelList: {
                    dataChannel: dataChannels,
                },
            },
        };
    }
}

export class DataChannelExtension {
    public static toJsonDto(
        c: DataChannelList.DataChannel
    ): DataChannelListDto.DataChannel {
        return {
            DataChannelID: {
                LocalID: c.dataChannelId.localId.verboseMode
                    ? c.dataChannelId.localId.builder
                          .withoutVerboseMode()
                          .build()
                          .toString()
                    : c.dataChannelId.localId.toString(),
                ShortID: c.dataChannelId.shortId,
                NameObject: c.dataChannelId.nameObject
                    ? {
                          NamingRule: c.dataChannelId.nameObject.namingRule,
                          AdditionalProperties:
                              c.dataChannelId.nameObject.customProperties,
                      }
                    : undefined,
            },
            Property: {
                DataChannelType: {
                    Type: c.property.dataChannelType.type,
                    CalculationPeriod:
                        c.property.dataChannelType.calculationPeriod,
                    UpdateCycle: c.property.dataChannelType.updateCycle,
                },
                Format: {
                    Type: c.property.format.type,
                    Restriction: c.property.format.restriction && {
                        Enumeration: c.property.format.restriction.enumeration,
                        FractionDigits:
                            c.property.format.restriction.fractionDigits,
                        Length: c.property.format.restriction.length,
                        MaxExclusive:
                            c.property.format.restriction.maxExclusive,
                        MaxInclusive:
                            c.property.format.restriction.maxInclusive,
                        MaxLength: c.property.format.restriction.maxLength,
                        MinExclusive:
                            c.property.format.restriction.minExclusive,
                        MinInclusive:
                            c.property.format.restriction.minInclusive,
                        MinLength: c.property.format.restriction.minLength,
                        Pattern: c.property.format.restriction.pattern,
                        TotalDigits: c.property.format.restriction.totalDigits,
                        WhiteSpace: c.property.format.restriction.whiteSpace
                            ? (c.property.format.restriction
                                  .whiteSpace as unknown as DataChannelListDto.RestrictionWhiteSpace)
                            : undefined,
                    },
                },
                Range: c.property.range
                    ? {
                          Low: c.property.range.low,
                          High: c.property.range.high,
                      }
                    : undefined,
                Unit: c.property.unit
                    ? {
                          UnitSymbol: c.property.unit.unitSymbol,
                          QuantityName: c.property.unit.quantityName,
                          AdditionalProperties: c.property.customProperties,
                      }
                    : undefined,
                QualityCoding: c.property.qualityCoding,
                AlertPriority: c.property.alertPriority,
                Name: c.property.name,
                Remarks: c.property.remarks,
                AdditionalProperties: c.property.customProperties,
            },
        };
    }

    public static async toDomainModel(
        c: DataChannelListDto.DataChannel
    ): Promise<DataChannelList.DataChannel> {
        const localId = await LocalId.parseAsync(c.DataChannelID.LocalID);
        return {
            dataChannelId: {
                localId: localId,
                shortId: c.DataChannelID.ShortID,
                nameObject: c.DataChannelID.NameObject && {
                    namingRule: c.DataChannelID.NameObject.NamingRule,
                    customProperties:
                        c.DataChannelID.NameObject.AdditionalProperties,
                },
            },
            property: {
                dataChannelType: {
                    type: c.Property.DataChannelType.Type,
                    calculationPeriod:
                        c.Property.DataChannelType.CalculationPeriod,
                    updateCycle: c.Property.DataChannelType.UpdateCycle,
                },
                format: {
                    type: c.Property.Format.Type,
                    restriction: c.Property.Format.Restriction && {
                        enumeration: c.Property.Format.Restriction.Enumeration,
                        fractionDigits:
                            c.Property.Format.Restriction.FractionDigits,
                        length: c.Property.Format.Restriction.Length,
                        maxExclusive:
                            c.Property.Format.Restriction.MaxExclusive,
                        maxInclusive:
                            c.Property.Format.Restriction.MaxInclusive,
                        maxLength: c.Property.Format.Restriction?.MaxLength,
                        minExclusive:
                            c.Property.Format.Restriction.MinExclusive,
                        minInclusive:
                            c.Property.Format.Restriction.MinInclusive,
                        minLength: c.Property.Format.Restriction.MinLength,
                        pattern: c.Property.Format.Restriction.Pattern,
                        totalDigits: c.Property.Format.Restriction.TotalDigits,
                        whiteSpace: c.Property.Format.Restriction
                            .WhiteSpace as unknown as DataChannelList.WhiteSpace,
                    },
                },
                range: c.Property.Range && {
                    low: c.Property.Range.Low,
                    high: c.Property.Range.High,
                },
                unit: c.Property.Unit && {
                    unitSymbol: c.Property.Unit.UnitSymbol,
                    quantityName: c.Property.Unit.QuantityName,
                    customProperties: c.Property.Unit.AdditionalProperties,
                },
                qualityCoding: c.Property.QualityCoding,
                alertPriority: c.Property.AlertPriority,
                name: c.Property.Name,
                remarks: c.Property.Remarks,
                customProperties: c.Property.AdditionalProperties,
            },
        };
    }
}
