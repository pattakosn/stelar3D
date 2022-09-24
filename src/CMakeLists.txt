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
)
target_link_libraries(stelar3D PUBLIC imgui::imgui glm::glm SDL2::SDL2main glad::glad)
target_compile_options(stelar3D PUBLIC
                   #$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)
if(OpenMP_CXX_FOUND)
    target_link_libraries(stelar3D PUBLIC OpenMP::OpenMP_CXX)
endif()
if(IPOsupported)
    set_property(TARGET stelar3D PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(get_all_GLextensions get_all_GLextensions.cpp)
target_link_libraries(get_all_GLextensions SDL2::SDL2 SDL2::SDL2main glad)
set_target_properties(get_all_GLextensions PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
if(IPOsupported)
    set_property(TARGET get_all_GLextensions PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_subdirectory(learnOpenGL)
