#include "MemoryManger.h"
// ��ʼ����̬��Ա
MemoryManger* MemoryManger::instance = nullptr;
std::once_flag MemoryManger::initInstanceFlag;