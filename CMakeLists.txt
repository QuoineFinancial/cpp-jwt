# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.

cmake_minimum_required (VERSION 3.13)

include (cmake/HunterGate.cmake)
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.154.tar.gz"
        SHA1 "d720f3775d4b896dab91f8249de04956e31804dd"
        LOCAL
)

project (CppJwt VERSION 1.2.0 LANGUAGES CXX)

include(GNUInstallDirs)

option(BUILD_TESTS "build the tests" OFF)
option(BUILD_EXAMPLES "build the examples" OFF)


hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

add_library(jwt INTERFACE)
file(GLOB_RECURSE interface_hpp RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "include/*.hpp")
file(GLOB_RECURSE interface_ipp RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS "include/*.ipp")
target_sources(jwt INTERFACE $<BUILD_INTERFACE:${interface_hpp} ${interface_ipp}>)

target_include_directories(
        jwt
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_link_libraries(jwt INTERFACE OpenSSL::SSL OpenSSL::Crypto nlohmann_json::nlohmann_json)

add_library(CppJwt::jwt ALIAS jwt)

#
# INSTALL
#

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")
set(pkgconfig_install_dir "lib/pkgconfig")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(pkg_config "${generated_dir}/${PROJECT_NAME}.pc")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_file("cmake/CppJwt.pc.in" "${pkg_config}" @ONLY)

configure_package_config_file(
        "cmake/Config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

INSTALL(TARGETS jwt
        EXPORT "${targets_export_name}"
        INCLUDES DESTINATION "${include_install_dir}")

INSTALL(DIRECTORY include/jwt DESTINATION include)

install(
        FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}"
)

install(
        FILES "${pkg_config}"
        DESTINATION "${pkgconfig_install_dir}"
)

install(
        EXPORT "${targets_export_name}"
        NAMESPACE "${namespace}"
        DESTINATION "${config_install_dir}"
)

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

