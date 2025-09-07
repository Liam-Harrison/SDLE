cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set output directories for dynamic libraries.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Prevent installing to system directories.
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

# Declare project.
project("SDLE")

if ((APPLE AND NOT CMAKE_SYSTEM_NAME MATCHES "Darwin") OR EMSCRIPTEN)
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")    # Disable shared builds on platforms where it does not make sense to use them
    set(SDL_SHARED OFF)
else()
    set(SDL_SHARED ON)
endif()

if(MSVC)
    if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
        add_definitions(/MP) # parallelize each target, unless Ninja is the generator.
    endif()
endif()

# Set name of the executable.
set(EXECUTABLE_NAME ${PROJECT_NAME})

# Set vendored directory.
set(VENDORED_DIR "${CMAKE_CURRENT_LIST_DIR}/vendored")

# Set resources directory.
set(RESOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/resources")

# Set unified source directory
set(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/source")

# Create the executable target.
add_executable(${EXECUTABLE_NAME})

# Collect all source files from the unified source directory
file(GLOB_RECURSE PROJECT_SOURCES
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.h"
    "${SOURCE_DIR}/*.hpp"
)

# Add all source files to the target
target_sources(${EXECUTABLE_NAME} PRIVATE ${PROJECT_SOURCES})

# Add all include directories
target_include_directories(${EXECUTABLE_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

# Add logo for Apple executable.
if(APPLE)
    target_sources("${EXECUTABLE_NAME}" PRIVATE "${RESOURCE_DIR}/textures/Logo.png")
endif()

# Add icon for Windows executable
if(WIN32)
    target_sources("${EXECUTABLE_NAME}" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/config/windows/app.rc")
endif()

# Set C++ version
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)

# SDL
add_subdirectory(${VENDORED_DIR}/SDL EXCLUDE_FROM_ALL)

# SDL_ttf
set(SDLTTF_VENDORED ON)         # tell SDL_ttf to build its own dependencies
add_subdirectory(${VENDORED_DIR}/SDL_ttf EXCLUDE_FROM_ALL)	

# SDL_mixer
set(SDLMIXER_MIDI_NATIVE OFF)   # disable formats we don't use to make the build faster and smaller.
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)     
set(SDLMIXER_MOD OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_MP3_MPG123 OFF)
set(SDLMIXER_VORBIS_VORBISFILE OFF)
set(SDLMIXER_FLAC_LIBFLAC OFF)
set(SDLMIXER_VENDORED ON)       # tell SDL_mixer to build its own dependencies
add_subdirectory(${VENDORED_DIR}/SDL_mixer EXCLUDE_FROM_ALL)

# SDL_image
set(SDLIMAGE_VENDORED ON)
set(SDLIMAGE_AVIF OFF)	        # disable formats we don't use to make the build faster and smaller.
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
add_subdirectory(${VENDORED_DIR}/SDL_image EXCLUDE_FROM_ALL)

# Link SDL. This also makes its include directory available to us. 
target_link_libraries(${EXECUTABLE_NAME} PUBLIC 
	SDL3_ttf::SDL3_ttf
	SDL3_mixer::SDL3_mixer
    SDL3_image::SDL3_image
    SDL3::SDL3
)

# SDL_Image bug: https://github.com/libsdl-org/SDL_image/issues/506
if (APPLE AND NOT BUILD_SHARED_LIBS)
    find_library(IO_LIB ImageIO REQUIRED)
    find_library(CS_LIB CoreServices REQUIRED)
    find_library(CT_LIB CoreText REQUIRED)
    find_library(CG_LIB CoreGraphics REQUIRED)
    find_library(CF_LIB CoreFoundation REQUIRED)
    target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${CF_LIB} ${CT_LIB} ${IO_LIB} ${CS_LIB} ${CG_LIB})
endif()

# Assets
if (APPLE)
    # Recursively add all files in the resources directory to the bundle's Resources folder
    file(GLOB_RECURSE RESOURCE_FILES "${RESOURCE_DIR}/*")

    foreach(RESOURCE_FILE ${RESOURCE_FILES})
        # Compute the relative path inside the resources directory
        file(RELATIVE_PATH relpath "${RESOURCE_DIR}" "${RESOURCE_FILE}")
        get_filename_component(relpath_dir "${relpath}" DIRECTORY)
        target_sources(${EXECUTABLE_NAME} PRIVATE "${RESOURCE_FILE}")
        set_property(SOURCE "${RESOURCE_FILE}" PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${relpath_dir}")
    endforeach()
else()
    # Recursively copy the entire resources directory to the output directory after build
    # First remove existing resources to clear old files, then copy fresh ones
    add_custom_command(
        TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCE_DIR}" "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources"
    )
endif()

set(RESOURCE_FILES "${RESOURCE_DIR}/Logo.png")

set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    # On macOS, make a proper .app bundle instead of a bare executable
    MACOSX_BUNDLE TRUE

    # Set the Info.plist file for Apple Mobile platforms. Without this file, app will not launch. 
    MACOSX_BUNDLE_INFO_PLIST "${SOURCE_DIR}/SDLE/Info.plist.in"

    # in Xcode, create a Scheme in the schemes dropdown for the app.
    XCODE_GENERATE_SCHEME TRUE

    # Identification for Xcode
    XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "com.liamh.sdle"
	XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.liamh.sdle"
	XCODE_ATTRIBUTE_CURRENTYEAR "${CURRENTYEAR}"
    RESOURCE "${RESOURCE_FILES}"
)

# on Visual Studio, set our app as the default project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${EXECUTABLE_NAME}")

# On macOS Platforms, ensure that the bundle is valid for distribution by calling fixup_bundle.
# note that fixup_bundle does not work on iOS, so you will want to use static libraries 
# or manually copy dylibs and set rpaths
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    # tell Install about the target, otherwise fixup won't know about the transitive dependencies
    install(TARGETS ${EXECUTABLE_NAME}
    	BUNDLE DESTINATION ./install COMPONENT Runtime
   	    RUNTIME DESTINATION ./install/bin COMPONENT Runtime
    )
	
    set(DEP_DIR "${CMAKE_BINARY_DIR}")  # where to look for dependencies when fixing up
    INSTALL(CODE 
        "include(BundleUtilities)
        fixup_bundle(\"$<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>\" \"\" \"${DEP_DIR}\")
        " 
    )
    set(CPACK_GENERATOR "DragNDrop")
    include(CPack)
endif()
