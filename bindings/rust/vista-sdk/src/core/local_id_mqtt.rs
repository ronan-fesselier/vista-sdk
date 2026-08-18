use std::ffi::CStr;
use std::str::FromStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::gmod_path::GmodPath;
use crate::core::local_id_builder::LocalIdBuilder;
use crate::core::metadata_tag::MetadataTag;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::local_id_mqtt as ffi;

/// MQTT-compatible LocalId implementation.
///
/// Provides MQTT-specific formatting over the same validated builder state as
/// [`crate::core::local_id::LocalId`]:
/// - Underscores instead of slashes in Gmod paths for topic compatibility
/// - No leading slash to match MQTT topic conventions
/// - No "meta/" prefix section for cleaner IoT topics
/// - Placeholder handling for missing components
pub struct MqttLocalId(*mut ffi::dnv_vista_sdk_local_id_mqtt_t);

impl Drop for MqttLocalId {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_local_id_mqtt_free(self.0) }
    }
}

impl MqttLocalId {
    /// Constructs MQTT LocalId from validated LocalIdBuilder
    pub fn create(builder: &LocalIdBuilder) -> Option<Self> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_create(builder.as_ffi_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(MqttLocalId(ptr))
        }
    }

    /// VIS version this MQTT LocalId was built for.
    pub fn version(&self) -> VisVersion {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_version(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_version returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in mqtt local id version");
        VisVersion::from_str(s).expect("unrecognized mqtt local id version string")
    }

    /// Primary item (Gmod path).
    pub fn primary_item(&self) -> &GmodPath {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_primary_item(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_primary_item returned NULL"
        );
        GmodPath::from_ptr(ptr)
    }

    /// Secondary item (Gmod path), if set.
    pub fn secondary_item(&self) -> Option<&GmodPath> {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_secondary_item(self.0) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPath::from_ptr(ptr))
        }
    }

    /// Metadata tag for the given codebook slot, if set.
    pub fn metadata_tag(&self, name: CodebookName) -> Option<&MetadataTag> {
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_mqtt_metadata_tag(self.0, codebook_names::to_ffi(name))
        };
        if ptr.is_null() {
            None
        } else {
            Some(MetadataTag::from_ptr(ptr))
        }
    }

    /// Get the Quantity metadata tag (`None` if not set)
    pub fn quantity(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Quantity)
    }

    /// Get the Content metadata tag (`None` if not set)
    pub fn content(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Content)
    }

    /// Get the Calculation metadata tag (`None` if not set)
    pub fn calculation(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Calculation)
    }

    /// Get the State metadata tag (`None` if not set)
    pub fn state(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::State)
    }

    /// Get the Command metadata tag (`None` if not set)
    pub fn command(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Command)
    }

    /// Get the Type metadata tag (`None` if not set)
    pub fn r#type(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Type)
    }

    /// Get the Position metadata tag (`None` if not set)
    pub fn position(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Position)
    }

    /// Get the Detail metadata tag (`None` if not set)
    pub fn detail(&self) -> Option<&MetadataTag> {
        self.metadata_tag(CodebookName::Detail)
    }

    /// Builder that produced this MQTT LocalId.
    pub fn builder(&self) -> &LocalIdBuilder {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_builder(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_builder returned NULL"
        );
        LocalIdBuilder::from_ptr(ptr)
    }
}

impl PartialEq for MqttLocalId {
    fn eq(&self, other: &Self) -> bool {
        self.builder() == other.builder()
    }
}

impl Eq for MqttLocalId {}

impl std::fmt::Display for MqttLocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_to_string(self.0) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_to_string returned NULL"
        );
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in mqtt local id string");
        let result = f.write_str(s);
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

impl std::fmt::Debug for MqttLocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "MqttLocalId({})", self)
    }
}
