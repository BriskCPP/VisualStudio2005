#pragma once
#include "Direct3D9.h"

namespace Direct3D
{
	namespace v9
	{
		namespace mesh
		{
			namespace text
			{
				TextMesh::Scale::Scale(D3DXVECTOR3 sideLength,D3DXVECTOR3 scale /* = D3DXVECTOR3(1.0f,1.0f,1.0f) */)
				{
					this->sideLength = sideLength;
					this->scale = scale;
				}
				TextMesh::Scale::Scale()
				{
					this->scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
				}
				D3DXVECTOR3 TextMesh::Scale::setByScale(D3DXVECTOR3 scale)
				{
					this->sideLength.x *= scale.x;
					this->sideLength.y *= scale.y;
					this->sideLength.z *= scale.z;

					this->scale = scale;
					return this->sideLength;
				}
				D3DXVECTOR3 TextMesh::Scale::setBySideLength(D3DXVECTOR3 sideLength)
				{
					//以下三个式子等号右边的最后那个括号相当于取消原来的scale，将长度还原到一倍长度
					this->scale.x = sideLength.x / (this->sideLength.x / this->scale.x);
					this->scale.y = sideLength.y / (this->sideLength.y / this->scale.y);
					this->scale.z = sideLength.z / (this->sideLength.z / this->scale.z);

					this->sideLength = sideLength;
					return this->scale;
				}
				D3DXVECTOR3 TextMesh::Scale::getScale()
				{
					return this->scale;
				}
				D3DXVECTOR3 TextMesh::Scale::getSideLength()
				{
					return this->sideLength;
				}
				//辅助转换函数
				int TextMesh::convertStringToWideChar(std::string toConvert,LPWSTR *result)
				{
					return convertMultiByteToWideChar(toConvert.c_str(),result);
				}
				int TextMesh::convertMultiByteToWideChar(const char *toConvert,LPWSTR *result)
				{
					int resultLength = MultiByteToWideChar(CP_ACP,0,toConvert,-1,NULL,0);
					*result = (wchar_t*)malloc( resultLength*sizeof(wchar_t) );
					MultiByteToWideChar(CP_ACP,0, toConvert, -1, *result, resultLength);
					return resultLength;
				}

				TextMesh::TextMesh(IDirect3DDevice9 *device,std::string content,std::string font,int weight /* = 200 */)
				{
					//0.创建文本网格
					LPWSTR fontName;
					convertStringToWideChar(font,&fontName);
					HFONT hFont = CreateFont(10,10,0,0,weight,0,0,0,DEFAULT_CHARSET,0,0,0,0,(LPCWSTR)fontName);
					delete[] fontName;

					LPWSTR tempContent;
					convertStringToWideChar(content,&tempContent);
					//我还是想爬得更高去看看更大的舞台
					//我想飞的更高


					//创建字体
					HDC hdc = CreateCompatibleDC(0);//创建环境
					HFONT originalFont = (HFONT)SelectObject(hdc,hFont);//将字体选入设备环境,返回上一个字体，以备重新选择
					D3DXCreateText(device,hdc,(LPCWSTR)tempContent,0.000001f,0.000001f,&this->d3dxMesh,NULL,NULL);//创建文本网格
					SelectObject(hdc,originalFont);//选择原来的字体
					DeleteObject(hFont);
					DeleteDC(hdc);

					delete[] tempContent;

					//1.进行预变换
					//将锚点安排到文字的中心
					struct {D3DXVECTOR3 leftBottom,rightTop;} bound;
					bound.rightTop.x = -(1024.0f*1024.0f*1024.0f);
					bound.rightTop.y = -(1024.0f*1024.0f*1024.0f);
					bound.rightTop.z = -(1024.0f*1024.0f*1024.0f);
					bound.leftBottom.x = 1024.0f*1024.0f*1024.0f;
					bound.leftBottom.y = 1024.0f*1024.0f*1024.0f;
					bound.leftBottom.z = 1024.0f*1024.0f*1024.0f;

					struct Direct3D::v9::mesh::text::TextVertex *pVertex = NULL;
					this->d3dxMesh->LockVertexBuffer(0,(void **)&pVertex);
					for(DWORD i = 0;i < this->d3dxMesh->GetNumVertices();i++)
					{
						bound.leftBottom.x = pVertex[i].x<bound.leftBottom.x?pVertex[i].x:bound.leftBottom.x;
						bound.leftBottom.y = pVertex[i].y<bound.leftBottom.y?pVertex[i].y:bound.leftBottom.y;
						bound.leftBottom.z = pVertex[i].z<bound.leftBottom.z?pVertex[i].z:bound.leftBottom.z;

						bound.rightTop.x = pVertex[i].x>bound.rightTop.x?pVertex[i].x:bound.rightTop.x;
						bound.rightTop.y = pVertex[i].y>bound.rightTop.y?pVertex[i].y:bound.rightTop.y;
						bound.rightTop.z = pVertex[i].z>bound.rightTop.z?pVertex[i].z:bound.rightTop.z;
					}
					//获得上下限之后，应用变换
					D3DXVECTOR3 initialCenter = D3DXVECTOR3(
						(bound.rightTop.x + bound.leftBottom.x)/2,
						(bound.rightTop.y + bound.leftBottom.y)/2,
						(bound.rightTop.z + bound.leftBottom.z)/2
						);//生成文本网格时的中心
					for(DWORD i = 0;i < d3dxMesh->GetNumVertices();i++)
					{
						//进行平移，将文本网格移动到中点
						pVertex[i].x -= initialCenter.x;
						pVertex[i].y -= initialCenter.y;
						pVertex[i].z -= initialCenter.z;
					}
					this->d3dxMesh->UnlockVertexBuffer();

					this->scale = TextMesh::Scale(D3DXVECTOR3(
						bound.rightTop.x - bound.leftBottom.x,
						bound.rightTop.y - bound.leftBottom.y,
						bound.rightTop.z - bound.leftBottom.z
						));
					this->center = D3DXVECTOR3(0,0,0);
					this->translation = D3DXVECTOR3(0,0,0);
					this->rotation = D3DXVECTOR3(0,0,0);
				}
			}

			MeshContainer::MeshContainer(ID3DXMesh *d3dxMesh /* = NULL */)
			{
				this->d3dxMesh = d3dxMesh;
			}
			MeshContainer::~MeshContainer()
			{
				this->d3dxMesh->Release();
			}
			HRESULT MeshContainer::DrawSubset(DWORD attributeId /* = 0 */)
			{
				return this->d3dxMesh->DrawSubset(attributeId);
			}
		}
	}
}