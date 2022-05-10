using Microsoft.Extensions.DependencyInjection;

using Vista.SDK;
using Vista.SDK.Tests;

namespace Vista.Tests.SDK;

public class GmodVersioningTests
{
    [Fact]
    public async Task Test_GmodVersioning_Loads()
    {
        // Arrange
        var (_, vis) = VISTests.GetVis();

        // Act
        var gmodVersioning = await vis.GetGmodVersioning();

        // Assert
        Assert.NotNull(gmodVersioning);
        Assert.True(gmodVersioning.TryGetVersioningNode("3-5a", out _));
        Assert.True(gmodVersioning.TryGetVersioningNode("3-4a", out _));
    }

    public static IEnumerable<object[]> Valid_Test_Data_Path =>
        new string[][]
        {
            new string[] { "411.1/C101.72/I101", "411.1/C101.72/I101" },
            new string[] { "323.51/H362.1", "323.61/H362.1" },
            new string[] { "321.38/C906", "321.39/C906" },
            new string[] { "511.331/C221", "C121.31/C221" },
            new string[] { "C101.22/S61", "C101.93/S61" },
            new string[] { "411.1/C101.31-5", "411.1/C101.31-5" },
            new string[] { "S203.2/S101", "S203.3/S110.1/S101" },
        };

    [Theory]
    [MemberData(nameof(Valid_Test_Data_Path))]
    public async Task Test_GmodVersioning_ConvertPath(string inputPath, string expectedPath)
    {
        var (_, vis) = VISTests.GetVis();
        var gmod = await vis.GetGmod(VisVersion.v3_4a);

        var sourcePath = gmod.ParsePath(inputPath);

        var gmodVersioning = await vis.GetGmodVersioning();

        var targetPath = await gmodVersioning.ConvertPath(
            VisVersion.v3_4a,
            sourcePath,
            VisVersion.v3_5a
        );

        Assert.NotNull(targetPath);
        Assert.Equal(expectedPath, targetPath?.ToString());
    }

    public static IEnumerable<string[]> Valid_Test_Data_Node =>
        new string[][]
        {
            new string[] { "1014.211", String.Empty, "1014.211" },
            new string[] { "323.5", String.Empty, "323.6" },
            new string[] { "412.72", String.Empty, "412.7i" },
            new string[] { "323.4", String.Empty, "323.5" },
            new string[] { "323.5", String.Empty, "323.6" },
            new string[] { "323.51", String.Empty, "323.61" },
            new string[] { "323.6", String.Empty, "323.7" },
            new string[] { "C101.212", String.Empty, "C101.22" },
            new string[] { "C101.22", String.Empty, "C101.93" },
            new string[] { "511.31", String.Empty, "C121.1" },
            new string[] { "C101.31", "5", "C101.31" }
        };

    [Theory]
    [MemberData(nameof(Valid_Test_Data_Node))]
    public async Task Test_GmodVersioning_ConvertNode(
        string inputCode,
        string location,
        string expectedCode
    )
    {
        var (_, vis) = VISTests.GetVis();

        var gmod = await vis.GetGmod(VisVersion.v3_4a);
        var targetGmod = await vis.GetGmod(VisVersion.v3_5a);

        var sourceNode = gmod[inputCode] with { Location = location };
        var expectedNode = targetGmod[expectedCode];

        var gmodVersioning = await vis.GetGmodVersioning();
        var versioningNode = gmodVersioning["3-5a"];

        var targetNode = await gmodVersioning.ConvertNode(
            VisVersion.v3_4a,
            sourceNode,
            VisVersion.v3_5a
        );

        Assert.Equal(expectedNode.Code, targetNode.Code);
        Assert.Same(expectedNode, targetNode);
    }
}
