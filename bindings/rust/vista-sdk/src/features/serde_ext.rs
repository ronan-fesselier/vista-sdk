//! Optional `serde` interoperability (feature `serde`).
//!
//! This module adds adapters around the C++ JSON writer/parser.
//! This module is only compiled when the `serde` feature is enabled.

use serde_json::Error as JsonError;

use crate::transport::serializable_document::{DocumentKind, SerializableDocument};

/// Converts a [`serde_json::Value`] into a [`SerializableDocument`].
///
/// Numbers become integers when they have no fractional part and fit in `i64`, otherwise doubles.
impl From<serde_json::Value> for SerializableDocument {
    fn from(value: serde_json::Value) -> Self {
        use serde_json::Value;

        match value {
            Value::Null => SerializableDocument::null(),
            Value::Bool(b) => SerializableDocument::from_bool(b),
            Value::Number(n) => {
                if let Some(i) = n.as_i64() {
                    SerializableDocument::from_i64(i)
                } else {
                    SerializableDocument::from_f64(n.as_f64().unwrap_or_default())
                }
            }
            Value::String(s) => SerializableDocument::from_string(&s),
            Value::Array(items) => {
                let mut array = SerializableDocument::array();
                for item in items {
                    array.push_back(item.into());
                }
                array
            }
            Value::Object(entries) => {
                let mut object = SerializableDocument::object();
                for (key, item) in entries {
                    object.set(&key, item.into());
                }
                object
            }
        }
    }
}

/// Converts a [`SerializableDocument`] into a [`serde_json::Value`].
///
/// Fails if the document holds a tree deeper than the recursion this conversion supports, or if an
/// object key is not valid UTF-8.
impl TryFrom<&SerializableDocument> for serde_json::Value {
    type Error = serde_json::Error;

    fn try_from(doc: &SerializableDocument) -> Result<Self, Self::Error> {
        use serde_json::Value;

        let value = match doc.kind() {
            DocumentKind::Null => Value::Null,
            DocumentKind::Boolean => Value::Bool(doc.as_bool().unwrap_or_default()),
            DocumentKind::Integer => Value::Number(doc.as_i64().unwrap_or_default().into()),
            DocumentKind::Double => serde_json::Number::from_f64(doc.as_f64().unwrap_or_default())
                .map(Value::Number)
                .unwrap_or(Value::Null),
            DocumentKind::String => Value::String(doc.as_str().unwrap_or_default().to_string()),
            DocumentKind::Array => {
                let mut items = Vec::with_capacity(doc.array_len());
                for index in 0..doc.array_len() {
                    let element = doc.array_at(index).ok_or_else(|| {
                        <JsonError as serde::de::Error>::custom("array element out of range")
                    })?;
                    items.push(Value::try_from(&element.to_owned())?);
                }
                Value::Array(items)
            }
            DocumentKind::Object => {
                let mut map = serde_json::Map::with_capacity(doc.object_len());
                for index in 0..doc.object_len() {
                    let key = doc.object_key_at(index).ok_or_else(|| {
                        <JsonError as serde::de::Error>::custom("object key out of range")
                    })?;
                    let value = doc.object_value_at(index).ok_or_else(|| {
                        <JsonError as serde::de::Error>::custom("object value out of range")
                    })?;
                    map.insert(key.to_string(), Value::try_from(&value.to_owned())?);
                }
                Value::Object(map)
            }
        };

        Ok(value)
    }
}

/// Serializes a transport package by delegating to the C++ JSON writer.
macro_rules! impl_serialize {
    ($type:ty, $to_json:path) => {
        impl serde::Serialize for $type {
            fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
                let json = $to_json(self, false);
                let value: serde_json::Value = serde_json::from_str(&json).map_err(|e| {
                    <S::Error as serde::ser::Error>::custom(format!(
                        "SDK produced invalid JSON: {e}"
                    ))
                })?;
                value.serialize(serializer)
            }
        }
    };
}

/// Strips a leading UTF-8 byte order mark, if present.
///
/// `serde_json` rejects a BOM at the start of a document, while the C++ parser behind this
/// SDK tolerates one. Removing it here keeps both paths in agreement.
fn strip_bom(json: &str) -> &str {
    json.strip_prefix('\u{feff}').unwrap_or(json)
}

/// Deserializes a transport package by delegating to the C++ JSON parser.
///
/// Accepts either:
/// - a JSON document string, the common case (`serde_json::from_str::<T>(json)`), or
/// - an already-structured value such as an object or array (`serde_json::from_value`).
///
/// Both shapes are normalized to a JSON string before being handed to the C++ parser, so
/// the C++ side remains the single source of truth for deserialization.
///
/// A leading UTF-8 byte order mark (`EF BB BF`) is stripped: `serde_json` rejects it, while
/// the underlying C++ parser tolerates it. Documents produced by Windows tooling or
/// exported from the SDK's own test fixtures commonly carry one.
macro_rules! impl_deserialize {
    ($type:ty, $from_json:path) => {
        impl<'de> serde::Deserialize<'de> for $type {
            fn deserialize<D: serde::Deserializer<'de>>(deserializer: D) -> Result<Self, D::Error> {
                use serde::de::{Error as _, Visitor};
                use serde::Deserialize as _;

                struct PackageVisitor;

                impl<'de> Visitor<'de> for PackageVisitor {
                    type Value = String;

                    fn expecting(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                        f.write_str("a JSON document string or a JSON value")
                    }

                    fn visit_str<E: serde::de::Error>(self, v: &str) -> Result<String, E> {
                        Ok(strip_bom(v).to_string())
                    }

                    fn visit_string<E: serde::de::Error>(self, v: String) -> Result<String, E> {
                        Ok(strip_bom(&v).to_string())
                    }

                    fn visit_map<A: serde::de::MapAccess<'de>>(
                        self,
                        access: A,
                    ) -> Result<String, A::Error> {
                        let value = serde_json::Value::deserialize(
                            serde::de::value::MapAccessDeserializer::new(access),
                        )?;
                        serde_json::to_string(&value).map_err(A::Error::custom)
                    }

                    fn visit_seq<A: serde::de::SeqAccess<'de>>(
                        self,
                        access: A,
                    ) -> Result<String, A::Error> {
                        let value = serde_json::Value::deserialize(
                            serde::de::value::SeqAccessDeserializer::new(access),
                        )?;
                        serde_json::to_string(&value).map_err(A::Error::custom)
                    }
                }

                let json = deserializer.deserialize_any(PackageVisitor)?;
                $from_json(&json).map_err(D::Error::custom)
            }
        }
    };
}

impl_serialize!(
    crate::transport::datachannel::data_channel::DataChannelListPackage,
    crate::transport::datachannel::data_channel_json::to_json
);
impl_deserialize!(
    crate::transport::datachannel::data_channel::DataChannelListPackage,
    crate::transport::datachannel::data_channel_json::from_json
);

impl_serialize!(
    crate::transport::timeseries::time_series_data::TimeSeriesDataPackage,
    crate::transport::timeseries::time_series_data_json::to_json
);
impl_deserialize!(
    crate::transport::timeseries::time_series_data::TimeSeriesDataPackage,
    crate::transport::timeseries::time_series_data_json::from_json
);

impl_serialize!(
    crate::transport::datachannel::data_channel_dto::DtoPackage,
    crate::transport::datachannel::data_channel_dto::to_json
);
impl_deserialize!(
    crate::transport::datachannel::data_channel_dto::DtoPackage,
    crate::transport::datachannel::data_channel_dto::from_json
);

impl_serialize!(
    crate::transport::timeseries::time_series_data_dto::TsdDtoPackage,
    crate::transport::timeseries::time_series_data_dto::to_json
);
impl_deserialize!(
    crate::transport::timeseries::time_series_data_dto::TsdDtoPackage,
    crate::transport::timeseries::time_series_data_dto::from_json
);

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    fn roundtrip(value: serde_json::Value) -> serde_json::Value {
        let document = SerializableDocument::from(value);
        serde_json::Value::try_from(&document).expect("conversion back should succeed")
    }

    #[test]
    fn scalars_roundtrip() {
        assert_eq!(roundtrip(json!(null)), json!(null));
        assert_eq!(roundtrip(json!(true)), json!(true));
        assert_eq!(roundtrip(json!(42)), json!(42));
        assert_eq!(roundtrip(json!("hello")), json!("hello"));
    }

    #[test]
    fn doubles_roundtrip() {
        assert_eq!(roundtrip(json!(1.5)), json!(1.5));
    }

    #[test]
    fn arrays_roundtrip_preserving_order() {
        assert_eq!(roundtrip(json!([1, 2, 3])), json!([1, 2, 3]));
        assert_eq!(roundtrip(json!([])), json!([]));
    }

    #[test]
    fn objects_roundtrip_preserving_insertion_order() {
        let value = json!({ "b": 1, "a": 2 });
        assert_eq!(roundtrip(value), json!({ "b": 1, "a": 2 }));
    }

    #[test]
    fn nested_roundtrip() {
        let value = json!({
            "sensorModel": "TempSensor-XYZ-2000",
            "accuracy": 0.001,
            "validated": true,
            "readings": [1, 2, 3],
            "meta": { "unit": "degC" }
        });
        assert_eq!(roundtrip(value.clone()), value);
    }
}
