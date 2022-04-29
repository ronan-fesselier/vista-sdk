using System.Diagnostics.CodeAnalysis;
using System.Text;

using Microsoft.Extensions.ObjectPool;

namespace Vista.SDK.Internal;

internal static class StringBuilderPool
{
    private static readonly ObjectPool<StringBuilder> _pool =
        new DefaultObjectPoolProvider().CreateStringBuilderPool(
            initialCapacity: 128,
            maximumRetainedCapacity: 1024
        );

    public static StringBuilderLease Get() => new StringBuilderLease(_pool.Get());

    public struct StringBuilderLease : IDisposable
    {
        private StringBuilder? _builder;

        public readonly StringBuilder Builder
        {
            get
            {
                var builder = _builder;
                if (builder is null)
                    ThrowInvalidOperation();
                return builder;
            }
        }

        internal StringBuilderLease(StringBuilder builder) => _builder = builder;

        public void Dispose()
        {
            var builder = _builder;
            _builder = null;
            if (builder is not null)
                _pool.Return(builder);
        }

        public override string ToString()
        {
            var builder = _builder;
            if (builder is null)
                ThrowInvalidOperation();

            return builder.ToString();
        }

        [DoesNotReturn]
        private static void ThrowInvalidOperation() =>
            throw new InvalidOperationException(
                "Tried to access StringBuilder after it was returned to pool"
            );
    }
}

internal static class StringBuilderExtensions
{
    public static StringBuilder AppendIf<T>(
        this StringBuilder builder,
        in T? value,
        Func<T, string> appendValue,
        char? suffix = null
    )
    {
        if (value is null)
            return builder;

        builder.Append(appendValue(value));

        if (suffix is not null)
            builder.Append(suffix.Value);

        return builder;
    }

    public static StringBuilder AppendMeta(this StringBuilder builder, in MetadataTag? tag)
    {
        if (tag is null)
            return builder;

        tag.Value.ToString(builder);

        return builder;
    }
}
