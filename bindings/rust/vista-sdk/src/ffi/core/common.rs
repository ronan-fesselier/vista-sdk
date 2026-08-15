use std::ffi::c_char;

extern "C" {
    pub(crate) fn dnv_vista_sdk_string_free(str: *mut c_char);
}

/// Write an owned C string to a formatter, then free it.
///
/// # Safety
/// `ptr` must be non-null, point to a valid NUL-terminated string, and have been
/// allocated by the C library (freed via `dnv_vista_sdk_string_free`).
/// Invalid UTF-8 bytes are replaced with U+FFFD rather than panicking.
pub(crate) unsafe fn display_owned_cstr(
    ptr: *mut c_char,
    f: &mut std::fmt::Formatter<'_>,
) -> std::fmt::Result {
    // SAFETY: caller guarantees ptr is non-null.
    let s = unsafe { std::ffi::CStr::from_ptr(ptr) }.to_string_lossy();
    let result = f.write_str(&s);
    // SAFETY: ptr was allocated by the library and is freed exactly once.
    unsafe { dnv_vista_sdk_string_free(ptr) };
    result
}
