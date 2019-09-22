#pragma once

#include "flow_steps.hpp"
#include <utility>

namespace bflow::detail
{
template <typename event_t, typename iteration_t>
result _flow<event_t, iteration_t>::process(event_t event) {
  if (not _current) return result::rejected;
  return iteration_t::advance(_current, _current.process(event), _steps);
}

struct one_shot_iteration
{
  template <typename iterator_t, typename step_collection_t>
  static inline result advance(iterator_t& current, result result, const step_collection_t&) {
    if (result != result::completed) return result;
    return ++current ? result::accepted : result::completed;
  }
};

struct looped_iteration
{
  template <typename iterator_t, typename step_collection_t>
  static inline result advance(iterator_t& current, result result, const step_collection_t& steps) {
    if (result != result::completed) return result;
    if (++current) return result::accepted;

    current = steps.begin();
    return result::completed;
  }
};

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

template <typename event_t, typename iteration_t>
template <typename... steps_t>
_flow<event_t, iteration_t> _flow<event_t, iteration_t>::of(steps_t&&... steps) {
  return _flow<event_t, iteration_t>(detail::make_steps<event_t>(std::forward<steps_t>(steps)...));
}

template <typename event_t, typename iteration_t>
_flow<event_t, iteration_t>::_flow(detail::steps<event_t>&& steps) : _steps(std::move(steps)) {}

template <typename event_t, typename selection_t>
template <typename... steps_t>
steps_selection<event_t, selection_t> steps_selection<event_t, selection_t>::of(steps_t&&... steps) {
  return steps_selection<event_t, selection_t>(detail::make_steps<event_t>(std::forward<steps_t>(steps)...));
}

template <typename event_t, typename selection_t>
steps_selection<event_t, selection_t>::steps_selection(detail::steps<event_t>&& steps) : _steps(std::move(steps)) {}

} // namespace bflow::detail
