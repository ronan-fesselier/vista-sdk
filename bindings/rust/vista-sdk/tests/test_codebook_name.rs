use vista_sdk::core::codebook_name::{codebook_names, CodebookName};

#[test]
fn enum_values_are_sequential() {
    assert_eq!(CodebookName::Quantity as u8, 1);
    assert_eq!(CodebookName::Content as u8, 2);
    assert_eq!(CodebookName::Calculation as u8, 3);
    assert_eq!(CodebookName::State as u8, 4);
    assert_eq!(CodebookName::Command as u8, 5);
    assert_eq!(CodebookName::Type as u8, 6);
    assert_eq!(CodebookName::FunctionalServices as u8, 7);
    assert_eq!(CodebookName::MaintenanceCategory as u8, 8);
    assert_eq!(CodebookName::ActivityType as u8, 9);
    assert_eq!(CodebookName::Position as u8, 10);
    assert_eq!(CodebookName::Detail as u8, 11);
}

#[test]
fn from_prefix_valid_prefixes() {
    assert_eq!(
        codebook_names::from_prefix("qty"),
        Some(CodebookName::Quantity)
    );
    assert_eq!(
        codebook_names::from_prefix("cnt"),
        Some(CodebookName::Content)
    );
    assert_eq!(
        codebook_names::from_prefix("calc"),
        Some(CodebookName::Calculation)
    );
    assert_eq!(
        codebook_names::from_prefix("state"),
        Some(CodebookName::State)
    );
    assert_eq!(
        codebook_names::from_prefix("cmd"),
        Some(CodebookName::Command)
    );
    assert_eq!(
        codebook_names::from_prefix("type"),
        Some(CodebookName::Type)
    );
    assert_eq!(
        codebook_names::from_prefix("funct.svc"),
        Some(CodebookName::FunctionalServices)
    );
    assert_eq!(
        codebook_names::from_prefix("maint.cat"),
        Some(CodebookName::MaintenanceCategory)
    );
    assert_eq!(
        codebook_names::from_prefix("act.type"),
        Some(CodebookName::ActivityType)
    );
    assert_eq!(
        codebook_names::from_prefix("pos"),
        Some(CodebookName::Position)
    );
    assert_eq!(
        codebook_names::from_prefix("detail"),
        Some(CodebookName::Detail)
    );
}

#[test]
fn from_prefix_invalid_prefix_returns_none() {
    assert_eq!(codebook_names::from_prefix("invalid"), None);
}

#[test]
fn from_prefix_empty_string_returns_none() {
    assert_eq!(codebook_names::from_prefix(""), None);
}

#[test]
fn from_prefix_case_sensitive() {
    assert_eq!(codebook_names::from_prefix("QTY"), None);
    assert_eq!(codebook_names::from_prefix("Qty"), None);
    assert_eq!(codebook_names::from_prefix("CNT"), None);
}

#[test]
fn from_prefix_whitespace_not_trimmed() {
    assert_eq!(codebook_names::from_prefix(" qty"), None);
    assert_eq!(codebook_names::from_prefix("qty "), None);
    assert_eq!(codebook_names::from_prefix(" qty "), None);
}

#[test]
fn from_prefix_similar_but_invalid_prefixes_return_none() {
    assert_eq!(codebook_names::from_prefix("quantity"), None);
    assert_eq!(codebook_names::from_prefix("content"), None);
}

#[test]
fn to_prefix_valid_codebook_names() {
    assert_eq!(codebook_names::to_prefix(CodebookName::Quantity), "qty");
    assert_eq!(codebook_names::to_prefix(CodebookName::Content), "cnt");
    assert_eq!(codebook_names::to_prefix(CodebookName::Calculation), "calc");
    assert_eq!(codebook_names::to_prefix(CodebookName::State), "state");
    assert_eq!(codebook_names::to_prefix(CodebookName::Command), "cmd");
    assert_eq!(codebook_names::to_prefix(CodebookName::Type), "type");
    assert_eq!(
        codebook_names::to_prefix(CodebookName::FunctionalServices),
        "funct.svc"
    );
    assert_eq!(
        codebook_names::to_prefix(CodebookName::MaintenanceCategory),
        "maint.cat"
    );
    assert_eq!(
        codebook_names::to_prefix(CodebookName::ActivityType),
        "act.type"
    );
    assert_eq!(codebook_names::to_prefix(CodebookName::Position), "pos");
    assert_eq!(codebook_names::to_prefix(CodebookName::Detail), "detail");
}

#[test]
fn from_prefix_to_prefix_roundtrip() {
    let prefixes = [
        "qty",
        "cnt",
        "calc",
        "state",
        "cmd",
        "type",
        "funct.svc",
        "maint.cat",
        "act.type",
        "pos",
        "detail",
    ];

    for prefix in prefixes {
        let name = codebook_names::from_prefix(prefix).unwrap();
        assert_eq!(codebook_names::to_prefix(name), prefix);
    }
}
