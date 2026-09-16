"""
    VistaError <: Exception

Error returned by Vista SDK operations that can fail.
"""
struct VistaError <: Exception
    kind::ErrorKind
    message::String
end

function Base.showerror(io::IO, e::VistaError)
    print(io, "VistaError(", e.kind, "): ", e.message)
end

"""
    last_error() -> VistaError

Return the last error recorded by the SDK on the current thread.
"""
function last_error()
    kind = ErrorKind(ffi_last_error_kind())
    ptr = ffi_last_error_message()
    message = ptr == C_NULL ? "" : unsafe_string(ptr)
    VistaError(kind, message)
end

"""
    clear_error()

Clear the last error recorded by the SDK on the current thread.
"""
function clear_error()
    ffi_clear_error()
end
