#include "../../application.h"
#include "threads.h"


#include "../../Message/message.h"
#include "../ParameterStore.h"

namespace Application
{
	namespace HUD
	{
		namespace Thread
		{
			void displayError(int handlerNumber,int errorCode,bool releaseCard = true);
			DWORD WINAPI receiveDataFrom1553B(LPVOID lpParam)
			{
				//0.初始化板卡
				unsigned short int deviceNumber, moduleNumber;
				/*
				puts("Default device number for single board when ExcConfig not used:\n");
				puts("M4k-1553MCH -> 25, PCI/MCH -> 29\n");

				printf("Enter the device number of the card you wish to use.\n");
				scanf("%hu", &deviceNumber);
				printf("Enter the module number.\n");
				scanf("%hu", &moduleNumber);
				printf("Calling Init_Module_Mch with device %hu module %hu\n", deviceNumber, moduleNumber);
				*/

				deviceNumber = 29;
				moduleNumber = 0;
				//这是固定的，不需要改动

				int errorCode;
				//char errorString[255];

				int handlerNumber = Init_Module_MCH(deviceNumber, moduleNumber);
				/*
				if (handlerNumber < 0)
				{
					//有错误发生  错误码就是上一个函数返回的负数
					errorCode = handlerNumber;
					Get_Error_String_MCH(errorCode, 200, errorString);
					//printf("Init_Module_MCH Failure. %s\n", errorString);
					//system("pause");
					//exit(0);		//在初始化时失败，故不需要释放资源
				}*/

				if(handlerNumber<0)
				{
					displayError(handlerNumber,handlerNumber,false);
				}

				//1.设置模式
				short int macroDefinedMode = RT_MODE_MCH;
				errorCode = Set_Mode_MCH(handlerNumber, macroDefinedMode);
				if (errorCode < 0)
				{
					/*
					Get_Error_String_MCH(errorCode, 200, errorString);
					//printf("%s\n", errorString);
					Release_Module_MCH(handlerNumber);
					system("pause");
					exit(0);
					*/
					displayError(handlerNumber,errorCode);
				}
				//todo
				//设置当前远程终端地址
				printf("请设置当前远程终端的地址\n");
				short int remoteTerminalAddress = 4;
				errorCode = Set_RT_Num_MCH(handlerNumber, remoteTerminalAddress);
				if (errorCode < 0)
				{
					/*
					Get_Error_String_MCH(errorCode, 200, errorString);
					//printf("%s\n", errorString);
					Release_Module_MCH(handlerNumber);
					system("pause");
					exit(0);
					*/
					displayError(handlerNumber,errorCode);
				}

				//设置子地址
				unsigned short int subAddress = 4;
				//这个子地址在主循环中还要使用

				//设置这个子地址产生接收中断
				errorCode = Set_Subaddr_Int_MCH(handlerNumber, subAddress, RECEIVE, STANDARD_CMD, ACCESS_AND_BROADCAST);
				if (errorCode < 0)
				{
					/*
					Get_Error_String_MCH(errorCode, 200, errorString);
					//printf("%s\n", errorString);
					Release_Module_MCH(handlerNumber);
					system("pause");
					exit(0);
					*/
					displayError(handlerNumber,errorCode);
				}

				//允许子地址产生中断
				/* Allow interrupts on sub address access. This is called once. */
				errorCode = Set_Interrupt_Mask_MCH(handlerNumber, SUBAD);
				if (errorCode < 0)
				{
					/*
					Get_Error_String_MCH(errorCode, 200, errorString);
					//printf("%s\n", errorString);
					Release_Module_MCH(handlerNumber);
					system("pause");
					exit(0);
					*/
					displayError(handlerNumber,errorCode);
				}

				errorCode = Run_RT_MCH(handlerNumber);
				if (errorCode < 0)
				{
					/*
					Get_Error_String_MCH(errorCode, 200, errorString);
					//printf("%s\n", errorString);
					Release_Module_MCH(handlerNumber);
					system("pause");
					exit(0);
					*/
					
					displayError(handlerNumber,errorCode);
				}

				short int dataBuffer[64] = {0};
				//设置完毕  进入主循环
				while (true)
				{
					Wait_For_Interrupt_MCH(handlerNumber, INFINITE);
					//Read_Datablk_MCH(handlerNumber, subAddress, 0, dataBuffer, STANDARD_CMD, RECEIVE);
					Read_Datablk_MCH(handlerNumber, subAddress, 0, (unsigned short int *)dataBuffer, STANDARD_CMD, RECEIVE);
					//BYTE i = 0;
					//for (i = 0; i < 32; i++) printf("%hu ", dataBuffer[i+2]);
					//printf("\n");

					//todo

					Application::HUD::ParameterStore::getInstance()->setPitch(((double)dataBuffer[2]/30000)*90);
					Application::HUD::ParameterStore::getInstance()->setRoll(((double)dataBuffer[3]/30000)*180);
				}
			}


			void displayError(int handlerNumber,int errorCode,bool releaseCard /* = true */)
			{
				//
				if (errorCode < 0)
				{
					char errorString[255];
					Get_Error_String_MCH(errorCode, 200, errorString);
					MessageBoxA(0,errorString,0,0);
					if(releaseCard) Release_Module_MCH(handlerNumber);
					//system("pause");
					//exit(0);
				}
			}
		}
	}
}