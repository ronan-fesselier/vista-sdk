# ==============================================================================
# VISTA-SDK-CPP - C++ Library Compilers Settings
# ==============================================================================

include(CheckCXXSourceCompiles)

if(MSVC)
	add_compile_options(/std:c++20)
	add_compile_options(/MP)
	add_compile_options(/W4)
	add_compile_options(/Wall)
	add_compile_options(/WX-)
	add_compile_options(/permissive-)
	add_compile_options(/fp:precise)
	add_compile_options(/Zc:__cplusplus)
	add_compile_options(/Zc:inline)
	add_compile_options(/Zc:preprocessor)
	add_compile_options(/external:anglebrackets)
	add_compile_options(/external:W0)
	#add_compile_options(/analyze:WX-)
	#add_compile_options(/analyze:external-)
	#add_compile_options(/analyze)

	#add_compile_options(/wd4061) # switch not handled
	#add_compile_options(/wd4100) # unreferenced formal parameter
	#add_compile_options(/wd4189) # local variable is initialized but not referenced
	#add_compile_options(/wd4244) # conversion from 'type1' to 'type2', possible loss of data
	#add_compile_options(/wd4267) # conversion from 'size_t' to 'type', possible loss of data
	#add_compile_options(/wd4365) # conversion signed/unsigned mismatch
	#add_compile_options(/wd4388) # signed/unsigned mismatch
	#add_compile_options(/wd4456) # declaration hides previous local declaration
	#add_compile_options(/wd4514) # unreferenced inline function has been removed
	#add_compile_options(/wd4625) # copy constructor was implicitly defined as deleted
	#add_compile_options(/wd4626) # assignement operator was implicitly defined as deleted
	add_compile_options(/wd4710) # function not inlined
	add_compile_options(/wd4711) # function 'function' selected for inline expansion
	add_compile_options(/wd4820) # padding
	add_compile_options(/wd4866) # compiler may not enforce left-to-right evaluation order for call to operator_name
	#add_compile_options(/wd4868) # compiler may not enforce left-to-right evaluation order in braced initializer list
	#add_compile_options(/wd5026) # move constructor was implicitly defined as deleted
	#add_compile_options(/wd5027) # move assignement operator was implicitly defined as deleted
	#add_compile_options(/wd5038) # will be initialized after
	add_compile_options(/wd5045) # Qspectre
	#add_compile_options(/wd5267) # definition of implicit copy constructor/assignment operator is deprecated because it has a user-provided assignment operator/copy constructor
	#add_compile_options(/wd6246) # local declaration of 'variable' hides declaration of same name in outer scope
	#add_compile_options(/wd6326) # potential comparison of a constant with another constant
	#add_compile_options(/wd6387) # this does not adhere to the specification for the function

	add_compile_options(/arch:AVX)
	add_compile_options(/arch:AVX2)
else() # GCC / Clang
	add_compile_options(-Wall)
	add_compile_options(-Wextra)
	add_compile_options(-Wpedantic)
	add_compile_options(-msse4.2)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-fdiagnostics-color=always)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		add_compile_options(-std=c++20)
		add_compile_options(-fno-pie)
		add_compile_options(-fPIC)
		add_compile_options(-Wno-pre-c++17-compat)
		add_compile_options(-Wno-c++98-compat)
		add_compile_options(-fcolor-diagnostics)
	endif()
endif()
