#pragma once

#include "flow_steps.hpp"
#include <utility>

namespace bflow
{
template <typename event_t>
result linear<event_t>::process(event_t event) {
    if (not _current) return result::rejected;

    auto result = _current.process(event);

    if (result == result::completed) {
        _current++;
        return _current ? result::accepted : result::completed;
    }
    return result;
}

namespace detail
{
template <typename event_t>
auto make_steps() {
    return detail::steps<event_t>();
}

template <typename event_t, typename step_t, typename... steps_t>
auto make_steps(step_t&& step, steps_t&&... steps) {
    auto flow = make_steps<event_t>(std::forward<steps_t>(steps)...);
    flow.push_front(std::forward<step_t>(step));
    return flow;
}
} // namespace detail

template <typename event_t>
template <typename... steps_t>
linear<event_t> linear<event_t>::of(steps_t&&... steps) {
    return linear(detail::make_steps<event_t>(std::forward<steps_t>(steps)...));
}

template <typename event_t>
linear<event_t>::linear(detail::steps<event_t>&& steps) : _steps(std::move(steps)) {}

} // namespace bflow
