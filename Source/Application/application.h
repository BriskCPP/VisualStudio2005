#pragma once

//导入MIL-STD-1553B PCI MCH的库
#include "./MIL-STD-1553B/Proto_mch.h"
#pragma comment(lib,"Excalibur1553B.lib")
//因为学院楼该死的MIL-STD-1553B需要Visual C++ 6.0才能编译，所以只能该死地采用了动态链接库
//我有一万个想骂人

#include "./Message/message.h"
#include "./HUD/ParameterStore.h"
#include "./HUD/Thread/threads.h"
#include "./HUD/AttitudeIndicator.h"
#include "./HUD/HeightIndicator.h"