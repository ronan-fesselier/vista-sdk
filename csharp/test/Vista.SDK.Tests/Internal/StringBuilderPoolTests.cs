using Vista.SDK.Internal;

namespace Vista.SDK.Tests.Internal;

public class StringBuilderPoolTests
{
    [Fact]
    public void Test_Roundtrip()
    {
        using var lease = StringBuilderPool.Get();

        lease.Builder.Append(1);

        Assert.Equal("1", lease.ToString());
    }

    [Fact]
    public void Test_Use_After_Free_Throws_Exception()
    {
        var lease = StringBuilderPool.Get();
        lease.Dispose();

        Assert.Throws<InvalidOperationException>(() => lease.ToString());
        Assert.Throws<InvalidOperationException>(() => lease.Builder);
    }

    [Fact]
    public void Test_Builder_Is_Cleaned()
    {
        var lease = StringBuilderPool.Get();

        var builder = lease.Builder;
        builder.Append('a');
        Assert.Equal(1, builder.Length);

        lease.Dispose();

        Assert.Equal(0, builder.Length);
    }
}
