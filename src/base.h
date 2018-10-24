#pragma once

#include <list>
#include <cmath>
#include <array>
#include <vector>
#include <limits>
#include <string>
#include <memory>
#include <cassert>
#include <optional>
#include <iostream>
#include <algorithm>
#include <functional>

#define NOMINMAX
#include <Windows.h>

#include "third/sformat.h"

constexpr auto PI = (3.1415926f);
#define D2R(d)	(PI / 180.0f * (d))
#define R2D(r)	(180.0f / PI * (r))
#define F2B(v)	((v) * 255)

template <int N>
size_t INDEX(size_t i, size_t size)
{
    return (i + N) % size;
}

template <>
inline size_t INDEX<0>(size_t i, size_t size)
{
    return i;
}