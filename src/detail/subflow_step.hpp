#pragma once

#include "linked_step.hpp"
#include <utility>

namespace bflow::detail
{
template <typename event_t, typename subflow_t>
class subflow_step : public linked_step<event_t>
{
public:
  subflow_step(subflow_t&& subflow, typename linked_step<event_t>::ptr&& next)
      : linked_step<event_t>(std::move(next)), _subflow(std::move(subflow)) {}

  result process(event_t e) override { return _process<subflow_t>(e); }

  explicit operator bool() const override { return true; }

private:
  template <typename object_t>
  inline auto _process(event_t e) -> decltype(std::declval<object_t>().process(std::declval<event_t>()), result()) {
    return _subflow.process(e);
  }

  template <typename object_t>
  inline auto _process(event_t e) -> decltype(std::declval<object_t>()(std::declval<event_t>()), result()) {
    return _subflow(e);
  }

  subflow_t _subflow;
};

template <typename event_t, typename subflow_t>
auto create_step(subflow_t&& subflow, typename linked_step<event_t>::ptr&& next) {
  return std::make_unique<subflow_step<event_t, subflow_t>>(std::move(subflow), std::move(next));
}
} // namespace bflow::detail
