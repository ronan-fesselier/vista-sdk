import { GmodPath, VIS, VisVersion } from "../lib";
import GmodPaths from "../../testdata/GmodPaths.json";

describe("GmodPath", () => {
    const vis = VIS.instance;
    const version = VisVersion.v3_4a;
    const gmodPromise = vis.getGmod(version);
    const locationsPromise = vis.getLocations(version);

    test.each(GmodPaths.Valid)("Valid path %p", async (testPath) => {
        const gmod = await gmodPromise;
        const locations = await locationsPromise;

        const path = GmodPath.tryParse(testPath, locations, gmod);

        expect(path).toBeTruthy();
    });

    test.each(GmodPaths.Invalid)("Invalid path %p", async (testPath) => {
        const gmod = await gmodPromise;
        const locations = await locationsPromise;

        const path = GmodPath.tryParse(testPath, locations, gmod);

        expect(path).toBeFalsy();
    });

    test.each([
        ["411.1/C101.72/I101", "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101"],
        ["612.21-1/C701.13/S93", "VE/600a/610/612/612.2/612.2i/612.21-1/CS10/C701/C701.1/C701.13/S93"]
    ])("Full string parsing %p", async (shortPathStr, expectedFullPathStr) => {
        const gmod = await gmodPromise;
        const locations = await locationsPromise;

        const path = GmodPath.parse(shortPathStr, locations, gmod);
        const fullString = path.toFullPathString();
        expect(fullString).toEqual(expectedFullPathStr);

        const parsedPath = GmodPath.tryParseFromFullPath(fullString, gmod, locations);
        expect(parsedPath).toBeTruthy();
        expect(path.equals(parsedPath)).toBe(true);
        expect(path.toFullPathString()).toEqual(fullString);
        expect(parsedPath!.toFullPathString()).toEqual(fullString);
        expect(path.toString()).toEqual(shortPathStr);
        expect(parsedPath!.toString()).toEqual(shortPathStr);
    });
});
