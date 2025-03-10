#include "pch.h"
#include "CppUnitTest.h"
////#include "../Shared/GeneratePacket.h"
////#include "../Shared/SendPacket.h"
////#include "../Shared/EndPacket.h"
////#include "../Shared/AckPacket.h"
////#include "../Shared/PacketFactory.h"
#include "../Shared/Date.h"
//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//
namespace UnitTests
{
	//TEST_CLASS(PacketUnitTests)
	//{
	//public:

	//	TEST_METHOD(TestMethod1)
	//	{

	//	}
	//};
	TEST_CLASS(DateUnitTests)
	{
	public:

		TEST_METHOD(TestConstructorValidDate)
		{
			const char* dateString = "3_3_2023 14:53:2";
			Date date(dateString);
			std::string expectedDate = "03_03_2023 14:53:02";
			Assert::AreEqual(std::string(date), expectedDate);
		}
	};
	//TEST_CLASS(TelemetryDataUnitTests)
	//{
	//public:

	//	TEST_METHOD(TestMethod1)
	//	{

	//	}
	//};
}
