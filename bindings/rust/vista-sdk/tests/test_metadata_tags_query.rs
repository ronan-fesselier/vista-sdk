use vista_sdk::*;

#[test]
fn empty_query_matches_any_local_id() {
    let query = MetadataTagsQueryBuilder::create().build();

    let local_id1 = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        .expect("valid LocalId string");
    assert!(query.r#match(&local_id1));

    let local_id2 =
        LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
            .expect("valid LocalId string");
    assert!(query.r#match(&local_id2));
}

#[test]
fn empty_query_with_exact_mode_matches_only_local_ids_with_no_tags() {
    let query = MetadataTagsQueryBuilder::create()
        .with_allow_other_tags(false)
        .build();

    let with_tags = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        .expect("valid LocalId string");
    assert!(!query.r#match(&with_tags));
}

#[test]
fn single_tag_subset_mode() {
    let query = MetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Content, "sea.water")
        .with_allow_other_tags(true)
        .build();

    let local_id1 = LocalId::from_string(
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
    )
    .expect("valid LocalId string");
    assert!(query.r#match(&local_id1));

    let local_id2 =
        LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
            .expect("valid LocalId string");
    assert!(!query.r#match(&local_id2));

    let local_id3 = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        .expect("valid LocalId string");
    assert!(!query.r#match(&local_id3));
}

#[test]
fn single_tag_exact_mode() {
    let query = MetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Content, "exhaust.gas")
        .with_allow_other_tags(false)
        .build();

    let local_id1 = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/cnt-exhaust.gas")
        .expect("valid LocalId string");
    assert!(query.r#match(&local_id1));

    let local_id2 =
        LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
            .expect("valid LocalId string");
    assert!(!query.r#match(&local_id2));
}

#[test]
fn multiple_tags_subset_mode() {
    let query = MetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Quantity, "temperature")
        .with_tag(CodebookName::Content, "exhaust.gas")
        .with_allow_other_tags(true)
        .build();

    let local_id1 = LocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(query.r#match(&local_id1));

    let local_id2 = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        .expect("valid LocalId string");
    assert!(!query.r#match(&local_id2));

    let local_id3 =
        LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-sea.water")
            .expect("valid LocalId string");
    assert!(!query.r#match(&local_id3));
}

#[test]
fn multiple_tags_exact_mode() {
    let query = MetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Quantity, "temperature")
        .with_tag(CodebookName::Content, "exhaust.gas")
        .with_allow_other_tags(false)
        .build();

    let local_id1 =
        LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
            .expect("valid LocalId string");
    assert!(query.r#match(&local_id1));

    let local_id2 = LocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(!query.r#match(&local_id2));
}

#[test]
fn from_local_id_subset_mode() {
    let source =
        LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
            .expect("valid LocalId string");

    let query = MetadataTagsQueryBuilder::from_local_id(&source, true).build();

    assert!(query.r#match(&source));

    let local_id2 = LocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(query.r#match(&local_id2));
}

#[test]
fn from_local_id_exact_mode() {
    let source =
        LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
            .expect("valid LocalId string");

    let query = MetadataTagsQueryBuilder::from_local_id(&source, false).build();

    assert!(query.r#match(&source));

    let local_id2 = LocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(!query.r#match(&local_id2));
}

#[test]
fn builder_immutability_with_tag_returns_new_builder() {
    let builder1 = MetadataTagsQueryBuilder::create();
    let builder2 = builder1.with_tag(CodebookName::Quantity, "temperature");

    let query1 = builder1.build();
    let query2 = builder2.build();

    let local_id = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        .expect("valid LocalId string");

    assert!(query1.r#match(&local_id));
    assert!(query2.r#match(&local_id));
}

#[test]
fn chained_with_tag_calls() {
    let query = MetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Quantity, "temperature")
        .with_tag(CodebookName::Content, "exhaust.gas")
        .with_tag(CodebookName::Position, "inlet")
        .with_allow_other_tags(false)
        .build();

    let local_id = LocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(query.r#match(&local_id));
}
