# Vista SDK C++ - Command-Line Tools

## json-validator-cli - ISO 19848 JSON Validator

Validate JSON files against ISO 19848 schemas:

```bash
json-validator-cli datachannel DataChannelList.json                 # Validate DataChannelList
json-validator-cli timeseries TimeSeriesData.json                   # Validate TimeSeriesData
json-validator-cli cross DataChannelList.json TimeSeriesData.json   # Cross-validate TSD against DCL
json-validator-cli --help                                           # Show all options
```

## Usage Examples

### Example: Validating JSON

```bash
$ ./json-validator-cli datachannel DataChannelList.sample.json
Validating: "DataChannelList.sample.json"
Schema: DataChannelList.schema.json

[OK] Validation succeeded
     File conforms to DataChannelList.schema.json
     Schema: DataChannelListPackage (2020-12)
```

## Building

```bash
cmake -B build -S cpp -DCMAKE_BUILD_TYPE=Release -DDNV_VISTA_SDK_BUILD_TOOLS=ON
cmake --build build -j$(nproc)
```

## See Also

- [Main SDK Documentation](../README.md)
- [Sample Programs](../samples/README.md)
- [Benchmark Results](../benchmarks/README.md)
