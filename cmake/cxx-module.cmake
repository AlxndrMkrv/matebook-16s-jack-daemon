function(add_module TARGET)
    set(SOURCES ${ARGN})
    if (NOT SOURCES)
        message(FATAL_ERROR "No sources provided for module `${TARGET}`")
    endif()

    set(PRIMARY_INTERFACE_FOUND false)
    set(IFACE_SRC "")
    set(IMPL_SRC "")
    foreach(SRC ${SOURCES})
        if(${SRC} MATCHES "m$")
            list(APPEND IFACE_SRC ${SRC})
            if(${SRC} MATCHES "^${TARGET}.")
                set(PRIMARY_INTERFACE_FOUND true)
            endif()
        else()
            list(APPEND IMPL_SRC ${SRC})
        endif()
    endforeach()

    if(NOT ${PRIMARY_INTERFACE_FOUND})
        message(FATAL_ERROR
            "Primary module interface unit (`${TARGET}.___m`) not found")
    endif()

    set(PCM_DIR ${PROJECT_BINARY_DIR}/CMakeFiles/${TARGET}.dir)

    add_library(${TARGET})
    target_sources(${TARGET}
        PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES
        FILES ${IFACE_SRC})
    if(${IMPL_SRC})
        target_sources(${TARGET} PRIVATE FILES ${IMPL_SRC})
    endif()
    target_compile_options(${TARGET} PRIVATE
        -fmodules
        -fno-implicit-module-maps)
    target_compile_options(${TARGET} PUBLIC -fprebuilt-module-path=${PCM_DIR})
endfunction()
