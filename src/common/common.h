#ifndef __COMMON_H__
#define __COMMON_H__
#pragma once

#include <stdint.h>
#if BUILD_LIB
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#endif