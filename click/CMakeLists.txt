file(GLOB_RECURSE CLICK_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} * *.*
)
add_custom_target(ClickSrcs ALL SOURCES ${CLICK_FILES})

if (UNCONFINED)
    set(APPARMOR_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/openstore/sageteamail.apparmor)
else()
    set(APPARMOR_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail.apparmor)
endif()

set(CLICK_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail.json)

set(OTHER_MANIFEST
    ${CMAKE_CURRENT_SOURCE_DIR}/.excludes
    ${APPARMOR_MANIFEST}
    ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail.sagetea_sageteamail.application
    ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail.sagetea_sageteamail.service
    ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail.url-dispatcher
    ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail-content.json
    ${CMAKE_CURRENT_SOURCE_DIR}/sageteamailhelper-aa.json
    ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail-helper.json
)

if (CLICK_MODE)
    configure_file(${CLICK_MANIFEST} ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/manifest.json
        ${OTHER_MANIFEST}
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
    install(
        PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail-launch
        DESTINATION ${BIN_DIR}
    )
    install(
        PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/sageteamail-helper
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
endif()
