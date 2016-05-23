link_libraries(glfw)


if (BUILD_SHARED_LIBS)
    add_definitions(-DGLFW_DLL)
    link_libraries("${OPENGL_gl_LIBRARY}" "${MATH_LIBRARY}" "${GLU_LIBRARY}")
else()
    link_libraries(${glfw_LIBRARIES})
endif()

include_directories("${GLFW_SOURCE_DIR}/include"
                    "${GLFW_SOURCE_DIR}/deps")

if ("${OPENGL_INCLUDE_DIR}")
    include_directories("${OPENGL_INCLUDE_DIR}")
endif()

set(GLAD "${GLFW_SOURCE_DIR}/deps/glad/glad.h"
         "${GLFW_SOURCE_DIR}/deps/glad.c")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")

add_executable(lab3 WIN32 MACOSX_BUNDLE lab3.c)
add_executable(pog WIN32 MACOSX_BUNDLE chernovik.c)
#target_link_libraries(particles "${CMAKE_THREAD_LIBS_INIT}" "${RT_LIBRARY}")

#set(WINDOWS_BINARIES my_proj)

#set_target_properties(${WINDOWS_BINARIES} PROPERTIES FOLDER "GLFW3/Examples")

