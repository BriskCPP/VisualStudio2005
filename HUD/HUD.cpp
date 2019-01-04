#include "../Source/Application/application.h"
#include "../Source/Direct3D9/Direct3D9.h"

#include <string>
#include <map>
#include <list>
#include <queue>

bool display(double totalTime,double deltaTime,IDirect3DDevice9 *device,HWND windowHandle);


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE previousDeprecatedInstanceHandle,LPSTR lpCmdLine,int nShowCommand)
{
	IDirect3DDevice9 *device;
	HWND windowHandle;
	if(!Direct3D::v9::initialization::initialize(
		hInstance, 
		(WNDPROC) Direct3D::v9::initialization::defaultWindowProc,
		800, 800, false, D3DDEVTYPE_HAL, &device,&windowHandle
		)
		)
	{
		MessageBoxA(0,"initial failed",0,0);
		return 0;
	}

	//定义摄像机
	Direct3D::v9::configuration::setStaticCamera(device,D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,1.0f),D3DX_PI*0.5f,600,600,0.0f,1048576.0f);
	//Direct3D::v9::configuration::setStaticCamera(device,D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DX_PI*0.5f,800,800,0.0f,1048576.0f);

	//设置线框模型
	//由于display中已经设置了按点渲染，所以不用设置线框
	//device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	//默认按面填充

	device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);//高洛德着色模式
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE); //全屏抗锯齿


	//现在启用了光照，因为有了3D字体
	//设置白光
	D3DCOLORVALUE colorValue;
	colorValue.a = 1.0f;
	colorValue.r = 1.0f;
	colorValue.g = 1.0f;
	colorValue.b = 1.0f;

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = colorValue;
	light.Diffuse   = colorValue;
	light.Specular  = colorValue;
	light.Direction = D3DXVECTOR3(0.0f,0.0f,1.0f);

	device->SetLight(0,&light);
	device->LightEnable(0,true);
	//在渲染中需要计算光照
	device->SetRenderState(D3DRS_LIGHTING,true);


	//启动子线程
	unsigned short int serverPort = 941;
	std::queue<std::list<UINT32>> messageQueue;

	std::map<std::string, void *> parameters;
	parameters["MessageQueue"] = (void *)&messageQueue;
	parameters["ServerPort"] = (void *)&serverPort;
	DWORD threadId;
	//HANDLE threadHandle1 = CreateThread(NULL, 0, Application::HUD::Thread::receiveMessageBySocket, (LPVOID *)(&parameters), 0, &threadId);



	//进入主循环
	Direct3D::v9::enterMessageLoop(display,device,windowHandle);

	//释放设备
	device->Release();
	return 0;
}


bool display(double totalTime,double deltaTime,IDirect3DDevice9 *device,HWND windowHandle)
{
	if(device)
	{
		//VertexBuffer应该是每一次显示单独创建的
		//因为图元的数量可能改变
		//但是应该有一个全局变量保存所有的DisplayComponent
		/////////////////////////////////////////////
		using Direct3D::v9::primitive::buffer::VertexBuffer;
		using Direct3D::v9::primitive::vertex::VertexWithColor;



		/*
		注释掉的这是最基本的用法
		VertexBuffer<struct VertexWithColor> vertexBuffer(device,3,D3DUSAGE_WRITEONLY);
		struct VertexWithColor *vertexPointer = vertexBuffer.lock();

		struct VertexWithColor tempVertex;

		tempVertex.x = -1;
		tempVertex.y = -1;
		tempVertex.z = 2;
		tempVertex.color = D3DCOLOR_XRGB(0,255,0);
		vertexPointer[0] = tempVertex;

		tempVertex.x = 0;
		tempVertex.y = 1;
		tempVertex.z = 2;
		vertexPointer[1] = tempVertex;

		tempVertex.x = 1;
		tempVertex.y = -1;
		tempVertex.z = 2;
		vertexPointer[2] = tempVertex;

		vertexBuffer.unlock();
		//////////////////////////////////
		*/

		/*
		struct VertexWithColor tempVertex;

		std::list<struct Direct3D::v9::primitive::vertex::VertexWithColor> vertexList;

		tempVertex.x = -50;
		tempVertex.y = 0;
		tempVertex.z = 1000;
		tempVertex.color = D3DCOLOR_XRGB(0,255,0);
		vertexList.push_back(tempVertex);


		tempVertex.x = 50;
		tempVertex.y = 0;
		tempVertex.z = 1000;
		vertexList.push_back(tempVertex);

		tempVertex.x = 0;
		tempVertex.y = -50;
		tempVertex.z = 1000;
		vertexList.push_back(tempVertex);

		tempVertex.x = 0;
		tempVertex.y = 50;
		tempVertex.z = 1000;
		vertexList.push_back(tempVertex);

		Application::DisplayComponent displayComponent(vertexList);


		displayComponent.setTranslation(D3DXVECTOR3(0,0,0));
		displayComponent.setRotation(D3DXVECTOR3(0,0,0));
		displayComponent.setScale(D3DXVECTOR3(1,1,1));
		*/

		Application::HUD::DisplayComponent::AttitudeIndicator a(-0.12,0.4);
		Direct3D::v9::text::Text text1(device,std::string("NWPU"),std::string("Microsoft YaHei"));


		//text1.setTranslation(D3DXVECTOR3(0,0,500.0f));
		//text1.setRotation(D3DXVECTOR3(0,0,D3DX_PI/6));
		//text1.setScale(D3DXVECTOR3(500.0f,500.0f,0.001f));


		///////////////////////////////////
		device->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0x80,0x80,0x80),1.0f,0
			);

		device->BeginScene();
		//开始绘制
		
		std::list<Application::DisplayComponent *> componentList;
		componentList.push_back(&a);


		VertexBuffer<struct VertexWithColor> vertexBuffer = Application::vertexBufferFactory::getVertexBuffer(device,componentList);
		//设置图元来源
		device->SetStreamSource(0,vertexBuffer.getDirect3D9VertexBufferPointer(),0,sizeof(struct VertexWithColor));
		//设置灵活顶点格式
		device->SetFVF(VertexWithColor::flexibleVectorFormat);
		device->DrawPrimitive(D3DPT_LINELIST,0,vertexBuffer.getLength());	//绘制线段数组
		//device->DrawPrimitive(D3DPT_POINTLIST,0,3);//按点绘制


		//设置材质
		//////////////////////////////////////////
		D3DCOLORVALUE colorValue;
		colorValue.a = 1.0f;
		colorValue.r = 0.0f;
		colorValue.g = 1.0f;
		colorValue.b = 0.0f;

		D3DMATERIAL9 material;
		material.Ambient = colorValue;
		material.Diffuse = colorValue;
		material.Specular = colorValue;

		colorValue.r = 0;
		colorValue.g = 0;
		colorValue.b = 0;

		material.Emissive = colorValue;
		material.Power = 2.0f;
		device->SetMaterial(&material);

		//绘制3D文本
		//下面的部分测试绘制3D字体

		//text1.getMesh()->DrawSubset(0);
		//text1.getMesh(D3DXVECTOR3(-500,0,500),D3DXVECTOR3(0,500,500.001f))->DrawSubset(0);
		text1.getMesh(D3DXVECTOR3(0,0,500),D3DXVECTOR3(500,500,500.001f))->DrawSubset(0);
		//textMesh->DrawSubset(0);

		//////////////////////////////////////////
		device->EndScene();

		//将后台缓冲区的内容投射上前台
		device->Present(0,0,0,0);

		//释放资源
		vertexBuffer.getDirect3D9VertexBufferPointer()->Release();
		//textMesh->Release();
	}
	return true;
}


//deprecated
//////////////////////////////////////////////
int convertMultiByteToWideChar(std::string toConvert,wchar_t **result)
{
	//还是应该用标准库std::string
	//对吧
	const char *charArray = toConvert.c_str();

	int resultLength = MultiByteToWideChar(CP_ACP,0,charArray,-1,NULL,0);
	*result = (wchar_t*)malloc( resultLength*sizeof(wchar_t) );
	MultiByteToWideChar(CP_ACP,0, charArray, -1, (LPWSTR)(*result), resultLength);
	free((void *)charArray);
	return resultLength;
}