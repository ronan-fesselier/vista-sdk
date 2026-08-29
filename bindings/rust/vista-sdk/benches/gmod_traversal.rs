use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid");

    c.bench_function("FullTraversal", |b| b.iter(|| gmod.iter().count()));
}

criterion_group!(benches, bench);
criterion_main!(benches);
