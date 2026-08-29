use criterion::{criterion_group, criterion_main, Criterion};
use std::hint::black_box;

use vista_sdk::transport::datachannel::data_channel_json;

static DCL_JSON: &str = include_str!("../../../../cpp/tests/transport/_files/DataChannelList.json");

fn bench(c: &mut Criterion) {
    let package = data_channel_json::from_json(DCL_JSON).expect("reference file must parse");

    c.bench_function("Serialize", |b| {
        b.iter(|| black_box(data_channel_json::to_json(&package, false)))
    });

    c.bench_function("Deserialize", |b| {
        b.iter(|| black_box(data_channel_json::from_json(black_box(DCL_JSON))))
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
