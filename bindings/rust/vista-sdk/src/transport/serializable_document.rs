use std::ffi::{CStr, CString};
use std::marker::PhantomData;
use std::ptr::NonNull;

use crate::ffi::transport::serializable_document as ffi;

/// Discriminator for the value kind held by a [`SerializableDocument`].
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum DocumentKind {
    /// JSON `null`.
    Null,
    /// JSON boolean (`true` / `false`).
    Boolean,
    /// JSON integer (64-bit signed).
    Integer,
    /// JSON floating point number (64-bit double).
    Double,
    /// JSON string.
    String,
    /// JSON array.
    Array,
    /// JSON object (insertion-ordered key/value pairs).
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

// SAFETY: the wrapped pointer uniquely owns its C++ document tree with no shared mutable
// aliasing outside this type, so transferring or sharing that ownership across threads is
// sound (subject to the usual external synchronization for `Sync` access).
unsafe impl Send for SerializableDocument {}
unsafe impl Sync for SerializableDocument {}

impl SerializableDocument {
    /// Constructs a JSON null.
    pub fn null() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_null() };
        Self {
            ptr: NonNull::new(ptr).expect("null() returned null"),
        }
    }

    /// Constructs a JSON boolean.
    pub fn from_bool(value: bool) -> Self {
        // SAFETY: no pointer arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_boolean(value as i32) };
        Self {
            ptr: NonNull::new(ptr).expect("from_boolean returned null"),
        }
    }

    /// Constructs a JSON integer.
    pub fn from_i64(value: i64) -> Self {
        // SAFETY: no pointer arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_integer(value) };
        Self {
            ptr: NonNull::new(ptr).expect("from_integer returned null"),
        }
    }

    /// Constructs a JSON floating point number.
    pub fn from_f64(value: f64) -> Self {
        // SAFETY: no pointer arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_double(value) };
        Self {
            ptr: NonNull::new(ptr).expect("from_double returned null"),
        }
    }

    /// Constructs a JSON string.
    pub fn from_string(value: &str) -> Self {
        let c = CString::new(value).expect("value contains a NUL byte");
        // SAFETY: c is a valid NUL-terminated C string. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_from_string(c.as_ptr()) };
        Self {
            ptr: NonNull::new(ptr).expect("from_string returned null"),
        }
    }

    /// Constructs an empty JSON array.
    pub fn array() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_array() };
        Self {
            ptr: NonNull::new(ptr).expect("array() returned null"),
        }
    }

    /// Constructs an empty JSON object.
    pub fn object() -> Self {
        // SAFETY: no arguments. Returns a non-null owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_object() };
        Self {
            ptr: NonNull::new(ptr).expect("object() returned null"),
        }
    }

    /// Returns the value kind held by this document.
    pub fn kind(&self) -> DocumentKind {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        kind_from_ffi(unsafe { ffi::dnv_vista_sdk_serializable_document_kind(self.ptr.as_ptr()) })
    }

    /// Returns `true` if this document holds a JSON null.
    pub fn is_null(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_null(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON boolean.
    pub fn is_boolean(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_boolean(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON integer.
    pub fn is_integer(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_integer(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON floating point number.
    pub fn is_double(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_double(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON string.
    pub fn is_string(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_string(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON array.
    pub fn is_array(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_array(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this document holds a JSON object.
    pub fn is_object(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_object(self.ptr.as_ptr()) }) != 0
    }

    /// Returns the boolean value.
    /// Returns `None` if this document is not a boolean.
    pub fn as_bool(&self) -> Option<bool> {
        if !self.is_boolean() {
            return None;
        }
        Some(
            // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
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
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_integer(self.ptr.as_ptr()) })
    }

    /// Returns the floating point value.
    /// Returns `None` if this document is not a double.
    pub fn as_f64(&self) -> Option<f64> {
        if !self.is_double() {
            return None;
        }
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_double(self.ptr.as_ptr()) })
    }

    /// Returns the string value.
    /// Returns `None` if this document is not a string.
    pub fn as_str(&self) -> Option<&str> {
        if !self.is_string() {
            return None;
        }
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        let raw = unsafe { ffi::dnv_vista_sdk_serializable_document_as_string(self.ptr.as_ptr()) };
        if raw.is_null() {
            return None;
        }
        // `unwrap_or` keeps the borrow tied to the document (never panics), which `to_string_lossy`
        // could not do here without allocating.
        Some(
            // SAFETY: raw is non-null (checked above) and owned by `self` for its lifetime.
            unsafe { CStr::from_ptr(raw) }.to_str().unwrap_or(""),
        )
    }

    /// Returns the number of elements in this array, or 0 if not an array.
    pub fn array_len(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_serializable_document_array_size(self.ptr.as_ptr()) }
    }

    /// Returns a borrowed view of the array element at `index`.
    /// Returns `None` if not an array or `index` is out of range.
    pub fn array_at(&self, index: usize) -> Option<SerializableDocumentRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
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
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        unsafe { ffi::dnv_vista_sdk_serializable_document_object_size(self.ptr.as_ptr()) }
    }

    /// Returns the key at insertion-order `index`.
    /// Returns `None` if not an object or `index` is out of range.
    pub fn object_key_at(&self, index: usize) -> Option<&str> {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
        let raw = unsafe {
            ffi::dnv_vista_sdk_serializable_document_object_key_at(self.ptr.as_ptr(), index)
        };
        if raw.is_null() {
            None
        } else {
            Some(
                // SAFETY: raw is non-null (checked above) and owned by `self` for its lifetime.
                unsafe { CStr::from_ptr(raw) }.to_str().unwrap_or(""),
            )
        }
    }

    /// Returns a borrowed view of the object value at insertion-order `index`.
    /// Returns `None` if not an object or `index` is out of range.
    pub fn object_value_at(&self, index: usize) -> Option<SerializableDocumentRef<'_>> {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`.
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
    /// Returns `None` if not an object, `key` is not present, or `key` contains an interior NUL
    /// byte (which cannot be encoded as a C string).
    pub fn find(&self, key: &str) -> Option<SerializableDocumentRef<'_>> {
        let c = c_key(key)?;
        // SAFETY: self.ptr is non-null, and c is a valid NUL-terminated C string.
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
    ///
    /// Returns `false` when `key` contains an interior NUL byte.
    pub fn contains(&self, key: &str) -> bool {
        let Some(c) = c_key(key) else {
            return false;
        };
        // SAFETY: self.ptr is non-null, and c is a valid NUL-terminated C string.
        (unsafe {
            ffi::dnv_vista_sdk_serializable_document_contains(self.ptr.as_ptr(), c.as_ptr())
        }) != 0
    }

    /// Sets a key/value pair on this object, preserving insertion order.
    ///
    /// If this document is not yet an object it is reset to an empty object first.
    /// If `key` already exists its value is replaced in place.
    /// Takes ownership of `value`.
    ///
    /// # Panics
    /// Panics if `key` contains an interior NUL byte. Use
    /// [`try_set`](SerializableDocument::try_set) to handle that case explicitly.
    pub fn set(&mut self, key: &str, value: SerializableDocument) {
        if !self.try_set(key, value) {
            panic!("key contains an interior NUL byte");
        }
    }

    /// Fallible variant of [`set`](SerializableDocument::set).
    ///
    /// Returns `false` (and does not take ownership of `value`) when `key` contains an interior
    /// NUL byte.
    pub fn try_set(&mut self, key: &str, value: SerializableDocument) -> bool {
        let Some(c) = c_key(key) else {
            return false;
        };
        let val_ptr = value.ptr.as_ptr();
        std::mem::forget(value);
        // SAFETY: self.ptr and val_ptr are non-null, c is a valid NUL-terminated C string, and
        // val_ptr's ownership is transferred to the C++ document (value was `mem::forget`-ten).
        unsafe {
            ffi::dnv_vista_sdk_serializable_document_set(self.ptr.as_ptr(), c.as_ptr(), val_ptr)
        };
        true
    }

    /// Appends a value to this array.
    ///
    /// If this document is not yet an array it is reset to an empty array first.
    /// Takes ownership of `value`.
    pub fn push_back(&mut self, value: SerializableDocument) {
        let val_ptr = value.ptr.as_ptr();
        std::mem::forget(value);
        // SAFETY: self.ptr and val_ptr are non-null, and val_ptr's ownership is transferred
        // to the C++ document (value was `mem::forget`-ten).
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
        // SAFETY: self.ptr is owned by this `SerializableDocument` and freed exactly once.
        unsafe { ffi::dnv_vista_sdk_serializable_document_free(self.ptr.as_ptr()) };
    }
}

impl Clone for SerializableDocument {
    fn clone(&self) -> Self {
        // SAFETY: self.ptr is non-null and valid for the lifetime of `&self`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_clone(self.ptr.as_ptr()) };
        Self {
            ptr: NonNull::new(ptr).expect("clone returned null"),
        }
    }
}

impl PartialEq for SerializableDocument {
    fn eq(&self, other: &Self) -> bool {
        // SAFETY: self.ptr and other.ptr are non-null and valid for the lifetime of `&self`/`&other`.
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
    /// # Safety
    /// `ptr` must be non-null and valid for lifetime `'a`.
    pub(crate) unsafe fn from_ptr(ptr: *const ffi::dnv_vista_sdk_serializable_document_t) -> Self {
        Self {
            ptr,
            _marker: PhantomData,
        }
    }

    /// Produces an independent deep copy of this borrowed node.
    pub fn to_owned(&self) -> SerializableDocument {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`. Returns an owned pointer.
        let ptr = unsafe { ffi::dnv_vista_sdk_serializable_document_clone(self.ptr) };
        SerializableDocument {
            ptr: NonNull::new(ptr).expect("clone returned null"),
        }
    }

    /// Returns the value kind of this node.
    pub fn kind(&self) -> DocumentKind {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        kind_from_ffi(unsafe { ffi::dnv_vista_sdk_serializable_document_kind(self.ptr) })
    }

    /// Returns `true` if this node holds a JSON null.
    pub fn is_null(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_null(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON boolean.
    pub fn is_boolean(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_boolean(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON integer.
    pub fn is_integer(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_integer(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON floating point number.
    pub fn is_double(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_double(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON string.
    pub fn is_string(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_string(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON array.
    pub fn is_array(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_array(self.ptr) }) != 0
    }

    /// Returns `true` if this node holds a JSON object.
    pub fn is_object(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_object(self.ptr) }) != 0
    }

    /// Returns the boolean value, or `None` if not a boolean.
    pub fn as_bool(&self) -> Option<bool> {
        if !self.is_boolean() {
            return None;
        }
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        Some((unsafe { ffi::dnv_vista_sdk_serializable_document_as_boolean(self.ptr) }) != 0)
    }

    /// Returns the integer value, or `None` if not an integer.
    pub fn as_i64(&self) -> Option<i64> {
        if !self.is_integer() {
            return None;
        }
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_integer(self.ptr) })
    }

    /// Returns the floating point value, or `None` if not a double.
    pub fn as_f64(&self) -> Option<f64> {
        if !self.is_double() {
            return None;
        }
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_double(self.ptr) })
    }

    /// Returns the string value, or `None` if not a string.
    pub fn as_str(&self) -> Option<String> {
        if !self.is_string() {
            return None;
        }
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let raw = unsafe { ffi::dnv_vista_sdk_serializable_document_as_string(self.ptr) };
        if raw.is_null() {
            return None;
        }
        // SAFETY: raw is non-null (checked above) and owned by the referenced document for lifetime `'a`.
        Some(unsafe { lossy(raw) })
    }

    /// Returns the number of elements in this array, or 0 if not an array.
    pub fn array_len(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_serializable_document_array_size(self.ptr) }
    }

    /// Returns a borrowed view of the array element at `index`.
    /// Returns `None` if not an array or `index` is out of range.
    pub fn array_at(&self, index: usize) -> Option<SerializableDocumentRef<'a>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
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
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_serializable_document_object_size(self.ptr) }
    }

    /// Returns the key at insertion-order `index`, or `None` if out of range.
    pub fn object_key_at(&self, index: usize) -> Option<String> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let raw =
            unsafe { ffi::dnv_vista_sdk_serializable_document_object_key_at(self.ptr, index) };
        if raw.is_null() {
            None
        } else {
            // SAFETY: raw is non-null (checked above) and owned by the referenced document
            // for lifetime `'a`.
            Some(unsafe { lossy(raw) })
        }
    }

    /// Returns a borrowed view of the object value at insertion-order `index`.
    /// Returns `None` if not an object or `index` is out of range.
    pub fn object_value_at(&self, index: usize) -> Option<SerializableDocumentRef<'a>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
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
    /// Returns `None` if not an object, `key` is not present, or `key` contains an interior NUL
    /// byte (which cannot be encoded as a C string).
    pub fn find(&self, key: &str) -> Option<SerializableDocumentRef<'a>> {
        let c = c_key(key)?;
        // SAFETY: self.ptr is non-null, and c is a valid NUL-terminated C string.
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
    ///
    /// Returns `false` when `key` contains an interior NUL byte.
    pub fn contains(&self, key: &str) -> bool {
        let Some(c) = c_key(key) else {
            return false;
        };
        // SAFETY: self.ptr is non-null, and c is a valid NUL-terminated C string.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_contains(self.ptr, c.as_ptr()) }) != 0
    }
}

impl<'a> std::fmt::Debug for SerializableDocumentRef<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "SerializableDocumentRef({:?})", self.kind())
    }
}

/// Zero-copy **mutable** view into a [`SerializableDocument`] node owned by a parent C++ object
/// (e.g. the `customHeaders` of a DTO header).
///
/// Unlike [`SerializableDocumentRef`], this view allows mutating the document in place via
/// [`set`](SerializableDocumentRefMut::set) and [`push_back`](SerializableDocumentRefMut::push_back).
/// It does **not** own the document: the backing C++ object is freed by its parent, so this type
/// has no `Drop` and must not outlive the borrow it was derived from.
///
/// # Aliasing contract
///
/// The owner of the backing document is the **parent C++ object** (e.g. a `DtoHeaderRef`), not a
/// [`SerializableDocument`]. `SerializableDocumentRefMut` is obtained exclusively through
/// `&mut self` accessors on that parent (`custom_headers`, `ensure_custom_headers`, `find`...), so
/// the borrow checker already guarantees that no two mutable views derived from the *same* parent
/// accessor chain coexist.
///
/// The phantom lifetime is therefore borrowed from the parent, not from a `SerializableDocument`:
/// it is modelled as `PhantomData<&'a mut ()>` because there is no Rust type representing that
/// parent here. Callers must not hand out two views over the same node through different paths
/// (e.g. keep one from `custom_headers()` and take another from `ensure_custom_headers()`).
pub struct SerializableDocumentRefMut<'a> {
    ptr: NonNull<ffi::dnv_vista_sdk_serializable_document_t>,
    _marker: PhantomData<&'a mut ()>,
}

impl<'a> SerializableDocumentRefMut<'a> {
    /// # Safety
    /// `ptr` must be a valid non-null pointer to a `SerializableDocument` owned by a parent C++
    /// object live for at least `'a`. The caller must not free it, and must not hand out a second
    /// mutable view over the same node while this one is alive (see the aliasing contract on the
    /// type).
    pub(crate) unsafe fn from_ptr_mut(
        ptr: *mut ffi::dnv_vista_sdk_serializable_document_t,
    ) -> Self {
        Self {
            ptr: NonNull::new(ptr)
                .expect("SerializableDocumentRefMut::from_ptr_mut called with null"),
            _marker: PhantomData,
        }
    }

    /// Returns the kind of the held value.
    pub fn kind(&self) -> DocumentKind {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        kind_from_ffi(unsafe { ffi::dnv_vista_sdk_serializable_document_kind(self.ptr.as_ptr()) })
    }

    /// Returns `true` if this is a JSON null.
    pub fn is_null(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_null(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this is a JSON object.
    pub fn is_object(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_object(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this is a JSON boolean.
    pub fn is_boolean(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_boolean(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this is a JSON integer.
    pub fn is_integer(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_integer(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this is a JSON floating point number.
    pub fn is_double(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_double(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this is a JSON string.
    pub fn is_string(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_string(self.ptr.as_ptr()) }) != 0
    }

    /// Returns `true` if this is a JSON array.
    pub fn is_array(&self) -> bool {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        (unsafe { ffi::dnv_vista_sdk_serializable_document_is_array(self.ptr.as_ptr()) }) != 0
    }

    /// Returns the boolean value if this is a JSON boolean.
    pub fn as_bool(&self) -> Option<bool> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        Some(
            (unsafe { ffi::dnv_vista_sdk_serializable_document_as_boolean(self.ptr.as_ptr()) })
                != 0,
        )
    }

    /// Returns the 64-bit integer value if this is a JSON integer.
    pub fn as_i64(&self) -> Option<i64> {
        if !self.is_integer() {
            return None;
        }
        // SAFETY: self.ptr is non-null, valid, and holds an integer.
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_integer(self.ptr.as_ptr()) })
    }

    /// Returns the double value if this is a JSON floating point number.
    pub fn as_f64(&self) -> Option<f64> {
        if !self.is_double() {
            return None;
        }
        // SAFETY: self.ptr is non-null, valid, and holds a double.
        Some(unsafe { ffi::dnv_vista_sdk_serializable_document_as_double(self.ptr.as_ptr()) })
    }

    /// Returns the string value if this is a JSON string.
    pub fn as_str(&self) -> Option<&str> {
        if !self.is_string() {
            return None;
        }
        // SAFETY: self.ptr is non-null, valid, holds a string, and the returned pointer is
        // owned by the referenced document for lifetime `'a`.
        let raw = unsafe { ffi::dnv_vista_sdk_serializable_document_as_string(self.ptr.as_ptr()) };
        Some(
            // SAFETY: raw is non-null and owned by the referenced document for lifetime `'a`.
            unsafe { CStr::from_ptr(raw) }.to_str().unwrap_or(""),
        )
    }

    /// Returns the number of elements in this array, or 0 if not an array.
    pub fn array_len(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_serializable_document_array_size(self.ptr.as_ptr()) }
    }

    /// Returns a borrowed view of the array element at `index`.
    /// Returns `None` if not an array or `index` is out of range.
    pub fn array_at(&self, index: usize) -> Option<SerializableDocumentRefMut<'a>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let raw = unsafe {
            ffi::dnv_vista_sdk_serializable_document_array_at_mut(self.ptr.as_ptr(), index)
        };
        if raw.is_null() {
            None
        } else {
            // SAFETY: raw is non-null (checked above) and is a borrowed sub-node of a mutable document.
            Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(raw) })
        }
    }

    /// Returns the number of key/value pairs in this object, or 0 if not an object.
    pub fn object_len(&self) -> usize {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        unsafe { ffi::dnv_vista_sdk_serializable_document_object_size(self.ptr.as_ptr()) }
    }

    /// Returns the key at insertion-order `index`, or `None` if out of range.
    pub fn object_key_at(&self, index: usize) -> Option<&str> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let raw = unsafe {
            ffi::dnv_vista_sdk_serializable_document_object_key_at(self.ptr.as_ptr(), index)
        };
        if raw.is_null() {
            None
        } else {
            Some(
                // SAFETY: raw is non-null (checked above) and owned by the referenced document
                // for lifetime `'a`.
                unsafe { CStr::from_ptr(raw) }.to_str().unwrap_or(""),
            )
        }
    }

    /// Returns a borrowed view of the object value at insertion-order `index`.
    /// Returns `None` if not an object or `index` is out of range.
    pub fn object_value_at(&self, index: usize) -> Option<SerializableDocumentRefMut<'a>> {
        // SAFETY: self.ptr is non-null and valid for lifetime `'a`.
        let raw = unsafe {
            ffi::dnv_vista_sdk_serializable_document_object_value_at_mut(self.ptr.as_ptr(), index)
        };
        if raw.is_null() {
            None
        } else {
            // SAFETY: raw is non-null (checked above) and is a borrowed sub-node of a mutable document.
            Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(raw) })
        }
    }

    /// Looks up a key in this object.
    /// Returns `None` if not an object or `key` is not present.
    ///
    /// Also returns `None` when `key` contains an interior NUL byte, which cannot be encoded as a
    /// C string.
    pub fn find(&mut self, key: &str) -> Option<SerializableDocumentRefMut<'a>> {
        let c = c_key(key)?;
        // SAFETY: self.ptr is non-null, and c is a valid NUL-terminated C string.
        let raw = unsafe {
            ffi::dnv_vista_sdk_serializable_document_find_mut(self.ptr.as_ptr(), c.as_ptr())
        };
        if raw.is_null() {
            None
        } else {
            // SAFETY: raw is non-null (checked above) and is a borrowed sub-node of a mutable document.
            Some(unsafe { SerializableDocumentRefMut::from_ptr_mut(raw) })
        }
    }

    /// Sets a key/value pair on this object, preserving insertion order.
    ///
    /// If this document is not yet an object it is reset to an empty object first.
    /// If `key` already exists its value is replaced in place.
    /// Takes ownership of `value`.
    ///
    /// # Panics
    /// Panics if `key` contains an interior NUL byte. Use
    /// [`try_set`](SerializableDocumentRefMut::try_set) to handle that case explicitly.
    pub fn set(&mut self, key: &str, value: SerializableDocument) {
        if !self.try_set(key, value) {
            panic!("key contains an interior NUL byte");
        }
    }

    /// Fallible variant of [`set`](SerializableDocumentRefMut::set).
    ///
    /// Returns `false` (and leaks no value) when `key` contains an interior NUL byte.
    pub fn try_set(&mut self, key: &str, value: SerializableDocument) -> bool {
        let Some(c) = c_key(key) else {
            return false;
        };
        let val_ptr = value.ptr.as_ptr();
        std::mem::forget(value);
        // SAFETY: self.ptr and val_ptr are non-null, c is a valid NUL-terminated C string, and
        // val_ptr's ownership is transferred to the C++ document (value was `mem::forget`-ten).
        unsafe {
            ffi::dnv_vista_sdk_serializable_document_set(self.ptr.as_ptr(), c.as_ptr(), val_ptr)
        };
        true
    }

    /// Appends a value to this array.
    ///
    /// If this document is not yet an array it is reset to an empty array first.
    /// Takes ownership of `value`.
    pub fn push_back(&mut self, value: SerializableDocument) {
        let val_ptr = value.ptr.as_ptr();
        std::mem::forget(value);
        // SAFETY: self.ptr and val_ptr are non-null, and val_ptr's ownership is transferred
        // to the C++ document (value was `mem::forget`-ten).
        unsafe { ffi::dnv_vista_sdk_serializable_document_push_back(self.ptr.as_ptr(), val_ptr) };
    }

    /// Returns `true` if this is an object and contains `key`.
    ///
    /// Returns `false` when `key` contains an interior NUL byte.
    pub fn contains(&self, key: &str) -> bool {
        let Some(c) = c_key(key) else {
            return false;
        };
        // SAFETY: self.ptr is non-null, and c is a valid NUL-terminated C string.
        (unsafe {
            ffi::dnv_vista_sdk_serializable_document_contains(self.ptr.as_ptr(), c.as_ptr())
        }) != 0
    }
}

impl<'a> std::fmt::Debug for SerializableDocumentRefMut<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "SerializableDocumentRefMut({:?})", self.kind())
    }
}

/// SAFETY: caller must ensure `ptr` is non-null and valid for the duration of the call.
///
/// Invalid UTF-8 is replaced (lossy) instead of panicking: a JSON string coming from the C layer
/// must never abort the process.
unsafe fn lossy(ptr: *const std::ffi::c_char) -> String {
    CStr::from_ptr(ptr).to_string_lossy().into_owned()
}

/// Encodes `key` as a NUL-terminated C string.
///
/// Returns `None` when `key` contains an interior NUL byte (which cannot be represented in a
/// C string); callers map this to their own error/absence semantics instead of panicking.
fn c_key(key: &str) -> Option<CString> {
    CString::new(key).ok()
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

#[cfg(test)]
mod discriminant_sync {
    use super::*;

    #[test]
    fn document_kind_discriminants_match_ffi() {
        assert_eq!(
            DocumentKind::Null as u32,
            ffi::dnv_vista_sdk_serializable_document_kind_t::Null as u32
        );
        assert_eq!(
            DocumentKind::Boolean as u32,
            ffi::dnv_vista_sdk_serializable_document_kind_t::Boolean as u32
        );
        assert_eq!(
            DocumentKind::Integer as u32,
            ffi::dnv_vista_sdk_serializable_document_kind_t::Integer as u32
        );
        assert_eq!(
            DocumentKind::Double as u32,
            ffi::dnv_vista_sdk_serializable_document_kind_t::Double as u32
        );
        assert_eq!(
            DocumentKind::String as u32,
            ffi::dnv_vista_sdk_serializable_document_kind_t::String as u32
        );
        assert_eq!(
            DocumentKind::Array as u32,
            ffi::dnv_vista_sdk_serializable_document_kind_t::Array as u32
        );
        assert_eq!(
            DocumentKind::Object as u32,
            ffi::dnv_vista_sdk_serializable_document_kind_t::Object as u32
        );
    }
}
