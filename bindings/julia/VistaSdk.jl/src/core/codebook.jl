"""
    Codebook

A VIS codebook containing standard values and validation logic.

Borrowed from the parent [`Codebooks`](@ref) — do not store beyond the lifetime of the parent.
"""
struct Codebook
    _ptr::Ptr{Cvoid}
end

"""
    name(cb::Codebook) -> CodebookName

Return the name of this codebook.
"""
function name(cb::Codebook)
    CodebookName(ffi_codebook_name(cb._ptr))
end

"""
    standard_values(cb::Codebook) -> Vector{String}

Return all standard values in this codebook.
"""
function standard_values(cb::Codebook)
    count = ffi_codebook_standard_values_count(cb._ptr)
    result = String[]
    for i = 0:(count-1)
        ptr = ffi_codebook_standard_value_at(cb._ptr, i)
        ptr != C_NULL && push!(result, unsafe_string(ptr))
    end
    result
end

"""
    groups(cb::Codebook) -> Vector{String}

Return all group names in this codebook.
"""
function groups(cb::Codebook)
    count = ffi_codebook_groups_count(cb._ptr)
    result = String[]
    for i = 0:(count-1)
        ptr = ffi_codebook_group_at(cb._ptr, i)
        ptr != C_NULL && push!(result, unsafe_string(ptr))
    end
    result
end

"""
    has_group(cb::Codebook, group::AbstractString) -> Bool

Return `true` if `group` exists in this codebook.
"""
function has_group(cb::Codebook, group::AbstractString)
    GC.@preserve group ffi_codebook_has_group(
        cb._ptr,
        Base.unsafe_convert(Cstring, group),
    ) != 0
end

"""
    has_standard_value(cb::Codebook, value::AbstractString) -> Bool

Return `true` if `value` is a standard value in this codebook.
"""
function has_standard_value(cb::Codebook, value::AbstractString)
    GC.@preserve value ffi_codebook_has_standard_value(
        cb._ptr,
        Base.unsafe_convert(Cstring, value),
    ) != 0
end

"""
    validate_position(cb::Codebook, position::AbstractString) -> PositionValidationResult

Validate a position string. Only meaningful for the Position codebook.
"""
function validate_position(cb::Codebook, position::AbstractString)
    raw = GC.@preserve position ffi_codebook_validate_position(
        cb._ptr,
        Base.unsafe_convert(Cstring, position),
    )
    PositionValidationResult(raw)
end

"""
    create_tag(cb::Codebook, value::AbstractString) -> Union{MetadataTag,Nothing}

Create a [`MetadataTag`](@ref) for `value`, validating it against this codebook.
Returns `nothing` if the value is invalid.
"""
function create_tag(cb::Codebook, value::AbstractString)
    ptr = GC.@preserve value ffi_codebook_create_tag(
        cb._ptr,
        Base.unsafe_convert(Cstring, value),
    )
    ptr == C_NULL ? nothing : MetadataTag(ptr)
end
