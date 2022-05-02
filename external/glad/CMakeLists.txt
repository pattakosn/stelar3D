add_library(glad SHARED "src/glad.c")
target_include_directories(glad PUBLIC "include")

# following options found in glad2's CMakeLists.txt in its github repository
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
# if(GG_SHARED)
#if($<$<STREQUAL:$<TARGET_PROPERTY:glad2,TYPE>,SHARED_LIBRARY>:;$<TARGET_SONAME_FILE:glad2>>)
#    target_compile_definitions(glad2 PUBLIC GLAD_API_CALL_EXPORT)
#    set_target_properties(glad2 PROPERTIES DEFINE_SYMBOL "GLAD_API_CALL_EXPORT_BUILD")
#endif()
