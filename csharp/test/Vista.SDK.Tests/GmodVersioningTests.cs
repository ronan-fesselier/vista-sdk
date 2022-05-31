using Microsoft.Extensions.DependencyInjection;

using Vista.SDK;
using Vista.SDK.Tests;

namespace Vista.SDK.Tests;

public class GmodVersioningTests
{
    [Fact]
    public void Test_GmodVersioning_Loads()
    {
        // Arrange
        var (_, vis) = VISTests.GetVis();

        // Act
        var gmodVersioning = vis.GetGmodVersioning();

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
            new string[] { "511.331/C221", "511.31/C121.31/C221" },
            new string[] { "511.11-1/C101.663i/C663.5/CS6d", "511.31/C121.31/C221" },
            // These paths doesn't change
            new string[] { "411.1/C101.31-5", "411.1/C101.31-5" },
            new string[] { "1014.211/S110.1/S101", "1014.211/S110.1/S101" },
            new string[] { "441.1/E202", "441.1/E202" },
            new string[] { "621.21/S90", "621.21/S90" }
        };

    [Theory]
    [MemberData(nameof(Valid_Test_Data_Path))]
    public void Test_GmodVersioning_ConvertPath(string inputPath, string expectedPath)
    {
        var (_, vis) = VISTests.GetVis();
        var gmod = vis.GetGmod(VisVersion.v3_4a);
        var targetGmod = vis.GetGmod(VisVersion.v3_5a);
        var gmodVersioning = vis.GetGmodVersioning();

        var sourcePath = GmodPath.Parse(inputPath, gmod);
        var parsedPath = targetGmod.TryParsePath(expectedPath, out var parsedTargetPath);
        var targetPath = gmodVersioning.ConvertPath2(
            VisVersion.v3_4a,
            sourcePath,
            VisVersion.v3_5a
        );

        Assert.NotNull(sourcePath);
        Assert.Equal(inputPath, sourcePath?.ToString());

        Assert.True(parsedPath);
        Assert.Equal(expectedPath, parsedTargetPath?.ToString());

        // Assert.NotNull(targetPath);
        // Assert.Equal(expectedPath, targetPath?.ToString());
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
    public void Test_GmodVersioning_ConvertNode(
        string inputCode,
        string location,
        string expectedCode
    )
    {
        var (_, vis) = VISTests.GetVis();

        var gmod = vis.GetGmod(VisVersion.v3_4a);
        var targetGmod = vis.GetGmod(VisVersion.v3_5a);

        var sourceNode = gmod[inputCode] with { Location = location };
        var expectedNode = targetGmod[expectedCode];

        var gmodVersioning = vis.GetGmodVersioning();
        var versioningNode = gmodVersioning["3-5a"];

        var targetNode = gmodVersioning.ConvertNode(VisVersion.v3_4a, sourceNode, VisVersion.v3_5a);

        Assert.Equal(expectedNode.Code, targetNode.Code);
        Assert.Same(expectedNode, targetNode);
    }
}
