"""
    Vis

Central entry point for the Vista SDK.

# Examples
```julia
versions(vis())   # all known VIS versions
latest(vis())     # latest VIS version
```
"""
struct Vis
    _ptr::Ptr{Cvoid}
end

"""
    vis() -> Vis

Return the Vista SDK entry point.
"""
function vis()
    ptr = ffi_vis_instance()
    ptr == C_NULL && error("dnv_vista_sdk_vis_instance returned NULL")
    Vis(ptr)
end

"""
    versions(vis::Vis) -> Vector{VisVersion}

Return all known VIS versions as a [`VisVersion`](@ref) vector.
"""
function versions(v::Vis)
    count = ffi_vis_version_count(v._ptr)
    result = VisVersion[]
    for i = 0:(count-1)
        s = ffi_vis_version_at(v._ptr, i)
        s == C_NULL && continue
        push!(result, parse(VisVersion, unsafe_string(s)))
    end
    result
end

"""
    latest(vis::Vis) -> VisVersion

Return the latest VIS version known to the SDK.
"""
function latest(v::Vis)
    ptr = ffi_vis_latest(v._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_vis_latest returned NULL")
    parse(VisVersion, unsafe_string(ptr))
end

function _vis_get(v::Vis, version::VisVersion, ffi_fn::Function, wrap)
    s = string(version)
    ptr = GC.@preserve s ffi_fn(v._ptr, Base.unsafe_convert(Cstring, s))
    ptr == C_NULL && throw(last_error())
    wrap(ptr)
end

"""
    gmod(vis::Vis, version::VisVersion) -> Gmod

Return the [`Gmod`](@ref) for `version`. Throws [`VistaError`](@ref) if the version is not recognized.
"""
gmod(v::Vis, version::VisVersion) = _vis_get(v, version, ffi_vis_gmod, Gmod)

"""
    codebooks(vis::Vis, version::VisVersion) -> Codebooks

Return the [`Codebooks`](@ref) for `version`. Throws [`VistaError`](@ref) if the version is not recognized.
"""
codebooks(v::Vis, version::VisVersion) = _vis_get(v, version, ffi_vis_codebooks, Codebooks)

"""
    locations(vis::Vis, version::VisVersion) -> Locations

Return the [`Locations`](@ref) for `version`. Throws [`VistaError`](@ref) if the version is not recognized.
"""
locations(v::Vis, version::VisVersion) = _vis_get(v, version, ffi_vis_locations, Locations)
