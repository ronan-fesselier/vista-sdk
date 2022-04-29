### Codegen

The XML schemas are the main output of ISO 19848 standard.
The schema files in this project are copied directly from the standard PDF doc.
JSON and Avro schemas are created based on these.

Using Visual Studio

* Generate sample data for XML schema file
* "Paste special" the copied sample class
* Add NRT nullability type annotations (`?`)

Make necessary changes, like remove unusing, append `global::` for colliding names. 
