"""
    Codebooks

Container for all VIS codebooks for a specific version.

Borrowed from the VIS singleton — valid for the lifetime of the program.
"""
struct Codebooks
    _ptr::Ptr{Cvoid}
end

"""
    version(cbs::Codebooks) -> VisVersion

Return the VIS version these codebooks belong to.
"""
function version(cbs::Codebooks)
    ptr = ffi_codebooks_version(cbs._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_codebooks_version returned NULL")
    parse(VisVersion, unsafe_string(ptr))
end

"""
    Base.getindex(cbs::Codebooks, name::CodebookName) -> Codebook

Return the [`Codebook`](@ref) for `name`.
"""
function Base.getindex(cbs::Codebooks, name::CodebookName)
    ptr = ffi_codebooks_at(cbs._ptr, Cint(Integer(name)))
    ptr == C_NULL && error("dnv_vista_sdk_codebooks_at returned NULL for $name")
    Codebook(ptr)
end
