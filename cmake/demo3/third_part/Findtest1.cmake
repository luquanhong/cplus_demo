message(status "using Findlibtest1.cmak...")
FIND_PATH(LIBTEST_CXX_INCLUDE_DIR
    NAMES test1.h
    PATHS /usr/include/
)

FIND_LIBRARY(LIBTEST_CXX_LIBRARIES
    NAMES test1
    PATHS /usr/lib/
)
