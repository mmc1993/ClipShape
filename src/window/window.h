#pragma once

#include "../base.h"
#include <Windows.h>
#include <windowsx.h>
#include "../math/vec4.h"

class Renderer;

class Window {
public:
    Window();
    virtual ~Window();

    void Create(const std::string & title);
    void Move(std::uint16_t x, std::uint16_t y);
    void Size(std::uint16_t w, std::uint16_t h);
    void Loop();

    std::uint16_t GetWidth();
    std::uint16_t GetHeight();

    HWND GetHwnd() { return _hwnd; }
    HDC GetBufDC() { return _bufferDC; }
    HBITMAP GetBufBM() { return _bufferBM; }

	void InitBuffer();
    void DCClear();
    void DCDraw();

    void DCPoint(const Vec4 & point, float radius, const COLORREF & color);
    void DCLine(const Vec4 & start, const Vec4 & end, const COLORREF & color);
    void DCPolygon(const std::vector<Vec4> & polygon, const COLORREF & color);

protected:
	virtual void OnMessage(UINT uint, WPARAM wparam, LPARAM lparam) = 0;

private:
	HWND _hwnd;
	HDC _bufferDC;
    HBITMAP _bufferBM;

private:
	static LRESULT WINAPI OnWndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);
};