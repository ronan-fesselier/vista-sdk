"""
    dcl_dto_from_json(json::AbstractString) -> DclDtoPackage

Parse an ISO 19848 DataChannelList JSON payload into a [`DclDtoPackage`](@ref).
Throws [`VistaError`](@ref) if the input is invalid or cannot be parsed.
"""
function dcl_dto_from_json(json::AbstractString)
    s = String(json)
    ptr = GC.@preserve s ffi_dcl_dto_from_json(Base.unsafe_convert(Cstring, s))
    ptr == C_NULL && throw(VistaError(Runtime, "dcl_dto_from_json: invalid JSON"))
    DclDtoPackage(ptr)
end

"""
    dcl_dto_to_json(p::DclDtoPackage, pretty::Bool = false) -> String

Serialize a [`DclDtoPackage`](@ref) to an ISO 19848 DataChannelList JSON string.
Pass `pretty = true` for indented output.
"""
function dcl_dto_to_json(p::DclDtoPackage, pretty::Bool = false)
    ptr = ffi_dcl_dto_to_json(p._ptr, Cint(pretty ? 1 : 0))
    ptr == C_NULL && throw(VistaError(InvalidState, "dcl_dto_to_json returned NULL"))
    s = unsafe_string(ptr)
    ccall((:dnv_vista_sdk_string_free, VISTA_LIB), Cvoid, (Ptr{UInt8},), ptr)
    s
end
