#![warn(missing_docs)]
//! Vista SDK - Rust bindings over the C API.
//!
//! Provides idiomatic Rust wrappers over the Vista SDK C API, covering VIS identifiers,
//! temporal types, transport DTOs, and query builders.

pub mod core;
pub mod features;
pub(crate) mod ffi;
pub mod query;
pub mod transport;
pub mod types;

pub use core::codebook::{Codebook, PositionValidationResult};
pub use core::codebook_name::codebook_names;
pub use core::codebook_name::CodebookName;
pub use core::codebooks::Codebooks;
pub use core::error::{clear_error, last_error, ErrorKind, VistaError};
pub use core::gmod::{Gmod, TraversalHandlerResult};
pub use core::gmod_individualizable_set::GmodIndividualizableSet;
pub use core::gmod_node::{GmodNode, GmodNodeRef};
pub use core::gmod_node_metadata::GmodNodeMetadata;
pub use core::gmod_path::{GmodPath, GmodPathRef};
pub use core::imo_number::ImoNumber;
pub use core::local_id::{LocalId, LocalIdRef};
pub use core::local_id_builder::{LocalIdBuilder, LocalIdBuilderRef};
pub use core::local_id_mqtt::MqttLocalId;
pub use core::location::Location;
pub use core::location_builder::LocationBuilder;
pub use core::location_group::LocationGroup;
pub use core::locations::Locations;
pub use core::metadata_tag::{MetadataTag, MetadataTagRef};
pub use core::parsing_errors::{ErrorEntry, ParsingErrors};
pub use core::relative_location::RelativeLocation;
pub use core::universal_id::{UniversalId, UniversalIdRef};
pub use core::universal_id_builder::{UniversalIdBuilder, UniversalIdBuilderRef};
pub use core::vis::Vis;
pub use core::vis_version::VisVersion;

pub use query::gmod_path_query::GmodPathQuery;
pub use query::gmod_path_query_builder::{GmodPathQueryBuilder, GmodPathQueryBuilderRef};
pub use query::local_id_query::LocalIdQuery;
pub use query::local_id_query_builder::{LocalIdQueryBuilder, LocalIdQueryBuilderRef};
pub use query::metadata_tags_query::MetadataTagsQuery;
pub use query::metadata_tags_query_builder::{
    MetadataTagsQueryBuilder, MetadataTagsQueryBuilderRef,
};

pub use transport::datachannel::data_channel::{
    ConfigurationReference, ConfigurationReferenceRef, DataChannel, DataChannelId,
    DataChannelIdRef, DataChannelList, DataChannelListPackage, DataChannelListRef, DataChannelRef,
    DataChannelType, DataChannelTypeRef, Format, FormatRef, Header, HeaderRef, NameObject,
    NameObjectRef, Package, PackageRef, Property, PropertyRef, Range, RangeRef, Restriction,
    RestrictionRef, Unit, UnitRef, VersionInformation, VersionInformationRef, WhiteSpace,
};
pub use transport::datachannel::data_channel_dto;
pub use transport::datachannel::data_channel_json;
pub use transport::iso19848::{
    DataChannelTypeName, DataChannelTypeNames, FormatDataType, FormatDataTypeRef, FormatDataTypes,
    Iso19848, Value,
};
pub use transport::iso19848_version::Iso19848Version;
pub use transport::serializable_document::{
    DocumentKind, SerializableDocument, SerializableDocumentRef, SerializableDocumentRefMut,
};
pub use transport::ship_id::ShipId;
pub use transport::timeseries::data_channel_id::{TsdChannelId, TsdChannelIdRef};
pub use transport::timeseries::time_series_data::{
    EventData, EventDataRef, EventDataSet, EventDataSetRef, TabularData, TabularDataRef,
    TabularDataSet, TabularDataSetRef, TimeSeriesData, TimeSeriesDataPackage, TimeSeriesDataRef,
    TsdConfigRef, TsdConfigRefRef, TsdHeader, TsdPackage, TsdTimeSpan, TsdTimeSpanRef,
    ValidateCallback, ValidateResult, ValidationResult,
};
pub use transport::timeseries::time_series_data_dto;
pub use transport::timeseries::time_series_data_json;

pub use types::date_time::{DateTime, DateTimeFormat};
pub use types::date_time_offset::DateTimeOffset;
pub use types::decimal::{Decimal, RoundingMode};
pub use types::time_span::TimeSpan;
