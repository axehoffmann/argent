add_library(ogl "GLRenderEngine.h" "GLScreen.h" "GLScreen.cpp" "GLRenderEngine.cpp" "GL.h" "GLBuffer.h" "GLBuffer.cpp" "GLShader.h")
target_link_libraries(ogl PRIVATE rendering resources ${glfw} ${glew} ${ogl32lib})
