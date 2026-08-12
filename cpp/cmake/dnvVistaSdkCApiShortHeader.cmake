#==============================================================================
# dnv-vista-sdk-c - Short-alias header generation
#==============================================================================

#----------------------------------------------
# Paths
#----------------------------------------------

set(_c_api_headers_dir  "${CMAKE_CURRENT_SOURCE_DIR}/include/dnv/vista/sdk/c")
set(_c_api_short_header "${CMAKE_CURRENT_BINARY_DIR}/include/dnv/VistaSDK_c_api_short.h")
set(_c_api_short_header_in
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VistaSDK_c_api_short.h.in"
)

#----------------------------------------------
# Symbol collection
#----------------------------------------------

file(GLOB_RECURSE _c_api_header_files "${_c_api_headers_dir}/*.h")

set(_sym_names "")
foreach(_header IN LISTS _c_api_header_files)
    file(STRINGS "${_header}" _lines)
    set(_prev_was_api FALSE)
    foreach(_line IN LISTS _lines)
        if(_line MATCHES "DNV_VISTA_SDK_C_API.*(dnv_vista_sdk_[a-z][a-z0-9_]*)[ \t]*(\\(|$)")
            set(_m1 "${CMAKE_MATCH_1}")
            if(_m1 MATCHES "_t$")
                set(_prev_was_api TRUE)
            else()
                list(APPEND _sym_names "${_m1}")
                set(_prev_was_api FALSE)
            endif()
        elseif(_line MATCHES "DNV_VISTA_SDK_C_API" AND NOT _line MATCHES "\\(")
            set(_prev_was_api TRUE)
        elseif(_prev_was_api AND _line MATCHES "^[ \t]*(dnv_vista_sdk_[a-z][a-z0-9_]*)[ \t]*(\\(|;)")
            list(APPEND _sym_names "${CMAKE_MATCH_1}")
            set(_prev_was_api FALSE)
        else()
            set(_prev_was_api FALSE)
        endif()

        if(_line MATCHES "typedef struct dnv_vista_sdk_[a-z0-9_]+ (dnv_vista_sdk_[a-z0-9_]+_t)")
            set(_m1 "${CMAKE_MATCH_1}")
            list(APPEND _sym_names "${_m1}")
        endif()
        if(_line MATCHES "\\} (dnv_vista_sdk_[a-z0-9_]+_t);")
            set(_m1 "${CMAKE_MATCH_1}")
            list(APPEND _sym_names "${_m1}")
        endif()
        if(_line MATCHES "typedef [^(]+\\(\\*(dnv_vista_sdk_[a-z0-9_]+_t)\\)")
            set(_m1 "${CMAKE_MATCH_1}")
            list(APPEND _sym_names "${_m1}")
        endif()

        if(_line MATCHES "^[ \t]*(DNV_VISTA_SDK_[A-Z0-9_]+)[ \t]*(=|,|$)")
            set(_m1 "${CMAKE_MATCH_1}")
            list(APPEND _enum_names "${_m1}")
        endif()
    endforeach()
endforeach()

list(REMOVE_DUPLICATES _sym_names)
list(REMOVE_ITEM _sym_names "")
list(SORT _sym_names)

list(REMOVE_DUPLICATES _enum_names)
list(REMOVE_ITEM _enum_names "")
list(SORT _enum_names)

#----------------------------------------------
# Header generation
#----------------------------------------------

set(_defines "")
foreach(_full IN LISTS _sym_names)
    string(REGEX REPLACE "^dnv_vista_sdk_" "" _short "${_full}")
    string(APPEND _defines "#define ${_short} ${_full}\n")
endforeach()

foreach(_full IN LISTS _enum_names)
    string(REGEX REPLACE "^DNV_VISTA_SDK_" "" _short "${_full}")
    string(APPEND _defines "#define ${_short} ${_full}\n")
endforeach()

configure_file("${_c_api_short_header_in}" "${_c_api_short_header}" @ONLY)
