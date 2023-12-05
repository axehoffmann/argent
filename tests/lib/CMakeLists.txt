target_sources(tests PRIVATE 
							 "ptr_UTest.cpp" 
							 #"block_allocator_UTest.cpp" 
							 "mt_list_UTest.cpp"
							 "dyn_array_UTest.cpp"
)
target_link_libraries(tests lib)