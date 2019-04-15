# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.

cmake_minimum_required (VERSION 3.13)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE must be set and refer to a valid toolchain file")
endif()

include (cmake/HunterGate.cmake)
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.154.tar.gz"
        SHA1 "d720f3775d4b896dab91f8249de04956e31804dd"
        LOCAL
)

project (CppJwt)

option(BUILD_TESTS "build the tests")
option(BUILD_EXAMPLES "build the examples")


hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

add_library(jwt INTERFACE)
file(GLOB_RECURSE interface_hpp RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "include/*.hpp")
file(GLOB_RECURSE interface_ipp RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "include/*.ipp")
target_sources(jwt INTERFACE ${interface_hpp} ${interface_ipp})

target_include_directories(jwt INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(jwt INTERFACE $<INSTALL_INTERFACE:include>)
target_link_libraries(jwt INTERFACE OpenSSL::SSL OpenSSL::Crypto nlohmann_json::nlohmann_json)

add_library(CppJwt::jwt ALIAS jwt)

INSTALL(DIRECTORY include/jwt DESTINATION include)

if (BUILD_TESTS)
    hunter_add_package(GTest)
    find_package(GTest CONFIG REQUIRED)

    enable_testing()

    # Recurse into the "Hello" and "Demo" subdirectories. This does not actually
    # cause another cmake executable to run. The same process will walk through
    # the project's entire directory structure.
    add_subdirectory (tests)

    if (BUILD_EXAMPLES)
        add_subdirectory (examples)
    endif()

endif()

