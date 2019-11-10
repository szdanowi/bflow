#pragma once

#include "linked_step.hpp"
#include "steps.hpp"

namespace bflow::detail
{
template <typename event_t, typename selection_t>
class steps_selection : public linked_step<event_t>
{
  using self_t = steps_selection<event_t, selection_t>;

public:
  steps_selection() = delete;
  steps_selection(self_t&&) = default;
  steps_selection(const self_t&) = delete;

  self_t& operator=(self_t&&) = default;
  self_t& operator=(const self_t&) = delete;

  template <typename... steps_t>
  static self_t of(steps_t&&... steps) {
    return self_t(detail::make_steps<event_t>(std::forward<steps_t>(steps)...));
  }

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
  explicit steps_selection(steps<event_t>&& steps) : _steps(std::move(steps)) {}

  steps<event_t> _steps;
};

} // namespace bflow::detail
