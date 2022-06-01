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
            new string[] { "511.11/C101.663i/C663.5/CS6d", "511.11/C101.663i/C663.6/CS6d" },
            new string[] { "511.11-1/C101.663i/C663.5/CS6d", "511.11-1/C101.663i/C663.6/CS6d" },
            new string[] { "511.11-2/C101.663i/C663.5/CS6d", "511.11-2/C101.663i/C663.6/CS6d" },
            new string[] { "511.11-3/C101.663i/C663.5/CS6d", "511.11-3/C101.663i/C663.6/CS6d" },
            new string[] { "511.11-4/C101.663i/C663.5/CS6d", "511.11-4/C101.663i/C663.6/CS6d" },
            new string[] { "511.11/C101.663i/C663.5-4/CS6d", "511.11/C101.663i/C663.6-4/CS6d" },
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
        var targetPath = gmodVersioning.ConvertPath(VisVersion.v3_4a, sourcePath, VisVersion.v3_5a);

        var nodesWithLocation = sourcePath
            .GetFullPath()
            .Where(n => n.Node.Location is not null)
            .Select(n => n.Node.Code)
            .ToArray();
        targetGmod.Traverse(
            (parents, node) =>
            {
                Assert.Null(node.Location);
                return TraversalHandlerResult.Continue;
            }
        );

        Assert.NotNull(sourcePath);
        Assert.Equal(inputPath, sourcePath?.ToString());

        Assert.True(parsedPath);
        Assert.Equal(expectedPath, parsedTargetPath?.ToString());

        Assert.NotNull(targetPath);
        Assert.Equal(expectedPath, targetPath?.ToString());
    }

    public static IEnumerable<string?[]> Valid_Test_Data_Node =>
        new string?[][]
        {
            new string?[] { "1014.211", null, "1014.211" },
            new string?[] { "323.5", null, "323.6" },
            new string?[] { "412.72", null, "412.7i" },
            new string?[] { "323.4", null, "323.5" },
            new string?[] { "323.51", null, "323.61" },
            new string?[] { "323.6", null, "323.7" },
            new string?[] { "C101.212", null, "C101.22" },
            new string?[] { "C101.22", null, "C101.93" },
            new string?[] { "511.31", null, "C121.1" },
            new string?[] { "C101.31", "5", "C101.31" }
        };

    [Theory]
    [MemberData(nameof(Valid_Test_Data_Node))]
    public void Test_GmodVersioning_ConvertNode(
        string inputCode,
        string? location,
        string expectedCode
    )
    {
        var (_, vis) = VISTests.GetVis();

        var gmod = vis.GetGmod(VisVersion.v3_4a);
        var targetGmod = vis.GetGmod(VisVersion.v3_5a);

        var sourceNode = gmod[inputCode] with { Location = location };
        var expectedNode = targetGmod[expectedCode] with { Location = location };

        var gmodVersioning = vis.GetGmodVersioning();

        var targetNode = gmodVersioning.ConvertNode(VisVersion.v3_4a, sourceNode, VisVersion.v3_5a);

        Assert.Equal(expectedNode.Code, targetNode.Code);
        Assert.Equal(expectedNode.Location, targetNode.Location);
        Assert.Equal(expectedNode, targetNode);
    }
}
