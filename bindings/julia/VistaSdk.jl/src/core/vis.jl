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
