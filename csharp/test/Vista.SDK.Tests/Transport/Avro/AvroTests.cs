using System.Text.Json;

using Avro.IO;
using Avro.Specific;

using FluentAssertions;

using Vista.SDK.Transport.Avro.DataChannel;
using Vista.SDK.Transport.Avro.TimeSeriesData;

namespace Vista.SDK.Tests.Transport.Avro;

public class AvroTests
{
    [Fact]
    public void Test_Create()
    {
        var domainPackage = IsoMessageTests.TestDataChannelListPackage;
        var dto = domainPackage.ToAvroDto();
    }

    [Theory]
    [InlineData("Transport/Json/_files/DataChannelList.json")]
    public async Task DataChannelList_Json_Dto_To_Domain_And_Avro_Dto(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package =
            await JsonSerializer.DeserializeAsync<Vista.SDK.Transport.Json.DataChannel.DataChannelListPackage>(
                reader
            );
        Assert.NotNull(package);

        var domainModel = Vista.SDK.Transport.Json.DataChannel.Extensions.ToDomainModel(package!);
        Assert.NotNull(domainModel);
        var dto = domainModel.ToAvroDto();

        await using var writer = new FileStream("DataChannelList.avro", FileMode.OpenOrCreate);
        SerializeDataChannelList(writer, dto);
    }

    [Theory]
    [InlineData("Transport/Json/_files/TimeSeriesData.json")]
    public async Task TimeSeriesData_Json_Dto_To_Domain_And_Avro_Dto(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package =
            await JsonSerializer.DeserializeAsync<Vista.SDK.Transport.Json.TimeSeriesData.TimeSeriesDataPackage>(
                reader
            );
        Assert.NotNull(package);

        var domainModel = Vista.SDK.Transport.Json.TimeSeriesData.Extensions.ToDomainModel(
            package!
        );
        Assert.NotNull(domainModel);
        var dto = domainModel.ToAvroDto();

        await using var writer = new FileStream("TimeSeriesData.avro", FileMode.OpenOrCreate);
        SerializeTimeSeriesData(writer, dto);
    }

    [Theory]
    [InlineData("Transport/Avro/_files/TimeSeriesData.avro")]
    public async Task Test_TimeSeriesData_Deserialization(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package = DeserializeTimeSeriesData(reader);
        Assert.NotNull(package);
    }

    [Theory]
    [InlineData("Transport/Avro/_files/DataChannelList.avro")]
    public async Task Test_DataChannelList_Deserialization(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package = DeserializeDataChannelList(reader);
        Assert.NotNull(package);
    }

    [Theory]
    [InlineData("Transport/Avro/_files/TimeSeriesData.avro")]
    public async Task Test_TimeSeriesData_Serialization_Roundtrip(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package = DeserializeTimeSeriesData(reader);
        Assert.NotNull(package);

        using var serialized = new MemoryStream();
        SerializeTimeSeriesData(serialized, package);
        serialized.Position = 0;

        var deserialized = DeserializeTimeSeriesData(serialized);

        package.Should().BeEquivalentTo(deserialized);
    }

    [Theory]
    [InlineData("Transport/Avro/_files/DataChannelList.avro")]
    public async Task Test_DataChannelList_Serialization_Roundtrip(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package = DeserializeDataChannelList(reader);
        Assert.NotNull(package);

        using var serialized = new MemoryStream();
        SerializeDataChannelList(serialized, package);
        serialized.Position = 0;

        var deserialized = DeserializeDataChannelList(serialized);

        package.Should().BeEquivalentTo(deserialized);
    }

    [Theory]
    [InlineData("Transport/Avro/_files/DataChannelList.avro")]
    public async Task Test_DataChannelList_Domain_Model_Roundtrip(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package = DeserializeDataChannelList(reader);
        Assert.NotNull(package);

        var domainPackage = package!.ToDomainModel();
        var dto = domainPackage.ToAvroDto();

        dto.Should().BeEquivalentTo(package);
    }

    [Theory]
    [InlineData("Transport/Avro/_files/TimeSeriesData.avro")]
    public async Task Test_TimeSeriesData_Domain_Model_Roundtrip(string file)
    {
        await using var reader = new FileStream(
            file,
            FileMode.Open,
            FileAccess.Read,
            FileShare.Read
        );

        var package = DeserializeTimeSeriesData(reader);
        Assert.NotNull(package);

        var domainPackage = package!.ToDomainModel();
        var dto = domainPackage.ToAvroDto();

        dto.Should().BeEquivalentTo(package);
    }

    static TimeSeriesDataPackage DeserializeTimeSeriesData(Stream reader)
    {
        var avroReader = new SpecificReader<TimeSeriesDataPackage>(
            TimeSeriesDataPackage._SCHEMA,
            TimeSeriesDataPackage._SCHEMA
        );
        var decoder = new BinaryDecoder(reader);
        var package = avroReader.Read(new TimeSeriesDataPackage(), decoder);
        Assert.NotNull(package);
        return package;
    }

    static DataChannelListPackage DeserializeDataChannelList(Stream reader)
    {
        var avroReader = new SpecificReader<DataChannelListPackage>(
            DataChannelListPackage._SCHEMA,
            DataChannelListPackage._SCHEMA
        );
        var decoder = new BinaryDecoder(reader);
        var package = avroReader.Read(new DataChannelListPackage(), decoder);
        Assert.NotNull(package);
        return package;
    }

    static void SerializeTimeSeriesData(Stream writer, TimeSeriesDataPackage dto)
    {
        var avroWriter = new SpecificWriter<TimeSeriesDataPackage>(TimeSeriesDataPackage._SCHEMA);
        var encoder = new BinaryEncoder(writer);
        avroWriter.Write(dto, encoder);
    }

    static void SerializeDataChannelList(Stream writer, DataChannelListPackage dto)
    {
        var avroWriter = new SpecificWriter<DataChannelListPackage>(DataChannelListPackage._SCHEMA);
        var encoder = new BinaryEncoder(writer);
        avroWriter.Write(dto, encoder);
    }
}
