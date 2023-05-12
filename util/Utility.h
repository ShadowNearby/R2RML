//
// Created by yanjs on 2022/10/13.
//

#pragma once

//#include <ctime>
#include <fstream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>
#include <queue>
#include <utility>
#include <list>
#include <tuple>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <climits>
#include <atomic>
#include <functional>
#include <thread>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <folly/concurrency/ConcurrentHashMap.h>
#include "PairHash.h"
#include <mysqlx/xdevapi.h>

std::string toStdString(mysqlx::Value& value);