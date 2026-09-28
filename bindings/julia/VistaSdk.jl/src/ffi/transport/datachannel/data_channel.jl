@enum _FfiWhiteSpace::Int32 begin
    _WsPreserve = 0
    _WsReplace = 1
    _WsCollapse = 2
end

function ffi_dcl_restriction_create()
    ccall((:dnv_vista_sdk_dcl_restriction_create, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_dcl_restriction_free(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_restriction_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), r)
end

function ffi_dcl_restriction_enumeration_count(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_enumeration_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_enumeration_at(r::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_enumeration_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        r,
        index,
    )
end

function ffi_dcl_restriction_set_enumeration(
    r::Ptr{Cvoid},
    values::Ptr{Cstring},
    count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_enumeration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cstring}, Csize_t),
        r,
        values,
        count,
    )
end

function ffi_dcl_restriction_clear_enumeration(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_enumeration, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_fraction_digits(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_fraction_digits, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_fraction_digits(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_fraction_digits, VISTA_LIB),
        UInt32,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_set_fraction_digits(r::Ptr{Cvoid}, value::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_fraction_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_fraction_digits(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_fraction_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_length(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_restriction_has_length, VISTA_LIB), Cint, (Ptr{Cvoid},), r)
end

function ffi_dcl_restriction_length(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_restriction_length, VISTA_LIB), UInt32, (Ptr{Cvoid},), r)
end

function ffi_dcl_restriction_set_length(r::Ptr{Cvoid}, value::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_length(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_restriction_clear_length, VISTA_LIB), Cvoid, (Ptr{Cvoid},), r)
end

function ffi_dcl_restriction_has_max_exclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_max_exclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_max_exclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_max_exclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_set_max_exclusive(r::Ptr{Cvoid}, value::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_max_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_max_exclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_max_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_max_inclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_max_inclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_max_inclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_max_inclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_set_max_inclusive(r::Ptr{Cvoid}, value::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_max_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_max_inclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_max_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_max_length(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_max_length, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_max_length(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_restriction_max_length, VISTA_LIB), UInt32, (Ptr{Cvoid},), r)
end

function ffi_dcl_restriction_set_max_length(r::Ptr{Cvoid}, value::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_max_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_max_length(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_max_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_min_exclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_min_exclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_min_exclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_min_exclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_set_min_exclusive(r::Ptr{Cvoid}, value::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_min_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_min_exclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_min_exclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_min_inclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_min_inclusive, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_min_inclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_min_inclusive, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_set_min_inclusive(r::Ptr{Cvoid}, value::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_min_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_min_inclusive(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_min_inclusive, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_min_length(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_min_length, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_min_length(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_restriction_min_length, VISTA_LIB), UInt32, (Ptr{Cvoid},), r)
end

function ffi_dcl_restriction_set_min_length(r::Ptr{Cvoid}, value::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_min_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_min_length(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_min_length, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_pattern(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_restriction_pattern, VISTA_LIB), Cstring, (Ptr{Cvoid},), r)
end

function ffi_dcl_restriction_set_pattern(r::Ptr{Cvoid}, value::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_pattern, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_pattern(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_pattern, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_total_digits(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_total_digits, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_total_digits(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_total_digits, VISTA_LIB),
        UInt32,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_set_total_digits(r::Ptr{Cvoid}, value::UInt32)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_total_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, UInt32),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_total_digits(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_total_digits, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_has_white_space(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_has_white_space, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_white_space(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_white_space, VISTA_LIB),
        _FfiWhiteSpace,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_set_white_space(r::Ptr{Cvoid}, value::_FfiWhiteSpace)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_set_white_space, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiWhiteSpace),
        r,
        value,
    )
end

function ffi_dcl_restriction_clear_white_space(r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_restriction_clear_white_space, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        r,
    )
end

function ffi_dcl_restriction_validate_value(
    r::Ptr{Cvoid},
    value::Cstring,
    format::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_dcl_restriction_validate_value, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring, Ptr{Cvoid}),
        r,
        value,
        format,
    )
end

function ffi_dcl_range_create(low::Cdouble, high::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_range_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cdouble, Cdouble),
        low,
        high,
    )
end

function ffi_dcl_range_free(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_range_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), r)
end

function ffi_dcl_range_low(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_range_low, VISTA_LIB), Cdouble, (Ptr{Cvoid},), r)
end

function ffi_dcl_range_high(r::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_range_high, VISTA_LIB), Cdouble, (Ptr{Cvoid},), r)
end

function ffi_dcl_range_set_low(r::Ptr{Cvoid}, low::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_range_set_low, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        r,
        low,
    )
end

function ffi_dcl_range_set_high(r::Ptr{Cvoid}, high::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_range_set_high, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        r,
        high,
    )
end

function ffi_dcl_format_create(type_::Cstring)
    ccall((:dnv_vista_sdk_dcl_format_create, VISTA_LIB), Ptr{Cvoid}, (Cstring,), type_)
end

function ffi_dcl_format_free(f::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_format_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), f)
end

function ffi_dcl_format_type(f::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_format_type, VISTA_LIB), Cstring, (Ptr{Cvoid},), f)
end

function ffi_dcl_format_set_type(f::Ptr{Cvoid}, type_::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_format_set_type, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        f,
        type_,
    )
end

function ffi_dcl_format_restriction(f::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_format_restriction, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), f)
end

function ffi_dcl_format_set_restriction(f::Ptr{Cvoid}, r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_format_set_restriction, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        f,
        r,
    )
end

function ffi_dcl_format_clear_restriction(f::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_format_clear_restriction, VISTA_LIB), Cvoid, (Ptr{Cvoid},), f)
end

function ffi_dcl_format_validate_value(f::Ptr{Cvoid}, value::Cstring, out::Ptr{Ptr{Cvoid}})
    ccall(
        (:dnv_vista_sdk_dcl_format_validate_value, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring, Ptr{Ptr{Cvoid}}),
        f,
        value,
        out,
    )
end

function ffi_dcl_data_channel_type_create(type_::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        type_,
    )
end

function ffi_dcl_data_channel_type_free(dct::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_data_channel_type_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), dct)
end

function ffi_dcl_data_channel_type_type(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_type, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_data_channel_type_set_type(dct::Ptr{Cvoid}, type_::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_set_type, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        dct,
        type_,
    )
end

function ffi_dcl_data_channel_type_has_update_cycle(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_has_update_cycle, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_data_channel_type_update_cycle(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_update_cycle, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_data_channel_type_set_update_cycle(dct::Ptr{Cvoid}, value::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_set_update_cycle, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        dct,
        value,
    )
end

function ffi_dcl_data_channel_type_clear_update_cycle(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_clear_update_cycle, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_data_channel_type_has_calculation_period(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_has_calculation_period, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_data_channel_type_calculation_period(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_calculation_period, VISTA_LIB),
        Cdouble,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_data_channel_type_set_calculation_period(dct::Ptr{Cvoid}, value::Cdouble)
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_set_calculation_period, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cdouble),
        dct,
        value,
    )
end

function ffi_dcl_data_channel_type_clear_calculation_period(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_clear_calculation_period, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_data_channel_type_is_alert(dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_type_is_alert, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        dct,
    )
end

function ffi_dcl_name_object_create_default()
    ccall((:dnv_vista_sdk_dcl_name_object_create_default, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_dcl_name_object_create(naming_rule::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_name_object_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        naming_rule,
    )
end

function ffi_dcl_name_object_free(no::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_name_object_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), no)
end

function ffi_dcl_name_object_naming_rule(no::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_name_object_naming_rule, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        no,
    )
end

function ffi_dcl_name_object_set_naming_rule(no::Ptr{Cvoid}, naming_rule::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_name_object_set_naming_rule, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        no,
        naming_rule,
    )
end

function ffi_dcl_name_object_custom_name_objects(no::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_name_object_custom_name_objects, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        no,
    )
end

function ffi_dcl_name_object_set_custom_name_objects(no::Ptr{Cvoid}, value::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_name_object_set_custom_name_objects, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        no,
        value,
    )
end

function ffi_dcl_name_object_clear_custom_name_objects(no::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_name_object_clear_custom_name_objects, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        no,
    )
end

function ffi_dcl_unit_create(unit_symbol::Cstring)
    ccall((:dnv_vista_sdk_dcl_unit_create, VISTA_LIB), Ptr{Cvoid}, (Cstring,), unit_symbol)
end

function ffi_dcl_unit_free(u::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_unit_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), u)
end

function ffi_dcl_unit_unit_symbol(u::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_unit_unit_symbol, VISTA_LIB), Cstring, (Ptr{Cvoid},), u)
end

function ffi_dcl_unit_set_unit_symbol(u::Ptr{Cvoid}, unit_symbol::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_unit_set_unit_symbol, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        u,
        unit_symbol,
    )
end

function ffi_dcl_unit_quantity_name(u::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_unit_quantity_name, VISTA_LIB), Cstring, (Ptr{Cvoid},), u)
end

function ffi_dcl_unit_set_quantity_name(u::Ptr{Cvoid}, quantity_name::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_unit_set_quantity_name, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        u,
        quantity_name,
    )
end

function ffi_dcl_unit_clear_quantity_name(u::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_unit_clear_quantity_name, VISTA_LIB), Cvoid, (Ptr{Cvoid},), u)
end

function ffi_dcl_unit_set_custom_elements(u::Ptr{Cvoid}, value::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_unit_set_custom_elements, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        u,
        value,
    )
end

function ffi_dcl_unit_clear_custom_elements(u::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_unit_clear_custom_elements, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        u,
    )
end

function ffi_dcl_property_create(dct::Ptr{Cvoid}, fmt::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        dct,
        fmt,
    )
end

function ffi_dcl_property_free(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_data_channel_type(p::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_data_channel_type, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        p,
    )
end

function ffi_dcl_property_set_data_channel_type(p::Ptr{Cvoid}, dct::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_set_data_channel_type, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        p,
        dct,
    )
end

function ffi_dcl_property_format(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_format, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_set_format(p::Ptr{Cvoid}, fmt::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_set_format, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        p,
        fmt,
    )
end

function ffi_dcl_property_range(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_range, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_set_range(p::Ptr{Cvoid}, r::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_set_range, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        p,
        r,
    )
end

function ffi_dcl_property_clear_range(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_clear_range, VISTA_LIB), Cvoid, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_unit(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_unit, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_set_unit(p::Ptr{Cvoid}, u::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_set_unit, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        p,
        u,
    )
end

function ffi_dcl_property_clear_unit(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_clear_unit, VISTA_LIB), Cvoid, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_quality_coding(p::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_quality_coding, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        p,
    )
end

function ffi_dcl_property_set_quality_coding(p::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_property_set_quality_coding, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        p,
        v,
    )
end

function ffi_dcl_property_clear_quality_coding(p::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_clear_quality_coding, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        p,
    )
end

function ffi_dcl_property_alert_priority(p::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_alert_priority, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        p,
    )
end

function ffi_dcl_property_set_alert_priority(p::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_property_set_alert_priority, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        p,
        v,
    )
end

function ffi_dcl_property_clear_alert_priority(p::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_clear_alert_priority, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        p,
    )
end

function ffi_dcl_property_name(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_name, VISTA_LIB), Cstring, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_set_name(p::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_property_set_name, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        p,
        v,
    )
end

function ffi_dcl_property_clear_name(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_clear_name, VISTA_LIB), Cvoid, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_remarks(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_remarks, VISTA_LIB), Cstring, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_set_remarks(p::Ptr{Cvoid}, v::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_property_set_remarks, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        p,
        v,
    )
end

function ffi_dcl_property_clear_remarks(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_clear_remarks, VISTA_LIB), Cvoid, (Ptr{Cvoid},), p)
end

function ffi_dcl_property_custom_properties(p::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_custom_properties, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        p,
    )
end

function ffi_dcl_property_set_custom_properties(p::Ptr{Cvoid}, value::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_set_custom_properties, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        p,
        value,
    )
end

function ffi_dcl_property_clear_custom_properties(p::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_property_clear_custom_properties, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        p,
    )
end

function ffi_dcl_property_validate(p::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_property_validate, VISTA_LIB), Cint, (Ptr{Cvoid},), p)
end

function ffi_dcl_configuration_reference_create(id::Cstring, timestamp)
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, _FfiDateTimeOffset),
        id,
        timestamp,
    )
end

function ffi_dcl_configuration_reference_free(cr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        cr,
    )
end

function ffi_dcl_configuration_reference_id(cr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_id, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        cr,
    )
end

function ffi_dcl_configuration_reference_set_id(cr::Ptr{Cvoid}, id::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_set_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        cr,
        id,
    )
end

function ffi_dcl_configuration_reference_version(cr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_version, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        cr,
    )
end

function ffi_dcl_configuration_reference_set_version(cr::Ptr{Cvoid}, version::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_set_version, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        cr,
        version,
    )
end

function ffi_dcl_configuration_reference_clear_version(cr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_clear_version, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        cr,
    )
end

function ffi_dcl_configuration_reference_timestamp(cr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_timestamp, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        cr,
    )
end

function ffi_dcl_configuration_reference_set_timestamp(cr::Ptr{Cvoid}, timestamp)
    ccall(
        (:dnv_vista_sdk_dcl_configuration_reference_set_timestamp, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiDateTimeOffset),
        cr,
        timestamp,
    )
end

function ffi_dcl_version_information_create_default()
    ccall(
        (:dnv_vista_sdk_dcl_version_information_create_default, VISTA_LIB),
        Ptr{Cvoid},
        (),
    )
end

function ffi_dcl_version_information_create(
    naming_rule::Cstring,
    naming_scheme_version::Cstring,
)
    ccall(
        (:dnv_vista_sdk_dcl_version_information_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Cstring),
        naming_rule,
        naming_scheme_version,
    )
end

function ffi_dcl_version_information_free(vi::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_version_information_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        vi,
    )
end

function ffi_dcl_version_information_naming_rule(vi::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_version_information_naming_rule, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        vi,
    )
end

function ffi_dcl_version_information_set_naming_rule(vi::Ptr{Cvoid}, naming_rule::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_version_information_set_naming_rule, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        vi,
        naming_rule,
    )
end

function ffi_dcl_version_information_naming_scheme_version(vi::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_version_information_naming_scheme_version, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        vi,
    )
end

function ffi_dcl_version_information_set_naming_scheme_version(
    vi::Ptr{Cvoid},
    naming_scheme_version::Cstring,
)
    ccall(
        (:dnv_vista_sdk_dcl_version_information_set_naming_scheme_version, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        vi,
        naming_scheme_version,
    )
end

function ffi_dcl_version_information_reference_url(vi::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_version_information_reference_url, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        vi,
    )
end

function ffi_dcl_version_information_set_reference_url(vi::Ptr{Cvoid}, url::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_version_information_set_reference_url, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        vi,
        url,
    )
end

function ffi_dcl_version_information_clear_reference_url(vi::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_version_information_clear_reference_url, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        vi,
    )
end

function ffi_dcl_channel_id_create(local_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        local_id,
    )
end

function ffi_dcl_channel_id_free(cid::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_channel_id_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), cid)
end

function ffi_dcl_channel_id_local_id(cid::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_local_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        cid,
    )
end

function ffi_dcl_channel_id_set_local_id(cid::Ptr{Cvoid}, local_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_set_local_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        cid,
        local_id,
    )
end

function ffi_dcl_channel_id_short_id(cid::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_channel_id_short_id, VISTA_LIB), Cstring, (Ptr{Cvoid},), cid)
end

function ffi_dcl_channel_id_set_short_id(cid::Ptr{Cvoid}, short_id::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_set_short_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        cid,
        short_id,
    )
end

function ffi_dcl_channel_id_clear_short_id(cid::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_clear_short_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        cid,
    )
end

function ffi_dcl_channel_id_name_object(cid::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_name_object, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        cid,
    )
end

function ffi_dcl_channel_id_set_name_object(cid::Ptr{Cvoid}, no::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_set_name_object, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        cid,
        no,
    )
end

function ffi_dcl_channel_id_clear_name_object(cid::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_channel_id_clear_name_object, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        cid,
    )
end

function ffi_dcl_header_create(ship_id::Ptr{Cvoid}, data_channel_list_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ship_id,
        data_channel_list_id,
    )
end

function ffi_dcl_header_free(h::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_header_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), h)
end

function ffi_dcl_header_ship_id(h::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_header_ship_id, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), h)
end

function ffi_dcl_header_set_ship_id(h::Ptr{Cvoid}, ship_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_set_ship_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        h,
        ship_id,
    )
end

function ffi_dcl_header_data_channel_list_id(h::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_data_channel_list_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        h,
    )
end

function ffi_dcl_header_set_data_channel_list_id(h::Ptr{Cvoid}, cr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_set_data_channel_list_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        h,
        cr,
    )
end

function ffi_dcl_header_version_information(h::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_version_information, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        h,
    )
end

function ffi_dcl_header_set_version_information(h::Ptr{Cvoid}, vi::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_set_version_information, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        h,
        vi,
    )
end

function ffi_dcl_header_clear_version_information(h::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_clear_version_information, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        h,
    )
end

function ffi_dcl_header_author(h::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_header_author, VISTA_LIB), Cstring, (Ptr{Cvoid},), h)
end

function ffi_dcl_header_set_author(h::Ptr{Cvoid}, author::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_header_set_author, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Cstring),
        h,
        author,
    )
end

function ffi_dcl_header_clear_author(h::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_header_clear_author, VISTA_LIB), Cvoid, (Ptr{Cvoid},), h)
end

function ffi_dcl_header_has_date_created(h::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_header_has_date_created, VISTA_LIB), Cint, (Ptr{Cvoid},), h)
end

function ffi_dcl_header_date_created(h::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_date_created, VISTA_LIB),
        _FfiDateTimeOffset,
        (Ptr{Cvoid},),
        h,
    )
end

function ffi_dcl_header_set_date_created(h::Ptr{Cvoid}, date_created)
    ccall(
        (:dnv_vista_sdk_dcl_header_set_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, _FfiDateTimeOffset),
        h,
        date_created,
    )
end

function ffi_dcl_header_clear_date_created(h::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_clear_date_created, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        h,
    )
end

function ffi_dcl_header_custom_headers(h::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_custom_headers, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        h,
    )
end

function ffi_dcl_header_set_custom_headers(h::Ptr{Cvoid}, value::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_set_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        h,
        value,
    )
end

function ffi_dcl_header_clear_custom_headers(h::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_header_clear_custom_headers, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        h,
    )
end

function ffi_dcl_data_channel_create(channel_id::Ptr{Cvoid}, property::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        channel_id,
        property,
    )
end

function ffi_dcl_data_channel_free(dc::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_data_channel_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), dc)
end

function ffi_dcl_data_channel_channel_id(dc::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_channel_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        dc,
    )
end

function ffi_dcl_data_channel_set_channel_id(dc::Ptr{Cvoid}, cid::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_set_channel_id, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        dc,
        cid,
    )
end

function ffi_dcl_data_channel_property(dc::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_property, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        dc,
    )
end

function ffi_dcl_data_channel_set_property(dc::Ptr{Cvoid}, property::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_set_property, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        dc,
        property,
    )
end

function ffi_dcl_data_channel_list_create()
    ccall((:dnv_vista_sdk_dcl_data_channel_list_create, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_dcl_data_channel_list_free(list::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        list,
    )
end

function ffi_dcl_data_channel_list_size(list::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_size, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        list,
    )
end

function ffi_dcl_data_channel_list_at(list::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        list,
        index,
    )
end

function ffi_dcl_data_channel_list_from_short_id(list::Ptr{Cvoid}, short_id::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_from_short_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        list,
        short_id,
    )
end

function ffi_dcl_data_channel_list_from_local_id(list::Ptr{Cvoid}, local_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_from_local_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        list,
        local_id,
    )
end

function ffi_dcl_data_channel_list_add(list::Ptr{Cvoid}, dc::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_add, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        list,
        dc,
    )
end

function ffi_dcl_data_channel_list_remove(list::Ptr{Cvoid}, dc::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_remove, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        list,
        dc,
    )
end

function ffi_dcl_data_channel_list_clear(list::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_data_channel_list_clear, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        list,
    )
end

function ffi_dcl_package_create(header::Ptr{Cvoid}, data_channel_list::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_package_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        header,
        data_channel_list,
    )
end

function ffi_dcl_package_free(pkg::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_package_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), pkg)
end

function ffi_dcl_package_header(pkg::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_package_header, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), pkg)
end

function ffi_dcl_package_set_header(pkg::Ptr{Cvoid}, header::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_package_set_header, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        pkg,
        header,
    )
end

function ffi_dcl_package_data_channel_list(pkg::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_package_data_channel_list, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        pkg,
    )
end

function ffi_dcl_package_set_data_channel_list(pkg::Ptr{Cvoid}, list::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_package_set_data_channel_list, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        pkg,
        list,
    )
end

function ffi_dcl_list_package_create(pkg::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_list_package_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        pkg,
    )
end

function ffi_dcl_list_package_free(lp::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_dcl_list_package_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), lp)
end

function ffi_dcl_list_package_package(lp::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_list_package_package, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        lp,
    )
end

function ffi_dcl_list_package_set_package(lp::Ptr{Cvoid}, pkg::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_list_package_set_package, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        lp,
        pkg,
    )
end

function ffi_dcl_list_package_data_channel_list(lp::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_dcl_list_package_data_channel_list, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        lp,
    )
end

function ffi_dcl_list_package_from_json(json::Cstring)
    ccall(
        (:dnv_vista_sdk_dcl_list_package_from_json, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        json,
    )
end

function ffi_dcl_list_package_to_json(lp::Ptr{Cvoid}, pretty_print::Cint)
    ccall(
        (:dnv_vista_sdk_dcl_list_package_to_json, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid}, Cint),
        lp,
        pretty_print,
    )
end
