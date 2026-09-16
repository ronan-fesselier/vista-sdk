"""
    PositionValidationResult

Result of validating a position string against the Position codebook.

# Variants
- `VistaSdk.Invalid`        : invalid format, whitespace, or non-ISO characters
- `VistaSdk.InvalidOrder`   : numbers not at end or not alphabetically sorted
- `VistaSdk.InvalidGrouping`: duplicate groups (except the default group)
- `VistaSdk.Valid`          : standard value, number, or valid composite position
- `VistaSdk.Custom`         : custom value not in the standard codebook
"""
@enum PositionValidationResult::Int32 begin
    Invalid = 0
    InvalidOrder = 1
    InvalidGrouping = 2
    Valid = 100
    Custom = 101
end

export PositionValidationResult

function ffi_codebook_name(codebook::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_codebook_name, VISTA_LIB), Cint, (Ptr{Cvoid},), codebook)
end

function ffi_codebook_standard_values_count(codebook::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_codebook_standard_values_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        codebook,
    )
end

function ffi_codebook_standard_value_at(codebook::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_codebook_standard_value_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        codebook,
        index,
    )
end

function ffi_codebook_groups_count(codebook::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_codebook_groups_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        codebook,
    )
end

function ffi_codebook_group_at(codebook::Ptr{Cvoid}, index::Integer)
    ccall(
        (:dnv_vista_sdk_codebook_group_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        codebook,
        index,
    )
end

function ffi_codebook_has_group(codebook::Ptr{Cvoid}, group::Cstring)
    ccall(
        (:dnv_vista_sdk_codebook_has_group, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        codebook,
        group,
    )
end

function ffi_codebook_has_standard_value(codebook::Ptr{Cvoid}, value::Cstring)
    ccall(
        (:dnv_vista_sdk_codebook_has_standard_value, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        codebook,
        value,
    )
end

function ffi_codebook_validate_position(codebook::Ptr{Cvoid}, position::Cstring)
    ccall(
        (:dnv_vista_sdk_codebook_validate_position, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        codebook,
        position,
    )
end

function ffi_codebook_create_tag(codebook::Ptr{Cvoid}, value::Cstring)
    ccall(
        (:dnv_vista_sdk_codebook_create_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        codebook,
        value,
    )
end
