cmake_minimum_required(VERSION 3.3)

set(DEPNAME freetype)

set(SOURCES
../src/base/ftsystem.c
../src/base/ftinit.c
../src/base/ftdebug.c
../src/base/ftbase.c
../src/base/ftbbox.c
../src/base/ftglyph.c
../src/base/ftbdf.c
../src/base/ftbitmap.c
../src/base/ftcid.c
../src/base/ftfstype.c
../src/base/ftgasp.c
../src/base/ftgxval.c
../src/base/ftlcdfil.c
../src/base/ftmm.c
../src/base/ftotval.c
../src/base/ftpatent.c
../src/base/ftpfr.c
../src/base/ftstroke.c
../src/base/ftsynth.c
../src/base/fttype1.c
../src/base/ftwinfnt.c
../src/base/ftxf86.c
../src/bdf/bdf.c
../src/cff/cff.c
../src/cid/type1cid.c
../src/pcf/pcf.c
../src/pfr/pfr.c
../src/sfnt/sfnt.c
../src/truetype/truetype.c
../src/type1/type1.c
../src/type42/type42.c
../src/winfonts/winfnt.c
../src/raster/raster.c
../src/smooth/smooth.c
../src/autofit/autofit.c
../src/cache/ftcache.c
../src/gzip/ftgzip.c
../src/lzw/ftlzw.c
../src/gxvalid/gxvalid.c
../src/otvalid/otvalid.c
../src/psaux/psaux.c
../src/pshinter/pshinter.c
../src/psnames/psnames.c)

set(HEADERS )

include_directories(../include)
macro(cegui_add_library _NAME _SOURCES_VAR _HEADERS_VAR)
    add_library(${_NAME} SHARED ${${_SOURCES_VAR}} ${${_HEADERS_VAR}})

    set_target_properties(${_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../../release/lib/dynamic"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"

        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../../release/lib/dynamic"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../../../debug/lib/dynamic"

        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../../debug/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../../debug/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../../release/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/../../../debug/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../../../debug/bin")

    add_library(${_NAME}_static STATIC ${${_SOURCES_VAR}} ${${_HEADERS_VAR}})

    set_target_properties(${_NAME}_static PROPERTIES
        ARCHIVE_OUTPUT_NAME ${_NAME}
        LIBRARY_OUTPUT_NAME ${_NAME}
        RUNTIME_OUTPUT_NAME ${_NAME}

        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../../debug/lib/static"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../../debug/lib/static"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../../release/lib/static"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/../../../debug/lib/static"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../../../debug/lib/static"

        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../../debug/lib/static"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../../debug/lib/static"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../../release/lib/static"
        LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/../../../debug/lib/static"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../../../debug/lib/static"

        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../../debug/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../../debug/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../../release/bin"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/../../../debug/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../../../debug/bin")

endmacro()

cegui_add_library(${DEPNAME} SOURCES HEADERS)

set_target_properties(${DEPNAME} 	PROPERTIES
    COMPILE_DEFINITIONS FT2_BUILD_LIBRARY
	COMPILE_DEFINITIONS_DEBUG "FT_DEBUG_LEVEL_ERROR;FT_DEBUG_LEVEL_TRACE")


set_target_properties(${DEPNAME}_static PROPERTIES
    COMPILE_DEFINITIONS FT2_BUILD_LIBRARY
	COMPILE_DEFINITIONS_DEBUG "FT_DEBUG_LEVEL_ERROR;FT_DEBUG_LEVEL_TRACE")

#add_custom_command(TARGET ${DEPNAME} POST_BUILD 
#        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
#                                                   "${CMAKE_BINARY_DIR}/dependencies/include")

#add_custom_command(TARGET ${DEPNAME} POST_BUILD 
#                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../docs/LICENSE.TXT"
#                                                    "${CMAKE_BINARY_DIR}/dependencies/freetype-license.txt")
#add_custom_command(TARGET ${DEPNAME} POST_BUILD 
#                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../docs/FTL.TXT"
 #                                                   "${CMAKE_BINARY_DIR}/dependencies/FTL.TXT")
#add_custom_command(TARGET ${DEPNAME} POST_BUILD 
 #                  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../docs/GPL.TXT"
#                                                    "${CMAKE_BINARY_DIR}/dependencies/GPL.TXT")

