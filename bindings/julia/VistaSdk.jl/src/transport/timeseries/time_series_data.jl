"""
    TsdTimeSpanRef

Borrowed reference to a [`TsdTimeSpan`](@ref) obtained via [`time_span`](@ref).
Valid only for the lifetime of the owning object.
"""
struct TsdTimeSpanRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    TsdTimeSpan

ISO 19848 Table 25 - interval between two timestamps. Freed when garbage-collected.

Use [`TsdTimeSpan(start, end_)`](@ref) to construct. Throws if `start` is after `end_`.
"""
mutable struct TsdTimeSpan
    _ptr::Ptr{Cvoid}

    function TsdTimeSpan(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdTimeSpan: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_time_span_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TsdTimeSpan(start::DateTimeOffset, end_::DateTimeOffset) -> TsdTimeSpan

Construct a time span. Throws [`VistaError`](@ref) if `start` is after `end_`.
"""
function TsdTimeSpan(start::DateTimeOffset, end_::DateTimeOffset)
    ptr = ffi_tsd_time_span_create(_ffi(start), _ffi(end_))
    ptr == C_NULL &&
        throw(VistaError(InvalidArgument, "TsdTimeSpan: start must not be after end"))
    TsdTimeSpan(ptr)
end

"""
    start_time(ts) -> DateTimeOffset

Return the start timestamp of the time span.
"""
function start_time(ts::TsdTimeSpan)
    _from_ffi(ffi_tsd_time_span_start(ts._ptr))
end

function start_time(ts::TsdTimeSpanRef)
    _from_ffi(ffi_tsd_time_span_start(ts._ptr))
end

"""
    end_time(ts) -> DateTimeOffset

Return the end timestamp of the time span.
"""
function end_time(ts::TsdTimeSpan)
    _from_ffi(ffi_tsd_time_span_end(ts._ptr))
end

function end_time(ts::TsdTimeSpanRef)
    _from_ffi(ffi_tsd_time_span_end(ts._ptr))
end

"""
    set_start!(ts::TsdTimeSpan, start::DateTimeOffset)

Update the start timestamp. Throws if `start` is after the current end.
"""
function set_start!(ts::TsdTimeSpan, start::DateTimeOffset)
    ffi_tsd_time_span_set_start(ts._ptr, _ffi(start))
end

"""
    set_end!(ts::TsdTimeSpan, end_::DateTimeOffset)

Update the end timestamp. Throws if `end_` is before the current start.
"""
function set_end!(ts::TsdTimeSpan, end_::DateTimeOffset)
    ffi_tsd_time_span_set_end(ts._ptr, _ffi(end_))
end

"""
    TsdConfigRefRef

Borrowed reference to a [`TsdConfigRef`](@ref) obtained via [`system_configuration_at`](@ref)
or [`data_configuration`](@ref). Valid only for the lifetime of the owning object.
"""
struct TsdConfigRefRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    TsdConfigRef

ISO 19848 Table 26 - configuration reference for time series. Freed when garbage-collected.

Use [`TsdConfigRef(id, time_stamp)`](@ref) to construct.
"""
mutable struct TsdConfigRef
    _ptr::Ptr{Cvoid}

    function TsdConfigRef(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdConfigRef: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_config_ref_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TsdConfigRef(id::AbstractString, time_stamp::DateTimeOffset) -> TsdConfigRef

Construct a configuration reference with `id` and `time_stamp`.
"""
function TsdConfigRef(id::AbstractString, time_stamp::DateTimeOffset)
    str = String(id)
    ptr = GC.@preserve str ffi_tsd_config_ref_create(
        Base.unsafe_convert(Cstring, str),
        _ffi(time_stamp),
    )
    ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdConfigRef: NULL pointer"))
    TsdConfigRef(ptr)
end

"""
    config_id(r) -> String

Return the configuration identifier.
"""
function config_id(r::TsdConfigRef)
    unsafe_string(ffi_tsd_config_ref_id(r._ptr))
end

function config_id(r::TsdConfigRefRef)
    unsafe_string(ffi_tsd_config_ref_id(r._ptr))
end

"""
    set_config_id!(r::TsdConfigRef, id::AbstractString)

Set the configuration identifier.
"""
function set_config_id!(r::TsdConfigRef, id::AbstractString)
    str = String(id)
    GC.@preserve str ffi_tsd_config_ref_set_id(r._ptr, Base.unsafe_convert(Cstring, str))
end

"""
    time_stamp(r) -> DateTimeOffset

Return the timestamp of the configuration reference.
"""
function time_stamp(r::TsdConfigRef)
    _from_ffi(ffi_tsd_config_ref_timestamp(r._ptr))
end

function time_stamp(r::TsdConfigRefRef)
    _from_ffi(ffi_tsd_config_ref_timestamp(r._ptr))
end

"""
    set_time_stamp!(r::TsdConfigRef, ts::DateTimeOffset)

Set the timestamp.
"""
function set_time_stamp!(r::TsdConfigRef, ts::DateTimeOffset)
    ffi_tsd_config_ref_set_timestamp(r._ptr, _ffi(ts))
end

"""
    TsdHeader

ISO 19848 Table 24 - header for a time series package. Freed when garbage-collected.

Use [`TsdHeader(ship_id)`](@ref) to construct.
"""
mutable struct TsdHeader
    _ptr::Ptr{Cvoid}

    function TsdHeader(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdHeader: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_header_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TsdHeader(ship_id::ShipId) -> TsdHeader

Construct a time series header with the given ship identifier.
"""
function TsdHeader(ship_id::ShipId)
    ship_ptr = _ship_id_to_ptr(ship_id)
    ptr = ffi_tsd_header_create(ship_ptr)
    ffi_ship_id_free(ship_ptr)
    ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdHeader: NULL pointer"))
    TsdHeader(ptr)
end

"""
    ship_id(h::TsdHeader) -> ShipId

Return the ship identifier.
"""
function ship_id(h::TsdHeader)
    ptr = ffi_tsd_header_ship_id(h._ptr)
    _ship_id_from_ptr(ptr)
end

"""
    set_ship_id!(h::TsdHeader, sid::ShipId)

Set the ship identifier.
"""
function set_ship_id!(h::TsdHeader, sid::ShipId)
    ship_ptr = _ship_id_to_ptr(sid)
    ffi_tsd_header_set_ship_id(h._ptr, ship_ptr)
    ffi_ship_id_free(ship_ptr)
end

"""
    time_span(h::TsdHeader) -> Union{TsdTimeSpanRef,Nothing}

Return a borrowed view of the time span if set, or `nothing`.
"""
function time_span(h::TsdHeader)
    ptr = ffi_tsd_header_time_span(h._ptr)
    ptr == C_NULL ? nothing : TsdTimeSpanRef(ptr, h)
end

"""
    set_time_span!(h::TsdHeader, ts::TsdTimeSpan)

Set the time span. Copies `ts` into the header.
"""
function set_time_span!(h::TsdHeader, ts::TsdTimeSpan)
    ffi_tsd_header_set_time_span(h._ptr, ts._ptr)
end

"""
    clear_time_span!(h::TsdHeader)

Clear the time span.
"""
function clear_time_span!(h::TsdHeader)
    ffi_tsd_header_clear_time_span(h._ptr)
end

"""
    date_created(h::TsdHeader) -> Union{DateTimeOffset,Nothing}

Return the creation date if set.
"""
function date_created(h::TsdHeader)
    ffi_tsd_header_has_date_created(h._ptr) != 0 ?
    _from_ffi(ffi_tsd_header_date_created(h._ptr)) : nothing
end

"""
    set_date_created!(h::TsdHeader, ts::DateTimeOffset)

Set the creation date.
"""
function set_date_created!(h::TsdHeader, ts::DateTimeOffset)
    ffi_tsd_header_set_date_created(h._ptr, _ffi(ts))
end

"""
    clear_date_created!(h::TsdHeader)

Clear the creation date.
"""
function clear_date_created!(h::TsdHeader)
    ffi_tsd_header_clear_date_created(h._ptr)
end

"""
    date_modified(h::TsdHeader) -> Union{DateTimeOffset,Nothing}

Return the modification date if set.
"""
function date_modified(h::TsdHeader)
    ffi_tsd_header_has_date_modified(h._ptr) != 0 ?
    _from_ffi(ffi_tsd_header_date_modified(h._ptr)) : nothing
end

"""
    set_date_modified!(h::TsdHeader, ts::DateTimeOffset)

Set the modification date.
"""
function set_date_modified!(h::TsdHeader, ts::DateTimeOffset)
    ffi_tsd_header_set_date_modified(h._ptr, _ffi(ts))
end

"""
    clear_date_modified!(h::TsdHeader)

Clear the modification date.
"""
function clear_date_modified!(h::TsdHeader)
    ffi_tsd_header_clear_date_modified(h._ptr)
end

"""
    author(h::TsdHeader) -> Union{String,Nothing}

Return the author if set.
"""
function author(h::TsdHeader)
    cs = ffi_tsd_header_author(h._ptr)
    cs == C_NULL ? nothing : unsafe_string(cs)
end

"""
    set_author!(h::TsdHeader, a::AbstractString)

Set the author.
"""
function set_author!(h::TsdHeader, a::AbstractString)
    str = String(a)
    GC.@preserve str ffi_tsd_header_set_author(h._ptr, Base.unsafe_convert(Cstring, str))
end

"""
    clear_author!(h::TsdHeader)

Clear the author.
"""
function clear_author!(h::TsdHeader)
    ffi_tsd_header_clear_author(h._ptr)
end

"""
    system_configuration_count(h::TsdHeader) -> Int

Return the number of system configuration entries.
"""
function system_configuration_count(h::TsdHeader)
    Int(ffi_tsd_header_system_configuration_count(h._ptr))
end

"""
    system_configuration_at(h::TsdHeader, index::Int) -> Union{TsdConfigRefRef,Nothing}

Return a borrowed view of the configuration reference at `index` (1-based), or `nothing`.
"""
function system_configuration_at(h::TsdHeader, index::Int)
    ptr = ffi_tsd_header_system_configuration_at(h._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TsdConfigRefRef(ptr, h)
end

"""
    set_system_configuration!(h::TsdHeader, entries::AbstractVector{TsdConfigRef})

Set the system configuration list.
"""
function set_system_configuration!(h::TsdHeader, entries::AbstractVector{TsdConfigRef})
    ptrs = [e._ptr for e in entries]
    raw = isempty(ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(ptrs)
    ffi_tsd_header_set_system_configuration(h._ptr, raw, Csize_t(length(ptrs)))
end

"""
    clear_system_configuration!(h::TsdHeader)

Clear the system configuration list.
"""
function clear_system_configuration!(h::TsdHeader)
    ffi_tsd_header_clear_system_configuration(h._ptr)
end

"""
    custom_headers(h::TsdHeader) -> Union{SerializableDocumentRef,Nothing}

Return a borrowed view of the custom headers if set.
"""
function custom_headers(h::TsdHeader)
    ptr = ffi_tsd_header_custom_headers(h._ptr)
    ptr == C_NULL ? nothing : SerializableDocumentRef(ptr, h)
end

"""
    set_custom_headers!(h::TsdHeader, doc::SerializableDocument)

Set custom headers, transferring ownership of `doc`. Do not use `doc` after this call.
"""
function set_custom_headers!(h::TsdHeader, doc::SerializableDocument)
    ffi_tsd_header_set_custom_headers(h._ptr, _transfer_ownership(doc))
end

"""
    clear_custom_headers!(h::TsdHeader)

Clear the custom headers.
"""
function clear_custom_headers!(h::TsdHeader)
    ffi_tsd_header_clear_custom_headers(h._ptr)
end

"""
    TabularDataSetRef

Borrowed reference to a [`TabularDataSet`](@ref) obtained via [`data_set_at`](@ref).
Valid only for the lifetime of the owning object.
"""
struct TabularDataSetRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    TabularDataSet

ISO 19848 Table 30 - one row of tabular data. Freed when garbage-collected.

Use [`TabularDataSet(time_stamp, values)`](@ref) to construct.
"""
mutable struct TabularDataSet
    _ptr::Ptr{Cvoid}

    function TabularDataSet(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TabularDataSet: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_tabular_data_set_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TabularDataSet(time_stamp::DateTimeOffset, values::AbstractVector{<:AbstractString}) -> TabularDataSet

Construct a tabular data set with the given timestamp and string values.
"""
function TabularDataSet(
    time_stamp::DateTimeOffset,
    values::AbstractVector{<:AbstractString},
)
    strs = [Base.unsafe_convert(Cstring, s) for s in values]
    ptr = GC.@preserve values ffi_tsd_tabular_data_set_create(
        _ffi(time_stamp),
        pointer(strs),
        Csize_t(length(strs)),
    )
    ptr == C_NULL && throw(VistaError(InvalidArgument, "TabularDataSet: NULL pointer"))
    TabularDataSet(ptr)
end

"""
    time_stamp(ds) -> DateTimeOffset

Return the timestamp of the data set.
"""
function time_stamp(ds::TabularDataSet)
    _from_ffi(ffi_tsd_tabular_data_set_time_stamp(ds._ptr))
end

function time_stamp(ds::TabularDataSetRef)
    _from_ffi(ffi_tsd_tabular_data_set_time_stamp(ds._ptr))
end

"""
    values(ds) -> Vector{String}

Return all values in the data set.
"""
function values(ds::TabularDataSet)
    n = Int(ffi_tsd_tabular_data_set_value_count(ds._ptr))
    [unsafe_string(ffi_tsd_tabular_data_set_value_at(ds._ptr, Csize_t(i - 1))) for i = 1:n]
end

function values(ds::TabularDataSetRef)
    n = Int(ffi_tsd_tabular_data_set_value_count(ds._ptr))
    [unsafe_string(ffi_tsd_tabular_data_set_value_at(ds._ptr, Csize_t(i - 1))) for i = 1:n]
end

"""
    set_values!(ds::TabularDataSet, vals::AbstractVector{<:AbstractString})

Replace all values in the data set.
"""
function set_values!(ds::TabularDataSet, vals::AbstractVector{<:AbstractString})
    strs = [Base.unsafe_convert(Cstring, s) for s in vals]
    GC.@preserve vals ffi_tsd_tabular_data_set_set_values(
        ds._ptr,
        pointer(strs),
        Csize_t(length(strs)),
    )
end

"""
    quality(ds) -> Union{Vector{String},Nothing}

Return quality indicators if set, or `nothing`.
"""
function quality(ds::TabularDataSet)
    n = Int(ffi_tsd_tabular_data_set_quality_count(ds._ptr))
    n == 0 ? nothing :
    [
        unsafe_string(ffi_tsd_tabular_data_set_quality_at(ds._ptr, Csize_t(i - 1))) for
        i = 1:n
    ]
end

function quality(ds::TabularDataSetRef)
    n = Int(ffi_tsd_tabular_data_set_quality_count(ds._ptr))
    n == 0 ? nothing :
    [
        unsafe_string(ffi_tsd_tabular_data_set_quality_at(ds._ptr, Csize_t(i - 1))) for
        i = 1:n
    ]
end

"""
    set_quality!(ds::TabularDataSet, q::AbstractVector{<:AbstractString})

Set the quality indicators.
"""
function set_quality!(ds::TabularDataSet, q::AbstractVector{<:AbstractString})
    strs = [Base.unsafe_convert(Cstring, s) for s in q]
    GC.@preserve q ffi_tsd_tabular_data_set_set_quality(
        ds._ptr,
        pointer(strs),
        Csize_t(length(strs)),
    )
end

"""
    clear_quality!(ds::TabularDataSet)

Remove all quality indicators.
"""
function clear_quality!(ds::TabularDataSet)
    ffi_tsd_tabular_data_set_clear_quality(ds._ptr)
end

"""
    TabularDataRef

Borrowed reference to a [`TabularData`](@ref) obtained via [`tabular_data_at`](@ref).
Valid only for the lifetime of the owning object.
"""
struct TabularDataRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    TabularData

ISO 19848 Table 28 - column-oriented tabular data. Freed when garbage-collected.

Use [`TabularData(channel_ids, data_sets)`](@ref) to construct.
"""
mutable struct TabularData
    _ptr::Ptr{Cvoid}

    function TabularData(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TabularData: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_tabular_data_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TabularData(
        channel_ids::AbstractVector{TsdChannelId},
        data_sets::AbstractVector{TabularDataSet},
    ) -> TabularData

Construct tabular data from channel IDs and data sets.
"""
function TabularData(
    channel_ids::AbstractVector{TsdChannelId},
    data_sets::AbstractVector{TabularDataSet},
)
    id_ptrs = [id._ptr for id in channel_ids]
    ds_ptrs = [ds._ptr for ds in data_sets]
    raw_ids = isempty(id_ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(id_ptrs)
    raw_ds = isempty(ds_ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(ds_ptrs)
    ptr = ffi_tsd_tabular_data_create(
        raw_ids,
        Csize_t(length(id_ptrs)),
        raw_ds,
        Csize_t(length(ds_ptrs)),
    )
    ptr == C_NULL && throw(VistaError(InvalidArgument, "TabularData: NULL pointer"))
    TabularData(ptr)
end

"""
    channel_id_count(td) -> Int

Return the number of data channel IDs.
"""
function channel_id_count(td::TabularData)
    Int(ffi_tsd_tabular_data_channel_id_count(td._ptr))
end

function channel_id_count(td::TabularDataRef)
    Int(ffi_tsd_tabular_data_channel_id_count(td._ptr))
end

"""
    channel_id_at(td, index::Int) -> Union{TsdChannelIdRef,Nothing}

Return a borrowed view of the channel ID at `index` (1-based), or `nothing`.
"""
function channel_id_at(td::TabularData, index::Int)
    ptr = ffi_tsd_tabular_data_channel_id_at(td._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TsdChannelIdRef(ptr, td)
end

function channel_id_at(td::TabularDataRef, index::Int)
    ptr = ffi_tsd_tabular_data_channel_id_at(td._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TsdChannelIdRef(ptr, td._owner)
end

"""
    data_set_count(td) -> Int

Return the number of data sets.
"""
function data_set_count(td::TabularData)
    Int(ffi_tsd_tabular_data_data_set_count(td._ptr))
end

function data_set_count(td::TabularDataRef)
    Int(ffi_tsd_tabular_data_data_set_count(td._ptr))
end

"""
    data_set_at(td, index::Int) -> Union{TabularDataSetRef,Nothing}

Return a borrowed view of the data set at `index` (1-based), or `nothing`.
"""
function data_set_at(td::TabularData, index::Int)
    ptr = ffi_tsd_tabular_data_data_set_at(td._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TabularDataSetRef(ptr, td)
end

function data_set_at(td::TabularDataRef, index::Int)
    ptr = ffi_tsd_tabular_data_data_set_at(td._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TabularDataSetRef(ptr, td._owner)
end

"""
    validate(td::TabularData) -> Bool

Return `true` if channel count matches values-per-row in every data set.
"""
function validate(td::TabularData)
    ffi_tsd_tabular_data_validate(td._ptr) != 0
end

"""
    EventDataSetRef

Borrowed reference to an [`EventDataSet`](@ref) obtained via [`data_set_at`](@ref).
Valid only for the lifetime of the owning object.
"""
struct EventDataSetRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    EventDataSet

ISO 19848 Table 31 - one event data point. Freed when garbage-collected.

Use [`EventDataSet(time_stamp, channel_id, value)`](@ref) to construct.
"""
mutable struct EventDataSet
    _ptr::Ptr{Cvoid}

    function EventDataSet(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "EventDataSet: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_event_data_set_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    EventDataSet(
        time_stamp::DateTimeOffset,
        channel_id::TsdChannelId,
        value::AbstractString,
    ) -> EventDataSet

Construct an event data set.
"""
function EventDataSet(
    time_stamp::DateTimeOffset,
    channel_id::TsdChannelId,
    value::AbstractString,
)
    str = String(value)
    ptr = GC.@preserve str ffi_tsd_event_data_set_create(
        _ffi(time_stamp),
        channel_id._ptr,
        Base.unsafe_convert(Cstring, str),
    )
    ptr == C_NULL && throw(VistaError(InvalidArgument, "EventDataSet: NULL pointer"))
    EventDataSet(ptr)
end

"""
    time_stamp(eds) -> DateTimeOffset

Return the timestamp.
"""
function time_stamp(eds::EventDataSet)
    _from_ffi(ffi_tsd_event_data_set_time_stamp(eds._ptr))
end

function time_stamp(eds::EventDataSetRef)
    _from_ffi(ffi_tsd_event_data_set_time_stamp(eds._ptr))
end

"""
    channel_id(eds) -> TsdChannelIdRef

Return a borrowed view of the channel ID.
"""
function channel_id(eds::EventDataSet)
    ptr = ffi_tsd_event_data_set_data_channel_id(eds._ptr)
    TsdChannelIdRef(ptr, eds)
end

function channel_id(eds::EventDataSetRef)
    ptr = ffi_tsd_event_data_set_data_channel_id(eds._ptr)
    TsdChannelIdRef(ptr, eds._owner)
end

"""
    value(eds) -> String

Return the event value.
"""
function value(eds::EventDataSet)
    unsafe_string(ffi_tsd_event_data_set_value(eds._ptr))
end

function value(eds::EventDataSetRef)
    unsafe_string(ffi_tsd_event_data_set_value(eds._ptr))
end

"""
    quality(eds) -> Union{String,Nothing}

Return the quality indicator if set, or `nothing`.
"""
function quality(eds::EventDataSet)
    cs = ffi_tsd_event_data_set_quality(eds._ptr)
    cs == C_NULL ? nothing : unsafe_string(cs)
end

function quality(eds::EventDataSetRef)
    cs = ffi_tsd_event_data_set_quality(eds._ptr)
    cs == C_NULL ? nothing : unsafe_string(cs)
end

"""
    set_quality!(eds::EventDataSet, q::AbstractString)

Set the quality indicator.
"""
function set_quality!(eds::EventDataSet, q::AbstractString)
    str = String(q)
    GC.@preserve str ffi_tsd_event_data_set_set_quality(
        eds._ptr,
        Base.unsafe_convert(Cstring, str),
    )
end

"""
    clear_quality!(eds::EventDataSet)

Clear the quality indicator.
"""
function clear_quality!(eds::EventDataSet)
    ffi_tsd_event_data_set_clear_quality(eds._ptr)
end

"""
    EventDataRef

Borrowed reference to an [`EventData`](@ref) obtained via [`event_data`](@ref).
Valid only for the lifetime of the owning object.
"""
struct EventDataRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    EventData

ISO 19848 Table 29 - collection of event data sets. Freed when garbage-collected.

Use [`EventData()`](@ref) to construct an empty instance, then [`set_data_sets!`](@ref).
"""
mutable struct EventData
    _ptr::Ptr{Cvoid}

    function EventData(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "EventData: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_event_data_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    EventData() -> EventData

Construct an empty event data container.
"""
function EventData()
    ptr = ffi_tsd_event_data_create()
    ptr == C_NULL && throw(VistaError(InvalidArgument, "EventData: NULL pointer"))
    EventData(ptr)
end

"""
    data_set_count(ed) -> Int

Return the number of event data sets.
"""
function data_set_count(ed::EventData)
    Int(ffi_tsd_event_data_data_set_count(ed._ptr))
end

function data_set_count(ed::EventDataRef)
    Int(ffi_tsd_event_data_data_set_count(ed._ptr))
end

"""
    data_set_at(ed, index::Int) -> Union{EventDataSetRef,Nothing}

Return a borrowed view of the event data set at `index` (1-based), or `nothing`.
"""
function data_set_at(ed::EventData, index::Int)
    ptr = ffi_tsd_event_data_data_set_at(ed._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : EventDataSetRef(ptr, ed)
end

function data_set_at(ed::EventDataRef, index::Int)
    ptr = ffi_tsd_event_data_data_set_at(ed._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : EventDataSetRef(ptr, ed._owner)
end

"""
    set_data_sets!(ed::EventData, data_sets::AbstractVector{EventDataSet})

Set the list of event data sets.
"""
function set_data_sets!(ed::EventData, data_sets::AbstractVector{EventDataSet})
    ptrs = [ds._ptr for ds in data_sets]
    raw = isempty(ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(ptrs)
    ffi_tsd_event_data_set_data_set(ed._ptr, raw, Csize_t(length(ptrs)))
end

"""
    clear_data_sets!(ed::EventData)

Clear the event data sets.
"""
function clear_data_sets!(ed::EventData)
    ffi_tsd_event_data_clear_data_set(ed._ptr)
end

"""
    TimeSeriesDataRef

Borrowed reference to a [`TimeSeriesData`](@ref) obtained via
[`time_series_data_at`](@ref). Valid only for the lifetime of the owning object.
"""
struct TimeSeriesDataRef
    _ptr::Ptr{Cvoid}
    _owner::Any
end

"""
    TimeSeriesData

ISO 19848 Table 27 - one time series data block. Freed when garbage-collected.

Use [`TimeSeriesData()`](@ref) to construct.
"""
mutable struct TimeSeriesData
    _ptr::Ptr{Cvoid}

    function TimeSeriesData(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TimeSeriesData: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_time_series_data_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TimeSeriesData() -> TimeSeriesData

Construct an empty time series data block.
"""
function TimeSeriesData()
    ptr = ffi_tsd_time_series_data_create()
    ptr == C_NULL && throw(VistaError(InvalidArgument, "TimeSeriesData: NULL pointer"))
    TimeSeriesData(ptr)
end

"""
    data_configuration(tsd) -> Union{TsdConfigRefRef,Nothing}

Return a borrowed view of the data configuration reference if set.
"""
function data_configuration(tsd::TimeSeriesData)
    ptr = ffi_tsd_time_series_data_data_configuration(tsd._ptr)
    ptr == C_NULL ? nothing : TsdConfigRefRef(ptr, tsd)
end

function data_configuration(tsd::TimeSeriesDataRef)
    ptr = ffi_tsd_time_series_data_data_configuration(tsd._ptr)
    ptr == C_NULL ? nothing : TsdConfigRefRef(ptr, tsd._owner)
end

"""
    set_data_configuration!(tsd::TimeSeriesData, cfg::TsdConfigRef)

Set the data configuration reference. Copies `cfg`.
"""
function set_data_configuration!(tsd::TimeSeriesData, cfg::TsdConfigRef)
    ffi_tsd_time_series_data_set_data_configuration(tsd._ptr, cfg._ptr)
end

"""
    clear_data_configuration!(tsd::TimeSeriesData)

Clear the data configuration reference.
"""
function clear_data_configuration!(tsd::TimeSeriesData)
    ffi_tsd_time_series_data_clear_data_configuration(tsd._ptr)
end

"""
    tabular_data_count(tsd) -> Int

Return the number of tabular data entries.
"""
function tabular_data_count(tsd::TimeSeriesData)
    Int(ffi_tsd_time_series_data_tabular_data_count(tsd._ptr))
end

function tabular_data_count(tsd::TimeSeriesDataRef)
    Int(ffi_tsd_time_series_data_tabular_data_count(tsd._ptr))
end

"""
    tabular_data_at(tsd, index::Int) -> Union{TabularDataRef,Nothing}

Return a borrowed view of the tabular data at `index` (1-based), or `nothing`.
"""
function tabular_data_at(tsd::TimeSeriesData, index::Int)
    ptr = ffi_tsd_time_series_data_tabular_data_at(tsd._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TabularDataRef(ptr, tsd)
end

function tabular_data_at(tsd::TimeSeriesDataRef, index::Int)
    ptr = ffi_tsd_time_series_data_tabular_data_at(tsd._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TabularDataRef(ptr, tsd._owner)
end

"""
    set_tabular_data!(tsd::TimeSeriesData, entries::AbstractVector{TabularData})

Set the list of tabular data entries.
"""
function set_tabular_data!(tsd::TimeSeriesData, entries::AbstractVector{TabularData})
    ptrs = [e._ptr for e in entries]
    raw = isempty(ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(ptrs)
    ffi_tsd_time_series_data_set_tabular_data(tsd._ptr, raw, Csize_t(length(ptrs)))
end

"""
    clear_tabular_data!(tsd::TimeSeriesData)

Clear all tabular data entries.
"""
function clear_tabular_data!(tsd::TimeSeriesData)
    ffi_tsd_time_series_data_clear_tabular_data(tsd._ptr)
end

"""
    event_data(tsd) -> Union{EventDataRef,Nothing}

Return a borrowed view of the event data if set.
"""
function event_data(tsd::TimeSeriesData)
    ptr = ffi_tsd_time_series_data_event_data(tsd._ptr)
    ptr == C_NULL ? nothing : EventDataRef(ptr, tsd)
end

function event_data(tsd::TimeSeriesDataRef)
    ptr = ffi_tsd_time_series_data_event_data(tsd._ptr)
    ptr == C_NULL ? nothing : EventDataRef(ptr, tsd._owner)
end

"""
    set_event_data!(tsd::TimeSeriesData, ed::EventData)

Set the event data. Copies `ed`.
"""
function set_event_data!(tsd::TimeSeriesData, ed::EventData)
    ffi_tsd_time_series_data_set_event_data(tsd._ptr, ed._ptr)
end

"""
    clear_event_data!(tsd::TimeSeriesData)

Clear the event data.
"""
function clear_event_data!(tsd::TimeSeriesData)
    ffi_tsd_time_series_data_clear_event_data(tsd._ptr)
end

"""
    custom_data_kinds(tsd::TimeSeriesData) -> Union{SerializableDocumentRef,Nothing}

Return a borrowed view of custom data kinds if set.
"""
function custom_data_kinds(tsd::TimeSeriesData)
    ptr = ffi_tsd_time_series_data_custom_data_kinds(tsd._ptr)
    ptr == C_NULL ? nothing : SerializableDocumentRef(ptr, tsd)
end

"""
    set_custom_data_kinds!(tsd::TimeSeriesData, doc::SerializableDocument)

Set custom data kinds, transferring ownership of `doc`. Do not use `doc` after this call.
"""
function set_custom_data_kinds!(tsd::TimeSeriesData, doc::SerializableDocument)
    ffi_tsd_time_series_data_set_custom_data_kinds(tsd._ptr, _transfer_ownership(doc))
end

"""
    clear_custom_data_kinds!(tsd::TimeSeriesData)

Clear custom data kinds.
"""
function clear_custom_data_kinds!(tsd::TimeSeriesData)
    ffi_tsd_time_series_data_clear_custom_data_kinds(tsd._ptr)
end

"""
    ValidationResult

Aggregated result of a [`validate`](@ref) call on a [`TimeSeriesData`](@ref).
"""
struct ValidationResult
    _valid::Bool
    _errors::Vector{String}
end

"""
    is_valid(r::ValidationResult) -> Bool

Return `true` if all data points passed validation.
"""
is_valid(r::ValidationResult) = r._valid

"""
    errors(r::ValidationResult) -> Vector{String}

Return all error messages collected during validation.
"""
errors(r::ValidationResult) = r._errors

mutable struct _TsdValidateState
    callback::Function
    errors::Vector{String}
end

"""
    validate(
        tsd::TimeSeriesData,
        dc_package::DclListPackage,
        on_tabular::Function,
        on_event::Function,
    ) -> ValidationResult

Validate `tsd` against `dc_package`. `on_tabular` and `on_event` are called once per data
point with `(ts::DateTimeOffset, dc::DclDataChannelRef, val::Iso19848Value, quality::Union{String,Nothing})`.
Return `true` to accept the point. Return `false` or a `Vector{String}` of error messages to
reject it. Custom messages are forwarded into the result's [`errors`](@ref) list.
`dc` is a borrowed reference valid only for the duration of the callback. Do not store it.
"""
function validate(
    tsd::TimeSeriesData,
    dc_package::DclListPackage,
    on_tabular::Function,
    on_event::Function,
)
    tab_state = _TsdValidateState(on_tabular, String[])
    ev_state = _TsdValidateState(on_event, String[])

    cb = @cfunction(
        (ts_ffi, dc_ptr, val_ptr, qual_ptr, errors_ptr, userdata) -> begin
            state = unsafe_pointer_to_objref(userdata)[]::_TsdValidateState
            ts = _from_ffi(ts_ffi)
            dc = DclDataChannelRef(dc_ptr, nothing)
            val = _read_iso19848_value(val_ptr)
            qual = qual_ptr == C_NULL ? nothing : unsafe_string(qual_ptr)
            result = state.callback(ts, dc, val, qual)
            msgs = if result isa Bool
                result ? nothing : ["validation callback rejected data point"]
            elseif result isa Vector{String}
                isempty(result) ? nothing : result
            else
                nothing
            end
            if msgs !== nothing
                for msg in msgs
                    if errors_ptr != C_NULL
                        GC.@preserve msg ffi_tsd_validation_errors_add(
                            errors_ptr,
                            Base.unsafe_convert(Cstring, msg),
                        )
                    end
                    push!(state.errors, msg)
                end
                return Cint(0)
            end
            Cint(1)
        end,
        Cint,
        (_FfiDateTimeOffset, Ptr{Cvoid}, Ptr{Cvoid}, Cstring, Ptr{Cvoid}, Ptr{Cvoid})
    )

    tab_box = Ref{_TsdValidateState}(tab_state)
    ev_box = Ref{_TsdValidateState}(ev_state)

    valid = GC.@preserve tab_box ev_box begin
        ffi_tsd_time_series_data_validate(
            tsd._ptr,
            dc_package._ptr,
            cb,
            pointer_from_objref(tab_box),
            cb,
            pointer_from_objref(ev_box),
        ) != 0
    end

    all_errors = vcat(tab_state.errors, ev_state.errors)
    if !valid && isempty(all_errors)
        msg_ptr = ffi_last_error_message()
        if msg_ptr != C_NULL
            push!(all_errors, unsafe_string(msg_ptr))
        end
    end
    ValidationResult(valid, all_errors)
end

"""
    TsdPackage

ISO 19848 Table 23 - package wrapping a header and a list of [`TimeSeriesData`](@ref).
Freed when garbage-collected.

Use [`TsdPackage(header, time_series_data)`](@ref) to construct.
"""
mutable struct TsdPackage
    _ptr::Ptr{Cvoid}

    function TsdPackage(ptr::Ptr{Cvoid})
        ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdPackage: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_package_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TsdPackage(
        header::Union{TsdHeader,Nothing},
        time_series_data::AbstractVector{TimeSeriesData},
    ) -> TsdPackage

Construct a time series package. `header` may be `nothing`.
"""
function TsdPackage(
    header::Union{TsdHeader,Nothing},
    time_series_data::AbstractVector{TimeSeriesData},
)
    hptr = header === nothing ? C_NULL : header._ptr
    ptrs = [tsd._ptr for tsd in time_series_data]
    raw = isempty(ptrs) ? Ptr{Ptr{Cvoid}}(C_NULL) : pointer(ptrs)
    ptr = ffi_tsd_package_create(hptr, raw, Csize_t(length(ptrs)))
    ptr == C_NULL && throw(VistaError(InvalidArgument, "TsdPackage: NULL pointer"))
    TsdPackage(ptr)
end

"""
    has_header(pkg::TsdPackage) -> Bool

Return `true` if a header is set.
"""
function has_header(pkg::TsdPackage)
    ffi_tsd_package_header(pkg._ptr) != C_NULL
end

"""
    set_header!(pkg::TsdPackage, h::TsdHeader)

Set the header. Copies `h`.
"""
function set_header!(pkg::TsdPackage, h::TsdHeader)
    ffi_tsd_package_set_header(pkg._ptr, h._ptr)
end

"""
    time_series_data_count(pkg::TsdPackage) -> Int

Return the number of time series data entries.
"""
function time_series_data_count(pkg::TsdPackage)
    Int(ffi_tsd_package_time_series_data_count(pkg._ptr))
end

"""
    time_series_data_is_empty(pkg::TsdPackage) -> Bool

Return `true` if there are no time series data entries.
"""
time_series_data_is_empty(pkg::TsdPackage) = time_series_data_count(pkg) == 0

"""
    time_series_data_at(pkg::TsdPackage, index::Int) -> Union{TimeSeriesDataRef,Nothing}

Return a borrowed view of the time series data at `index` (1-based), or `nothing`.
"""
function time_series_data_at(pkg::TsdPackage, index::Int)
    ptr = ffi_tsd_package_time_series_data_at(pkg._ptr, Csize_t(index - 1))
    ptr == C_NULL ? nothing : TimeSeriesDataRef(ptr, pkg)
end

"""
    TimeSeriesDataPackage

Top-level time series data package (ISO 19848). Freed when garbage-collected.

Obtain via [`TimeSeriesDataPackage(pkg)`](@ref).
"""
mutable struct TimeSeriesDataPackage
    _ptr::Ptr{Cvoid}

    function TimeSeriesDataPackage(ptr::Ptr{Cvoid})
        ptr == C_NULL &&
            throw(VistaError(InvalidArgument, "TimeSeriesDataPackage: NULL pointer"))
        t = new(ptr)
        finalizer(t) do x
            ffi_tsd_data_package_free(x._ptr)
            x._ptr = C_NULL
        end
        t
    end
end

"""
    TimeSeriesDataPackage(pkg::TsdPackage) -> TimeSeriesDataPackage

Construct a top-level package from a [`TsdPackage`](@ref). Copies `pkg`.
"""
function TimeSeriesDataPackage(pkg::TsdPackage)
    ptr = ffi_tsd_data_package_create(pkg._ptr)
    ptr == C_NULL &&
        throw(VistaError(InvalidArgument, "TimeSeriesDataPackage: NULL pointer"))
    TimeSeriesDataPackage(ptr)
end

"""
    time_series_data_count(p::TimeSeriesDataPackage) -> Int

Return the number of [`TimeSeriesData`](@ref) entries in the inner package.
"""
function time_series_data_count(p::TimeSeriesDataPackage)
    pkg_ptr = ffi_tsd_data_package_package(p._ptr)
    pkg_ptr == C_NULL ? 0 : Int(ffi_tsd_package_time_series_data_count(pkg_ptr))
end

"""
    is_empty(p::TimeSeriesDataPackage) -> Bool

Return `true` if the package contains no [`TimeSeriesData`](@ref) entries.
"""
is_empty(p::TimeSeriesDataPackage) = time_series_data_count(p) == 0
