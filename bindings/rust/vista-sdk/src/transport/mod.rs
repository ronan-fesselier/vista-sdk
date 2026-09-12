//! ISO 19848 transport types: data channels, time series, and serializable documents.

/// Data channel list package types.
pub mod datachannel;
/// ISO 19848 package structures.
pub mod iso19848;
/// Enumeration of ISO 19848 versions.
pub mod iso19848_version;
/// Self-contained JSON value tree for `xs:any` extension points.
pub mod serializable_document;
/// Ship identifier (IMO number wrapper for transport).
pub mod ship_id;
/// Time series data package types.
pub mod timeseries;
