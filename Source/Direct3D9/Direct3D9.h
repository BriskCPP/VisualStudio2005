#pragma once
#include "d3d9.h"
#include "d3dx9.h"

#include <list>
#include <string>

namespace Direct3D
{
	namespace v9
	{
		namespace initialization
		{
			//if the master GPU support,return D3DCREATE_HARDWARE_VERTEXPROCESSING
			//otherwise return D3DCREATE_SOFTWARE_VERTEXPROCESSING
			UINT32 renderByHardwareOrSoftware(IDirect3D9 *direct3D9, D3DDEVTYPE deviceType);

			bool createDevice(IDirect3D9 *direct3D9, D3DDEVTYPE deviceType, HWND hwnd, UINT32 byHardwareOrSoftware,
				D3DPRESENT_PARAMETERS presentParameters, IDirect3DDevice9 **device
				);

			D3DPRESENT_PARAMETERS createPresentParameters(HWND hwnd, UINT16 width, UINT16 height, bool isFullScreen);

			HWND createWindow(HINSTANCE hInstance, WNDPROC wndProc, UINT16 width, UINT16 height, bool isFullScreen);

			LRESULT CALLBACK defaultWindowProc(
				HWND hwnd,
				UINT uMessage,
				WPARAM wParam,
				LPARAM lParam
				);

			bool initialize(HINSTANCE hInstance, WNDPROC wndProc, UINT16 width, UINT16 height, bool isFullScreen, D3DDEVTYPE deviceType, IDirect3DDevice9 **device,HWND *windowHandle = NULL);
		}

		void enterMessageLoop(
			bool (*displayFunction)(double totalTime,double deltaTime,IDirect3DDevice9 *device,HWND windowHandle),
			IDirect3DDevice9 *device,HWND windowHandle
			);

		namespace configuration
		{
			void setStaticCamera(
				IDirect3DDevice9 *device,
				D3DXVECTOR3 position,D3DXVECTOR3 target,
				float angle,UINT16 viewPortWidth,UINT16 viewPortHeight,
				float zMinToRender,float zMaxToRender
				);
		}

		namespace primitive
		{
			namespace vertex
			{
				struct VertexWithColor
				{
					float x,y,z;
					D3DCOLOR color;

					static const DWORD flexibleVectorFormat;
				};
			}
			namespace buffer
			{
				template<typename Vertex>
				class VertexBuffer
				{
				public:
					VertexBuffer(
						IDirect3DDevice9 *device,UINT length,
						DWORD usage = D3DUSAGE_WRITEONLY,
						D3DPOOL pool = D3DPOOL_MANAGED
						)
					{
						length = length>1048576?1048576:length;
						device->CreateVertexBuffer(length*sizeof(Vertex),usage,Vertex::flexibleVectorFormat,pool,&(this->d3dVectorBuffer),0);
						this->length = length;
					}

					VertexBuffer(
						IDirect3DDevice9 *device,std::list<Vertex> vertexList,
						DWORD usage = D3DUSAGE_WRITEONLY,
						D3DPOOL pool = D3DPOOL_MANAGED
						)
					{
						this->length = (UINT)vertexList.size();
						device->CreateVertexBuffer((this->length)*sizeof(Vertex),usage,Vertex::flexibleVectorFormat,pool,&(this->d3dVectorBuffer),0);


						Vertex *vertexPointer = NULL;
						this->d3dVectorBuffer->Lock(0,0,(void **)&vertexPointer,0);

						UINT index = 0;
						for(std::list<Vertex>::iterator vertexListIterator = vertexList.begin();
							vertexListIterator != vertexList.end();vertexListIterator++
							)
						{
							*(vertexPointer+index) = *vertexListIterator;
							index++;
						}
						this->d3dVectorBuffer->Unlock();
					}

					UINT getLength()
					{
						return this->length;
					}

					Vertex *lock()
					{
						Vertex *vertexPointer = NULL;
						this->d3dVectorBuffer->Lock(0,0,(void **)&vertexPointer,0);
						return vertexPointer;
					}
					void unlock()
					{
						this->d3dVectorBuffer->Unlock();
					}

					IDirect3DVertexBuffer9 *getDirect3D9VertexBufferPointer()
					{
						return this->d3dVectorBuffer;
					}

				private:
					IDirect3DVertexBuffer9 *d3dVectorBuffer;
					UINT length;
				};

				template<typename IndexType>
				class IndexBuffer
				{
				public:
					IndexBuffer(
						IDirect3DDevice9 *device,UINT length,
						DWORD usage = D3DUSAGE_WRITEONLY,
						D3DFORMAT format = D3DFMT_INDEX32,
						D3DPOOL pool = D3DPOOL_MANAGED
						)
					{
						length = length>1048576?1048576:length;
						switch(sizeof(IndexType))
						{
						case 2:
							format = D3DFMT_INDEX16;
							device->CreateIndexBuffer(length*sizeof(IndexType),usage,format,pool,&(this->d3dIndexBuffer),0);
							break;
						case 4:
							format = D3DFMT_INDEX32;
							device->CreateIndexBuffer(length*sizeof(IndexType),usage,format,pool,&(this->d3dIndexBuffer),0);
							break;
						default:
							device->CreateIndexBuffer(length*sizeof(IndexType),usage,format,pool,&(this->d3dIndexBuffer),0);
							break;
						}
						this->length = length;
					}
					UINT getLength()
					{
						return this->length;
					}

					IndexType *lock()
					{
						IndexType *indexType;
						this->d3dIndexBuffer->Lock(0,0,(void **)&indexType,0);
						return indexType;
					}
					void unlock()
					{
						this->d3dIndexBuffer->Unlock();
					}

				private:
					IDirect3DIndexBuffer9 *d3dIndexBuffer;
					UINT length;
				};
			}
		}

		namespace vector
		{
			D3DXVECTOR3 rotate(D3DXVECTOR3 vector,D3DXVECTOR3 rotation);
		}

		namespace mesh
		{
			class MeshContainer
			{
				//如果考虑可以由Text创建MeshContainer
				//可以创建一个友元函数
			public:
				//将Mesh平移到原点
				//不做rotation和scale
				//bool toOrigin();
				//我现在觉得这个不需要
				//默认绘制第一个子集
				//是D3DXMesh的Decorator
				virtual HRESULT DrawSubset(DWORD attributeId = 0);
			protected:
				//创建一个空的MeshContainer
				MeshContainer(ID3DXMesh *d3dxMesh = NULL);
				//在析构函数中Release
				~MeshContainer();
				//设置Mesh  只能由友元类和友元函数访问
				//center取在外接长方体的体积中心
				//在这个类里不包含transform的信息

				//定义
				ID3DXMesh *d3dxMesh;
			};
			namespace text
			{
				struct TextVertex
				{
					float x,y,z;
					float nx,ny,nz;
					//DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL对应的顶点格式
					//之所以知道从Text中获取的是这样的顶点格式，
					//就是调试输出了18也就是0x12，所以就是D3DFVF_XYZ | D3DFVF_NORMAL
				};

				//应该保持MeshContainer的职能单一
				//关于文字类的，都在派生的类中处理
				class TextMesh:public Direct3D::v9::mesh::MeshContainer
				{
					//我也不知道这里是不是应该用继承
					//还是应该用装饰器模式
					//这是一个设计上的问题，但是我还是感觉更像是应该用继承
				private:

					//下面定义了三个center translation rotation 和scale三个参数
					D3DXVECTOR3 center,translation,rotation;
					//需要定义一个单独的Cube类来标识吗？
					//不，应该将scale和边长组合成一个内部类，统一处理
					class Scale
					{
					private:
						D3DXVECTOR3 scale,sideLength;
					public:
						Scale();//作为内部类必须有默认构造函数
						Scale(D3DXVECTOR3 sideLength,D3DXVECTOR3 scale = D3DXVECTOR3(1.0f,1.0f,1.0f));
						D3DXVECTOR3 setByScale(D3DXVECTOR3 scale);
						D3DXVECTOR3 setBySideLength(D3DXVECTOR3 sideLength);
						D3DXVECTOR3 getScale();
						D3DXVECTOR3 getSideLength();
					} scale;
				public:
					static int convertStringToWideChar(std::string toConvert,LPWSTR *result);
					static int convertMultiByteToWideChar(const char *toConvert,LPWSTR *result);

					TextMesh(IDirect3DDevice9 *device,std::string content,std::string font,int weight = 200);

					//HRESULT DrawSubset(DWORD attributeId = 0);//覆盖MeshContainer基类的DrawSubSet，以应用变换后再输出
				};
			}
		}

		namespace text
		{
			//用于优化文字渲染输出
			struct TextVertex
			{
				float x,y,z;
				float nx,ny,nz;
				//DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL对应的顶点格式
				//之所以知道从Text中获取的是这样的顶点格式，
				//就是调试输出了18也就是0x12，所以就是D3DFVF_XYZ | D3DFVF_NORMAL
			};

			class Text
			{
			public:
				Text(IDirect3DDevice9 *device,std::string content,std::string font,int weight = 200);
				~Text();

				ID3DXMesh *getMesh();//获得D3DX内置的mesh对象，如果有变换，会先应用变换
				ID3DXMesh *getMesh(D3DXVECTOR3 leftBottom,D3DXVECTOR3 rightTop);//根据两个角点确定平移和缩放，忽略旋转
				
				void setTranslation(D3DXVECTOR3 translation);//设置平移
				void setScale(D3DXVECTOR3 scale);//设置缩放
				void setRotation(D3DXVECTOR3 rotation);//设置旋转  暂时不实现，因为太麻烦了

				D3DXVECTOR3 getLeftBottom();
				D3DXVECTOR3 getRightTop();




				
				static int convertStringToWideChar(std::string toConvert,LPWSTR *result);
				static int convertMultiByteToWideChar(const char *toConvert,LPWSTR *result);
			private:
				void pretransform();

				ID3DXMesh *d3dMesh;
				D3DXVECTOR3 center,translation,rotation,scale;
				struct Bound__ {D3DXVECTOR3 leftBottom,rightTop;} bound;
			};
		}
	}
}