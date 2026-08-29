use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::gmod::TraversalHandlerResult;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid");

    c.bench_function("FullTraversal/iter", |b| b.iter(|| gmod.iter().count()));

    c.bench_function("FullTraversal/traverse", |b| {
        b.iter(|| {
            let mut count = 0usize;
            gmod.traverse(
                |_parents, _node| {
                    count += 1;
                    TraversalHandlerResult::Continue
                },
                1,
            );
            count
        })
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
