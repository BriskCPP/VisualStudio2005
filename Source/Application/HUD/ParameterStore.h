#pragma once

#include <list>
#include "../Message/message.h"

namespace Application
{
	namespace HUD
	{
		class ParameterStore
		{
		private:
			static ParameterStore *instance;
			ParameterStore(void);
		public:
			static ParameterStore *getInstance();
			//以上是单例模式实现

			//根据从socket或ARINC 429总线接收到的消息，更新参数
			bool update(std::list<UINT32> newMessage);

			double getPitch();//41
			double getRoll();//42
			double getYaw();//43
			//下面这两个还没有实现
			UINT16 getHeight();//11
			UINT16 getSpeed();//12
		private:
			double pitch,roll,yaw;
			
		};
	}
}