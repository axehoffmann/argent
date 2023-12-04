target_sources(tests PRIVATE 
							 "archetype_UTest.cpp"
)
target_link_libraries(tests ecs core)