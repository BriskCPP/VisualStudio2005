#pragma once
#include "../../DisplayComponent.h"

namespace Application
{
	namespace HUD
	{
		namespace DisplayComponent
		{
			class AttitudeIndicator: public Application::DisplayComponent
			{
			public:
				AttitudeIndicator(double pitch,double roll,UINT R = 600,UINT Z = 1000);
				//���������ѧ������ԭ��ͺ��ˣ���֪��������ô������
				std::list<struct Direct3D::v9::primitive::vertex::VertexWithColor> getOutputVertexList();
			private:
				UINT R,Z;
				double pitch,roll;
			};
		}
	}
}