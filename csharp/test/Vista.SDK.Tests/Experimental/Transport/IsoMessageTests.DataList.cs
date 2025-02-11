using FluentAssertions;
using FluentAssertions.Equivalency;
using Vista.SDK.Experimental.Transport;
using Vista.SDK.Experimental.Transport.Json.DataList;
using Domain = Vista.SDK.Experimental.Transport.DataList;

namespace Vista.SDK.Experimental.Tests.Transport;

public partial class IsoMessageTests
{
    public static Domain.DataListPackage TestDataListPackage =>
        new Domain.DataListPackage(
            new Domain.Package(
                new Domain.Header(
                    AssetIdentifier.Parse("IMO1234567"),
                    new Domain.ConfigurationReference(
                        "DataList.xml",
                        "1.0",
                        DateTimeOffset.Parse("2016-01-01T00:00:00Z")
                    ),
                    new Domain.VersionInformation("some_naming_rule", "2.0", "http://somewhere.net"),
                    "Author1",
                    DateTimeOffset.Parse("2015-12-01T00:00:00Z"),
                    new Dictionary<string, object>() { ["nr:CustomHeaderElement"] = "Vender specific headers" }
                ),
                new Domain.DataList(

                    [
                        new Domain.Data(
                            new Domain.DataId(
                                PMSLocalId.Parse(
                                    "/dnv-v2-experimental/vis-3-6a/411.1/C101.661i-F/C621/meta/maint.cat-preventive/act.type-service"
                                ),
                                "0010",
                                new Domain.NameObject(
                                    "Naming_Rule",
                                    new Dictionary<string, object>()
                                    {
                                        ["nr:CustomNameObject"] = "Vender specific NameObject"
                                    }
                                )
                            ),
                            new Domain.Property(
                                new Domain.DataType("Inst", 1.0, null),
                                new Domain.Format(
                                    "Decimal",
                                    new Domain.Restriction(
                                        Enumeration: null,
                                        FractionDigits: 1,
                                        Length: null,
                                        MaxExclusive: null,
                                        MaxInclusive: 200.0,
                                        MaxLength: null,
                                        MinExclusive: null,
                                        MinInclusive: -150.0,
                                        MinLength: null,
                                        Pattern: null,
                                        TotalDigits: null,
                                        WhiteSpace: null
                                    )
                                ),
                                new Domain.Range(1.0, 0.0),
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
                    ]
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

        var dto = message.ToJsonDto();
        var message2 = dto.ToDomainModel();
        var localId1 = message.Package.DataList.Data[0].DataId.LocalId;
        var localId2 = message2.Package.DataList.Data[0].DataId.LocalId;
        localId1.Should().Be(localId2);
        Assert.StrictEqual(localId1, localId2);
        message2.Should().BeEquivalentTo(message, config => config.Using(new LocalIdEquivalency()));
        localId1.Should().Be(localId2);
        Assert.StrictEqual(localId1, localId2);
    }

    private sealed class LocalIdEquivalency : IEquivalencyStep
    {
        public EquivalencyResult Handle(
            Comparands comparands,
            IEquivalencyValidationContext context,
            IEquivalencyValidator nestedValidator
        )
        {
            if (comparands.Subject is ILocalId && comparands.Expectation is ILocalId)
            {
                comparands.Subject.Equals(comparands.Expectation).Should().BeTrue();
                return EquivalencyResult.AssertionCompleted;
            }

            return EquivalencyResult.ContinueWithNext;
        }
    }
}
