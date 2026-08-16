use std::ffi::CStr;

use crate::ffi::core::gmod_node_metadata as ffi;

/// Metadata for a Gmod (Generic Product Model) node.
///
/// Borrowed from the parent [`crate::core::gmod_node::GmodNode`], valid as long as it is.
#[repr(transparent)]
pub struct GmodNodeMetadata(ffi::dnv_vista_sdk_gmod_node_metadata_t);

impl GmodNodeMetadata {
    pub(crate) fn from_ptr<'a>(ptr: *const ffi::dnv_vista_sdk_gmod_node_metadata_t) -> &'a Self {
        assert!(
            !ptr.is_null(),
            "gmod node metadata pointer must not be NULL"
        );
        unsafe { &*(ptr as *const GmodNodeMetadata) }
    }

    fn as_ffi_ptr(&self) -> *const ffi::dnv_vista_sdk_gmod_node_metadata_t {
        self as *const Self as *const ffi::dnv_vista_sdk_gmod_node_metadata_t
    }

    /// Broad classification of this node (e.g. `"ASSET FUNCTION"`).
    pub fn category(&self) -> &str {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_metadata_category(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_node_metadata_category returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod node metadata category")
    }

    /// Specific type within the category (e.g. `"PROPULSION"`).
    pub fn r#type(&self) -> &str {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_metadata_type(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_node_metadata_type returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod node metadata type")
    }

    /// Category and type concatenated (e.g. `"ASSET FUNCTION.PROPULSION"`).
    pub fn full_type(&self) -> &str {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_metadata_full_type(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_node_metadata_full_type returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod node metadata full type")
    }

    /// Human-readable name for this node.
    pub fn name(&self) -> &str {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_metadata_name(self.as_ffi_ptr()) };
        assert!(
            !ptr.is_null(),
            "dnv_vista_sdk_gmod_node_metadata_name returned NULL"
        );
        unsafe { CStr::from_ptr(ptr) }
            .to_str()
            .expect("invalid UTF-8 in gmod node metadata name")
    }

    /// Common-language name, if defined.
    pub fn common_name(&self) -> Option<&str> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_metadata_common_name(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(ptr) }
                .to_str()
                .expect("invalid UTF-8 in gmod node metadata common name"),
        )
    }

    /// Technical definition, if defined.
    pub fn definition(&self) -> Option<&str> {
        let ptr = unsafe { ffi::dnv_vista_sdk_gmod_node_metadata_definition(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(ptr) }
                .to_str()
                .expect("invalid UTF-8 in gmod node metadata definition"),
        )
    }

    /// Common-language definition, if defined.
    pub fn common_definition(&self) -> Option<&str> {
        let ptr =
            unsafe { ffi::dnv_vista_sdk_gmod_node_metadata_common_definition(self.as_ffi_ptr()) };
        if ptr.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(ptr) }
                .to_str()
                .expect("invalid UTF-8 in gmod node metadata common definition"),
        )
    }

    /// Whether this node represents an installation substructure, if the field is present.
    pub fn install_substructure(&self) -> Option<bool> {
        let mut out: std::os::raw::c_int = 0;
        let ok = unsafe {
            ffi::dnv_vista_sdk_gmod_node_metadata_install_substructure(self.as_ffi_ptr(), &mut out)
        };
        if ok != 0 {
            Some(out != 0)
        } else {
            None
        }
    }

    /// Number of normal assignment name entries.
    pub fn normal_assignment_name_count(&self) -> usize {
        unsafe {
            ffi::dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count(self.as_ffi_ptr())
        }
    }

    /// Normal assignment name `(key, value)` at `index`, or `None` if out of bounds.
    pub fn normal_assignment_name_at(&self, index: usize) -> Option<(&str, &str)> {
        let key_ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_key_at(
                self.as_ffi_ptr(),
                index,
            )
        };
        if key_ptr.is_null() {
            return None;
        }
        let value_ptr = unsafe {
            ffi::dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at(
                self.as_ffi_ptr(),
                index,
            )
        };
        assert!(
            !value_ptr.is_null(),
            "dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at returned NULL"
        );

        let key = unsafe { CStr::from_ptr(key_ptr) }
            .to_str()
            .expect("invalid UTF-8 in normal assignment name key");
        let value = unsafe { CStr::from_ptr(value_ptr) }
            .to_str()
            .expect("invalid UTF-8 in normal assignment name value");
        Some((key, value))
    }

    /// Iterator over all normal assignment name `(key, value)` pairs.
    pub fn normal_assignment_names(&self) -> impl Iterator<Item = (&str, &str)> {
        (0..self.normal_assignment_name_count()).map(move |i| {
            self.normal_assignment_name_at(i)
                .expect("index within count() must be valid")
        })
    }
}
