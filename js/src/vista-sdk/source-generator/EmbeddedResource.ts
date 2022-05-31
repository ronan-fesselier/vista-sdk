import * as fs from "fs-extra";
import * as zlib from "zlib";
import { CodebookDto } from "../types/CodebookDto";
import { GmodDto } from "../types/GmodDto";

export class EmbeddedResource {
    private static readonly RESOURCE_DIR = "../../../resources";

    public static async getGmodVisVersions(): Promise<string[]> {
        const resources = this.getResourceNames().filter(
            (r) => r.endsWith(".gz") && r.includes("gmod")
        );

        if (resources.length === 0)
            throw new Error("Couldnt find any Gmod resource");

        const versions: string[] = [];

        for (const resource of resources) {
            const gmod = await this.readJsonGzip<GmodDto>(
                EmbeddedResource.RESOURCE_DIR + "/" + resource
            );
            console.log(resource, gmod.visRelease, Object.keys(gmod));

            versions.push(gmod.visRelease);
        }

        return versions;
    }

    public static async getGmod(visVersion: string) {
        const resource = this.getResourceNames().filter(
            (r) =>
                r.endsWith(".gz") &&
                r.includes("gmod") &&
                r.includes(visVersion)
        )[0];

        if (!resource)
            throw new Error(
                "Couldnt find Gmod resource for vis version: " + visVersion
            );

        return this.readJsonGzip<GmodDto>(
            EmbeddedResource.RESOURCE_DIR + "/" + resource
        );
    }

    public static async getCodebooks(visVersion: string) {
        const resource = this.getResourceNames().filter(
            (r) =>
                r.endsWith(".gz") &&
                r.includes("codebooks") &&
                r.includes(visVersion)
        )[0];

        if (!resource)
            throw new Error(
                "Couldnt find Codebook resource for vis version: " + visVersion
            );

        return this.readJsonGzip<CodebookDto>(
            EmbeddedResource.RESOURCE_DIR + "/" + resource
        );
    }

    public static getResourceNames() {
        return fs.readdirSync(EmbeddedResource.RESOURCE_DIR);
    }

    private static async readJsonGzip<T>(file: string): Promise<T> {
        if (!file.endsWith(".gz")) throw new Error("Invalid resource file");

        const jsonStr = await new Promise<string>((res) => {
            const unzip = zlib.createUnzip();
            const inputStream = fs.createReadStream(file);
            const stream = inputStream.pipe(unzip);

            const segments: Set<Buffer> = new Set();

            stream.on("data", (data) => {
                segments.add(data);
            });

            stream.on("end", () => {
                const result = Buffer.concat(Array.from(segments)).toString();
                res(result);
            });
        });
        return JSON.parse(jsonStr);
    }
}
