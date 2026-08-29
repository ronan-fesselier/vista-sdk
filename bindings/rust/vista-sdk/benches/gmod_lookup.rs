use std::collections::HashMap;

use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid");

    let mut map = HashMap::with_capacity(10000);
    for node in gmod.iter() {
        map.insert(node.code().to_string(), node);
    }

    c.bench_function("StdUnorderedMap", |b| {
        b.iter(|| {
            map.contains_key("VE")
                && map.contains_key("400a")
                && map.contains_key("400")
                && map.contains_key("H346.11112")
        })
    });

    c.bench_function("Gmod", |b| {
        b.iter(|| {
            gmod.get_node("VE").is_ok()
                && gmod.get_node("400a").is_ok()
                && gmod.get_node("400").is_ok()
                && gmod.get_node("H346.11112").is_ok()
        })
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
