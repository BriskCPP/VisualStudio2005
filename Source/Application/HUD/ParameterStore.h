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
			//�����ǵ���ģʽʵ��

			//���ݴ�socket��ARINC 429���߽��յ�����Ϣ�����²���
			bool update(std::list<UINT32> newMessage);

			double getPitch();//41
			double getRoll();//42
			double getYaw();//43
			//������������û��ʵ��
			UINT16 getHeight();//11
			UINT16 getSpeed();//12

			//�����⼸��setter method�����ǲ���ӵ�
			//����Ϊ��1553Bֻ����Ϊ���ѡ�������
			void setPitch(double pitch);
			void setRoll(double roll);

		private:
			double pitch,roll,yaw;
			
		};
	}
}