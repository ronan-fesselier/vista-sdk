namespace Vista.SDK.Internal;

internal static class StringExtensions
{
    public static SplitEnumerator Split(this string str, char delimiter) =>
        new SplitEnumerator(str.AsSpan(), delimiter);

    public static SplitEnumerator Split(this ReadOnlySpan<char> str, char delimiter) =>
        new SplitEnumerator(str, delimiter);

    // Adapted from https://www.meziantou.net/split-a-string-into-lines-without-allocation.htm
    public ref struct SplitEnumerator
    {
        private ReadOnlySpan<char> _str;
        private readonly char _delimiter;

        public SplitEnumerator(ReadOnlySpan<char> str, char delimiter)
        {
            _str = str;
            _delimiter = delimiter;
            Current = default;
        }

        // Needed to be compatible with the foreach operator
        public SplitEnumerator GetEnumerator() => this;

        public bool MoveNext()
        {
            var span = _str;
            if (span.Length == 0) // Reach the end of the string
                return false;

            var index = span.IndexOf(_delimiter);
            if (index == -1) // The string is composed of only one segment
            {
                _str = ReadOnlySpan<char>.Empty; // The remaining string is an empty string
                Current = new LineSplitEntry(span, ReadOnlySpan<char>.Empty);
                return true;
            }

            Current = new LineSplitEntry(span.Slice(0, index), span.Slice(index, 1));
            _str = span.Slice(index + 1);
            return true;
        }

        public LineSplitEntry Current { get; private set; }
    }

    public readonly ref struct LineSplitEntry
    {
        public LineSplitEntry(ReadOnlySpan<char> line, ReadOnlySpan<char> separator)
        {
            Line = line;
            Separator = separator;
        }

        public ReadOnlySpan<char> Line { get; }
        public ReadOnlySpan<char> Separator { get; }

        // This method allow to deconstruct the type, so you can write any of the following code
        // foreach (var entry in str.SplitLines()) { _ = entry.Line; }
        // foreach (var (line, endOfLine) in str.SplitLines()) { _ = line; }
        // https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/functional/deconstruct?WT.mc_id=DT-MVP-5003978#deconstructing-user-defined-types
        public void Deconstruct(out ReadOnlySpan<char> line, out ReadOnlySpan<char> separator)
        {
            line = Line;
            separator = Separator;
        }

        // This method allow to implicitly cast the type into a ReadOnlySpan<char>, so you can write the following code
        // foreach (ReadOnlySpan<char> entry in str.SplitLines())
        public static implicit operator ReadOnlySpan<char>(LineSplitEntry entry) => entry.Line;
    }
}
