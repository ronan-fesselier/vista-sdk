### Codegen

```powershell
# From project root
cd \TimeSeriesData; dotnet avrogen -s ../../../../schemas/json/TimeSeriesData.avsc .; cd ..
cd \DataChannelList; dotnet avrogen -s ../../../../schemas/json/DataChannelList.avsc .; cd ..
```

* avrogen creates a folderstructure for ouput, we keep the files flat, so flatten the folder structure
* in the generated files some cleanup may be necessary, for example
  * remove usings - 
    * s/using Avro.Specific;//
  * qualify types with `global::` where necessary
    * s/private Vista.SDK.Transport.Avro/private global::Vista.SDK.Transport.Avro/
    * s/public Vista.SDK.Transport.Avro/public global::Vista.SDK.Transport.Avro/
    * s/<Vista.SDK.Transport.Avro/<global::Vista.SDK.Transport.Avro/
    * s/(Vista.SDK.Transport.Avro/(global::Vista.SDK.Transport.Avro/
    * s/Avro.Schema.Parse/global::Avro.Schema.Parse/


### Orginal schema

Schema was initially generated from JSON schema: 

```js
// Install these packages
const jsonSchemaAvro = require('json-schema-to-avro');
const $RefParser = require("@apidevtools/json-schema-ref-parser");
 
const inJson = { /* Copy paste the JSON schema here */ };

$RefParser.dereference(inJson, (err, schema) => {
    if (err) {
        console.error(err);
    }
    else {
        const avro = jsonSchemaAvro.convert(schema)
        console.log(JSON.stringify(avro, null, 2));
        // This output is the avro schema, can be put into schema.avsc
    }
});

```