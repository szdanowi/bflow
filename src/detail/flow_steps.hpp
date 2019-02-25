#pragma once

#include "flow_linked_step.hpp"
#include "flow_step_iterator.hpp"
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

  iterator front() const;
  iterator end() const;

private:
  std::unique_ptr<linked_step<event_t>> _front;
};

template <typename event_t>
template <typename step_t>
void steps<event_t>::push_front(step_t&& step) {
  _front = create_step<event_t>(step, std::move(_front));
}

template <typename event_t>
auto steps<event_t>::end() const -> iterator {
  return iterator(element::universal_end);
}

template <typename event_t>
auto steps<event_t>::front() const -> iterator {
  return _front != nullptr ? iterator(*_front) : end();
}

} // namespace bflow::detail
