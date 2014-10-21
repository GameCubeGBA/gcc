// { dg-options "-std=gnu++11" }
// Copyright (C) 2011-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.


#include <unordered_set>
#include <testsuite_performance.h>

int main()
{
  using namespace __gnu_test;

  time_counter time;
  resource_counter resource;

  std::unordered_set<int> ref;
  for (int i = 0; i != 500000; ++i)
    ref.insert(i);

  start_counters(time, resource);

  for (unsigned i = 0; i < 500; ++i)
    std::unordered_set<int> v(ref);

  stop_counters(time, resource);
  report_performance(__FILE__, "unordered_set<int> copy", time, resource);

  return 0;
}
