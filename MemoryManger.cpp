#include "MemoryManger.h"
// 初始化静态成员
MemoryManger* MemoryManger::instance = nullptr;
std::once_flag MemoryManger::initInstanceFlag;