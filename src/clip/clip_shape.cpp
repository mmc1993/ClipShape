#include "clip_shape.h"
#include "../math/polygon.h"

void ClipShape::Clear()
{
    _points.clear();
}

void ClipShape::Push(const Vec4 & point)
{
    _points.push_back(point);
}

void ClipShape::Init(const Points & points)
{
    std::copy(points.begin(), points.end(), std::back_inserter(_points));
}

const ClipShape::Points & ClipShape::GetPoints() const
{
    return _points;
}

bool ClipShape::IsBeContains(const ClipLine & clipLine) const
{
    auto fn = [&](const Vec4 & point)
    {
        return Polygon::IsContains(point, clipLine);
    };
    return std::all_of(_points.begin(), _points.end(), fn);
}

std::vector<ClipShape> ClipShape::Clip(ClipLine clipLine) const
{
    std::vector<ClipShape> result;
    if (Roll(clipLine))
    {
        auto crossResult = std::move(CheckCross(clipLine));
        if (!crossResult.empty())
        {
            FlipCross(crossResult);

            auto clipA = ClipA(crossResult);
            auto ret1 = clipA.Clip(clipLine);
            std::copy(
                std::make_move_iterator(ret1.begin()),
                std::make_move_iterator(ret1.end()),
                std::back_inserter(result));
            result.push_back(clipA);

            //auto clipB = ClipB(crossResult);
            //auto ret2 = clipB.Clip(clipLine);
            //std::copy(
            //    std::make_move_iterator(ret2.begin()),
            //    std::make_move_iterator(ret2.end()),
            //    std::back_inserter(result));
            //result.push_back(clipB);
        }
    }
    return result;
}

bool ClipShape::Max(const CrossPoint & cp1, const CrossPoint & cp2) const
{
    assert(cp1.mLinkA != cp1.mLinkB);
    assert(cp2.mLinkA != cp2.mLinkB);
    assert(cp1.mPoint != cp2.mPoint);
    assert(cp1.mLinkA == cp2.mLinkA && cp1.mLinkB == cp2.mLinkB ||
           cp1.mLinkA != cp2.mLinkA && cp1.mLinkB != cp2.mLinkB);
    return cp1.mLinkA != cp2.mLinkA && cp1.mLinkA < cp2.mLinkA ||
        cp1.mLinkA == cp2.mLinkA && 
        _points.at(cp1.mLinkA).Unlerp(_points.at(cp1.mLinkB), cp1.mPoint) <
        _points.at(cp2.mLinkA).Unlerp(_points.at(cp2.mLinkB), cp2.mPoint);
}

ClipShape::CrossResult ClipShape::CheckCross(const Vec4 & a, const Vec4 & b) const
{
    CrossResult result;
    auto crossA = 0.0f;
    auto crossB = 0.0f;
    auto size = _points.size();
    for (auto i = 0; i != size; ++i)
    {
        auto & p1 = _points.at(i);
        auto & p2 = _points.at((i + 1) % size);
        if (Polygon::IsCross(a, b, p1, p2, &crossA, &crossB))
        {
            if (crossB != 0.0f && crossB != 1.0f)
            {
                result.emplace_back(a.Lerp(b, crossA), i, (i + 1) % size);
            }
        }
    }

    auto fn = [&](
        const CrossResult::value_type & cross1, 
        const CrossResult::value_type & cross2)
    {
        return (cross1.mPoint - a).LengthSqrt()
            < (cross2.mPoint - a).LengthSqrt();
    };
    std::sort(result.begin(), result.end(), fn);

    return std::move(result);
}

ClipShape::CrossResult ClipShape::CheckCross(const ClipLine & clipLine) const
{
    CrossResult result;
    auto size = clipLine.size();
    for (auto i = 0; i != size; ++i)
    {
        auto & a = clipLine.at(i);
        auto & b = clipLine.at((i + 1) % size);
        auto points = std::move(CheckCross(a, b));
        if (!points.empty())
        {
            for (auto & point : points)
            {
                result.push_back(point);
                if (result.size() > 1)
                { goto exit; }
            }
            if (result.size() == 1 && result.back().mPoint != b)
            { result.emplace_back(b); }
        }
        else if (!result.empty())
        {
            if (result.back().mPoint != a)
            { result.emplace_back(a); }
            if (result.back().mPoint != b)
            { result.emplace_back(b); }
        }
    }
    
exit:
    return result;
}

ClipShape ClipShape::ClipA(const CrossResult & crossResult) const
{
    //  ÖØÐ´
    ClipShape result;
    auto & front = crossResult.front();
    auto & back = crossResult.back();
    auto insert = _points.size();

    for (auto i = 0; 
        i != front.mLinkB && 
        i != _points.size(); ++i)
    {
        result.Push(_points.at(i));
    }

    if (front.mPoint != _points.at(front.mLinkA))
    {
        result.Push(crossResult.front().mPoint);
    }

    for (auto i = 1; i != crossResult.size(); ++i)
    {
        result.Push(crossResult.at(i).mPoint);
    }

    if (back.mLinkB != 0 &&
        back.mPoint != _points.at(back.mLinkB))
    {
        insert = back.mLinkB;
    }

    for (; insert != _points.size(); ++insert)
    {
        result.Push(_points.at(insert));
    }

    return result;
}

ClipShape ClipShape::ClipB(const CrossResult & crossResult) const
{
    //  ÖØÐ´
    ClipShape result;
    auto & front = crossResult.front();
    auto & back = crossResult.back();

    result.Push(front.mPoint);

    for (auto i = front.mLinkB; i != back.mLinkB; ++i)
    {
        result.Push(_points.at(i));
    }

    for (auto i = crossResult.size() - 1; i != 0; --i)
    {
        result.Push(crossResult.at(i).mPoint);
    }

    return result;
}

void ClipShape::FlipCross(CrossResult & result) const
{
    if (!Max(result.front(), result.back()))
    {
        std::reverse(result.begin(), result.end());
    }
}

bool ClipShape::Roll(ClipLine & clipLine) const
{
    if (Polygon::IsContains(clipLine.front(), _points))
    {
        auto fn = [&](const Vec4 & point)
        {
            return !Polygon::IsContains(point, _points);
        };
        auto it = std::find_if(clipLine.begin(), clipLine.end(), fn);

        if (it == clipLine.end()) { return false; }

        auto distance = std::distance(clipLine.begin(), it);

        std::copy(clipLine.begin(), 
            std::next(clipLine.begin(), distance), 
            std::back_inserter(clipLine));

        clipLine.erase(clipLine.begin(), 
            std::next(clipLine.begin(), distance));
    }
    return true;
}
