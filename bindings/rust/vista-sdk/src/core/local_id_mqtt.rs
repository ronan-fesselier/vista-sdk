use std::ffi::CStr;
use std::str::FromStr;

use crate::core::codebook_name::{codebook_names, CodebookName};
use crate::core::gmod_path::GmodPathRef;
use crate::core::local_id_builder::LocalIdBuilderRef;
use crate::core::metadata_tag::MetadataTagRef;
use crate::core::vis_version::VisVersion;
use crate::ffi::core::local_id_mqtt as ffi;

/// MQTT-compatible LocalIdRef implementation.
///
/// Provides MQTT-specific formatting over the same validated builder state as
/// [`crate::core::local_id::LocalIdRef`]:
/// - Underscores instead of slashes in Gmod paths for topic compatibility
/// - No leading slash to match MQTT topic conventions
/// - No "meta/" prefix section for cleaner IoT topics
/// - Placeholder handling for missing components
pub struct MqttLocalId(std::ptr::NonNull<ffi::dnv_vista_sdk_local_id_mqtt_t>);

impl Drop for MqttLocalId {
    fn drop(&mut self) {
        // SAFETY: self.0 is owned by this `MqttLocalId` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_local_id_mqtt_free(self.0.as_ptr()) }
    }
}

impl MqttLocalId {
    /// Constructs MQTT LocalIdRef from validated LocalIdBuilderRef
    pub fn create(builder: &LocalIdBuilderRef) -> Option<Self> {
        // SAFETY: builder is non-null. Returns an owned pointer or NULL.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_create(builder.as_ffi_ptr()) };
        std::ptr::NonNull::new(ptr).map(MqttLocalId)
    }

    /// VIS version this MQTT LocalIdRef was built for.
    pub fn version(&self) -> VisVersion {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_version(self.0.as_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_version returned NULL"
        );
        // SAFETY: ptr is non-null and owned by `self` for its lifetime.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in mqtt local id version");
        VisVersion::from_str(s).expect("unrecognized mqtt local id version string")
    }

    /// Primary item (Gmod path).
    pub fn primary_item(&self) -> &GmodPathRef {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_primary_item(self.0.as_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_primary_item returned NULL"
        );
        GmodPathRef::from_ptr(ptr)
    }

    /// Secondary item (Gmod path), if set.
    pub fn secondary_item(&self) -> Option<&GmodPathRef> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_secondary_item(self.0.as_ptr()) };
        if ptr.is_null() {
            None
        } else {
            Some(GmodPathRef::from_ptr(ptr))
        }
    }

    /// Metadata tag for the given codebook slot, if set.
    pub fn metadata_tag(&self, name: CodebookName) -> Option<&MetadataTagRef> {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr = unsafe {
            ffi::dnv_vista_sdk_local_id_mqtt_metadata_tag(
                self.0.as_ptr(),
                codebook_names::to_ffi(name),
            )
        };
        if ptr.is_null() {
            None
        } else {
            Some(MetadataTagRef::from_ptr(ptr))
        }
    }

    /// Get the Quantity metadata tag (`None` if not set)
    pub fn quantity(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Quantity)
    }

    /// Get the Content metadata tag (`None` if not set)
    pub fn content(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Content)
    }

    /// Get the Calculation metadata tag (`None` if not set)
    pub fn calculation(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Calculation)
    }

    /// Get the State metadata tag (`None` if not set)
    pub fn state(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::State)
    }

    /// Get the Command metadata tag (`None` if not set)
    pub fn command(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Command)
    }

    /// Get the Type metadata tag (`None` if not set)
    pub fn r#type(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Type)
    }

    /// Get the Position metadata tag (`None` if not set)
    pub fn position(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Position)
    }

    /// Get the Detail metadata tag (`None` if not set)
    pub fn detail(&self) -> Option<&MetadataTagRef> {
        self.metadata_tag(CodebookName::Detail)
    }

    /// Builder that produced this MQTT LocalIdRef.
    pub fn builder(&self) -> &LocalIdBuilderRef {
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_builder(self.0.as_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_builder returned NULL"
        );
        LocalIdBuilderRef::from_ptr(ptr)
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
        // SAFETY: self.0 is non-null and valid for the lifetime of `self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_local_id_mqtt_to_string(self.0.as_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_local_id_mqtt_to_string returned NULL"
        );
        // SAFETY: ptr is non-null and owned until freed below.
        let s = unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in mqtt local id string");
        let result = f.write_str(s);
        // SAFETY: ptr was allocated by the library and is freed exactly once.
        unsafe { crate::ffi::core::common::dnv_vista_sdk_string_free(ptr) };
        result
    }
}

impl std::fmt::Debug for MqttLocalId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "MqttLocalId({})", self)
    }
}
