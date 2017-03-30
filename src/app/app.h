#ifndef __App_h__
#define __App_h__
#include <common/common.h>

class API App
{
public:
  App(const char * title, uint32_t width, uint32_t height);
  ~App();

  void Run();

  void* Handle() const { return hWnd; }

private:
  void * hWnd;
};

#endif