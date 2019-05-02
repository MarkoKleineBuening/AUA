add_subdirectory(Test)

nm_add_library(AUA STATIC $<TARGET_OBJECTS:AUA.Test>)


