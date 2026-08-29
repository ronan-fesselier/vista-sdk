use criterion::{criterion_group, criterion_main, Criterion};
use std::hint::black_box;

use vista_sdk::*;

const SIMPLE: &str = "/dnv-v2/vis-3-4a/751/I101/meta/state-common.alarm";
const COMPLEX: &str =
    "/dnv-v2/vis-3-4a/1036.11/S90.3/S61/sec/1036.13i-1/C662.1/C661/meta/state-auto.control/detail-blow.off";

fn bench(c: &mut Criterion) {
    c.bench_function("Simple", |b| {
        b.iter(|| black_box(LocalId::from_string(black_box(SIMPLE))))
    });
    c.bench_function("Complex", |b| {
        b.iter(|| black_box(LocalId::from_string(black_box(COMPLEX))))
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
