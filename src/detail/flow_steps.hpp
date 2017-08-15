#pragma once

#include "flow_linked_step.hpp"
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

template <typename event_t>
class step_iterator
{
public:
    using element_t = linked_step<event_t>;
    using self_t = step_iterator<event_t>;

    explicit step_iterator(element_t& current) : _current(&current) {}

    explicit operator bool() const { return static_cast<bool>(*_current); }

    result process(event_t event) { return _current->process(event); }

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
