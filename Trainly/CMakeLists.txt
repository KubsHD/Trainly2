file(GLOB_RECURSE game_SRC RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "*.h"
    "*.cpp"
    "*.rc"
    "*.ico"
)

add_custom_command(OUTPUT Game.process_assets
        ALL
        DEPENDS ${game_SRC}
        COMMAND DataPacker buildContent ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:Game>/data
        COMMENT "Packing assets....")


add_executable(
    Game
    ${game_SRC}
    Game.process_assets
)

target_precompile_headers(Game PRIVATE src/stdafx.h)
target_link_libraries(Game DirectXTex DirectXTK SDL2-static SDL2main lua::lib lua::header sol2)


if (WIN32)
    set_property(TARGET Game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()




if(WIN32)
   #set_target_properties(Game PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   #set_target_properties(Game PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   #set_target_properties(Game PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(Game PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(Game PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Game PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
endif(WIN32)

#add_custom_command(OUTPUT Game.intermediate_step
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:Game>/data
#    COMMENT "Copying to output directory")


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
set_property(TARGET Game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Game>)

foreach(source IN LISTS game_SRC)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()