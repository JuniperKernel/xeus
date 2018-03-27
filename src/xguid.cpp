/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <array>
#include <cstddef>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <atomic>
#include <thread>

#include "xeus/xguid.hpp"
#include "xstring_utils.hpp"

using namespace std::chrono;
namespace xeus {
  std::atomic<long long> _ctr(0);
  static long long int current_time_nanos() {
    return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
  }
  xguid new_xguid() {
    std::stringstream s;
    s << _ctr.fetch_add(1) << current_time_nanos() << "_" << (std::this_thread::get_id());
    return s.str();
  }
}
