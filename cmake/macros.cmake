# Note: multiple arguments could not be resolved in this function. 
function(add_compile_flags _flags)
    if(${ARGC} GREATER 1)
        message(STATUS "Excess arguments to add_compile_flags! Args ${ARGN}")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_flags}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_flags}" PARENT_SCOPE)
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${_flags}" PARENT_SCOPE)
endfunction()

# Note: this macro should be called only by their own directory
macro(add_clean_files)
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ARGV}")
endmacro()
