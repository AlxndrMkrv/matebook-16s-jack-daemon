
configure_file(
    ${CMAKE_SOURCE_DIR}/package/PKGBUILD.in
    ${CMAKE_BINARY_DIR}/PKGBUILD
    @ONLY
)

add_custom_target(archlinux_package
    COMMAND makepkg -f
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building Arch Linux package"
    DEPENDS ${PROJECT_NAME}
)


