using FluentAssertions;
using Vista.SDK.Experimental.Transport;
using Vista.SDK.Experimental.Transport.DataList;

namespace Vista.SDK.Experimental.Tests.Transport;

public partial class IsoMessageTests
{
    public static DataListPackage TestDataListPackage =>
        new DataListPackage(
            new Package(
                new Header(
                    AssetIdentifier.Parse("IMO1234567"),
                    new ConfigurationReference("DataList.xml", "1.0", DateTimeOffset.Parse("2016-01-01T00:00:00Z")),
                    new VersionInformation("some_naming_rule", "2.0", "http://somewhere.net"),
                    "Author1",
                    DateTimeOffset.Parse("2015-12-01T00:00:00Z"),
                    new Dictionary<string, object>() { ["nr:CustomHeaderElement"] = "Vender specific headers" }
                ),
                new DataList(
                    new Data[]
                    {
                        new Data(
                            new Experimental.Transport.DataList.DataId(
                                PMSLocalId.Parse(
                                    "/dnv-v2-experimental/vis-3-6a/411.1/C101.661i-F/C621/meta/maint.cat-preventive/act.type-service"
                                ),
                                "0010",
                                new NameObject(
                                    "Naming_Rule",
                                    new Dictionary<string, object>()
                                    {
                                        ["nr:CustomNameObject"] = "Vender specific NameObject"
                                    }
                                )
                            ),
                            new Property(
                                new DataType("Inst", UpdateCycle: "1", CalculationPeriod: null),
                                new Format(
                                    "Decimal",
                                    new Restriction(
                                        Enumeration: null,
                                        FractionDigits: "1",
                                        Length: null,
                                        MaxExclusive: null,
                                        MaxInclusive: "200.0",
                                        MaxLength: null,
                                        MinExclusive: null,
                                        MinInclusive: "-150.0",
                                        MinLength: null,
                                        Pattern: null,
                                        TotalDigits: null,
                                        WhiteSpace: null
                                    )
                                ),
                                new Experimental.Transport.DataList.Range("1.0", "0.0"),
                                null,
                                "OPC_QUALITY",
                                AlertPriority: null,
                                "ME Air cooler No 1 - Fwd",
                                " Location: ECR, Manufacturer: AAA Company, Type: TYPE-AAA ",
                                new Dictionary<string, object>()
                                {
                                    ["nr:CustomPropertyElement"] = "Vender specific Property"
                                }
                            )
                        )
                    }
                )
            )
        );

    [Fact]
    public void Test_DataList()
    {
        var message = TestDataListPackage;

        Assert.NotNull(message);
    }

    [Fact]
    public void Test_DataList_Json()
    {
        var message = TestDataListPackage;

        var dto = Experimental.Transport.Json.DataList.Extensions.ToJsonDto(message);
        var message2 = Experimental.Transport.Json.DataList.Extensions.ToDomainModel(dto);

        message.Should().BeEquivalentTo(message2);
    }
}
