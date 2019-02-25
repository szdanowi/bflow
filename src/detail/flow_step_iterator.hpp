#pragma once

#include "flow_linked_step.hpp"

namespace bflow::detail
{
template <typename event_t>
class step_iterator
{
public:
  using element_t = linked_step<event_t>;
  using self_t = step_iterator<event_t>;

  explicit step_iterator(element_t& current) : _current(&current) {}

  result process(event_t event) { return _current->process(event); }

  explicit operator bool() const { return static_cast<bool>(*_current); }
  self_t next() const { return self_t(_current->next()); }

  self_t operator++() {
    _current = &_current->next();
    return *this;
  }

  const self_t operator++(int) {
    self_t result(*_current);
    ++(*this);
    return result;
  }

  bool operator==(const self_t& other) const { return _current == other._current; }
  bool operator!=(const self_t& other) const { return _current != other._current; }

private:
  element_t* _current;
};
} // namespace bflow::detail
