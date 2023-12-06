/*
 * Copyright 2023 The Fury Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gtest/gtest.h"
#include <deque>
#include <initializer_list>
#include <list>

#include "fury/meta/field_info.h"
#include "src/fury/meta/type_traits.h"

namespace fury {

namespace test {

using namespace meta;

struct A {
  int x;
  float y;
};

TEST(Meta, RemoveMemberPointer) {
  static_assert(std::is_same_v<RemoveMemberPointerT<int A::*>, int>);
  static_assert(std::is_same_v<RemoveMemberPointerT<bool A::*>, bool>);
}

TEST(Meta, IsSameValue) {
  static_assert(IsSameValue<&A::x, &A::x>);
  static_assert(!IsSameValue<&A::x, &A::y>);

  static_assert(!IsSameValue<1, true>);
  static_assert(IsSameValue<2, 2>);
}

TEST(Meta, ContainsValue) {
  static_assert(ContainsValue<1, 1, 2, 3>);
  static_assert(ContainsValue<2, 1, 2, 3>);
  static_assert(ContainsValue<3, 1, 2, 3>);
  static_assert(!ContainsValue<4, 1, 2, 3>);
  static_assert(ContainsValue<true, 1, true, &A::x, 'a'>);
  static_assert(ContainsValue<'a', 1, true, &A::x, 'a'>);
  static_assert(!ContainsValue<0, 1, true, &A::x, 'a'>);
}

TEST(Meta, IsUnique) {
  static_assert(IsUnique<1, 2, 3>::value);
  static_assert(IsUnique<1, false, true, 3, &A::x>::value);
  static_assert(!IsUnique<1, false, true, false, &A::x>::value);
  static_assert(!IsUnique<1, false, true, &A::x, 1>::value);
}

TEST(Meta, IsIterable) {
  static_assert(IsIterable<std::vector<int>>);
  static_assert(IsIterable<std::vector<std::vector<int>>>);
  static_assert(IsIterable<std::deque<float>>);
  static_assert(IsIterable<std::list<int>>);
  static_assert(IsIterable<std::set<int>>);
  static_assert(IsIterable<std::map<int, std::vector<unsigned>>>);
  static_assert(IsIterable<struct A[10]>);
  static_assert(IsIterable<float[2][2]>);
  static_assert(IsIterable<std::initializer_list<A>>);
  static_assert(IsIterable<std::string>);
  static_assert(IsIterable<std::string_view>);
}

} // namespace test

} // namespace fury

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
