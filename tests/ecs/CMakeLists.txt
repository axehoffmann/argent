target_sources(tests PRIVATE 
							 "new_archetype_UTest.cpp")
target_link_libraries(tests ecs core)