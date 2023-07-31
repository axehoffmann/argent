set(LOG_LIBS log_test log_print)
set(LOG_DEFS AG_LOGGING_CONSOLE=0 AG_LOGGING_CONSOLE=1)

foreach(val IN ZIP_LISTS LOG_LIBS LOG_DEFS)
	add_library(${val_0} STATIC "ConsoleWriter.h" "Log.h" "Log.cpp")
	target_compile_definitions(${val_0} PRIVATE ${val_1})
endforeach()