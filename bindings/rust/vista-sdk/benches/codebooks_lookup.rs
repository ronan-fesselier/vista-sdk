use std::collections::{BTreeMap, HashMap};

use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();
    let codebooks = vis
        .codebooks(VisVersion::V3_11a)
        .expect("V3_11a should be valid");

    let entries: &[(&str, usize)] = &[
        ("Quantity", 0),
        ("Content", 1),
        ("Position", 2),
        ("State", 3),
        ("Command", 4),
        ("Detail", 5),
        ("Calculation", 6),
        ("Type", 7),
    ];
    let keys = ["Quantity", "Position", "State", "Detail"];

    let hash_map: HashMap<&str, usize> = entries.iter().copied().collect();
    let btree_map: BTreeMap<&str, usize> = entries.iter().copied().collect();

    c.bench_function("StdUnorderedMap", |b| {
        b.iter(|| keys.iter().all(|k| hash_map.contains_key(k)))
    });

    c.bench_function("StdMap", |b| {
        b.iter(|| keys.iter().all(|k| btree_map.contains_key(k)))
    });

    c.bench_function("Codebooks", |b| {
        b.iter(|| {
            let _ = &codebooks[CodebookName::Quantity];
            let _ = &codebooks[CodebookName::Position];
            let _ = &codebooks[CodebookName::State];
            let _ = &codebooks[CodebookName::Detail];
        })
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
