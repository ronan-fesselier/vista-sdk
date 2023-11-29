using Newtonsoft.Json.Bson;
using Vista.SDK.Experimental.Transport;

namespace Vista.SDK.Experimental.Tests.Transport;

public class DataIdTests
{
    [Theory]
    [InlineData("/dnv-v2/vis-3-4a/1031/meta/cnt-refrigerant/state-leaking")]
    [InlineData("/dnv-v2/vis-3-4a/1021.1i-6P/H123/meta/qty-volume/cnt-cargo/pos~percentage")]
    [InlineData("/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened")]
    [InlineData("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet")]
    [InlineData(
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/~propulsion.engine/~cooling.system/meta/qty-temperature/cnt-exhaust.gas/pos-inlet"
    )]
    [InlineData(
        "/dnv-v2-experimental/vis-3-6a/632.32i-1/S110/meta/cnt-high.temperature.fresh.water/maint.cat-preventive/act.type-overhaul"
    )]
    public void Test_Parse_Equality(string dataIdStr)
    {
        var d1 = DataId.Parse(dataIdStr);
        var d2 = DataId.Parse(dataIdStr);

        Assert.Equal(d1, d2);

        var dict = new Dictionary<DataId, string>() { { d1, dataIdStr } };

        Assert.True(dict.TryGetValue(d2, out _));
    }

    [Fact]
    public void Test_Parse_Inequality()
    {
        var d1 = DataId.Parse(
            "/dnv-v2-experimental/vis-3-6a/632.32i-1/S110/meta/cnt-high.temperature.fresh.water/maint.cat-preventive/act.type-overhaul"
        );
        var d2 = DataId.Parse("/dnv-v2/vis-3-4a/1031/meta/cnt-refrigerant/state-leaking");
        var d3 = DataId.Parse("some short id");

        Assert.NotEqual(d1, d2);

        var dict = new Dictionary<DataId, string>() { { d1, "" }, { d3, "" } };

        Assert.True(dict.TryGetValue(d1, out _));
        Assert.False(dict.TryGetValue(d2, out _));
        Assert.True(dict.TryGetValue(d3, out _));
    }

    [Fact]
    public void Test_Equality()
    {
        var d0 = DataId.Parse("/dnv-v2/vis-3-7a/621.11i/H122/meta/qty-volume/cnt-low.sulphur.heavy.fuel.oil");
        var d1 = DataId.Parse("/dnv-v2/vis-3-7a/621.11i/H122/meta/qty-volume/cnt-low.sulphur.heavy.fuel.oil");
        var d2 = DataId.Parse("/dnv-v2/vis-3-7a/621.11i-1/H122/meta/qty-volume/cnt-low.sulphur.heavy.fuel.oil");
        var d3 = DataId.Parse("/dnv-v2/vis-3-7a/621.11i-S/H122/meta/qty-volume/cnt-low.sulphur.heavy.fuel.oil");
        var d4 = DataId.Parse("/dnv-v2/vis-3-7a/621.11i-1S/H122/meta/qty-volume/cnt-low.sulphur.heavy.fuel.oil");
        // PMS LocalId
        var d5 = DataId.Parse(
            "/dnv-v2-experimental/vis-3-6a/632.32i-1/S110/meta/cnt-high.temperature.fresh.water/maint.cat-preventive/act.type-overhaul"
        );
        var d6 = DataId.Parse(
            "/dnv-v2-experimental/vis-3-6a/632.32i-1S/S110/meta/cnt-high.temperature.fresh.water/maint.cat-preventive/act.type-overhaul"
        );

        var dict = new Dictionary<DataId, string>()
        {
            { d1, d1.ToString() },
            { d2, d2.ToString() },
            { d3, d3.ToString() },
            { d4, d4.ToString() },
            { d5, d5.ToString() },
            { d6, d6.ToString() }
        };

        var size = dict.Count;
        Assert.Equal(6, size);

        AssertDataIdEquality(d0, d1);
        AssertDataIdEquality(d1, d1);
        AssertDataIdEquality(d2, d2);
        AssertDataIdEquality(d3, d3);
        AssertDataIdEquality(d4, d4);
        AssertDataIdEquality(d5, d5);
        AssertDataIdEquality(d6, d6);

        Assert.NotEqual(d1, d2);
        Assert.NotEqual(d1, d3);
        Assert.NotEqual(d1, d4);
        Assert.NotEqual(d1, d5);
        Assert.NotEqual(d1, d6);

        Assert.NotEqual(d2, d1);
        Assert.NotEqual(d2, d3);
        Assert.NotEqual(d2, d4);
        Assert.NotEqual(d2, d5);
        Assert.NotEqual(d2, d6);

        Assert.NotEqual(d3, d1);
        Assert.NotEqual(d3, d2);
        Assert.NotEqual(d3, d4);
        Assert.NotEqual(d3, d5);
        Assert.NotEqual(d3, d6);

        Assert.NotEqual(d4, d1);
        Assert.NotEqual(d4, d2);
        Assert.NotEqual(d4, d3);
        Assert.NotEqual(d4, d5);
        Assert.NotEqual(d4, d6);

        Assert.NotEqual(d5, d1);
        Assert.NotEqual(d5, d2);
        Assert.NotEqual(d5, d3);
        Assert.NotEqual(d5, d4);
        Assert.NotEqual(d5, d6);

        Assert.NotEqual(d6, d1);
        Assert.NotEqual(d6, d2);
        Assert.NotEqual(d6, d3);
        Assert.NotEqual(d6, d4);
        Assert.NotEqual(d6, d5);

        Assert.True(dict.TryGetValue(d1, out var d1s));
        Assert.True(dict.TryGetValue(d2, out var d2s));
        Assert.True(dict.TryGetValue(d3, out var d3s));
        Assert.True(dict.TryGetValue(d4, out var d4s));
        Assert.True(dict.TryGetValue(d5, out var d5s));
        Assert.True(dict.TryGetValue(d6, out var d6s));

        Assert.Equal(d1.ToString(), d1s);
        Assert.Equal(d2.ToString(), d2s);
        Assert.Equal(d3.ToString(), d3s);
        Assert.Equal(d4.ToString(), d4s);
        Assert.Equal(d5.ToString(), d5s);
        Assert.Equal(d6.ToString(), d6s);
    }

    private void AssertDataIdEquality(DataId d0, DataId d1)
    {
        Assert.Equal(d0, d1);
        Assert.Equal(d0.GetHashCode(), d1.GetHashCode());
    }
}
