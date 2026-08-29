use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::transport::timeseries::time_series_data_json;

static TSD_JSON: &str = include_str!("../../../../cpp/tests/transport/_files/TimeSeriesData.json");

fn bench(c: &mut Criterion) {
    let package = time_series_data_json::from_json(TSD_JSON).expect("reference file must parse");

    c.bench_function("Serialize", |b| {
        b.iter(|| time_series_data_json::to_json(&package, false))
    });

    c.bench_function("Deserialize", |b| {
        b.iter(|| time_series_data_json::from_json(TSD_JSON))
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
