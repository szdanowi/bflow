#pragma once

#include "flow_linked_step.hpp"
#include "flow_steps.hpp"

namespace bflow::detail
{
template <typename event_t, typename selection_t>
class steps_selection : public linked_step<event_t>
{
public:
  template <typename... steps_t>
  static steps_selection<event_t, selection_t> of(steps_t&&... steps);

  result process(event_t e) override {
    auto outcome = bflow::result::rejected;
    for (auto& step : _steps) {
      auto result = step.process(e);
      if (result == bflow::result::completed) return result;
      if (result == bflow::result::accepted) outcome = result;
    }
    return outcome;
  }

private:
  using steps = detail::steps<event_t>;

  explicit steps_selection(steps&&);

  steps _steps;
};


template <typename event_t, typename selection_t>
template <typename... steps_t>
steps_selection<event_t, selection_t> steps_selection<event_t, selection_t>::of(steps_t&&... steps) {
  return steps_selection<event_t, selection_t>(detail::make_steps<event_t>(std::forward<steps_t>(steps)...));
}

template <typename event_t, typename selection_t>
steps_selection<event_t, selection_t>::steps_selection(detail::steps<event_t>&& steps) : _steps(std::move(steps)) {}

} // namespace bflow::detail
