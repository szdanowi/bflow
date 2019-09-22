target_include_directories(unit_tests PRIVATE .)
target_sources(unit_tests PRIVATE
        flow.hpp
        detail/flow.cpp
        detail/flow_steps.hpp
        detail/flow_templates_impl.hpp
        test/flow_tests.cpp
        test/flow_steps_tests.cpp
        examples/many_possiblities.cpp
)
