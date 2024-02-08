import { CodebookName, VisVersion } from "../lib";
import { getVIS, getVISMap } from "./Fixture";

describe("Codebooks", () => {
    const visVersion = VisVersion.v3_4a;

    beforeAll(() => {
        return getVISMap();
    });

    it("Codebooks load", () => {
        const { codebooks } = getVIS(visVersion);
        expect(codebooks).toBeTruthy();
        expect(codebooks.getCodebook(CodebookName.Position)).toBeTruthy();
    });

    it("Codebooks equality", () => {
        const { codebooks } = getVIS(visVersion);
        const codebook = codebooks.getCodebook(CodebookName.Position);
        expect(codebook.hasStandardValue("centre")).toBe(true);
    });
});
