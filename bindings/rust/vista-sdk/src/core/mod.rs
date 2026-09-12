//! VIS core types: identifiers, codebooks, graph model, and error handling.

/// A single VIS codebook (metadata tag collection).
pub mod codebook;
/// Enumeration of VIS codebook names.
pub mod codebook_name;
/// The full set of VIS codebooks for a given VIS version.
pub mod codebooks;
/// Error type returned by SDK operations.
pub mod error;
/// The Generic Product Model (Gmod) graph.
pub mod gmod;
/// A set of individualizable Gmod nodes.
pub mod gmod_individualizable_set;
/// A single node in the Gmod graph.
pub mod gmod_node;
/// Metadata associated with a Gmod node.
pub mod gmod_node_metadata;
/// A path through the Gmod graph.
pub mod gmod_path;
/// IMO vessel identification number.
pub mod imo_number;
/// A fully-qualified VIS local identifier.
pub mod local_id;
/// Builder for constructing a [`local_id::LocalId`].
pub mod local_id_builder;
/// MQTT-compatible representation of a local identifier.
pub mod local_id_mqtt;
/// A relative location within a VIS path.
pub mod location;
/// Builder for constructing a [`location::Location`].
pub mod location_builder;
/// Enumeration of location group categories.
pub mod location_group;
/// The set of all valid locations for a VIS version.
pub mod locations;
/// A metadata tag (codebook name + value pair).
pub mod metadata_tag;
/// Collection of typed parsing errors.
pub mod parsing_errors;
/// A relative location string (e.g. `"1P"`).
pub mod relative_location;
/// A universal VIS identifier (IMO number + local ID).
pub mod universal_id;
/// Builder for constructing a [`universal_id::UniversalId`].
pub mod universal_id_builder;
/// Entry point to load VIS data for a given version.
pub mod vis;
/// Enumeration of available VIS versions.
pub mod vis_version;
