#pragma once

#include <functional>
#include <iosfwd>
#include <memory>

namespace bflow
{
enum class result { rejected, accepted, completed };

namespace detail
{
template <typename event_t>
class steps;

template <typename event_t, typename iteration_t>
class _flow
{
public:
  using listener = std::function<void()>;

  _flow() = default;

  template <typename... steps_t>
  static _flow<event_t, iteration_t> of(steps_t&&... steps);

  result process(event_t event);

  inline void on_completion(listener completion_listener) { _completion_listener = completion_listener; }

private:
  using steps = detail::steps<event_t>;
  using current_step = typename steps::iterator;

  explicit _flow(steps&&);

  steps _steps;
  current_step _current = _steps.begin();
  listener _completion_listener = [] {};
};

template <typename event_t, typename selection_t>
class steps_selection;

struct one_shot_iteration;
struct looped_iteration;
struct any_consumes;
} // namespace detail

template <typename event_t>
using one_shot = detail::_flow<event_t, detail::one_shot_iteration>;

template <typename event_t>
using looped = detail::_flow<event_t, detail::looped_iteration>;

template <typename event_t>
using any = detail::steps_selection<event_t, detail::any_consumes>;

std::ostream& operator<<(std::ostream& os, result value);

} // namespace bflow

#include "detail/flow_templates_impl.hpp"
