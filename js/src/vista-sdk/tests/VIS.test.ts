import { EmbeddedResource } from "../source-generator/EmbeddedResource";
import { VisVersionExtension, VisVersions, VisVersion } from "../VisVersion";

describe("Vis", () => {
    test("VisVersion", () => {
        const version = VisVersion.v3_4a;
        const versionStr = VisVersionExtension.toVersionString(version);

        expect(versionStr).toEqual("3-4a");
        expect(version).toEqual(VisVersions.parse(versionStr));
    });

    test("EmbeddedResource", async () => {
        const resourceNames = EmbeddedResource.getResourceNames().filter((n) =>
            n.includes("gmod")
        );
        expect(resourceNames).not.toHaveLength(0);

        const json = await EmbeddedResource.readJsonGzip(
            "./resources/" + resourceNames[0]
        );

        expect(json).not.toBeFalsy();
    });
});
