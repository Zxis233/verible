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

#include "common/strings/string_memory_map.h"

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "absl/memory/memory.h"
#include "common/strings/range.h"
#include "common/util/range.h"

namespace verible {
namespace {

// Function to get the owned address range of the underlying string.
static absl::string_view StringViewKey(
    const std::unique_ptr<const std::string>& owned) {
  return absl::string_view(*ABSL_DIE_IF_NULL(owned));
}

typedef StringMemoryMap<std::unique_ptr<const std::string>, StringViewKey>
    StringSet;

TEST(StringMemoryMapTest, EmptyOwnsNothing) {
  const StringSet sset;
  EXPECT_EQ(sset.find("not-owned-anywhere"), nullptr);
}

static absl::string_view InsertStringCopy(StringSet* sset, const char* text) {
  const auto new_iter =
      sset->insert(absl::make_unique<std::string>(text));  // copy
  return make_string_view_range(new_iter->first.first, new_iter->first.second);
}

static void ForAllSubstringRanges(
    absl::string_view sv, const std::function<void(absl::string_view)>& func) {
  for (auto start_iter = sv.begin(); start_iter != sv.end(); ++start_iter) {
    for (auto end_iter = start_iter; end_iter != sv.end(); ++end_iter) {
      if (start_iter == end_iter) continue;  // skip empty ranges
      func(make_string_view_range(start_iter, end_iter));
    }
  }
}

TEST(StringMemoryMapTest, OneElement) {
  StringSet sset;
  const absl::string_view sv(InsertStringCopy(&sset, "OWNED"));

  // Check all valid substring ranges
  ForAllSubstringRanges(sv, [&sset, sv](absl::string_view subrange) {
    const auto* f = sset.find(subrange);
    ASSERT_NE(f, nullptr) << "subrange returned nullptr: " << subrange;
    const absl::string_view fv(**f);
    EXPECT_TRUE(BoundsEqual(fv, sv)) << "got: " << fv << " vs. " << sv;
    EXPECT_EQ(fv, "OWNED");
  });
}

TEST(StringMemoryMapTest, MultipleElements) {
  StringSet sset;
  // There's no telling where these allocated strings will reside in memory.
  const absl::string_view sv1(InsertStringCopy(&sset, "AAA"));
  const absl::string_view sv2(InsertStringCopy(&sset, "BBBB"));
  const absl::string_view sv3(InsertStringCopy(&sset, "CCCCC"));

  // Check all valid substring ranges
  ForAllSubstringRanges(sv1, [&sset, sv1](absl::string_view subrange) {
    const auto* f = sset.find(subrange);
    ASSERT_NE(f, nullptr) << "subrange returned nullptr: " << subrange;
    const absl::string_view fv(*ABSL_DIE_IF_NULL(*f));
    EXPECT_TRUE(BoundsEqual(fv, sv1)) << "got: " << fv << " vs. " << sv1;
    EXPECT_EQ(fv, "AAA");
  });
  ForAllSubstringRanges(sv2, [&sset, sv2](absl::string_view subrange) {
    const auto* f = sset.find(subrange);
    ASSERT_NE(f, nullptr) << "subrange returned nullptr: " << subrange;
    const absl::string_view fv(*ABSL_DIE_IF_NULL(*f));
    EXPECT_TRUE(BoundsEqual(fv, sv2)) << "got: " << fv << " vs. " << sv2;
    EXPECT_EQ(fv, "BBBB");
  });
  ForAllSubstringRanges(sv3, [&sset, sv3](absl::string_view subrange) {
    const auto* f = sset.find(subrange);
    ASSERT_NE(f, nullptr) << "subrange returned nullptr: " << subrange;
    const absl::string_view fv(*ABSL_DIE_IF_NULL(*f));
    EXPECT_TRUE(BoundsEqual(fv, sv3)) << "got: " << fv << " vs. " << sv3;
    EXPECT_EQ(fv, "CCCCC");
  });
}

}  // namespace
}  // namespace verible
