# --------------------------------------------------------------------------- #
# basic settings

cmake_minimum_required(VERSION 3.22.1)

project(text_editor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CSS_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms_ui/)
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# enable settings for Qt

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# set build directive depending on os

if(WIN32)
    message("--- OS Windows!")

    set(CMAKE_PREFIX_PATH "C:/user_programs/qt/6.5.1/mingw_64")

    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/config/text_editor.rc")
elseif(UNIX)
    message("--- OS Unix!")

    set(CMAKE_PREFIX_PATH "/user_files_volodya/qt/6.4.0/gcc_64/")
endif()
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# find framework Qt

find_package(Qt6 COMPONENTS
                 Core
                 Gui
                 Widgets
                 REQUIRED)
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# settings directories

function(GetSubdirectories result current_directory)
    list(APPEND result ${current_directory})

    file(GLOB_RECURSE subdirectories LIST_DIRECTORIES true "${current_directory}/*")

    foreach(directory ${subdirectories})
        if(IS_DIRECTORY ${directory})
            list(APPEND result ${directory})
        endif()
    endforeach()

    set(${result} PARENT_SCOPE)
endfunction()

GetSubdirectories(HEADER_DIRECTORIES   "headers" )
GetSubdirectories(SOURCE_DIRECTORIES   "sources" )

include_directories(${HEADER_DIRECTORIES}
                    ${SOURCE_DIRECTORIES})
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# variable for 'settings executable(program)'
# source files
file(GLOB_RECURSE SOURCES "sources/*.cpp")

# header files
file(GLOB_RECURSE HEADERS "headers/*.hpp")

# ui files
file(GLOB_RECURSE FORMS "forms_ui/*.ui")

# other files
set(OTHER_WIN32 resources.qrc
                ${APP_ICON_RESOURCE_WINDOWS})

set(OTHER_UNIX resources.qrc)

# ready list of main files
set(MAIN ${SOURCES}
         ${HEADERS}
         ${FORMS})
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# executable(program) settings
if(WIN32)
    add_executable(text_editor
                   ${MAIN}
                   ${OTHER_WIN32})
elseif(UNIX)
    add_executable(text_editor
                   ${MAIN}
                   ${OTHER_UNIX})
endif()

target_link_libraries(text_editor
                      PRIVATE Qt::Core
                      PRIVATE Qt::Gui
                      PRIVATE Qt::Widgets)
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# special settings
if(WIN32)
    set_target_properties(text_editor PROPERTIES WIN32_EXECUTABLE ON)
endif()
# --------------------------------------------------------------------------- #
