"""
    CodebookName

Enumeration of VIS codebook types.

# Variants
- `Quantity`           : physical quantity (e.g. temperature, pressure)
- `Content`            : substance or medium the sensor measures
- `Calculation`        : calculation method applied to the measured value
- `State`              : operating state of the equipment
- `Command`            : control command issued to equipment
- `Type`               : equipment or component type
- `FunctionalServices` : functional service the equipment belongs to
- `MaintenanceCategory`: maintenance category
- `ActivityType`       : activity type performed on the equipment
- `Position`           : physical position on the vessel
- `Detail`             : additional detail qualifier
"""
@enum CodebookName::Int32 begin
    Quantity = 1
    Content = 2
    Calculation = 3
    State = 4
    Command = 5
    Type = 6
    FunctionalServices = 7
    MaintenanceCategory = 8
    ActivityType = 9
    Position = 10
    Detail = 11
end

export CodebookName

"""
    codebook_name_from_prefix(prefix::AbstractString) -> Union{CodebookName,Nothing}

Return the [`CodebookName`](@ref) matching `prefix`, or `nothing` if not recognized.
"""
function codebook_name_from_prefix(prefix::AbstractString)
    out = Ref{Cint}(0)
    ok = GC.@preserve prefix ffi_codebook_name_from_prefix(
        Base.unsafe_convert(Cstring, prefix),
        out,
    )
    ok == 0 ? nothing : CodebookName(out[])
end

"""
    codebook_name_to_prefix(name::CodebookName) -> String

Return the string prefix for `name` (e.g. `"qty"` for `Quantity`).
"""
function codebook_name_to_prefix(name::CodebookName)
    ptr = ffi_codebook_name_to_prefix(Cint(Integer(name)))
    ptr == C_NULL && error("dnv_vista_sdk_codebook_names_to_prefix returned NULL")
    unsafe_string(ptr)
end

function Base.string(name::CodebookName)
    ptr = ffi_codebook_name_to_string(Cint(Integer(name)))
    ptr == C_NULL && error("dnv_vista_sdk_codebook_names_to_string returned NULL")
    unsafe_string(ptr)
end

Base.show(io::IO, name::CodebookName) = print(io, Base.string(name))
