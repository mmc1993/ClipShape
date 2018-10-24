#pragma once

#include "vec4.h"
#include "../base.h"

class Polygon {
public:
    using Points = std::vector<Vec4>;

    //  �߶��ཻ
    static bool IsCross(const Vec4 & a, const Vec4 & b, const Vec4 & c, const Vec4 & d)
    {
        auto ab = b - a; auto cd = d - c;
        return ab.Cross(c - a).z * ab.Cross(d - a).z <= 0
            && cd.Cross(a - c).z * cd.Cross(b - c).z <= 0;
    }

    static bool OnLine(const Vec4 & a, const Vec4 & b, const Vec4 & point)
    {
        return point.x >= std::min(a.x, b.x)
            && point.x <= std::max(a.x, b.x)
            && point.y >= std::min(a.y, b.y)
            && point.y <= std::max(a.y, b.y)
            && (point - a).Cross(b - point).z == 0;
    }

    //  �߶ν���
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

    //  �ж��߶��ڶ������, �ų������α�ƽ�е����.
    static bool IsContains(const Vec4 & a, const Vec4 & b, const Points & points)
    {
        if (!Polygon::IsContains(a, points) || 
            !Polygon::IsContains(b, points))
        {
            return false;
        }

        std::vector<Vec4> set;
        auto crossA = 0.0f;
        auto crossB = 0.0f;
        auto size = points.size();
        for (auto i = 0; i != size; ++i)
        {
            auto & c = points.at(INDEX<0>(i, size));
            auto & d = points.at(INDEX<1>(i, size));
            if (Polygon::IsCross(a, b, c, d, &crossA, &crossB))
            {
                if (crossA != 0.0f && crossA != 1.0f && 
                    crossB != 0.0f && crossB != 1.0f)
                {
                    return false;
                }
                else
                {
                    set.push_back(a.Lerp(b, crossA));
                }
            }
        }

        auto fn = [](auto & a, auto & b)
        {
            return a.x < b.x 
                || a.x == b.x && a.y < b.y;
        };
        std::sort(set.begin(), set.end(), fn);

        for (auto i = 0; i != set.size(); ++i)
        {
            auto isOn = false;
            auto & a = set.at(INDEX<0>(i, set.size()));
            auto & b = set.at(INDEX<1>(i, set.size()));
            if (a == b) { continue; }

            if (!IsContains(a.Lerp(b, 0.5f), points, &isOn) || isOn)
            {
                return false;
            }
        }
        return true;
    }

    static bool IsContains(const Vec4 & point, const Points & points, bool * isOn = nullptr)
    {
        assert(points.size() >= 3);
        auto count = (size_t)0;
        auto length = points.size();
        auto rayEnd = Vec4(point.x + 5000.0f, point.y);
        for (auto i = 0; i != length; ++i)
        {
            auto & a = points.at(INDEX<0>(i, length));
            auto & b = points.at(INDEX<1>(i, length));
            if (OnLine(a, b, point))
            {
                if (isOn != nullptr)
                {
                    *isOn = true;
                }
                return true;
            }
            if (a.y != b.y)
            {
                if (IsCross(point, rayEnd, a, b))
                {
                    std::max(a.y, b.y) > point.y && ++count;
                }
            }
        }
        return (count % 2 != 0);
    }
};