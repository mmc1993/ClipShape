#pragma once

#include "window.h"
#include "../math/vec4.h"
#include "../clip/clip_shape.h"

class AppWindow: public Window {
public:
    struct DrawNode {
        Vec4 v;

        COLORREF c;

        DrawNode(const Vec4 v, const COLORREF & c)
        {
            this->v = v;
            this->c = c;
        }
    };

    using Area = std::list<Vec4>;

    using HitRet = std::tuple<std::vector<size_t>, std::vector<Area>>;

public:
	AppWindow();
	~AppWindow();

protected:
	virtual void OnMessage(UINT uint, WPARAM wparam, LPARAM lparam) override;

private:
    void InitData();

	void Update();

    void BegHit(float x, float y, float radius, size_t num);

    void EndHit();

    void RunHit();
    
private:
    std::vector<DrawNode> _drawNodes;

    std::list<ClipShape> _areas;

    ClipShape::ClipLine _hit;
};