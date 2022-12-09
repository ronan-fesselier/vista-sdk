import { GmodPath, VisVersion } from "../lib";
import GmodPaths from "../../testdata/GmodPaths.json";

describe("GmodPath", () => {
    test.each(GmodPaths.Valid)("Valid path %p", async (testPath) => {
        const path = await GmodPath.tryParseAsync(testPath, VisVersion.v3_4a);

        expect(path).toBeTruthy();
    });

    test.each(GmodPaths.Invalid)("Invalid path %p", async (testPath) => {
        const path = await GmodPath.tryParseAsync(testPath, VisVersion.v3_4a);

        expect(path).toBeFalsy();
    });
});
