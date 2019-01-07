#pragma once
#include "../../Direct3D9/Direct3D9.h"

namespace Application
{
	namespace HUD
	{
		class AttitudeIndicator: public Application::DisplayComponent
		{
		public:
			//以角度值位单位的视野角度，民航一般为20
			//以角度值为单位的每分隔的角度，一般为5或10
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