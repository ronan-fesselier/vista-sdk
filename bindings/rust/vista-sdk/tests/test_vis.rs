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
fn versions_are_ordered() {
    let versions = Vis::instance().versions();

    for w in versions.windows(2) {
        assert!(w[0] < w[1]);
    }
}

#[test]
fn concurrent_singleton_access_is_thread_safe() {
    use std::thread;

    let mut handles = Vec::new();

    for _ in 0..10 {
        handles.push(thread::spawn(move || {
            let vis = Vis::instance();
            let versions = vis.versions();
            assert!(!versions.is_empty());
            assert_eq!(vis.latest(), *versions.last().unwrap());
        }));
    }

    for h in handles {
        h.join().unwrap();
    }
}
