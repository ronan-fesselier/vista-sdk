use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();

    let _ = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let _ = vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid");

    c.bench_function("GmodCacheAccess_v3_4a", |b| {
        b.iter(|| vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid"))
    });

    c.bench_function("GmodCacheAccess_v3_7a", |b| {
        b.iter(|| vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid"))
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
