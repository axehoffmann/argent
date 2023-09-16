add_library(ecs "ArchetypeCollection.cpp" "ArchetypeCollection.h" 
				"Component.cpp" "Component.h"
				"ECS.h" "ECSTypes.h" 
				"Query.cpp" "Query.h"
				"World.cpp" "World.h" 
				"System.cpp" "System.h" 
				"EntityRef.h" 
				"Entity.h")

add_subdirectory(events)