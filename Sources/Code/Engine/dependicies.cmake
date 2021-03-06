if(NOT MAIN_PROJECT_SOURCE_ROOT)
    message(FATAL_ERROR "Required cmake variable MAIN_PROJECT_SOURCE_ROOT not defined")
endif()

if(BUILD_PLATFORM STREQUAL "Windows")

    set(LIB_PREFIX "")
    set(LIB_SHARED_EXTENSION ".dll")
    set(LIB_STATIC_EXTENSION ".lib")

elseif(BUILD_PLATFORM STREQUAL "Linux" OR BUILD_PLATFORM STREQUAL "Android")

    set(LIB_PREFIX "lib")
    set(LIB_STATIC_EXTENSION ".a")
    set(LIB_SHARED_EXTENSION ".so")

endif()

set(LIBS_ROOT_DIR ${MAIN_PROJECT_SOURCE_ROOT}/../Libs)

set(STB_INCLUDE_DIR ${LIBS_ROOT_DIR}/STB CACHE PATH "STB Include dir")
set(RAPID_JSON_INCLUDE_DIR ${LIBS_ROOT_DIR}/RapidJSON/include CACHE PATH "RapidJSON Include dir")

function(CreateStaticLibDependecy LibName LibIncludeDir LibLinkBinary)
    set(${LibName}_INCLUDE_DIR
        ${LibIncludeDir}
        CACHE PATH "${LibName} Include Dir")

    if(NOT EXISTS ${${LibName}_INCLUDE_DIR})
        message(FATAL_ERROR "Cant find include dir of libary: '${LibName}' at location: ${${LibName}_INCLUDE_DIR}")
    endif()

    set(${LibName}_LIB_STATIC_BIN
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${LIB_PREFIX}${LibLinkBinary}${LIB_STATIC_EXTENSION}
        CACHE PATH "${LibName} Library Path")

    if(NOT EXISTS ${${LibName}_LIB_STATIC_BIN})
        message(FATAL_ERROR "Cant find link binary of libary: '${LibName}' at location: ${${LibName}_LIB_STATIC_BIN}")
    endif()

    message(STATUS "Create dependecy to static lib ${LibName} : ${${LibName}_LIB_STATIC_BIN}")
endfunction()

function(CreateSharedLibDependecy LibName LibIncludeDir LibLinkBinary)
    set(${LibName}_INCLUDE_DIR
        ${LibIncludeDir}
        CACHE PATH "${LibName} Include Dir")

    if(NOT EXISTS ${${LibName}_INCLUDE_DIR})
        message(FATAL_ERROR "Cant find include dir of libary: '${LibName}' at location: ${${LibName}_INCLUDE_DIR}")
    endif()

    set(${LibName}_LIB_SHARED_BIN
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${LIB_PREFIX}${LibLinkBinary}${LIB_SHARED_EXTENSION}
        CACHE PATH "${LibName} Library Path")

    if(NOT EXISTS ${${LibName}_LIB_SHARED_BIN})
        message(FATAL_ERROR "Cant find link binary of libary: '${LibName}' at location: ${${LibName}_LIB_SHARED_BIN}")
    endif()

    message(STATUS "Create dependecy to shared lib ${LibName} : ${${LibName}_LIB_SHARED_BIN}")
endfunction()

if(BUILD_PLATFORM STREQUAL "Linux")

    find_package(ZLIB REQUIRED)
    set(ZLIB_LIB_SHARED_BIN ${ZLIB_LIBRARIES} CACHE PATH "ZLib Library Path")

    find_package(Threads REQUIRED)
    set(THREADS_LIB_SHARED_BIN ${CMAKE_THREAD_LIBS_INIT} CACHE PATH "Thread Library Path")

    CreateSharedLibDependecy("GLAD"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GLAD/include
        glad
    )

    CreateSharedLibDependecy("FREETYPE2"
        ${LIBS_ROOT_DIR}/FreeType2/include
        freetype
    )

    CreateSharedLibDependecy("GLFW"
        ${LIBS_ROOT_DIR}/GLFW/include
        glfw
    )

    CreateSharedLibDependecy("OPENAL_SOFT"
        ${LIBS_ROOT_DIR}/OpenAL-Soft/include
        openal
    )

    CreateStaticLibDependecy("GTEST"
        ${LIBS_ROOT_DIR}/GTEST/googletest/include
        gtestd
    )

elseif(BUILD_PLATFORM STREQUAL "Windows")

    CreateStaticLibDependecy("GLAD"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GLAD/include
        glad
    )

    CreateStaticLibDependecy("FREETYPE2"
        ${LIBS_ROOT_DIR}/FreeType2/include
        freetype
    )

    CreateStaticLibDependecy("GLFW"
        ${LIBS_ROOT_DIR}/GLFW/include
        glfw3dll
    )

    CreateStaticLibDependecy("OPENAL_SOFT"
        ${LIBS_ROOT_DIR}/OpenAL-Soft/include
        OpenAL32
    )

    CreateStaticLibDependecy("ZLIB"
        ${LIBS_ROOT_DIR}/ZLIB
        zlib
    )

    CreateStaticLibDependecy("GTEST"
        ${LIBS_ROOT_DIR}/GTEST/googletest/include
        gtestd
    )

elseif(BUILD_PLATFORM STREQUAL "Android")

    CreateStaticLibDependecy("OBOE"
        ${LIBS_ROOT_DIR}/Oboe/include
        oboe
    )

    CreateStaticLibDependecy("FREETYPE2"
        ${LIBS_ROOT_DIR}/FreeType2/include
        freetype
    )

endif()