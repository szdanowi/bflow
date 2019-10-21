#pragma once

#include "steps.hpp"
#include "result.hpp"
#include <functional>
#include <utility>

namespace bflow::detail
{
template <typename event_t, typename iteration_t>
class flow
{
public:
  using listener = std::function<void()>;

  flow() = default;

  template <typename... steps_t>
  static flow<event_t, iteration_t> of(steps_t&&... steps);

  result process(event_t event);

  inline void on_completion(listener completion_listener) { _completion_listener = completion_listener; }

private:
  using current_step = typename steps<event_t>::iterator;

  explicit flow(steps<event_t>&&);

  steps<event_t> _steps;
  current_step _current = _steps.begin();
  listener _completion_listener = [] {};
};

template <typename event_t, typename iteration_t>
result flow<event_t, iteration_t>::process(event_t event) {
  if (not _current) return result::rejected;
  auto result = iteration_t::advance(_current, _current.process(event), _steps);
  if (result == result::completed) _completion_listener();
  return result;
}

template <typename event_t, typename iteration_t>
template <typename... steps_t>
flow<event_t, iteration_t> flow<event_t, iteration_t>::of(steps_t&&... steps) {
  return flow<event_t, iteration_t>(make_steps<event_t>(std::forward<steps_t>(steps)...));
}

template <typename event_t, typename iteration_t>
flow<event_t, iteration_t>::flow(steps<event_t>&& steps) : _steps(std::move(steps)) {}

} // namespace bflow::detail
