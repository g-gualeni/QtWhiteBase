cmake_minimum_required(VERSION 3.5)

project(WhiteBase VERSION 0.0.0.0 LANGUAGES CXX)
set(LEGAL_COPYRIGHT "(C) 2023 G.Gualeni@Gmail.com")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WIN_APPLICATION_ICON "icons/WhiteBase.ico")
set(RESOURCE_FILE  ${PROJECT_NAME}.rc)

configure_file(${RESOURCE_FILE}.in ${RESOURCE_FILE})
configure_file(Version.h.in Version.h)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui

        ${PROJECT_NAME}.qrc

        ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/Version.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WhiteBase
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WhiteBase APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WhiteBase SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WhiteBase
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(WhiteBase PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(WhiteBase PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WhiteBase)
endif()
set_target_properties(WhiteBase PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WhiteBase
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WhiteBase)
endif()

################################################################
# Deploy folder creation
if(CMAKE_BUILD_TYPE MATCHES "Release")

    get_target_property(QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)

    string(REGEX MATCH "Qt/.+bin" BUILD_PACK_01 ${QMAKE_EXECUTABLE})
    string(REPLACE "_64/bin" "_64bit" BUILD_PACK_02 ${BUILD_PACK_01})
    string(REPLACE "/bin" "_32bit" BUILD_PACK_03 ${BUILD_PACK_02})
    string(REPLACE "/" "-" BUILD_PACK ${BUILD_PACK_03})

    # set(BASE_DEPLOY_FOLDER "${PROJECT_SOURCE_DIR}/../") - try using top level path
    set(BASE_DEPLOY_FOLDER "${CMAKE_SOURCE_DIR}/../")

    if(APPLE)
        string(APPEND DEPLOY_FOLDER ${BASE_DEPLOY_FOLDER}
            ${PROJECT_NAME} "-macOS-" ${BUILD_PACK} "/")
        string(APPEND DEPLOY_FOLDER_BUNDLE ${DEPLOY_FOLDER} "/"
            ${PROJECT_NAME} "-" ${PROJECT_VERSION} ".app")

        add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD COMMAND ${CMAKE_COMMAND} -E
            copy_directory $<TARGET_BUNDLE_DIR:${PROJECT_NAME}>
            ${DEPLOY_FOLDER_BUNDLE}
            )

    elseif(UNIX)
        string(APPEND DEPLOY_FOLDER ${BASE_DEPLOY_FOLDER}
            ${PROJECT_NAME} "-Linux-" ${BUILD_PACK} "/")
        string(APPEND DEPLOY_FOLDER_BUNDLE ${DEPLOY_FOLDER} "/"
            ${PROJECT_NAME} "-" ${PROJECT_VERSION})

        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E
            copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            ${DEPLOY_FOLDER_BUNDLE}
            )

    elseif(WIN32)
        string(APPEND DEPLOY_FOLDER ${BASE_DEPLOY_FOLDER} ${PROJECT_NAME}
            "-Win-" ${BUILD_PACK} "/")
        string(APPEND DEPLOY_FOLDER_BUNDLE ${DEPLOY_FOLDER} "/"
            ${PROJECT_NAME} "-" ${PROJECT_VERSION} "-Win-64bit")

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E
            copy $<TARGET_FILE:${PROJECT_NAME}>
            "${DEPLOY_FOLDER_BUNDLE}"
            )

    endif()

    make_directory(${DEPLOY_FOLDER_BUNDLE})

    get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)
    find_program(DEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
    find_program(DEPLOYQT_EXECUTABLE macdeployqt HINTS "${QT_BIN_DIR}")
    if(DEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            env "${DEPLOYQT_EXECUTABLE}" "${DEPLOY_FOLDER_BUNDLE}"
            )
    endif()

endif()


