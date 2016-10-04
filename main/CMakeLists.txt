set(SRCS
	main.c
   )

add_executable(main ${SRCS})
target_link_libraries(main pthread lockfreeq)
