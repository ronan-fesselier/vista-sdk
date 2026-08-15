use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

#[test]
fn instance_returns_same_object() {
    let instance1 = Vis::instance();
    let instance2 = Vis::instance();

    assert_eq!(instance1.versions(), instance2.versions());
    assert_eq!(instance1.latest(), instance2.latest());
}

#[test]
fn versions_returns_all_versions() {
    let versions = Vis::instance().versions();

    assert_eq!(versions, VisVersion::all());
}

#[test]
fn versions_are_ordered() {
    let versions = Vis::instance().versions();

    for w in versions.windows(2) {
        assert!(w[0] < w[1]);
    }
}

#[test]
fn latest_returns_latest_version() {
    assert_eq!(Vis::instance().latest(), VisVersion::latest());
}

#[test]
fn concurrent_singleton_access_is_thread_safe() {
    use std::thread;

    let mut handles = Vec::new();

    for _ in 0..10 {
        handles.push(thread::spawn(move || {
            let vis = Vis::instance();
            assert_eq!(vis.versions().len(), VisVersion::all().len());
            assert_eq!(vis.latest(), VisVersion::latest());
        }));
    }

    for h in handles {
        h.join().unwrap();
    }
}
