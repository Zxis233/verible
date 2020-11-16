// Copyright 2017-2020 The Verible Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef VERIBLE_COMMON_STRINGS_STRING_MEMORY_MAP_H_
#define VERIBLE_COMMON_STRINGS_STRING_MEMORY_MAP_H_

#include <utility>

#include "absl/strings/string_view.h"
#include "common/util/interval_map.h"

namespace verible {

namespace internal {
inline std::pair<absl::string_view::const_iterator,
                 absl::string_view::const_iterator>
string_view_to_pair(absl::string_view sv) {
  return std::make_pair(sv.begin(), sv.end());
}
}  // namespace internal

// StringMemoryMap maps (non-owned) string_views to owned memory.
// This class provides a set-like interface to objects of type T.
// T is any type of object that *owns* some string memory, whose
// address range serves as a key.  Since string addresses are the result of
// allocation, they are guaranteed to not overlap.
//   T needs to be move-able, like a std::unique_ptr.
//   Note: std::string is move-able, but *not* guaranteed to maintain the same
//     address after std::move-ing.
// RangeOf is any functor that returns a string_view of memory owned by objects
// of type T.
//
// It is expected that the owned string memory address range never changes over
// the lifetime of objects stored in this map, in other words, RangeOf(obj) will
// always return the same range for the same object.  One way to ensure this is
// to make the underlying storage a 'const std::string'.
//
template <class T, absl::string_view RangeOf(const T&)>
class StringMemoryMap {
  typedef absl::string_view::const_iterator key_type;
  typedef DisjointIntervalMap<key_type, T> map_type;

 public:
  typedef typename map_type::iterator iterator;

  StringMemoryMap() = default;

  StringMemoryMap(const StringMemoryMap&) = delete;
  StringMemoryMap(StringMemoryMap&&) = delete;
  StringMemoryMap& operator=(const StringMemoryMap&) = delete;
  StringMemoryMap& operator=(StringMemoryMap&&) = delete;

  // Returns reference to the object in the set that owns the memory range of
  // string 's', or else nullptr if 's' does not fall entirely within one of the
  // intervals in the map.
  const T* find(absl::string_view sv) const {
    const auto iter = memory_map_.find(internal::string_view_to_pair(sv));
    if (iter == memory_map_.end()) return nullptr;
    return &iter->second;
  }

  // Move-inserts an element into the set, keyed on the memory range owned
  // by the object.
  iterator insert(T&& t) {
    const absl::string_view key = RangeOf(t);
    return memory_map_.must_emplace(internal::string_view_to_pair(key),
                                    std::move(t));
  }

 private:
  map_type memory_map_;
};

}  // namespace verible

#endif  // VERIBLE_COMMON_STRINGS_STRING_MEMORY_MAP_H_
