target_include_directories(unit_tests PRIVATE .)

# find -s . -iname "*.cpp" | sed -e 's/^\.\//        /g'
target_sources(unit_tests PRIVATE
        detail/flow.cpp
        test/flow_steps_tests.cpp
        test/flow_tests.cpp
)
