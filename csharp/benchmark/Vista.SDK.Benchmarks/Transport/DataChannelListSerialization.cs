using Avro.IO;
using Avro.Specific;
using BenchmarkDotNet.Columns;
using BenchmarkDotNet.Engines;
using BenchmarkDotNet.Exporters;
using BenchmarkDotNet.Exporters.Csv;
using BenchmarkDotNet.Reports;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Toolchains;
using BenchmarkDotNet.Toolchains.InProcess;
using BenchmarkDotNet.Toolchains.InProcess.NoEmit;
using ICSharpCode.SharpZipLib.BZip2;
using Microsoft.Diagnostics.Tracing.Parsers.IIS_Trace;
using System.Globalization;
using System.Text.Json;
using Vista.SDK.Transport.Avro.DataChannel;
using Vista.SDK.Transport.Json.DataChannel;
using DataChannelListJsonPackage = Vista.SDK.Transport.Json.DataChannel.DataChannelListPackage;
using DataChannelListAvroPackage = Vista.SDK.Transport.Avro.DataChannel.DataChannelListPackage;

namespace Vista.SDK.Benchmarks.Transport;

[Config(typeof(Config))]
public class DataChannelListSerialization
{
    private MemoryStream _memoryStream;
    private MemoryStream _compressionStream;

    private DataChannelListJsonPackage _jsonPackage;
    private DataChannelListAvroPackage _avroPackage;
    private BinaryEncoder _avroEncoder;
    private SpecificWriter<DataChannelListAvroPackage> _avroWriter;

    private static readonly Lazy<DataChannelListSerialization> _bench =
        new(
            () =>
            {
                var bench = new DataChannelListSerialization();
                bench.Setup();
                return bench;
            }
        );
    private static readonly Dictionary<string, long> _payloadSizes = new();

    public class Config : ManualConfig
    {
        public Config()
        {
            AddJob(Job.ShortRun);
            AddColumn(new CategoriesColumn());
            AddColumn(
                new TagColumn(
                    "Payload size",
                    name =>
                    {
                        _ = _bench.Value;

                        if (!name.Contains("_"))
                            return GetPayloadSize(_payloadSizes[name]);

                        var values = new List<string>();
                        foreach (var compressionLevelArgs in GetCompressionLevels())
                        {
                            var compressionLevel = (int)compressionLevelArgs[0];
                            var size = GetPayloadSize(_payloadSizes[$"{name}-{compressionLevel}"]);
                            values.Add($"level {compressionLevel} / {size}");
                        }

                        return string.Join(", ", values);
                    }
                )
            );
            AddExporter(CsvMeasurementsExporter.Default);
            AddExporter(RPlotExporter.Default);
            AddDiagnoser(MemoryDiagnoser.Default);
            WithOrderer(
                new DefaultOrderer(SummaryOrderPolicy.Method, MethodOrderPolicy.Alphabetical)
            );
            AddLogicalGroupRules(BenchmarkLogicalGroupRule.ByCategory);

            SummaryStyle = SummaryStyle.Default.WithRatioStyle(RatioStyle.Percentage);

            this.KeepBenchmarkFiles(true);
        }

        static string GetPayloadSize(long bytes) =>
            SizeValue.FromBytes(bytes).ToString(CultureInfo.InvariantCulture, "0.##");
    }

    [GlobalSetup]
    public void Setup()
    {
        var text = File.ReadAllText("Transport/_files/DataChannelList.json");
        _jsonPackage = JsonSerializer.Deserialize<DataChannelListJsonPackage>(text);
        var domainModel = _jsonPackage!.ToDomainModel();
        _avroPackage = domainModel.ToAvroDto();

        _memoryStream = new MemoryStream();
        _compressionStream = new MemoryStream();
        _avroWriter = new SpecificWriter<DataChannelListAvroPackage>(
            DataChannelListAvroPackage._SCHEMA
        );
        _avroEncoder = new BinaryEncoder(_memoryStream);

        Json();
        _payloadSizes[nameof(Json)] = _memoryStream.Length;

        Avro();
        _payloadSizes[nameof(Avro)] = _memoryStream.Length;

        foreach (var compressionLevelArgs in GetCompressionLevels())
        {
            var compressionLevel = (int)compressionLevelArgs[0];

            Json_Bzip2(compressionLevel);
            _payloadSizes[$"{nameof(Json_Bzip2)}-{compressionLevel}"] = _compressionStream.Length;

            Avro_Bzip2(compressionLevel);
            _payloadSizes[$"{nameof(Avro_Bzip2)}-{compressionLevel}"] = _compressionStream.Length;
        }
    }

    [GlobalCleanup]
    public void Cleanup()
    {
        _memoryStream.Dispose();
    }

    [Benchmark(Description = "Json")]
    [BenchmarkCategory("Uncompressed")]
    public void Json()
    {
        _memoryStream.SetLength(0);
        JsonSerializer.Serialize(_memoryStream, _jsonPackage);
    }

    [Benchmark(Description = "Json")]
    [ArgumentsSource(nameof(GetCompressionLevels))]
    [BenchmarkCategory("Bzip2")]
    public void Json_Bzip2(int CompressionLevel)
    {
        _memoryStream.SetLength(0);
        _compressionStream.SetLength(0);
        JsonSerializer.Serialize(_memoryStream, _jsonPackage);
        _memoryStream.Position = 0;
        BZip2.Compress(_memoryStream, _compressionStream, false, CompressionLevel);
    }

    [Benchmark(Description = "Avro")]
    [BenchmarkCategory("Uncompressed")]
    public void Avro()
    {
        _memoryStream.SetLength(0);
        _avroWriter.Write(_avroPackage, _avroEncoder);
    }

    [Benchmark(Description = "Avro")]
    [ArgumentsSource(nameof(GetCompressionLevels))]
    [BenchmarkCategory("Bzip2")]
    public void Avro_Bzip2(int CompressionLevel)
    {
        _memoryStream.SetLength(0);
        _compressionStream.SetLength(0);
        _avroWriter.Write(_avroPackage, _avroEncoder);
        _memoryStream.Position = 0;
        BZip2.Compress(_memoryStream, _compressionStream, false, CompressionLevel);
    }

    public static IEnumerable<object[]> GetCompressionLevels()
    {
        yield return new object[] { 5 };
        yield return new object[] { 9 };
    }
}
