use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::transport::datachannel::data_channel_json;

static DCL_JSON: &str = include_str!("../../../../cpp/tests/transport/_files/DataChannelList.json");

fn bench(c: &mut Criterion) {
    let package = data_channel_json::from_json(DCL_JSON).expect("reference file must parse");
    let list = package
        .data_channel_list()
        .expect("package must have a DataChannelList");

    let short_id = list
        .at(0)
        .and_then(|ch| ch.channel_id().short_id().map(|s| s.to_string()))
        .expect("first channel must have a ShortId");

    let local_id_str = list
        .at(0)
        .and_then(|ch| ch.channel_id().local_id_string().map(|s| s.to_string()))
        .expect("first channel must have a LocalId string");

    let local_id = OwnedLocalId::from_string(&local_id_str).expect("LocalId string must parse");

    c.bench_function("ByShortId", |b| b.iter(|| list.from_short_id(&short_id)));

    c.bench_function("ByLocalId", |b| b.iter(|| list.from_local_id(&local_id)));
}

criterion_group!(benches, bench);
criterion_main!(benches);
