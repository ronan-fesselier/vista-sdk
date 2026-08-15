use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

#[test]
fn to_string_returns_formatted_tag_without_trailing_separator() {
    let vis = Vis::instance();
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let quantity_tag = codebooks[CodebookName::Quantity].create_tag("temperature");
    assert!(quantity_tag.is_some());
    assert_eq!(quantity_tag.unwrap().to_string(), "qty-temperature");

    let content_tag = codebooks[CodebookName::Content].create_tag("exhaust.gas");
    assert!(content_tag.is_some());
    assert_eq!(content_tag.unwrap().to_string(), "cnt-exhaust.gas");

    let position_tag = codebooks[CodebookName::Position].create_tag("inlet");
    assert!(position_tag.is_some());
    assert_eq!(position_tag.unwrap().to_string(), "pos-inlet");
}
