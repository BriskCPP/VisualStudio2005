#include "Direct3D9.h"

namespace Direct3D
{
	namespace v9
	{
		namespace text
		{
			Text::Text(IDirect3DDevice9 *device,std::string content,std::string font,int weight /* = 200 */)
			{
				//最基本的构造函数

				LPWSTR fontName;
				convertStringToWideChar(font,&fontName);
				HFONT hFont = CreateFont(10,5,0,0,weight,0,0,0,DEFAULT_CHARSET,0,0,0,0,(LPCWSTR)fontName);
				delete[] fontName;

				LPWSTR tempContent;
				convertStringToWideChar(content,&tempContent);
				//我还是想爬得更高去看看更大的舞台
				//我想飞的更高


				//创建字体
				HDC hdc = CreateCompatibleDC(0);//创建环境
				HFONT originalFont = (HFONT)SelectObject(hdc,hFont);//将字体选入设备环境,返回上一个字体，以备重新选择
				D3DXCreateText(device,hdc,(LPCWSTR)tempContent,0.000001f,0.000001f,&this->d3dMesh,NULL,NULL);//创建文本网格
				SelectObject(hdc,originalFont);//选择原来的字体
				DeleteObject(hFont);
				DeleteDC(hdc);

				delete[] tempContent;

				this->pretransform();//预变换

				//写算法真的是非常消耗脑筋的工作，我也不知道老师为什么  就一定要……逼我们做一些非常困难的事情
			}

			void Text::pretransform()
			{
				//进行预变换
				//将锚点安排到文字的中心
				this->bound.rightTop.x = -(1024.0f*1024.0f*1024.0f);
				this->bound.rightTop.y = -(1024.0f*1024.0f*1024.0f);
				this->bound.rightTop.z = -(1024.0f*1024.0f*1024.0f);
				this->bound.leftBottom.x = 1024.0f*1024.0f*1024.0f;
				this->bound.leftBottom.y = 1024.0f*1024.0f*1024.0f;
				this->bound.leftBottom.z = 1024.0f*1024.0f*1024.0f;

				struct Direct3D::v9::text::TextVertex *pVertex = NULL;
				this->d3dMesh->LockVertexBuffer(0,(void **)&pVertex);
				for(DWORD i = 0;i < this->d3dMesh->GetNumVertices();i++)
				{
					this->bound.leftBottom.x = pVertex[i].x<this->bound.leftBottom.x?pVertex[i].x:this->bound.leftBottom.x;
					this->bound.leftBottom.y = pVertex[i].y<this->bound.leftBottom.y?pVertex[i].y:this->bound.leftBottom.y;
					this->bound.leftBottom.z = pVertex[i].z<this->bound.leftBottom.z?pVertex[i].z:this->bound.leftBottom.z;

					this->bound.rightTop.x = pVertex[i].x>this->bound.rightTop.x?pVertex[i].x:this->bound.rightTop.x;
					this->bound.rightTop.y = pVertex[i].y>this->bound.rightTop.y?pVertex[i].y:this->bound.rightTop.y;
					this->bound.rightTop.z = pVertex[i].z>this->bound.rightTop.z?pVertex[i].z:this->bound.rightTop.z;
				}
				//获得上下限之后，应用变换
				D3DXVECTOR3 initialCenter = D3DXVECTOR3(
					(this->bound.rightTop.x + this->bound.leftBottom.x)/2,
					(this->bound.rightTop.y + this->bound.leftBottom.y)/2,
					(this->bound.rightTop.z + this->bound.leftBottom.z)/2
					);//生成文本网格时的中心
				for(DWORD i = 0;i < this->d3dMesh->GetNumVertices();i++)
				{
					//进行平移
					pVertex[i].x -= initialCenter.x;
					pVertex[i].y -= initialCenter.y;
					pVertex[i].z -= initialCenter.z;
				}
				this->d3dMesh->UnlockVertexBuffer();
				this->center = D3DXVECTOR3(0,0,0);
				this->bound.leftBottom.x -= initialCenter.x;
				this->bound.rightTop.x -= initialCenter.x;
				this->bound.leftBottom.y -= initialCenter.y;
				this->bound.rightTop.y -= initialCenter.y;
				this->bound.leftBottom.z -= initialCenter.z;
				this->bound.rightTop.z -= initialCenter.z;

				this->translation = D3DXVECTOR3(0,0,0);
				this->rotation = D3DXVECTOR3(0,0,0);
				this->scale = D3DXVECTOR3(1,1,1);
			}

			ID3DXMesh *Text::getMesh(D3DXVECTOR3 leftBottom,D3DXVECTOR3 rightTop)
			{
				//
				D3DXVECTOR3 translation = rightTop/2 + leftBottom/2;
				D3DXVECTOR3 scale;
				scale.x = (rightTop.x-leftBottom.x)/(this->bound.rightTop.x - this->bound.leftBottom.x);
				scale.y = (rightTop.y-leftBottom.y)/(this->bound.rightTop.y - this->bound.leftBottom.y);
				scale.z = (rightTop.z-leftBottom.z)/(this->bound.rightTop.z - this->bound.leftBottom.z);

				if(scale.x<scale.y) scale.y = scale.x;
				if(scale.x>scale.y) scale.x = scale.y;

				this->setTranslation(translation);
				this->setScale(scale);
				this->setRotation(D3DXVECTOR3(0.0f,0.0f,0.0f));
				//
				return this->getMesh();
			}

			ID3DXMesh *Text::getMesh()
			{
				ID3DXMesh *transformedMesh = this->d3dMesh;
				//以后都是对这个进行操作，不再改变原始的
				//由于中心在setTranslation()时就已经改变了
				//所以必须先进行平移操作
				//0.平移
				struct Direct3D::v9::text::TextVertex *pVertex = NULL;
				transformedMesh->LockVertexBuffer(0,(void **)&pVertex);
				for(DWORD i = 0;i < transformedMesh->GetNumVertices();i++)
				{
					pVertex[i].x += this->translation.x;
					pVertex[i].y += this->translation.y;
					pVertex[i].z += this->translation.z;
					//暂时不改变顶点法线试试效果
				}
				
				//1.旋转(todo)
				//旋转的时候要改变法向量，所以就比较复杂，暂时留着
				//emmmmmm……我做了一下抽象，应该没问题了吧
				for(DWORD i = 0;i < transformedMesh->GetNumVertices();i++)
				{
					D3DXVECTOR3 relative;
					relative.x = pVertex[i].x - this->center.x;
					relative.y = pVertex[i].y - this->center.y;
					relative.z = pVertex[i].z - this->center.z;

					D3DXVECTOR3 rotated = Direct3D::v9::vector::rotate(relative,this->rotation);
					pVertex[i].x = this->center.x + rotated.x;
					pVertex[i].y = this->center.y + rotated.y;
					pVertex[i].z = this->center.z + rotated.z;

					//下面要旋转法向量
					D3DXVECTOR3 n = D3DXVECTOR3(pVertex[i].nx,pVertex[i].ny,pVertex[i].nz);
					rotated = Direct3D::v9::vector::rotate(n,this->rotation);
					pVertex[i].nx = rotated.x;
					pVertex[i].ny = rotated.y;
					pVertex[i].nz = rotated.z;
				}
				//2.缩放，缩放不改变中心
				for(DWORD i = 0;i < transformedMesh->GetNumVertices();i++)
				{
					D3DXVECTOR3 relative;
					relative.x = pVertex[i].x - this->center.x;
					relative.y = pVertex[i].y - this->center.y;
					relative.z = pVertex[i].z - this->center.z;
					pVertex[i].x = this->center.x + relative.x*this->scale.x;
					pVertex[i].y = this->center.y + relative.y*this->scale.y;
					pVertex[i].z = this->center.z + relative.z*this->scale.z;
					//暂时不改变顶点法线
					//因为真的不知道顶点法线会影响什么
				}
				transformedMesh->UnlockVertexBuffer();

				return transformedMesh;
			}


			Text::~Text()
			{
				this->d3dMesh->Release();
			}
			void Text::setTranslation(D3DXVECTOR3 translation)
			{
				this->translation = translation;
				//平移要改变中心，否则就不对了
				this->center.x += this->translation.x;
				this->center.y += this->translation.y;
				this->center.z += this->translation.z;
				//上下限也要改变
				this->bound.leftBottom.x += this->translation.x;
				this->bound.leftBottom.y += this->translation.y;
				this->bound.leftBottom.z += this->translation.z;
				this->bound.rightTop.x += this->translation.x;
				this->bound.rightTop.y += this->translation.y;
				this->bound.rightTop.z += this->translation.z;
			}
			void Text::setRotation(D3DXVECTOR3 rotation)
			{
				this->rotation = rotation;

				//改变上下边界
				this->bound.leftBottom = Direct3D::v9::vector::rotate((this->bound.leftBottom - this->center),this->rotation);
				this->bound.rightTop = Direct3D::v9::vector::rotate((this->bound.rightTop - this->center),this->rotation);
			}
			void Text::setScale(D3DXVECTOR3 scale)
			{
				this->scale = scale;

				//缩放要改变上下边界
				D3DXVECTOR3 relative;
				//先处理下边界
				relative.x = this->bound.leftBottom.x - this->center.x;
				relative.y = this->bound.leftBottom.y - this->center.y;
				relative.z = this->bound.leftBottom.z - this->center.z;
				this->bound.leftBottom.x = this->center.x + this->scale.x*relative.x;
				this->bound.leftBottom.y = this->center.y + this->scale.y*relative.y;
				this->bound.leftBottom.z = this->center.z + this->scale.z*relative.z;
				//再处理上边界
				relative.x = this->bound.rightTop.x - this->center.x;
				relative.y = this->bound.rightTop.y - this->center.y;
				relative.z = this->bound.rightTop.z - this->center.z;
				this->bound.rightTop.x = this->center.x + this->scale.x*relative.x;
				this->bound.rightTop.y = this->center.y + this->scale.y*relative.y;
				this->bound.rightTop.z = this->center.z + this->scale.z*relative.z;
			}

			D3DXVECTOR3 Text::getLeftBottom()
			{
				return this->bound.leftBottom;
			}
			D3DXVECTOR3 Text::getRightTop()
			{
				return this->bound.rightTop;
			}

			//辅助转换函数
			int Text::convertStringToWideChar(std::string toConvert,LPWSTR *result)
			{
				return convertMultiByteToWideChar(toConvert.c_str(),result);
			}
			int Text::convertMultiByteToWideChar(const char *toConvert,LPWSTR *result)
			{
				int resultLength = MultiByteToWideChar(CP_ACP,0,toConvert,-1,NULL,0);
				*result = (wchar_t*)malloc( resultLength*sizeof(wchar_t) );
				MultiByteToWideChar(CP_ACP,0, toConvert, -1, *result, resultLength);
				return resultLength;
			}
		}
	}
}