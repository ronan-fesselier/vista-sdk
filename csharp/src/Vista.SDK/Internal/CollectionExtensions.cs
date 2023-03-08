#if NETSTANDARD2_0
namespace System.Collections.Generic
{
    //
    // Summary:
    //     Provides extension methods for generic collections.
    public static class CollectionExtensions
    {
        //
        // Summary:
        //     Tries to get the value associated with the specified key in the dictionary.
        //
        // Parameters:
        //   dictionary:
        //     A dictionary with keys of type TKey and values of type TValue.
        //
        //   key:
        //     The key of the value to get.
        //
        // Type parameters:
        //   TKey:
        //     The type of the keys in the dictionary.
        //
        //   TValue:
        //     The type of the values in the dictionary.
        //
        // Returns:
        //     A TValue instance. When the method is successful, the returned object is the
        //     value associated with the specified key. When the method fails, it returns the
        //     default value for TValue.
        //
        // Exceptions:
        //   T:System.ArgumentNullException:
        //     dictionary is null.
        public static TValue? GetValueOrDefault<TKey, TValue>(
            this IReadOnlyDictionary<TKey, TValue> dictionary,
            TKey key
        ) => dictionary.TryGetValue(key, out var v) ? v : default;

        //
        // Summary:
        //     Tries to get the value associated with the specified key in the dictionary.
        //
        // Parameters:
        //   dictionary:
        //     A dictionary with keys of type TKey and values of type TValue.
        //
        //   key:
        //     The key of the value to get.
        //
        //   defaultValue:
        //     The default value to return when the dictionary cannot find a value associated
        //     with the specified key.
        //
        // Type parameters:
        //   TKey:
        //     The type of the keys in the dictionary.
        //
        //   TValue:
        //     The type of the values in the dictionary.
        //
        // Returns:
        //     A TValue instance. When the method is successful, the returned object is the
        //     value associated with the specified key. When the method fails, it returns defaultValue.
        //
        // Exceptions:
        //   T:System.ArgumentNullException:
        //     dictionary is null.
        public static TValue GetValueOrDefault<TKey, TValue>(
            this IReadOnlyDictionary<TKey, TValue> dictionary,
            TKey key,
            TValue defaultValue
        ) => dictionary.TryGetValue(key, out var v) ? v : defaultValue;
    }
}
#endif
