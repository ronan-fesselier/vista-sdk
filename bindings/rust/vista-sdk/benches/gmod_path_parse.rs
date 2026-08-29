use std::cell::Cell;

use criterion::{criterion_group, criterion_main, Criterion};
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

const SHORT_PATHS: [&str; 6] = [
    "411.1/C101.72/I101",
    "511/C101.63/S90",
    "411.1/C101.63/S90",
    "621.21/C101.72/I101",
    "511/C101.72/I101",
    "411.1/C101.31/I101",
];

const FULL_PATHS: [&str; 6] = [
    "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101",
    "VE/500a/510/511/CS1/C101/C101.6/C101.63/S90",
    "VE/400a/410/411/411i/411.1/CS1/C101/C101.6/C101.63/S90",
    "VE/600a/620/621/621.2/621.2i/621.21/CS1/C101/C101.7/C101.72/I101",
    "VE/500a/510/511/CS1/C101/C101.7/C101.72/I101",
    "VE/400a/410/411/411i/411.1/CS1/C101/C101.3/C101.31/I101",
];

const SHORT_PATHS_INDIVIDUALIZED: [&str; 4] = [
    "612.21-1/C701.13/S93",
    "612.21-2/C701.13/S93",
    "612.21-1/C701.13/S90",
    "612.21-2/C701.13/S90",
];

const FULL_PATHS_INDIVIDUALIZED: [&str; 4] = [
    "VE/600a/610/612/612.2/612.2i/612.21-1/CS10/C701/C701.1/C701.13/S93",
    "VE/600a/610/612/612.2/612.2i/612.21-2/CS10/C701/C701.1/C701.13/S93",
    "VE/600a/610/612/612.2/612.2i/612.21-1/CS10/C701/C701.1/C701.13/S90",
    "VE/600a/610/612/612.2/612.2i/612.21-2/CS10/C701/C701.1/C701.13/S90",
];

fn bench(c: &mut Criterion) {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let short_paths_idx = Cell::new(0usize);
    c.bench_function("FromShortPath", |b| {
        b.iter(|| {
            let i = short_paths_idx.get();
            short_paths_idx.set((i + 1) % SHORT_PATHS.len());
            OwnedGmodPath::from_short_path(SHORT_PATHS[i], gmod, locations)
        })
    });

    let full_paths_idx = Cell::new(0usize);
    c.bench_function("FromFullPath", |b| {
        b.iter(|| {
            let i = full_paths_idx.get();
            full_paths_idx.set((i + 1) % FULL_PATHS.len());
            OwnedGmodPath::from_full_path(FULL_PATHS[i], gmod, locations)
        })
    });

    let short_individualized_idx = Cell::new(0usize);
    c.bench_function("FromShortPathIndividualized", |b| {
        b.iter(|| {
            let i = short_individualized_idx.get();
            short_individualized_idx.set((i + 1) % SHORT_PATHS_INDIVIDUALIZED.len());
            OwnedGmodPath::from_short_path(SHORT_PATHS_INDIVIDUALIZED[i], gmod, locations)
        })
    });

    let full_individualized_idx = Cell::new(0usize);
    c.bench_function("FromFullPathIndividualized", |b| {
        b.iter(|| {
            let i = full_individualized_idx.get();
            full_individualized_idx.set((i + 1) % FULL_PATHS_INDIVIDUALIZED.len());
            OwnedGmodPath::from_full_path(FULL_PATHS_INDIVIDUALIZED[i], gmod, locations)
        })
    });
}

criterion_group!(benches, bench);
criterion_main!(benches);
