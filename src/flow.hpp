#pragma once

#include "detail/flow_templates_impl.hpp"
#include "detail/steps_selection.hpp"
#include "detail/iteration/looped.hpp"
#include "detail/iteration/one_shot.hpp"
#include "flow_result.hpp"
#include <iosfwd>

namespace bflow
{
namespace detail
{
struct any_consumes;
} // namespace detail

template <typename event_t>
using one_shot = detail::flow<event_t, detail::iteration::one_shot>;

template <typename event_t>
using looped = detail::flow<event_t, detail::iteration::looped>;

template <typename event_t>
using any = detail::steps_selection<event_t, detail::any_consumes>;

std::ostream& operator<<(std::ostream& os, result value);

} // namespace bflow
