add_subdirectory(Alias)

nm_add_library(AUA STATIC $<TARGET_OBJECTS:AUA.Alias>)
