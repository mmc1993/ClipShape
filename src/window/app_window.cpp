#include "app_window.h"
#include "../third/json.h"
#include "../math/polygon.h"

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnMessage(UINT uint, WPARAM wparam, LPARAM lparam)
{
	switch (uint)
	{
	case WM_CREATE:
		{
			SetTimer(GetHwnd(), 1001, 33, nullptr);

            InitData();
		}
		break;
	case WM_TIMER:
		{
			if (wparam == 1001) 
			{ 
                DCClear();
				Update();
                DCDraw();
            }
		}
		break;
    case WM_LBUTTONDOWN:
        {
            BegHit(
                (float)GET_X_LPARAM(lparam), 
                (float)GET_Y_LPARAM(lparam), 50.0f, 32);
        }
        break;
    case WM_MOUSEMOVE:
        {
            if (wparam & MK_LBUTTON)
            {
                BegHit(
                    (float)GET_X_LPARAM(lparam),
                    (float)GET_Y_LPARAM(lparam), 50.0f, 32);
                RunHit();
            }
        }
        break;
    case WM_LBUTTONUP:
        {
            RunHit();
            EndHit();
        }
        break;
	}
}

void AppWindow::InitData()
{
    auto jvalue = JValue::FromFile("res/data.json");
    for (auto jarea : *jvalue.Get("areas"))
    {
        std::vector<Vec4> verts;
        for (auto jvert : *jarea)
        {
            verts.push_back({
                    jvert->Get("x")->ToFloat(),
                    jvert->Get("y")->ToFloat()
                });
        }
        ClipShape area;
        area.Init(verts);
        _areas.push_back(area);
    }
}

void AppWindow::Update()
{
    //  »æÖÆÇøÓò
    for (auto &area : _areas)
    {
        DCPolygon(area.GetPoints(), RGB(255, 255, 255));
    }

    for (auto & node : _drawNodes)
    {
        DCPoint(node.v, 5, node.c);
    }

    DCPolygon(_hit, RGB(255, 0, 0));
}

void AppWindow::BegHit(float x, float y, float radius, size_t num)
{
    EndHit();
    const auto step = (PI * 2) / num;
    for (auto i = 0; i != num; ++i)
    {
        _hit.emplace_back(
            radius * std::cos(i * step) + x,
            radius * std::sin(i * step) + y);
    }
}

void AppWindow::EndHit()
{
    _hit.clear();
}

void AppWindow::RunHit()
{
    for (auto it = _areas.begin(); it != _areas.end(); )
    {
        auto iter = it++;
        if (iter->IsBeContains(_hit))
        {
            _areas.erase(iter);
        }
        else
        {
            if (auto list = iter->Clip(_hit); !list.empty())
            {
                _areas.insert(iter,
                    list.begin(),
                    list.end());
                _areas.erase(iter);
            }
        }
    }
}
