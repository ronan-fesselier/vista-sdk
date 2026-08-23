use std::ffi::{CStr, CString};
use std::marker::PhantomData;
use std::ptr::NonNull;

use crate::ffi::transport::serializable_document as ffi;

/// Discriminator for the value kind held by a [`SerializableDocument`].
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum DocumentKind {
    Null,
    Boolean,
    Integer,
    Double,
    String,
    Array,
    Object,
}

/// Self-contained JSON value tree for ISO 19848 `xs:any` extension points.
///
/// Represents arbitrary JSON content (null, boolean, integer, floating point, string,
/// array, or object). Insertion order of object keys is preserved to support lossless
/// round-trip serialization.
///
/// Use [`SerializableDocumentRef`] for zero-copy read access to array elements and
/// object values borrowed from a parent document.
///
/// # Adaptation
///
/// There is no built-in dependency on any JSON library. Adapt your own JSON type by
/// implementing `From<YourJsonType> for SerializableDocument` and
/// `From<SerializableDocument> for YourJsonType` in your own crate.
pub struct SerializableDocument {
    ptr: NonNull<ffi::dnv_vista_sdk_serializable_document_t>,
}

/// Zero-copy read-only view into a [`SerializableDocument`] node.
///
/// Obtained from [`SerializableDocument::find`], [`SerializableDocument::array_at`],
/// or [`SerializableDocument::object_value_at`]. Valid only as long as the owning
/// document lives. Call [`to_owned`](SerializableDocumentRef::to_owned) to produce an
/// independent deep copy.
pub struct SerializableDocumentRef<'a> {
    ptr: *const ffi::dnv_vista_sdk_serializable_document_t,
    _marker: PhantomData<&'a SerializableDocument>,
}

unsafe impl Send for SerializableDocument {}
unsafe impl Sync for SerializableDocument {}

impl SerializableDocument {
    /// Constructs a JSON null.
    pub fn null() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_null() };
        Self {
            ptr: NonNull::new(ptr).expect("null() returned null"),
        }
    }

    /// Constructs a JSON boolean.
    pub fn from_bool(value: bool) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_boolean(value as i32) };
        Self {
            ptr: NonNull::new(ptr).expect("from_boolean returned null"),
        }
    }

    /// Constructs a JSON integer.
    pub fn from_i64(value: i64) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_integer(value) };
        Self {
            ptr: NonNull::new(ptr).expect("from_integer returned null"),
        }
    }

    /// Constructs a JSON floating point number.
    pub fn from_f64(value: f64) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_double(value) };
        Self {
            ptr: NonNull::new(ptr).expect("from_double returned null"),
        }
    }

    /// Constructs a JSON string.
    pub fn from_str(value: &str) -> Self {
        let c = CString::new(value).expect("value contains a NUL byte");
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_string(c.as_ptr()) };
        Self {
            ptr: NonNull::new(ptr).expect("from_string returned null"),
        }
    }

    /// Constructs an empty JSON array.
    pub fn array() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_array() };
        Self {
            ptr: NonNull::new(ptr).expect("array() returned null"),
        }
    }

    /// Constructs an empty JSON object.
    pub fn object() -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_object() };
        Self {
            ptr: NonNull::new(ptr).expect("object() returned null"),
        }
    }

    /// Returns the value kind held by this document.
    pub fn kind(&self) -> DocumentKind {
        kind_from_ffi(unsafe { ffi::dnv_vista_sdk_serializable_document_kind(self.ptr.as_ptr()) })
    }

    /// Returns `true` if this document holds a JSON null.
    pub fn is_null(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_null(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON boolean.
    pub fn is_boolean(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_boolean(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON integer.
    pub fn is_integer(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_integer(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON floating point number.
    pub fn is_double(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_double(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON string.
    pub fn is_string(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_string(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON array.
    pub fn is_array(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_array(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON object.
    pub fn is_object(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_object(self.ptr.as_ptr()) }) != 0
    }

    /// Returns the boolean value.
    /// Returns `None` if this document is not a boolean.
    pub fn as_bool(&self) -> Option<bool> {
        if !self.is_boolean() {
            return None;
        }
        Some(
            (unsafe { ffi::dnv_vista_sdk_serializable_document_as_boolean(self.ptr.as_ptr()) })
                != 0,
        )
    }

    /// Returns the integer value.
    /// Returns `None` if this document is not an integer.
    pub fn as_i64(&self) -> Option<i64> {
        if !self.is_integer() {
            return None;
        }
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_integer(self.ptr.as_ptr()) })
    }

    /// Returns the floating point value.
    /// Returns `None` if this document is not a double.
    pub fn as_f64(&self) -> Option<f64> {
        if !self.is_double() {
            return None;
        }
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_double(self.ptr.as_ptr()) })
    }

    /// Returns the string value.
    /// Returns `None` if this document is not a string.
    pub fn as_str(&self) -> Option<&str> {
        if !self.is_string() {
            return None;
        }
        let raw = unsafe { ffi::dnv_vista_sdk_serializable_document_as_string(self.ptr.as_ptr()) };
        if raw.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(raw) }
                .to_str()
                .expect("invalid UTF-8 in document string"),
        )
    }

    /// Returns the number of elements in this array, or 0 if not an array.
    pub fn array_len(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_serializable_document_array_size(self.ptr.as_ptr()) }
    }

    /// Returns a borrowed view of the array element at `index`.
    /// Returns `None` if not an array or `index` is out of range.
    pub fn array_at(&self, index: usize) -> Option<SerializableDocumentRef<'_>> {
        let raw =
            unsafe { ffi::dnv_vista_sdk_serializable_document_array_at(self.ptr.as_ptr(), index) };
        if raw.is_null() {
            None
        } else {
            Some(SerializableDocumentRef {
                ptr: raw,
                _marker: PhantomData,
            })
        }
    }

    /// Returns the number of key/value pairs in this object, or 0 if not an object.
    pub fn object_len(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_serializable_document_object_size(self.ptr.as_ptr()) }
    }

    /// Returns the key at insertion-order `index`.
    /// Returns `None` if not an object or `index` is out of range.
    pub fn object_key_at(&self, index: usize) -> Option<&str> {
        let raw = unsafe {
            ffi::dnv_vista_sdk_serializable_document_object_key_at(self.ptr.as_ptr(), index)
        };
        if raw.is_null() {
            None
        } else {
            Some(
                unsafe { CStr::from_ptr(raw) }
                    .to_str()
                    .expect("invalid UTF-8 in object key"),
            )
        }
    }

    /// Returns a borrowed view of the object value at insertion-order `index`.
    /// Returns `None` if not an object or `index` is out of range.
    pub fn object_value_at(&self, index: usize) -> Option<SerializableDocumentRef<'_>> {
        let raw = unsafe {
            ffi::dnv_vista_sdk_serializable_document_object_value_at(self.ptr.as_ptr(), index)
        };
        if raw.is_null() {
            None
        } else {
            Some(SerializableDocumentRef {
                ptr: raw,
                _marker: PhantomData,
            })
        }
    }

    /// Looks up a key in this object.
    /// Returns `None` if not an object or `key` is not present.
    pub fn find(&self, key: &str) -> Option<SerializableDocumentRef<'_>> {
        let c = CString::new(key).expect("key contains a NUL byte");
        let raw =
            unsafe { ffi::dnv_vista_sdk_serializable_document_find(self.ptr.as_ptr(), c.as_ptr()) };
        if raw.is_null() {
            None
        } else {
            Some(SerializableDocumentRef {
                ptr: raw,
                _marker: PhantomData,
            })
        }
    }

    /// Returns `true` if this is an object and contains `key`.
    pub fn contains(&self, key: &str) -> bool {
        let c = CString::new(key).expect("key contains a NUL byte");
        (unsafe {
            ffi::dnv_vista_sdk_serializable_document_contains(self.ptr.as_ptr(), c.as_ptr())
        }) != 0
    }

    /// Sets a key/value pair on this object, preserving insertion order.
    ///
    /// If this document is not yet an object it is reset to an empty object first.
    /// If `key` already exists its value is replaced in place.
    /// Takes ownership of `value`.
    pub fn set(&mut self, key: &str, value: SerializableDocument) {
        let c = CString::new(key).expect("key contains a NUL byte");
        let val_ptr = value.ptr.as_ptr();
        std::mem::forget(value);
        unsafe {
            ffi::dnv_vista_sdk_serializable_document_set(self.ptr.as_ptr(), c.as_ptr(), val_ptr)
        };
    }

    /// Appends a value to this array.
    ///
    /// If this document is not yet an array it is reset to an empty array first.
    /// Takes ownership of `value`.
    pub fn push_back(&mut self, value: SerializableDocument) {
        let val_ptr = value.ptr.as_ptr();
        std::mem::forget(value);
        unsafe { ffi::dnv_vista_sdk_serializable_document_push_back(self.ptr.as_ptr(), val_ptr) };
    }

    pub(crate) fn into_raw(self) -> *mut ffi::dnv_vista_sdk_serializable_document_t {
        let raw = self.ptr.as_ptr();
        std::mem::forget(self);
        raw
    }
}

impl Drop for SerializableDocument {
    fn drop(&mut self) {
        unsafe { ffi::dnv_vista_sdk_serializable_document_free(self.ptr.as_ptr()) };
    }
}

impl Clone for SerializableDocument {
    fn clone(&self) -> Self {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_clone(self.ptr.as_ptr()) };
        Self {
            ptr: NonNull::new(ptr).expect("clone returned null"),
        }
    }
}

impl PartialEq for SerializableDocument {
    fn eq(&self, other: &Self) -> bool {
        (unsafe {
            ffi::dnv_vista_sdk_serializable_document_equals(self.ptr.as_ptr(), other.ptr.as_ptr())
        }) != 0
    }
}

impl Eq for SerializableDocument {}

impl std::fmt::Debug for SerializableDocument {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "SerializableDocument({:?})", self.kind())
    }
}

impl<'a> SerializableDocumentRef<'a> {
    pub(crate) fn from_ptr(ptr: *const ffi::dnv_vista_sdk_serializable_document_t) -> Self {
        Self {
            ptr,
            _marker: PhantomData,
        }
    }

    /// Produces an independent deep copy of this borrowed node.
    pub fn to_owned(&self) -> SerializableDocument {
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_clone(self.ptr) };
        SerializableDocument {
            ptr: NonNull::new(ptr).expect("clone returned null"),
        }
    }

    /// Returns the value kind of this node.
    pub fn kind(&self) -> DocumentKind {
        kind_from_ffi(unsafe { ffi::dnv_vista_sdk_serializable_document_kind(self.ptr) })
    }

    /// Returns `true` if this node holds a JSON null.
    pub fn is_null(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_null(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON boolean.
    pub fn is_boolean(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_boolean(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON integer.
    pub fn is_integer(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_integer(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON floating point number.
    pub fn is_double(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_double(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON string.
    pub fn is_string(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_string(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON array.
    pub fn is_array(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_array(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON object.
    pub fn is_object(&self) -> bool {
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_object(self.ptr) }) != 0
    }

    /// Returns the boolean value, or `None` if not a boolean.
    pub fn as_bool(&self) -> Option<bool> {
        if !self.is_boolean() {
            return None;
        }
        Some((unsafe { ffi::dnv_vista_sdk_serializable_document_as_boolean(self.ptr) }) != 0)
    }

    /// Returns the integer value, or `None` if not an integer.
    pub fn as_i64(&self) -> Option<i64> {
        if !self.is_integer() {
            return None;
        }
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_integer(self.ptr) })
    }

    /// Returns the floating point value, or `None` if not a double.
    pub fn as_f64(&self) -> Option<f64> {
        if !self.is_double() {
            return None;
        }
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_double(self.ptr) })
    }

    /// Returns the string value, or `None` if not a string.
    pub fn as_str(&self) -> Option<&str> {
        if !self.is_string() {
            return None;
        }
        let raw = unsafe { ffi::dnv_vista_sdk_serializable_document_as_string(self.ptr) };
        if raw.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(raw) }
                .to_str()
                .expect("invalid UTF-8 in document string"),
        )
    }

    /// Returns the number of elements in this array, or 0 if not an array.
    pub fn array_len(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_serializable_document_array_size(self.ptr) }
    }

    /// Returns a borrowed view of the array element at `index`.
    /// Returns `None` if not an array or `index` is out of range.
    pub fn array_at(&self, index: usize) -> Option<SerializableDocumentRef<'a>> {
        let raw = unsafe { ffi::dnv_vista_sdk_serializable_document_array_at(self.ptr, index) };
        if raw.is_null() {
            None
        } else {
            Some(SerializableDocumentRef {
                ptr: raw,
                _marker: PhantomData,
            })
        }
    }

    /// Returns the number of key/value pairs in this object, or 0 if not an object.
    pub fn object_len(&self) -> usize {
        unsafe { ffi::dnv_vista_sdk_serializable_document_object_size(self.ptr) }
    }

    /// Returns the key at insertion-order `index`, or `None` if out of range.
    pub fn object_key_at(&self, index: usize) -> Option<&str> {
        let raw =
            unsafe { ffi::dnv_vista_sdk_serializable_document_object_key_at(self.ptr, index) };
        if raw.is_null() {
            None
        } else {
            Some(
                unsafe { CStr::from_ptr(raw) }
                    .to_str()
                    .expect("invalid UTF-8 in object key"),
            )
        }
    }

    /// Returns a borrowed view of the object value at insertion-order `index`.
    /// Returns `None` if not an object or `index` is out of range.
    pub fn object_value_at(&self, index: usize) -> Option<SerializableDocumentRef<'a>> {
        let raw =
            unsafe { ffi::dnv_vista_sdk_serializable_document_object_value_at(self.ptr, index) };
        if raw.is_null() {
            None
        } else {
            Some(SerializableDocumentRef {
                ptr: raw,
                _marker: PhantomData,
            })
        }
    }

    /// Looks up a key in this object.
    /// Returns `None` if not an object or `key` is not present.
    pub fn find(&self, key: &str) -> Option<SerializableDocumentRef<'a>> {
        let c = CString::new(key).expect("key contains a NUL byte");
        let raw = unsafe { ffi::dnv_vista_sdk_serializable_document_find(self.ptr, c.as_ptr()) };
        if raw.is_null() {
            None
        } else {
            Some(SerializableDocumentRef {
                ptr: raw,
                _marker: PhantomData,
            })
        }
    }

    /// Returns `true` if this is an object and contains `key`.
    pub fn contains(&self, key: &str) -> bool {
        let c = CString::new(key).expect("key contains a NUL byte");
        (unsafe { ffi::dnv_vista_sdk_serializable_document_contains(self.ptr, c.as_ptr()) }) != 0
    }
}

impl<'a> std::fmt::Debug for SerializableDocumentRef<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "SerializableDocumentRef({:?})", self.kind())
    }
}

fn kind_from_ffi(k: ffi::dnv_vista_sdk_serializable_document_kind_t) -> DocumentKind {
    match k {
        ffi::dnv_vista_sdk_serializable_document_kind_t::Null => DocumentKind::Null,
        ffi::dnv_vista_sdk_serializable_document_kind_t::Boolean => DocumentKind::Boolean,
        ffi::dnv_vista_sdk_serializable_document_kind_t::Integer => DocumentKind::Integer,
        ffi::dnv_vista_sdk_serializable_document_kind_t::Double => DocumentKind::Double,
        ffi::dnv_vista_sdk_serializable_document_kind_t::String => DocumentKind::String,
        ffi::dnv_vista_sdk_serializable_document_kind_t::Array => DocumentKind::Array,
        ffi::dnv_vista_sdk_serializable_document_kind_t::Object => DocumentKind::Object,
    }
}
