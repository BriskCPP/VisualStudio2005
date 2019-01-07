#pragma once
#include "../../Direct3D9/Direct3D9.h"

namespace Application
{
	namespace HUD
	{
		class AttitudeIndicator: public Application::DisplayComponent
		{
		public:
			//�ԽǶ�ֵλ��λ����Ұ�Ƕȣ���һ��Ϊ20
			//�ԽǶ�ֵΪ��λ��ÿ�ָ��ĽǶȣ�һ��Ϊ5��10
			AttitudeIndicator(
				UINT8 fieldAngle,UINT8 anglePerDivision,
				UINT R = 800,UINT Z = 1000);

		private:
			Direct3D::v9::resource::primitive::LineList<
				struct Direct3D::v9::resource::primitive::vertex::VertexWithColor
			> horizontalLineList;
		}
	}
}