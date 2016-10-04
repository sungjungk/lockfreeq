set(TARGET lockfreeq)

set(SRCS
	lockfreeq.c
   )

add_library(${TARGET} SHARED ${SRCS})
