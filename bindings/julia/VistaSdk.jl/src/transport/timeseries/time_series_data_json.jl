"""
    tsd_from_json(json::AbstractString) -> TimeSeriesDataPackage

Parse an ISO 19848 TimeSeriesData JSON payload into a [`TimeSeriesDataPackage`](@ref).
Throws [`VistaError`](@ref) if the input is invalid or cannot be parsed.
"""
function tsd_from_json(json::AbstractString)
    s = String(json)
    ptr = GC.@preserve s ffi_tsd_data_package_from_json(Base.unsafe_convert(Cstring, s))
    ptr == C_NULL && throw(VistaError(Runtime, "tsd_from_json: invalid JSON"))
    TimeSeriesDataPackage(ptr)
end

"""
    tsd_to_json(p::TimeSeriesDataPackage, pretty::Bool = false) -> String

Serialize a [`TimeSeriesDataPackage`](@ref) to an ISO 19848 TimeSeriesData JSON string.
Pass `pretty = true` for indented output.
"""
function tsd_to_json(p::TimeSeriesDataPackage, pretty::Bool = false)
    ptr = ffi_tsd_data_package_to_json(p._ptr, Cint(pretty ? 1 : 0))
    ptr == C_NULL && throw(VistaError(InvalidState, "tsd_to_json returned NULL"))
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    s
end
