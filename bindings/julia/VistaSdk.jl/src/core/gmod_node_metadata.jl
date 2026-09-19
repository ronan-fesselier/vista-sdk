"""
    GmodNodeMetadata

Borrowed view of a node's metadata. Valid for the lifetime of the owning `Gmod`.
Do not store beyond the scope where the `Gmod` is alive.
"""
struct GmodNodeMetadata
    _ptr::Ptr{Cvoid}
end

"""
    category(m::GmodNodeMetadata) -> String

Return the node category string (e.g. `"ASSET"`, `"FUNCTION"`).
"""
function category(m::GmodNodeMetadata)
    ptr = ffi_gmod_node_metadata_category(m._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_node_metadata_category returned NULL")
    unsafe_string(ptr)
end

"""
    node_type(m::GmodNodeMetadata) -> String

Return the short node type string (e.g. `"CO"`, `"CA"`).
"""
function node_type(m::GmodNodeMetadata)
    ptr = ffi_gmod_node_metadata_type(m._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_node_metadata_type returned NULL")
    unsafe_string(ptr)
end

"""
    full_type(m::GmodNodeMetadata) -> String

Return the full node type string including category prefix.
"""
function full_type(m::GmodNodeMetadata)
    ptr = ffi_gmod_node_metadata_full_type(m._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_node_metadata_full_type returned NULL")
    unsafe_string(ptr)
end

"""
    name(m::GmodNodeMetadata) -> String

Return the node name.
"""
function name(m::GmodNodeMetadata)
    ptr = ffi_gmod_node_metadata_name(m._ptr)
    ptr == C_NULL && error("dnv_vista_sdk_gmod_node_metadata_name returned NULL")
    unsafe_string(ptr)
end

"""
    common_name(m::GmodNodeMetadata) -> Union{String,Nothing}

Return the common name, or `nothing` if absent.
"""
function common_name(m::GmodNodeMetadata)
    ptr = ffi_gmod_node_metadata_common_name(m._ptr)
    ptr == C_NULL ? nothing : unsafe_string(ptr)
end

"""
    definition(m::GmodNodeMetadata) -> Union{String,Nothing}

Return the node definition text, or `nothing` if absent.
"""
function definition(m::GmodNodeMetadata)
    ptr = ffi_gmod_node_metadata_definition(m._ptr)
    ptr == C_NULL ? nothing : unsafe_string(ptr)
end

"""
    common_definition(m::GmodNodeMetadata) -> Union{String,Nothing}

Return the common definition text, or `nothing` if absent.
"""
function common_definition(m::GmodNodeMetadata)
    ptr = ffi_gmod_node_metadata_common_definition(m._ptr)
    ptr == C_NULL ? nothing : unsafe_string(ptr)
end

"""
    install_substructure(m::GmodNodeMetadata) -> Union{Bool,Nothing}

Return the install-substructure flag, or `nothing` if not set.
"""
function install_substructure(m::GmodNodeMetadata)
    out = Ref{Cint}(0)
    ok = ffi_gmod_node_metadata_install_substructure(m._ptr, out)
    ok != 0 ? (out[] != 0) : nothing
end

"""
    normal_assignment_name_count(m::GmodNodeMetadata) -> Int

Return the number of normal assignment name entries.
"""
function normal_assignment_name_count(m::GmodNodeMetadata)
    Int(ffi_gmod_node_metadata_normal_assignment_name_count(m._ptr))
end

"""
    normal_assignment_name_at(m::GmodNodeMetadata, index::Int) -> Union{Tuple{String,String},Nothing}

Return the `(key, value)` pair at 1-based `index`, or `nothing` if out of range.
"""
function normal_assignment_name_at(m::GmodNodeMetadata, index::Int)
    key_ptr =
        ffi_gmod_node_metadata_normal_assignment_name_key_at(m._ptr, Csize_t(index - 1))
    key_ptr == C_NULL && return nothing
    val_ptr =
        ffi_gmod_node_metadata_normal_assignment_name_value_at(m._ptr, Csize_t(index - 1))
    val_ptr == C_NULL && error("key present but value missing at index $index")
    (unsafe_string(key_ptr), unsafe_string(val_ptr))
end

"""
    normal_assignment_names(m::GmodNodeMetadata) -> Vector{Tuple{String,String}}

Return all normal assignment name entries as a vector of `(key, value)` pairs.
"""
function normal_assignment_names(m::GmodNodeMetadata)
    n = normal_assignment_name_count(m)
    [
        (
            unsafe_string(
                ffi_gmod_node_metadata_normal_assignment_name_key_at(
                    m._ptr,
                    Csize_t(i - 1),
                ),
            ),
            unsafe_string(
                ffi_gmod_node_metadata_normal_assignment_name_value_at(
                    m._ptr,
                    Csize_t(i - 1),
                ),
            ),
        ) for i = 1:n
    ]
end
