import {
    VisVersion,
    Gmod,
    Codebooks,
    Locations,
    VIS,
    VisVersions,
} from "../lib";

export let visMap: Map<
    VisVersion,
    {
        gmod: Gmod;
        codebooks: Codebooks;
        locations: Locations;
    }
> = new Map();

let visPromise: Promise<typeof visMap>;

export const getVISMap = async () => {
    if (!visPromise) {
        visPromise = VIS.instance.getVISMap(VisVersions.all).then((m) => {
            visMap = m;
            return visMap;
        });
    }
    return visPromise;
};

export const getVIS = (version: VisVersion) => {
    return visMap.get(version)!;
};
