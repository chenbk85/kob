

FILE(GLOB KSL_SRC "ksl/*.cc")
FILE(GLOB STRINGS_SRC "strings/*.cc")
FILE(GLOB SYS_SRC "sys/*.cc")
FILE(GLOB EVENT_SRC "event/*.cc")
FILE(GLOB NET_SRC "net/*.cc")
FILE(GLOB DC_SRC "strings/double-conversion/*.cc")

SET(
    KOB_SRC
        ${KSL_SRC}
        ${STRINGS_SRC}
        ${DC_SRC}
        ${EVENT_SRC}
        ${NET_SRC}
    )

add_library(kob_static STATIC ${KOB_SRC})
target_link_libraries(kob_static pthread)