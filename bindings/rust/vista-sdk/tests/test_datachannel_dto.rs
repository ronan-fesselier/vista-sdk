use vista_sdk::*;

static VALID_JSON: &str =
    include_str!("../../../../cpp/tests/transport/_files/DataChannelList.json");

#[test]
fn from_json_parses_reference_file() {
    let dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let _ = dto;
}

#[test]
fn from_json_empty_returns_err() {
    let result = data_channel_dto::from_json("");
    assert!(result.is_err());
}

#[test]
fn from_json_malformed_returns_err() {
    let result = data_channel_dto::from_json("{ not valid json");
    assert!(result.is_err());
}

#[test]
fn to_json_roundtrip() {
    let dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let json = data_channel_dto::to_json(&dto, false);
    assert!(!json.is_empty());
    let dto2 = data_channel_dto::from_json(&json).expect("roundtrip parse should succeed");
    let json2 = data_channel_dto::to_json(&dto2, false);
    assert_eq!(json, json2);
}

#[test]
fn dto_pkg_header_ship_id() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let header = pkg.header();
    let ship_id = header.ship_id();
    assert!(!ship_id.is_empty());
}

#[test]
fn dto_header_set_author_roundtrip() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header();
    header.set_author("test-author");
    let got = header.author();
    assert_eq!(got.as_deref(), Some("test-author"));
    header.clear_author();
    assert!(header.author().is_none());
}

#[test]
fn dto_header_cfg_ref_id_and_timestamp() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header();
    let cfg = header.cfg_ref();
    let id = cfg.id();
    assert!(!id.is_empty());
    let ts = cfg.timestamp();
    assert!(!ts.is_empty());
}

#[test]
fn dto_channel_list_count_and_first_local_id() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut list = pkg.channel_list();
    let count = list.count();
    assert!(count > 0);
    let mut ch = list.at(0).expect("first channel should exist");
    let id = ch.channel_id();
    let local_id = id.local_id();
    assert!(!local_id.is_empty());
}

#[test]
fn dto_channel_property_ch_type() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut list = pkg.channel_list();
    let mut ch = list.at(0).expect("first channel should exist");
    let mut prop = ch.property();
    let ch_type = prop.ch_type();
    let type_str = ch_type.type_str();
    assert!(!type_str.is_empty());
}

#[test]
fn dto_channel_property_format_type() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut list = pkg.channel_list();
    let mut ch = list.at(0).expect("first channel should exist");
    let mut prop = ch.property();
    let fmt = prop.format();
    let type_str = fmt.type_str();
    assert!(!type_str.is_empty());
}

#[test]
fn to_dto_and_back_to_domain() {
    let domain = data_channel_json::from_json(VALID_JSON).expect("domain parse should succeed");
    let dto = data_channel_dto::to_dto(&domain).expect("to_dto should succeed");
    let domain2 = data_channel_dto::to_domain(&dto).expect("to_domain should succeed");
    let json1 = data_channel_json::to_json(&domain, false);
    let json2 = data_channel_json::to_json(&domain2, false);
    assert_eq!(json1, json2);
}

fn with_first_property<
    F: FnOnce(&mut vista_sdk::transport::datachannel::data_channel_dto::DtoPropertyRef<'_>),
>(
    f: F,
) {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut list = pkg.channel_list();
    let mut ch = list.at(0).expect("first channel should exist");
    let mut prop = ch.property();
    f(&mut prop);
}

#[test]
fn header_ver_info_ensure_and_clear() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header();

    header.ensure_ver_info();
    assert!(header.has_ver_info());
    {
        let mut vi = header
            .ver_info()
            .expect("ver_info should exist after ensure");
        vi.set_naming_rule("dnv-v2");
        vi.set_naming_scheme_version("3.0a");
        assert_eq!(vi.naming_rule(), "dnv-v2");
        assert_eq!(vi.naming_scheme_version(), "3.0a");

        assert!(vi.has_reference_url());
        assert_eq!(vi.reference_url().as_deref(), Some("http://somewhere.net"));

        vi.set_reference_url("https://docs.vista.dnv.com");
        assert!(vi.has_reference_url());
        assert_eq!(
            vi.reference_url().as_deref(),
            Some("https://docs.vista.dnv.com")
        );

        vi.clear_reference_url();
        assert!(!vi.has_reference_url());
        assert!(vi.reference_url().is_none());
    }

    header.clear_ver_info();
    assert!(!header.has_ver_info());
}

#[test]
fn header_date_created_set_and_clear() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header();

    header.set_date_created("2026-09-20T00:00:00Z");
    assert!(header.has_date_created());
    assert_eq!(
        header.date_created().as_deref(),
        Some("2026-09-20T00:00:00Z")
    );

    header.clear_date_created();
    assert!(!header.has_date_created());
    assert!(header.date_created().is_none());
}

#[test]
fn cfg_ref_version_set_and_clear() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header();
    let mut cfg = header.cfg_ref();

    cfg.set_version("2.0");
    assert!(cfg.has_version());
    assert_eq!(cfg.version().as_deref(), Some("2.0"));

    cfg.clear_version();
    assert!(!cfg.has_version());
    assert!(cfg.version().is_none());
}

#[test]
fn channel_list_push_and_remove() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut list = pkg.channel_list();

    let before = list.count();
    {
        let mut ch = list.push();
        let mut id = ch.channel_id();
        id.set_local_id("/dnv-v2/vis-3-11a/411.1/meta/qty-temperature");
        id.set_short_id("Pushed");
    }
    assert_eq!(list.count(), before + 1);

    {
        let mut ch = list.at(before).expect("pushed channel should exist");
        assert_eq!(ch.channel_id().short_id().as_deref(), Some("Pushed"));
    }

    list.remove(before);
    assert_eq!(list.count(), before);
}

#[test]
fn channel_id_short_id_set_and_clear() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut list = pkg.channel_list();
    let mut ch = list.at(0).expect("first channel should exist");
    let mut id = ch.channel_id();

    id.set_short_id("SHORT1");
    assert!(id.has_short_id());
    assert_eq!(id.short_id().as_deref(), Some("SHORT1"));

    id.clear_short_id();
    assert!(!id.has_short_id());
}

#[test]
fn property_range_ensure_set_and_clear() {
    with_first_property(|prop| {
        {
            let range = prop.ensure_range();
            let _ = range;
        }
        assert!(prop.range().is_some());

        {
            let mut r = prop.range().expect("range should exist after ensure");
            r.set_low(-10.0);
            r.set_high(100.0);
            assert_eq!(r.low(), -10.0);
            assert_eq!(r.high(), 100.0);
        }

        prop.clear_range();
        assert!(prop.range().is_none());
    });
}

#[test]
fn property_unit_ensure_set_and_clear() {
    with_first_property(|prop| {
        {
            let u = prop.ensure_unit();
            let _ = u;
        }
        assert!(prop.unit().is_some());

        {
            let mut u = prop.unit().expect("unit should exist after ensure");
            u.set_symbol("m/s");
            assert_eq!(u.symbol(), "m/s");

            u.set_quantity_name("Speed");
            assert!(u.has_quantity_name());
            assert_eq!(u.quantity_name().as_deref(), Some("Speed"));

            u.clear_quantity_name();
            assert!(!u.has_quantity_name());
        }

        prop.clear_unit();
        assert!(prop.unit().is_none());
    });
}

#[test]
fn property_quality_coding_set_and_clear() {
    with_first_property(|prop| {
        prop.set_quality_coding("OPC_QUALITY");
        assert!(prop.has_quality_coding());
        assert_eq!(prop.quality_coding().as_deref(), Some("OPC_QUALITY"));

        prop.clear_quality_coding();
        assert!(!prop.has_quality_coding());
    });
}

#[test]
fn property_alert_priority_set_and_clear() {
    with_first_property(|prop| {
        prop.set_alert_priority("Alarm");
        assert!(prop.has_alert_priority());
        assert_eq!(prop.alert_priority().as_deref(), Some("Alarm"));

        prop.clear_alert_priority();
        assert!(!prop.has_alert_priority());
    });
}

#[test]
fn property_name_and_remarks_set_and_clear() {
    with_first_property(|prop| {
        prop.set_name("Main Engine");
        assert!(prop.has_name());
        assert_eq!(prop.name().as_deref(), Some("Main Engine"));
        prop.clear_name();
        assert!(!prop.has_name());

        prop.set_remarks("Location: ECR");
        assert!(prop.has_remarks());
        assert_eq!(prop.remarks().as_deref(), Some("Location: ECR"));
        prop.clear_remarks();
        assert!(!prop.has_remarks());
    });
}

#[test]
fn property_custom_properties_ensure_write_and_clear() {
    with_first_property(|prop| {
        {
            let mut custom = prop.ensure_custom_properties();
            custom.set("normalPressure", SerializableDocument::from_string("4.2"));
            custom.set("isCritical", SerializableDocument::from_bool(true));
        }
        assert!(prop.has_custom_properties());

        {
            let custom = prop
                .custom_properties()
                .expect("custom properties should exist");
            assert!(custom.contains("normalPressure"));
            assert!(custom.contains("isCritical"));
        }

        prop.clear_custom_properties();
        assert!(!prop.has_custom_properties());
    });
}

#[test]
fn property_custom_properties_roundtrip_through_json() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    {
        let mut pkg = dto.pkg();
        let mut list = pkg.channel_list();
        let mut ch = list.at(0).expect("first channel should exist");
        let mut prop = ch.property();
        let mut custom = prop.ensure_custom_properties();
        custom.set("auditTag", SerializableDocument::from_string("rust-test"));
    }

    let json = data_channel_dto::to_json(&dto, false);
    assert!(
        json.contains("auditTag") && json.contains("rust-test"),
        "serialized DTO should carry the value written through the mutable view: {json}"
    );

    let mut reparsed = data_channel_dto::from_json(&json).expect("reparse should succeed");
    let mut pkg = reparsed.pkg();
    let mut list = pkg.channel_list();
    let mut ch = list.at(0).expect("first channel should exist");
    let mut prop = ch.property();
    let custom = prop
        .custom_properties()
        .expect("custom properties should survive round-trip");
    assert!(custom.contains("auditTag"));
}

#[test]
fn property_ch_type_set_and_clear() {
    with_first_property(|prop| {
        let mut ct = prop.ch_type();
        ct.set_type("Average");
        ct.set_update_cycle(5.0);
        ct.set_calculation_period(60.0);
        assert_eq!(ct.type_str(), "Average");
        assert_eq!(ct.update_cycle(), Some(5.0));
        assert_eq!(ct.calculation_period(), Some(60.0));

        ct.clear_update_cycle();
        ct.clear_calculation_period();
        assert_eq!(ct.update_cycle(), None);
        assert_eq!(ct.calculation_period(), None);
    });
}

#[test]
fn format_restriction_enumeration_set_and_read() {
    with_first_property(|prop| {
        let mut fmt = prop.format();
        {
            let _r = fmt.ensure_restriction();
        }
        assert!(fmt.restriction().is_some());

        let mut r = fmt
            .restriction()
            .expect("restriction should exist after ensure");
        r.set_enumeration(&["On", "Off", "Standby"]);
        let values = r.enumeration();
        assert_eq!(values, vec!["On", "Off", "Standby"]);

        r.clear_enumeration();
        assert!(r.enumeration().is_empty());
    });
}

#[test]
fn format_restriction_numeric_facets_set_and_clear() {
    with_first_property(|prop| {
        let mut fmt = prop.format();
        fmt.set_type("Decimal");
        assert_eq!(fmt.type_str(), "Decimal");

        {
            let _r = fmt.ensure_restriction();
        }
        let mut r = fmt
            .restriction()
            .expect("restriction should exist after ensure");

        r.set_min_inclusive(0.0);
        r.set_max_inclusive(360.0);
        assert!(r.has_min_inclusive());
        assert_eq!(r.min_inclusive(), Some(0.0));
        assert_eq!(r.max_inclusive(), Some(360.0));

        r.clear_min_inclusive();
        r.clear_max_inclusive();
        assert!(!r.has_min_inclusive());
        assert!(!r.has_max_inclusive());
    });
}

#[test]
fn format_restriction_string_and_int_facets() {
    with_first_property(|prop| {
        let mut fmt = prop.format();
        {
            let _r = fmt.ensure_restriction();
        }
        let mut r = fmt
            .restriction()
            .expect("restriction should exist after ensure");

        r.set_pattern("[a-z]+");
        assert!(r.has_pattern());
        assert_eq!(r.pattern().as_deref(), Some("[a-z]+"));
        r.clear_pattern();
        assert!(!r.has_pattern());

        r.set_fraction_digits(3);
        assert_eq!(r.fraction_digits(), Some(3));
        r.clear_fraction_digits();
        assert_eq!(r.fraction_digits(), None);

        r.set_max_length(12);
        assert_eq!(r.max_length(), Some(12));
        r.clear_max_length();
        assert_eq!(r.max_length(), None);

        r.set_white_space("Collapse");
        assert!(r.has_white_space());
        assert_eq!(r.white_space().as_deref(), Some("Collapse"));
        r.clear_white_space();
        assert!(!r.has_white_space());
    });
}

#[test]
fn name_object_ensure_custom_and_clear() {
    let mut dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut list = pkg.channel_list();
    let mut ch = list.at(0).expect("first channel should exist");
    let mut id = ch.channel_id();

    id.ensure_name_object();
    {
        let mut no = id.name_object().expect("name object should exist");
        no.set_naming_rule("dnv-v2");
        assert_eq!(no.naming_rule(), "dnv-v2");

        {
            let mut custom = no.ensure_custom();
            custom.set("sensorModel", SerializableDocument::from_string("A-10"));
        }
        assert!(no.has_custom());

        {
            let custom = no.custom().expect("custom should exist");
            assert!(custom.contains("sensorModel"));
        }

        no.clear_custom();
        assert!(!no.has_custom());
    }

    id.clear_name_object();
    assert!(id.name_object().is_none());
}

#[test]
fn unit_custom_elements_ensure_write_and_clear() {
    with_first_property(|prop| {
        let mut u = prop.ensure_unit();
        {
            let mut custom = u.ensure_custom_elements();
            custom.set("siEquivalent", SerializableDocument::from_string("pascal"));
        }
        assert!(u.has_custom_elements());

        {
            let custom = u.custom_elements().expect("custom elements should exist");
            assert!(custom.contains("siEquivalent"));
        }

        u.clear_custom_elements();
        assert!(!u.has_custom_elements());
    });
}

/// Builds a valid domain package with the reference JSON-derived shape.
fn reference_domain_package() -> DataChannelListPackage {
    data_channel_dto::from_json(VALID_JSON)
        .and_then(|dto| data_channel_dto::to_domain(&dto))
        .expect("reference JSON should convert to a domain package")
}

#[test]
fn dto_to_domain_to_dto_is_lossless() {
    // domain -> DTO -> domain -> DTO must reach a fixed point: the second DTO must
    // serialize to the exact same JSON as the first one.
    let dto1 = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let domain = data_channel_dto::to_domain(&dto1).expect("to_domain should succeed");
    let dto2 = data_channel_dto::to_dto(&domain).expect("to_dto should succeed");

    let json1 = data_channel_dto::to_json(&dto1, false);
    let json2 = data_channel_dto::to_json(&dto2, false);
    assert_eq!(json1, json2, "domain -> dto -> domain must be lossless");
}

#[test]
fn dto_and_domain_json_serializers_agree() {
    let dto = data_channel_dto::from_json(VALID_JSON).expect("parse should succeed");
    let domain = data_channel_dto::to_domain(&dto).expect("to_domain should succeed");

    let from_dto = data_channel_dto::to_json(&dto, false);
    let from_domain = data_channel_json::to_json(&domain, false);
    assert_eq!(
        from_dto, from_domain,
        "dto_to_json and data_channel_json::to_json must agree on the same document"
    );
}

#[test]
fn domain_roundtrip_preserves_structure() {
    let domain = reference_domain_package();
    let dto = data_channel_dto::to_dto(&domain).expect("to_dto should succeed");
    let back = data_channel_dto::to_domain(&dto).expect("to_domain should succeed");

    let original = domain
        .data_channel_list()
        .expect("domain package should expose a data channel list");
    let rebuilt = back
        .data_channel_list()
        .expect("rebuilt package should expose a data channel list");

    assert_eq!(
        original.len(),
        rebuilt.len(),
        "round-trip must preserve the number of data channels"
    );

    for i in 0..original.len() {
        let a = original.at(i).expect("channel should exist");
        let b = rebuilt.at(i).expect("channel should exist");
        assert_eq!(a.channel_id().short_id(), b.channel_id().short_id());
        assert_eq!(
            a.channel_id().local_id_string(),
            b.channel_id().local_id_string()
        );
    }
}

#[test]
fn dto_patch_before_serialization_is_visible_in_json() {
    // The whole point of the DTO layer: patch fields that the domain model cannot express.
    let domain = reference_domain_package();
    let mut dto = data_channel_dto::to_dto(&domain).expect("to_dto should succeed");

    {
        let mut pkg = dto.pkg();
        let mut header = pkg.header();
        header.set_author("export-pipeline");

        let mut custom = header.ensure_custom_headers();
        custom.set(
            "exportedBy",
            SerializableDocument::from_string("vista-sdk-test"),
        );
    }

    let json = data_channel_dto::to_json(&dto, false);
    assert!(
        json.contains("export-pipeline"),
        "patched author must appear"
    );
    assert!(
        json.contains("exportedBy"),
        "patched custom header must appear"
    );
    assert!(json.contains("vista-sdk-test"), "patched value must appear");
}
