#pragma once

#include <iosfwd>
#include <memory>

namespace bflow
{
enum class result { rejected, accepted, completed };

namespace detail
{
template <typename event_t>
class steps;
}

template <typename event_t>
class linear
{
public:
    linear() = default;

    template <typename... steps_t>
    static linear<event_t> of(steps_t&&... steps);

    result process(event_t event);

private:
    using steps = detail::steps<event_t>;
    using current_step = typename steps::iterator;

    explicit linear(steps&&);

    steps _steps;
    current_step _current = _steps.front();
};

std::ostream& operator<<(std::ostream& os, result value);

} // namespace bflow

#include "detail/flow_templates_impl.hpp"
