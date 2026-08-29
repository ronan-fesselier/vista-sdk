use criterion::{criterion_group, criterion_main, Criterion};
use std::hint::black_box;

use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();
    let codebooks = vis
        .codebooks(VisVersion::V3_11a)
        .expect("V3_11a should be valid");

    let quantity = &codebooks[CodebookName::Quantity];
    let state = &codebooks[CodebookName::State];
    let position = &codebooks[CodebookName::Position];

    c.bench_function("Quantity_hasStandardValue_hit", |b| {
        b.iter(|| quantity.has_standard_value(black_box("temperature")))
    });

    c.bench_function("Quantity_hasStandardValue_miss", |b| {
        b.iter(|| quantity.has_standard_value(black_box("not_a_real_quantity")))
    });

    c.bench_function("Quantity_createTag_standard", |b| {
        b.iter(|| quantity.create_tag(black_box("temperature")))
    });

    c.bench_function("Quantity_createTag_custom", |b| {
        b.iter(|| quantity.create_tag(black_box("custom_measurement")))
    });

    c.bench_function("State_hasGroup_hit", |b| {
        b.iter(|| state.has_group(black_box("Running")))
    });

    c.bench_function("State_hasGroup_miss", |b| {
        b.iter(|| state.has_group(black_box("NotARealGroup")))
    });

    c.bench_function("Position_validatePosition_simple", |b| {
        b.iter(|| position.validate_position(black_box("centre")))
    });

    c.bench_function("Position_validatePosition_composite", |b| {
        b.iter(|| position.validate_position(black_box("centre-starboard-2")))
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
