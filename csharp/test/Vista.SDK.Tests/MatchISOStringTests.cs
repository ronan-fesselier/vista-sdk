namespace Vista.SDK.Tests;

public class MatchISOStringTests
{
    public record SmokeContext
    {
        public int Count { get; set; } = 0;
        public int Succeeded { get; set; } = 0;
        public List<(string LocalId, Exception? e)> Errors { get; } = new();
    }

    const string AllAllowedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~";

    public static IEnumerable<object[]> Valid_Characters =>
        AllAllowedCharacters.Split("", StringSplitOptions.RemoveEmptyEntries).Select(c => new object[] { c[0] });
    public static IEnumerable<object[]> Spot_Test_Data =>

        [
            ["test", true],
            ["TeST", true],
            ["with space", false],
            ["#%/*", false],
        ];

    [Theory]
    [MemberData(nameof(Valid_Characters))]
    public void AllValidCharacters(char c) => Assert.True(VIS.MatchISOSubString(c));

    [Fact]
    public void AllAllowedInOne() => Assert.True(VIS.MatchISOSubString(AllAllowedCharacters));

    [Theory]
    [MemberData(nameof(Spot_Test_Data))]
    public void SpotTests(string input, bool expected) => Assert.Equal(expected, VIS.MatchISOSubString(input));

    [Fact]
    public async Task SmokeTest_Parsing()
    {
        var file = File.OpenRead("testdata/LocalIds.txt");

        var reader = new StreamReader(file);

        var context = new SmokeContext();

        string? localIdStr;

        while ((localIdStr = await reader.ReadLineAsync()) is not null)
        {
            try
            {
                var match = VIS.MatchISOString(localIdStr);
                if (!match)
                    context.Errors.Add((localIdStr, null));
                else
                {
                    context.Succeeded++;
                }
            }
            catch (Exception ex)
            {
                context.Errors.Add((localIdStr, ex));
            }
            context.Count++;
        }

        if (context.Errors.Any())
        {
            context
                .Errors
                .ForEach(e =>
                {
                    Console.WriteLine($"Failed to parse {e.LocalId} with error {e.e?.Message ?? "Not a match"}");
                });
        }

        Assert.Empty(context.Errors);
        Assert.Equal(context.Succeeded, context.Count);
    }
}
