namespace Vista.SDK.Tests;

public class LocalIdQueryTests
{
    public sealed record InputData(string LocalId, LocalIdQuery Query, bool ExpectedMatch);

    public static IEnumerable<object[]> Test_Data =>

        [
            [
                new InputData(
                    "/dnv-v2/vis-3-4a/1021.1i-6P/H123/meta/qty-volume/cnt-cargo/pos~percentage",
                    LocalIdQueryBuilder.New().WithPrimaryItem(GmodPath.Parse("1021.1i-6P/H123", VisVersion.v3_4a), false).Build(),
                    true
                )
            ],
            [
                new InputData(
                    "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
                    LocalIdQueryBuilder.New().WithTag(CodebookName.Content, "sea.water").Build(),
                    true
                )
            ],
            [
                new InputData(
                    "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
                    LocalIdQueryBuilder.New().WithPrimaryItem(GmodPath.Parse("411.1/C101.31-1", VisVersion.v3_4a), true).Build(),
                    false
                )
            ],
            [
                new InputData(
                    "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/~propulsion.engine/~cooling.system/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
                    LocalIdQueryBuilder.New().WithPrimaryItem(GmodPath.Parse("411.1-2/C101.63/S206", VisVersion.v3_4a), false).Build(),
                    true
                )
            ],
            [
                new InputData(
                    "/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
                    LocalIdQueryBuilder.New().WithSecondaryItem(GmodPath.Parse("/411.1/C101.31-2", VisVersion.v3_4a), false).Build(),
                    true
                )
            ],
            [
                new InputData(
                    "/dnv-v2/vis-3-4a/511.11-21O/C101.67/S208/meta/qty-pressure/cnt-air/state-low",
                    LocalIdQueryBuilder.New().WithPrimaryItem(GmodPath.Parse("411.1", VisVersion.v3_4a), false).Build(),
                    false
                )
            ]
        ];

    [Theory]
    [MemberData(nameof(Test_Data))]
    public void Test_Matches(InputData data)
    {
        var localId = LocalId.Parse(data.LocalId);
        Assert.NotNull(localId);

        var match = data.Query.Match(localId);
        Assert.Equal(data.ExpectedMatch, match);
    }

    [Fact]
    public void Test_Happy_Path()
    {
        var localId = LocalId.Parse(
            "/dnv-v2/vis-3-4a/1036.13i-1/C662/sec/411.1-2/C101/meta/qty-pressure/cnt-cargo/state-high.high/pos-stage-3/detail-discharge"
        );

        var primaryItem = localId.PrimaryItem;
        var secondaryItem = localId.SecondaryItem!;

        // Match exact
        LocalIdQueryBuilder builder = LocalIdQueryBuilder.From(localId);
        LocalIdQuery query = builder.Build();
        Assert.True(query.Match(localId));

        MatchCombination(individualized: true);
        MatchCombination(individualized: false);

        void MatchCombination(bool individualized)
        {
            // Match primary
            builder = LocalIdQueryBuilder.New().WithPrimaryItem(primaryItem, individualized);
            query = builder.Build();
            Assert.True(query.Match(localId));

            // Match secondary
            builder = LocalIdQueryBuilder.New().WithSecondaryItem(secondaryItem, individualized);
            query = builder.Build();
            Assert.True(query.Match(localId));

            // Match tags
            builder = LocalIdQueryBuilder.New();
            foreach (var tag in localId.MetadataTags)
                builder = builder.WithTag(tag.Name, tag.Value);
            query = builder.Build();
            Assert.True(query.Match(localId));

            // Match primary and secondary
            builder = LocalIdQueryBuilder
                .New()
                .WithPrimaryItem(primaryItem, individualized)
                .WithSecondaryItem(secondaryItem, individualized);
            query = builder.Build();
            Assert.True(query.Match(localId));

            // Match primary and tags
            builder = LocalIdQueryBuilder.New().WithPrimaryItem(primaryItem, individualized);
            foreach (var tag in localId.MetadataTags)
                builder = builder.WithTag(tag.Name, tag.Value);
            query = builder.Build();
            Assert.True(query.Match(localId));

            // Match secondary and tags
            builder = LocalIdQueryBuilder.New().WithSecondaryItem(secondaryItem, individualized);
            foreach (var tag in localId.MetadataTags)
                builder = builder.WithTag(tag.Name, tag.Value);
            query = builder.Build();
            Assert.True(query.Match(localId));

            // Match primary, secondary, and tags
            builder = LocalIdQueryBuilder
                .New()
                .WithPrimaryItem(primaryItem, individualized)
                .WithSecondaryItem(secondaryItem, individualized);
            foreach (var tag in localId.MetadataTags)
                builder = builder.WithTag(tag.Name, tag.Value);
            query = builder.Build();
            Assert.True(query.Match(localId));
        }
    }

    [Fact]
    public void Test_Variations()
    {
        var localId = LocalId.Parse(
            "/dnv-v2/vis-3-4a/1036.13i-1/C662/sec/411.1-2/C101/meta/qty-pressure/cnt-cargo/state-high.high/pos-stage-3/detail-discharge"
        );

        var primaryItem = GmodPath.Parse("1036.13i-2/C662", VisVersion.v3_4a);

        var builder = LocalIdQueryBuilder.New().WithPrimaryItem(primaryItem, true);
        var query = builder.Build();
        Assert.False(query.Match(localId));

        builder = builder.WithPrimaryItem(primaryItem, false);
        query = builder.Build();
        Assert.True(query.Match(localId));

        primaryItem = primaryItem.WithoutLocations();
        builder = LocalIdQueryBuilder.New().WithPrimaryItem(primaryItem, true);
        query = builder.Build();
        Assert.False(query.Match(localId));

        builder = builder.WithPrimaryItem(primaryItem, false);
        query = builder.Build();
        Assert.True(query.Match(localId));
    }

    [Fact]
    public async void Test_Consistency_Smoke_Test()
    {
        await using (var file = File.OpenRead("testdata/LocalIds.txt"))
        {
            using var reader = new StreamReader(file, leaveOpen: true);

            var errored = new List<(string Path, Exception? exception)>();

            string? localIdStr;
            while ((localIdStr = await reader.ReadLineAsync()) is not null)
            {
                try
                {
                    var localId = LocalId.Parse(localIdStr);
                    var builder = LocalIdQueryBuilder.From(localId);
                    Assert.NotNull(builder);
                    var query = builder.Build();
                    Assert.NotNull(query);
                    var match = query.Match(localId);
                    if (match == false)
                        errored.Add((localIdStr, null));
                }
                catch (Exception e)
                {
                    errored.Add((localIdStr, e));
                }
            }
            if (errored.Count > 0)
            {
                foreach (var (path, exception) in errored)
                {
                    Console.WriteLine($"Failed on {path}");
                    if (exception != null)
                        Console.WriteLine(exception);
                }
            }
            Assert.Empty(errored);
        }
    }
}
