#pragma once

#include "vec4.h"
#include "../base.h"

class Polygon {
public:
    using Points = std::vector<Vec4>;

    //  线段相交
    static bool IsCross(const Vec4 & a, const Vec4 & b, const Vec4 & c, const Vec4 & d)
    {
        auto ab = b - a; auto cd = d - c;
        return ab.Cross(c - a).z * ab.Cross(d - a).z <= 0
            && cd.Cross(a - c).z * cd.Cross(b - c).z <= 0;
    }

    //  线段交点
    static bool IsCross(const Vec4 & a, const Vec4 & b, const Vec4 & c, const Vec4 & d, float *crossA, float *crossB)
    {
        assert(crossA != nullptr);
        assert(crossB != nullptr);
        auto ab = b - a;
        auto cd = d - c;
        auto cross = ab.Cross(cd).z;
        if (cross != 0)
        {
            *crossA = (c - a).Cross(cd).z / cross;
            *crossB = ab.Cross((d - a)).z / cross;
            return *crossA >= 0.0f && *crossA <= 1.0f
                && *crossB >= 0.0f && *crossB <= 1.0f;
        }
        return false;
    }

    static bool IsContains(const Vec4 & point, const Points & points)
    {
        assert(points.size() >= 3);
        auto count = (size_t)0;
        auto length = points.size();
        auto rayEnd = Vec4(point.x + 5000.0f, point.y);
        for (auto i = 0; i != length; ++i)
        {
            auto & a = points.at(i);
            auto & b = points.at((i + 1) % length);
            if (a.y == b.y)
            {
                if (a.y == point.y &&
                    point.x > std::min(a.x, b.x) &&
                    point.x < std::max(a.x, b.x))
                {
                    return true;
                }
            }
            else
            {
                if (IsCross(point, rayEnd, a, b))
                {
                    std::max(a.y, b.y) > point.y && ++count;
                }
            }
        }

        return count % 2 != 0;
    }
};