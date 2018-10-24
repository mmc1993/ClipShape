
#include "window/app_window.h"

int main()
{
    //auto a = Vec4(222, 377);
    //auto b = Vec4(222, 477);
    //auto c = Vec4(40, 423);
    //auto d = Vec4(1105, 433);
    //auto crossA = 0.0f;
    //auto crossB = 0.0f;
    //auto ret1 = Polygon::IsCross(a, b, c, d, &crossA, &crossB);
    //auto e = a.Lerp(b, crossA);
    //auto ret2 = Polygon::IsCross(e, b, c, d, &crossA, &crossB);
    //auto f = e.Lerp(b, crossA);
    //auto ret3 =  FLT_EPSILON >= std::abs(-1.192092896e-08F);

    AppWindow app;
    app.Create("xxx");
    app.Size(1280, 720);
    app.Move(100, 100);
    app.Loop();
    return 0;
}