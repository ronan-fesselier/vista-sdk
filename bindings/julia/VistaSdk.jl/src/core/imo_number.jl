"""
    ImoNumber

An International Maritime Organization (IMO) number: a unique seven-digit
identifier assigned to maritime vessels, with a check digit for validation.

# Examples
```julia
imo = parse(ImoNumber, "9074729")
string(imo)   # "IMO9074729"
value(imo)    # 9074729
```
"""
struct ImoNumber
    _value::UInt32
end

"""
    ImoNumber(n::Integer) -> ImoNumber

Construct an IMO number from an integer. Throws [`VistaError`](@ref) if invalid.
"""
function ImoNumber(n::Integer)
    ptr = ffi_imo_number_create(Cint(n))
    if ptr == C_NULL
        throw(last_error())
    end
    v = ffi_imo_number_value(ptr)
    ffi_imo_number_free(ptr)
    ImoNumber(UInt32(v))
end

"""
    Base.parse(::Type{ImoNumber}, s::AbstractString) -> ImoNumber

Parse an IMO number from a string (with or without `"IMO"` prefix).
Throws [`VistaError`](@ref) if invalid.
"""
function Base.parse(::Base.Type{ImoNumber}, s::AbstractString)
    ptr = GC.@preserve s ffi_imo_number_from_string(Base.unsafe_convert(Cstring, s))
    if ptr == C_NULL
        throw(last_error())
    end
    v = ffi_imo_number_value(ptr)
    ffi_imo_number_free(ptr)
    ImoNumber(UInt32(v))
end

"""
    is_valid(n::Integer) -> Bool

Return `true` if `n` satisfies the IMO number checksum without constructing one.
"""
function is_valid(n::Integer)
    ffi_imo_number_is_valid(Cint(n)) != 0
end

"""
    value(imo::ImoNumber) -> Int

Return the raw seven-digit integer value.
"""
function value(imo::ImoNumber)
    Int(imo._value)
end

Base.show(io::IO, imo::ImoNumber) = print(io, "IMO", imo._value)
Base.string(imo::ImoNumber) = string("IMO", imo._value)
Base.:(==)(a::ImoNumber, b::ImoNumber) = a._value == b._value
Base.hash(imo::ImoNumber, h::UInt) = hash(imo._value, h)
