set(INCLUDE_FILES
        include/bst.h
        include/node.h
)

include_directories(include)

add_library(bst INTERFACE ${INCLUDE_FILES})