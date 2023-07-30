target_sources(tests PRIVATE "World_ITest.cpp" 
							 "System_UTest.cpp" 
							 "Archetype_UTest.cpp" 
							 "Component_UTest.cpp" 
							 "EntityRef_ITest.cpp" 
							 "ECS_Mock.h")
target_link_libraries(tests ecs)