#pragma once

#include "../base.h"
#include "../math/vec4.h"
#include "../math/polygon.h"

class ClipShape {
public:
    struct CrossPoint {
        Vec4 mPoint;        //  交点
        size_t mLinkA;      //  交点上游
        size_t mLinkB;      //  交点下游
        CrossPoint(
            const Vec4 & point = Vec4(), 
            const size_t linkA = 0, 
            const size_t linkB = 0) 
            : mPoint(point)
            , mLinkA(linkA)
            , mLinkB(linkB)
        { }
    };

    using Points = std::vector<Vec4>;

    using ClipLine = std::vector<Vec4>;

    using CrossResult = std::vector<CrossPoint>;

    using ClipResult = std::pair<ClipShape, ClipShape>;

public:
    ClipShape();

    ClipShape(ClipShape && other);

    ClipShape & operator=(ClipShape && other);

    ClipShape(const ClipShape & other) = default;

    ClipShape & operator=(const ClipShape & other) = default;
    
    void Clear();

    void Push(const Vec4 & point);

    void Init(const Points & points);

    const Points & GetPoints() const;

    bool IsBeContains(const ClipLine & clipLine) const;

    std::vector<ClipShape> Clip(ClipLine clipLine) const;

private:
    bool Max(const CrossPoint & cp1, const CrossPoint & cp2) const;

    CrossResult CheckCross(const Vec4 & a, const Vec4 & b) const;
    CrossResult CheckCross(const ClipLine & clipLine) const;
    bool CheckCross(const CrossResult & crossResult) const;

    ClipShape ClipA(const CrossResult & crossResult) const;
    ClipShape ClipB(const CrossResult & crossResult) const;

    void FlipCross(CrossResult & result) const;

    bool Roll(ClipLine & clipLine) const;

private:
    Points _points;
};