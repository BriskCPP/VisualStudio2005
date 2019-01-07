#include "./ParameterStore.h"

namespace Application
{
	namespace HUD
	{
		ParameterStore::ParameterStore(void)
		{
			this->pitch = 0;
			this->roll = 0;
			this->yaw = 0;
		}
		ParameterStore *ParameterStore::instance = NULL;
		ParameterStore *ParameterStore::getInstance()
		{
			while(instance == NULL) instance = new ParameterStore();
			return instance;
		}
		//以上是单例模式实现
		//下面是最讨厌的根据……收到的信息更新数据的函数
		bool ParameterStore::update(std::list<UINT32> newMessage)
		{
			struct Application::Message::Message message;
			if(Application::Message::Util::convert32bitDataListToMessage(newMessage,&message))
			{
				//转换成功
				switch(message.type)
				{
				case 41:
					//pitch
					if(message.isDouble)
					{
						this->pitch = message.data.float64;
						return true;
					}
					if(message.isFloat)
					{
						this->pitch = message.data.float32;
						return true;
					}
					return false;
					break;
				case 42:
					//roll
					if(message.isDouble)
					{
						this->roll = message.data.float64;
						return true;
					}
					if(message.isFloat)
					{
						this->roll = message.data.float32;
						return true;
					}
					return false;
					break;
				case 43:
					//yaw
					if(message.isDouble)
					{
						this->yaw = message.data.float64;
						return true;
					}
					if(message.isFloat)
					{
						this->yaw = message.data.float32;
						return true;
					}
					return false;
					break;
				default:
					return false;
				}
			}
			else return false;
		}

		double ParameterStore::getPitch()
		{
			return this->pitch;
		}
		double ParameterStore::getRoll()
		{
			return this->roll;
		}
		double ParameterStore::getYaw()
		{
			return this->yaw;
		}
	}
}