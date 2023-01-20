configure_file(assets_path.h.in assets_path.h)

add_library(stelar3D
    ogl_context.cpp ogl_context.h
    ogl_debug.cpp ogl_debug.h
    stb_image.c stb_image.h
    handle_events.cpp handle_events.h
    attributes_binding_object.cpp attributes_binding_object.h
    vertex_array.cpp vertex_array.h
    element_array.cpp element_array.h
    texture.cpp texture.h
    datapoints.h
    mesh_item.cpp mesh_item.h
    model.cpp model.h
    frame_buffer.cpp frame_buffer.h
    render_buffer.cpp render_buffer.h
    win_cam_pos_fps.cpp win_cam_pos_fps.h
    shader.cpp shader.h
    depth_map_artifacts.cpp depth_map_artifacts.h
    depth_map_fixed.cpp depth_map_fixed.h
    depth_cubemap.cpp depth_cubemap.h
    find_file_path.cpp find_file_path.h
    ${CMAKE_CURRENT_BINARY_DIR}/assets_path.h
)
target_link_libraries(stelar3D PUBLIC imgui::imgui glm::glm glad::glad assimp::assimp)
target_compile_options(stelar3D PUBLIC
                   #$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)
target_include_directories(stelar3D PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
if(OpenMP_CXX_FOUND)
    target_link_libraries(stelar3D PUBLIC OpenMP::OpenMP_CXX)
endif()
if(IPOsupported)
    set_property(TARGET stelar3D PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(get_all_GLextensions get_all_GLextensions.cpp)
target_link_libraries(get_all_GLextensions
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        glad
)
set_target_properties(get_all_GLextensions PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
if(IPOsupported)
    set_property(TARGET get_all_GLextensions PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_subdirectory(learnOpenGL)
