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
    case WM_MOUSEMOVE:
        {
            if (wparam & MK_LBUTTON)
            {
                auto radius = 50.0f;
                _hit = {
                    {
                        -radius + GET_X_LPARAM(lparam),
                        -radius + GET_Y_LPARAM(lparam)
                    },
                    {
                        radius + GET_X_LPARAM(lparam),
                        -radius + GET_Y_LPARAM(lparam)
                    },
                    {
                        radius + GET_X_LPARAM(lparam),
                        radius + GET_Y_LPARAM(lparam)
                    },
                    {
                        -radius + GET_X_LPARAM(lparam),
                        radius + GET_Y_LPARAM(lparam)
                    }
                };
            }
        }
        break;
    case WM_LBUTTONUP:
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
                    auto list = iter->Clip(_hit);
                    if (!list.empty())
                    {
                        _areas.insert(iter, 
                            list.begin(), 
                            list.end());
                        _areas.erase(iter);
                    }
                }
            }
            _hit.clear();
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
    //  ªÊ÷∆«¯”Ú
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
