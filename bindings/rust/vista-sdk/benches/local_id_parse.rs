use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::local_id::OwnedLocalId;

const SIMPLE: &str = "/dnv-v2/vis-3-4a/751/I101/meta/state-common.alarm";

const COMPLEX: &str =
    "/dnv-v2/vis-3-4a/1036.11/S90.3/S61/sec/1036.13i-1/C662.1/C661/meta/state-auto.control/detail-blow.off";

fn bench(c: &mut Criterion) {
    c.bench_function("Simple", |b| b.iter(|| OwnedLocalId::from_string(SIMPLE)));

    c.bench_function("Complex", |b| b.iter(|| OwnedLocalId::from_string(COMPLEX)));
}

criterion_group!(benches, bench);
criterion_main!(benches);
