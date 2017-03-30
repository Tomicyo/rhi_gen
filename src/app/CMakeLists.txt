set(TARGET app)
add_library(${TARGET} SHARED app.h app.cpp)
target_compile_definitions(${TARGET} PRIVATE -DBUILD_LIB=1)