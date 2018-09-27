set(PLUGIN_NAME Sample)

set(WPEFRAMEWORK_PLUGIN_SAMPLE_AUTOSTART true CACHE STRING true)
set(WPEFRAMEWORK_PLUGIN_SAMPLE_OOP false CACHE STRING true)

include(module) # Setup default stuff needed for the cmake framework thingy.

set(PLUGIN_OPTIONS)

set(PLUGIN_SOURCES
        Module.cpp
        Sample.cpp
        SampleImplementation.cpp)

include(default_targets) # This is a regular plugin no need to edit the defaults

write_config(${PLUGIN_NAME})
