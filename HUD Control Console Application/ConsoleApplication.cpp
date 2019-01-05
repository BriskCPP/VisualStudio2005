#include <cstdio>
#include <iostream>

#include "../Source/Application/Message/message.h"

#include <list>
#include <string>

int main(int argc, char **argv)
{
	std::list<UINT32> testDataList;
	testDataList.push_back(0xff);
	testDataList.push_back(0x80);

	using Application::Message::Util::split32bitDataListToString;
	using Application::Message::Util::charArrayTo32bitDataList;
	std::string testResult = split32bitDataListToString(testDataList);
	std::list<UINT32> resultDataList = charArrayTo32bitDataList(testResult.c_str(),testResult.size());

	for(std::list<UINT32>::iterator dataListIter= resultDataList.begin();
		dataListIter != resultDataList.end();dataListIter++
		)
	{
		std::cout<<*dataListIter<<"\t";
	}
}