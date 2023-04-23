
file(GLOB_RECURSE packer_SRC RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "**.h"
    "**.cpp"
    "**.rc"
    "**.ico"
)

add_executable(DataPacker ${packer_SRC})

target_link_libraries(DataPacker assimp DirectXTex d3dcompiler)

add_custom_command(TARGET DataPacker POST_BUILD
        DEPENDS ${PROJECT_SOURCES}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:assimp>/$<TARGET_FILE_NAME:assimp>" "$<TARGET_FILE_DIR:DataPacker>"
    )

if(WIN32)
   set_target_properties(DataPacker PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(DataPacker PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(DataPacker PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(DataPacker PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   #set_target_properties(DataPacker PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  # set_target_properties(DataPacker PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
endif(WIN32)
