use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path_no_loc = OwnedGmodPath::from_short_path("411.1/C101.72/I101", gmod, locations)
        .expect("failed to parse path without location");

    let path_with_loc = OwnedGmodPath::from_short_path("612.21-1/C701.13/S93", gmod, locations)
        .expect("failed to parse path with location");

    c.bench_function("ConvertPath", |b| {
        b.iter(|| vis.convert_path(VisVersion::V3_4a, &path_no_loc, VisVersion::V3_5a))
    });

    c.bench_function("ConvertPathWithLocation", |b| {
        b.iter(|| vis.convert_path(VisVersion::V3_4a, &path_with_loc, VisVersion::V3_5a))
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
