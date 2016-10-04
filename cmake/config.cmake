include(CheckTypeSize)

option(CFG_DEBUG "DEBUG OPTION" TRUE)

check_type_size("void *" VOID_P)
if(VOID_P EQUAL 4)
	option(ARCHFLAGS "CFLAG" m32)
endif()

# Enable color output
set(CMAKE_COLOR_MAKEFILE ON)
