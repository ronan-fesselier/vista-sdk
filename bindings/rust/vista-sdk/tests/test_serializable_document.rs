use vista_sdk::transport::serializable_document::{DocumentKind, SerializableDocument};

#[test]
fn null_default_construction() {
    let doc = SerializableDocument::null();
    assert!(doc.is_null());
    assert_eq!(doc.kind(), DocumentKind::Null);
}

#[test]
fn boolean_construction() {
    let t = SerializableDocument::from_bool(true);
    let f = SerializableDocument::from_bool(false);

    assert!(t.is_boolean());
    assert_eq!(t.kind(), DocumentKind::Boolean);
    assert_eq!(t.as_bool(), Some(true));
    assert_eq!(f.as_bool(), Some(false));
}

#[test]
fn integer_construction() {
    let doc = SerializableDocument::from_i64(42);
    assert!(doc.is_integer());
    assert_eq!(doc.kind(), DocumentKind::Integer);
    assert_eq!(doc.as_i64(), Some(42));
}

#[test]
fn integer_negative_value() {
    let doc = SerializableDocument::from_i64(-1000);
    assert!(doc.is_integer());
    assert_eq!(doc.as_i64(), Some(-1000));
}

#[test]
fn double_construction() {
    let doc = SerializableDocument::from_f64(3.14);
    assert!(doc.is_double());
    assert_eq!(doc.kind(), DocumentKind::Double);
    let v = doc.as_f64().expect("should be double");
    assert!((v - 3.14).abs() < 1e-10);
}

#[test]
fn string_construction() {
    let doc = SerializableDocument::from_str("hello");
    assert!(doc.is_string());
    assert_eq!(doc.kind(), DocumentKind::String);
    assert_eq!(doc.as_str(), Some("hello"));
}

#[test]
fn array_static_factory() {
    let doc = SerializableDocument::array();
    assert!(doc.is_array());
    assert_eq!(doc.kind(), DocumentKind::Array);
    assert_eq!(doc.array_len(), 0);
}

#[test]
fn object_static_factory() {
    let doc = SerializableDocument::object();
    assert!(doc.is_object());
    assert_eq!(doc.kind(), DocumentKind::Object);
    assert_eq!(doc.object_len(), 0);
}

#[test]
fn kind_only_one_is_true() {
    let null = SerializableDocument::null();
    assert!(null.is_null());
    assert!(!null.is_boolean());
    assert!(!null.is_integer());
    assert!(!null.is_double());
    assert!(!null.is_string());
    assert!(!null.is_array());
    assert!(!null.is_object());
}

#[test]
fn as_bool_returns_none_if_not_boolean() {
    let doc = SerializableDocument::from_i64(1);
    assert_eq!(doc.as_bool(), None);
}

#[test]
fn as_i64_returns_none_if_not_integer() {
    let doc = SerializableDocument::from_f64(1.0);
    assert_eq!(doc.as_i64(), None);
}

#[test]
fn as_f64_returns_none_if_not_double() {
    let doc = SerializableDocument::from_str("3.14");
    assert_eq!(doc.as_f64(), None);
}

#[test]
fn as_str_returns_none_if_not_string() {
    let doc = SerializableDocument::from_bool(true);
    assert_eq!(doc.as_str(), None);
}

#[test]
fn array_at_returns_none_if_not_array() {
    let doc = SerializableDocument::null();
    assert!(doc.array_at(0).is_none());
}

#[test]
fn object_find_returns_none_if_not_object() {
    let doc = SerializableDocument::from_i64(0);
    assert!(doc.find("missing").is_none());
}

#[test]
fn object_set_and_find() {
    let mut doc = SerializableDocument::object();
    doc.set("key", SerializableDocument::from_str("value"));

    let found = doc.find("key").expect("key should exist");
    assert!(found.is_string());
    assert_eq!(found.as_str(), Some("value"));
}

#[test]
fn object_contains() {
    let mut doc = SerializableDocument::object();
    doc.set("x", SerializableDocument::from_i64(1));

    assert!(doc.contains("x"));
    assert!(!doc.contains("y"));
}

#[test]
fn object_find_returns_none_on_missing_key() {
    let doc = SerializableDocument::object();
    assert!(doc.find("missing").is_none());
}

#[test]
fn object_set_replaces_existing_key() {
    let mut doc = SerializableDocument::object();
    doc.set("k", SerializableDocument::from_i64(1));
    doc.set("k", SerializableDocument::from_i64(2));

    assert_eq!(doc.object_len(), 1);
    assert_eq!(doc.find("k").and_then(|v| v.as_i64()), Some(2));
}

#[test]
fn object_set_preserves_insertion_order() {
    let mut doc = SerializableDocument::object();
    doc.set("a", SerializableDocument::from_i64(1));
    doc.set("b", SerializableDocument::from_i64(2));
    doc.set("c", SerializableDocument::from_i64(3));

    assert_eq!(doc.object_len(), 3);
    assert_eq!(doc.object_key_at(0), Some("a"));
    assert_eq!(doc.object_key_at(1), Some("b"));
    assert_eq!(doc.object_key_at(2), Some("c"));
}

#[test]
fn object_set_on_non_object_resets_to_object() {
    let mut doc = SerializableDocument::from_str("was a string");
    doc.set("key", SerializableDocument::from_bool(true));

    assert!(doc.is_object());
    assert!(doc.contains("key"));
}

#[test]
fn array_push_back_appends() {
    let mut doc = SerializableDocument::array();
    doc.push_back(SerializableDocument::from_i64(10));
    doc.push_back(SerializableDocument::from_i64(20));

    assert_eq!(doc.array_len(), 2);
    assert_eq!(doc.array_at(0).and_then(|v| v.as_i64()), Some(10));
    assert_eq!(doc.array_at(1).and_then(|v| v.as_i64()), Some(20));
}

#[test]
fn array_push_back_on_non_array_resets_to_array() {
    let mut doc = SerializableDocument::from_bool(false);
    doc.push_back(SerializableDocument::from_i64(1));

    assert!(doc.is_array());
    assert_eq!(doc.array_len(), 1);
}

#[test]
fn equality_same_primitive_values() {
    assert_eq!(
        SerializableDocument::from_i64(42),
        SerializableDocument::from_i64(42)
    );
    assert_ne!(
        SerializableDocument::from_i64(42),
        SerializableDocument::from_i64(43)
    );
    assert_eq!(
        SerializableDocument::from_str("x"),
        SerializableDocument::from_str("x")
    );
    assert_ne!(
        SerializableDocument::from_str("x"),
        SerializableDocument::from_str("y")
    );
}

#[test]
fn equality_different_types_are_not_equal() {
    assert_ne!(
        SerializableDocument::from_i64(1),
        SerializableDocument::from_bool(true)
    );
    assert_ne!(
        SerializableDocument::from_f64(1.0),
        SerializableDocument::from_i64(1)
    );
}

#[test]
fn equality_objects_with_same_content() {
    let mut a = SerializableDocument::object();
    a.set("x", SerializableDocument::from_i64(1));

    let mut b = SerializableDocument::object();
    b.set("x", SerializableDocument::from_i64(1));

    assert_eq!(a, b);
}

#[test]
fn equality_objects_with_different_insertion_order_are_not_equal() {
    let mut a = SerializableDocument::object();
    a.set("x", SerializableDocument::from_i64(1));
    a.set("y", SerializableDocument::from_i64(2));

    let mut b = SerializableDocument::object();
    b.set("y", SerializableDocument::from_i64(2));
    b.set("x", SerializableDocument::from_i64(1));

    assert_ne!(a, b);
}

#[test]
fn clone_produces_independent_copy() {
    let mut original = SerializableDocument::object();
    original.set("k", SerializableDocument::from_i64(99));

    let mut copy = original.clone();
    copy.set("k", SerializableDocument::from_i64(0));

    assert_eq!(original.find("k").and_then(|v| v.as_i64()), Some(99));
}

#[test]
fn borrowed_ref_to_owned() {
    let mut doc = SerializableDocument::object();
    doc.set("k", SerializableDocument::from_i64(7));

    let owned = doc.find("k").expect("key should exist").to_owned();
    assert!(owned.is_integer());
    assert_eq!(owned.as_i64(), Some(7));
}

#[test]
fn array_at_out_of_range_returns_none() {
    let doc = SerializableDocument::array();
    assert!(doc.array_at(0).is_none());
}

#[test]
fn object_key_at_out_of_range_returns_none() {
    let doc = SerializableDocument::object();
    assert!(doc.object_key_at(0).is_none());
}

#[test]
fn nested_object() {
    let mut inner = SerializableDocument::object();
    inner.set("n", SerializableDocument::from_i64(42));

    let mut outer = SerializableDocument::object();
    outer.set("inner", inner);

    let inner_ref = outer.find("inner").expect("inner should exist");
    assert!(inner_ref.is_object());
    let n = inner_ref.find("n").expect("n should exist");
    assert_eq!(n.as_i64(), Some(42));
}
