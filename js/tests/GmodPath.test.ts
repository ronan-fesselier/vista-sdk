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
        [
            "411.1/C101.72/I101",
            "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101",
        ],
        [
            "612.21-1/C701.13/S93",
            "VE/600a/610/612/612.2/612.2i-1/612.21-1/CS10/C701/C701.1/C701.13/S93",
        ],
    ])("Full string parsing %p", async (shortPathStr, expectedFullPathStr) => {
        const gmod = await gmodPromise;
        const locations = await locationsPromise;

        const path = GmodPath.parse(shortPathStr, locations, gmod);
        const fullString = path.toFullPathString();
        expect(fullString).toEqual(expectedFullPathStr);

        const parsedPath = GmodPath.tryParseFromFullPath(
            fullString,
            gmod,
            locations
        );
        expect(parsedPath).toBeTruthy();
        expect(path.equals(parsedPath)).toBe(true);
        expect(path.toFullPathString()).toEqual(fullString);
        expect(parsedPath!.toFullPathString()).toEqual(fullString);
        expect(path.toString()).toEqual(shortPathStr);
        expect(parsedPath!.toString()).toEqual(shortPathStr);
    });

    test("GmodPath withLocation", async () => {
        const gmod = await gmodPromise;
        const locations = await locationsPromise;

        const path = gmod.parseFromFullPath(
            "VE/400a/410/411/411i/411.1/CS1/C101/C101.3/C101.3i/C101.31/C101.311-1",
            locations
        );
        const newLocation = locations.parse("2");

        // Test setting location on the path node
        path.withLocation(newLocation);
        expect(path.node.location?.toString()).toEqual(newLocation.toString());
        expect(path.getNode("C101.31").toString()).toEqual("C101.31-2");
        expect(path.getNode("C101.3i").toString()).toEqual("C101.3i-2");

        // Test setting location on a parent in the path
        path.withLocation(newLocation, gmod.getNode("411.1"));
        expect(path.getNode("411.1")?.toString()).toEqual("411.1-2");
        expect(path.getNode("411")?.toString()).not.toEqual("411-2");

        expect(() =>
            path.withLocation(newLocation, gmod.getNode("411"))
        ).toThrowError();

        // Test setting location on the path node
        const newPath = path.withoutLocation();
        newPath.withLocation(newLocation, gmod.getNode("C101.3i"));

        expect(newPath.getNode("C101.31").toString()).toEqual("C101.31-2");
    });

    test("toFullPathString", async () => {
        const gmod = await gmodPromise;
        const locations = await locationsPromise;

        let path = gmod.parsePath("511.11-1/C101.663i-1/C663", locations);

        expect(path.toFullPathString()).toEqual(
            "VE/500a/510/511/511.1/511.1i-1/511.11-1/CS1/C101/C101.6/C101.66/C101.663/C101.663i-1/C663"
        );

        path = gmod.parsePath("846/G203.32-2/S110.2-1/E31", locations);
        expect(path.toFullPathString()).toEqual(
            "VE/800a/840/846/G203/G203.3-2/G203.32-2/S110/S110.2-1/CS1/E31"
        );
    });
});
