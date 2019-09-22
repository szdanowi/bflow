#include "flow.hpp"
#include <ostream>

namespace bflow
{
namespace
{
const char* to_str(result value) {
  switch (value) {
    case result::rejected: return "rejected";
    case result::accepted: return "accepted";
    case result::completed: return "completed";
  }
}
} // namespace

std::ostream& operator<<(std::ostream& os, result value) {
  os << to_str(value);
  return os;
}
} // namespace bflow
