import * as fs from "fs-extra";
import { EmbeddedResource } from "./EmbeddedResource";
import { VisGenerator } from "./VisGenerator";

module.exports = (async () => {
    const out_path = "./VisVersion.ts";

    // Remove existing file
    if (fs.existsSync(out_path)) fs.unlinkSync(out_path);

    const visVersions = await EmbeddedResource.getGmodVisVersions();
    console.log(visVersions);

    const file = VisGenerator.assembleVisVersionFile(visVersions);

    fs.writeFileSync(out_path, file);
})();
