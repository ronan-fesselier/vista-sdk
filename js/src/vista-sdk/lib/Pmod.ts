import { LocalIdBuilder } from "./LocalId.Builder";
import { LocalId } from "./LocalId";
import { Gmod } from "./Gmod";
import { ImoNumber } from "./ImoNumber";
import { GmodDto, GmodNodeDto } from "./types/GmodDto";
import { PmodInfo } from "./types/Pmod";
import { PmodDto } from "./types/PmodDto";
import { GmodNode } from "./GmodNode";

export class Pmod {
    private _model: Gmod;
    private _info?: PmodInfo;

    private constructor(model: Gmod, info?: PmodInfo) {
        this._model = model;
        this._info = info;
    }

    public static createFromDto(pmodDto: PmodDto, gmod: Gmod) {
        const gmodDto: GmodDto = {
            items: pmodDto.items.map((item) => {
                const node = gmod.getNode(item.code);
                return {
                    category: node.metadata.category,
                    code: item.code,
                    name: item.name,
                    type: node.metadata.type,
                    commonDefinition: node.metadata.commonDefinition,
                    commonName: item.name,
                    definition: node.metadata.definition,
                    installSubstructure: node.metadata.installSubstructure,
                    normalAssignmentNames: Object.fromEntries(
                        node.metadata.normalAssignmentNames
                    ),
                    location: item.location,
                    id: item.id,
                } as GmodNodeDto;
            }),
            relations: pmodDto.relations.map((r) => [r[0], r[1]]),
            visRelease: gmod.visVersion,
        };
        return new Pmod(new Gmod(gmod.visVersion, gmodDto), {
            imoNumber: new ImoNumber(pmodDto.info.imo_number),
            timestamp: new Date(pmodDto.info.timestamp),
            vesselId: pmodDto.info.vessel_id,
            vesselName: pmodDto.info.vessel_name,
        });
    }

    public static createFromLocalIds(
        localIds: LocalId[] | LocalIdBuilder[],
        gmod: Gmod,
        info?: PmodInfo
    ): Pmod {
        const nodeMap = new Map<string, GmodNode>();
        const relations = new Map<string, [string, string]>();

        for (const item of localIds) {
            const localId = "builder" in item ? item.builder : item;

            const primaryItemFullPath =
                localId.primaryItem?.getFullPath() ?? [];
            const secondaryItemFullPath =
                localId.secondaryItem?.getFullPath() ?? [];

            const paths = [primaryItemFullPath, secondaryItemFullPath];

            for (const fullPath of paths) {
                for (let i = 0; i < fullPath.length; i++) {
                    const node = fullPath[i];
                    const nodeId = node.toString();
                    if (!nodeMap.has(nodeId)) nodeMap.set(nodeId, node);

                    if (node.code === "VE" || i === 0) continue;

                    const parent = fullPath[i - 1];
                    if (!parent) continue;
                    const parentId = parent.toString();
                    const relationId = parentId + "-" + nodeId;
                    if (!relations.has(relationId))
                        relations.set(relationId, [parentId, nodeId]);
                }
            }
        }

        const uniqueGmodNodes: GmodNode[] = [...nodeMap.values()];

        const gmodDto: GmodDto = {
            items: uniqueGmodNodes.map((node) => {
                return {
                    category: node.metadata.category,
                    code: node.code,
                    name: node.metadata.name,
                    type: node.metadata.type,
                    commonDefinition: node.metadata.commonDefinition,
                    commonName: node.metadata.commonName,
                    definition: node.metadata.definition,
                    installSubstructure: node.metadata.installSubstructure,
                    normalAssignmentNames: Object.fromEntries(
                        node.metadata.normalAssignmentNames
                    ),
                    location: node.location,
                    id: node.toString(),
                };
            }),
            relations: [...relations.values()],
            visRelease: gmod.visVersion,
        };

        return new Pmod(new Gmod(gmod.visVersion, gmodDto), info);
    }

    public static create(gmod: Gmod, info?: PmodInfo) {
        return new Pmod(gmod, info);
    }

    public get model() {
        return this._model;
    }

    public get info() {
        return this._info;
    }
}
