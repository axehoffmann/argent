add_library(core "BoundingBox.cpp" "Transform.cpp" "BoundingBox.h" "Camera.h" "Transform.h" "Utility.h" "Engine.h" "Engine.cpp" "Grid.h" "Stats.h" "Stats.cpp")
add_subdirectory(cvars)
target_link_libraries(core cvars)