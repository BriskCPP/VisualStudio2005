#pragma once

//����MIL-STD-1553B PCI MCH�Ŀ�
#include "./MIL-STD-1553B/Proto_mch.h"
#pragma comment(lib,"Excalibur1553B.lib")
//��ΪѧԺ¥������MIL-STD-1553B��ҪVisual C++ 6.0���ܱ��룬����ֻ�ܸ����ز����˶�̬���ӿ�
//����һ���������

#include "./Message/message.h"
#include "./HUD/ParameterStore.h"
#include "./HUD/Thread/threads.h"
#include "./HUD/AttitudeIndicator.h"
#include "./HUD/HeightIndicator.h"