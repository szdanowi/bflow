#pragma once

#include "linked_step.hpp"
#include "subflow_step.hpp"
#include "step_iterator.hpp"
#include <list>
#include <memory>
#include <utility>

namespace bflow::detail
{
template <typename event_t>
class step_iterator;

template <typename event_t>
class steps
{
public:
  using element = linked_step<event_t>;
  using iterator = step_iterator<event_t>;

  steps() = default;

  template <typename step_t>
  void push_front(step_t&& step);

  iterator begin() const;
  iterator end() const;

private:
  std::unique_ptr<linked_step<event_t>> _front;
};

template <typename event_t>
template <typename step_t>
void steps<event_t>::push_front(step_t&& step) {
  _front = create_step<event_t>(std::move(step), std::move(_front));
}

template <typename event_t>
auto steps<event_t>::end() const -> iterator {
  return iterator(element::universal_end);
}

template <typename event_t>
auto steps<event_t>::begin() const -> iterator {
  return _front != nullptr ? iterator(*_front) : end();
}

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

} // namespace bflow::detail
