//! Optional integrations that are only compiled when their Cargo feature is enabled.
//!
//! Nothing here is exported at the crate root: feature-gated modules must not participate in the
//! crate's glob re-exports, otherwise a build without the feature would break.

/// `serde` interoperability (feature `serde`).
#[cfg(feature = "serde")]
pub mod serde_ext;
