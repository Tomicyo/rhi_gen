#if _WIN32
#include <Windows.h>
#endif
#include "app.h"

#if _WIN32
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT32 msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}
#endif

App::App(const char * title, uint32_t width, uint32_t height)
  : hWnd(nullptr)
{
#if _WIN32
  WNDCLASS windowClass;
  windowClass.style = 0;
  windowClass.lpfnWndProc = &WindowProc;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = GetModuleHandle(NULL);
  windowClass.hIcon = NULL;
  windowClass.hCursor = 0;
  windowClass.hbrBackground = 0;
  windowClass.lpszMenuName = NULL;
  windowClass.lpszClassName = title;
  RegisterClassA(&windowClass);

  RECT rc = { 0, 0, width, height };
  ::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  hWnd = CreateWindowA(title, title, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL,
    GetModuleHandle(NULL), NULL);

  ::ShowWindow((HWND)hWnd, SW_NORMAL);
  ::UpdateWindow((HWND)hWnd);
#endif
}

App::~App()
{
}

void App::Run()
{
#if _WIN32
  MSG message;
  while (true)
  {
    while (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
      ::TranslateMessage(&message);
      ::DispatchMessage(&message);
    }
    if (message.message == WM_QUIT)
    {
      break;
    }
    else
    {
      // call back
    }
  }
#endif
}
