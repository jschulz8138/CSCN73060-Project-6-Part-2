#include "pch.h"
#include "CppUnitTest.h"
#include "../Shared/PacketFactory.h"
#include "../Shared/Date.h"
#include "../Shared/TelemetryData.h"
#include "../Shared/Packet.h"
#include "../Shared/FuelType.h"
#include "../Shared/ProtocolFlag.h"
#include <iostream>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(PacketFactoryTests)
	{
	public:
		TEST_METHOD(CreateVariant1Tests)
		{
			std::unique_ptr<Packet> pkt = PacketFactory::create(ProtocolFlag::SENDDATA, 10, TelemetryData(Date("03_05_2022 15:24:2"), 1, POUNDS));
			Assert::AreEqual((int)ProtocolFlag::SENDDATA, (int)pkt->getFlag());
		}
		//TEST_METHOD(CreateVariant2Tests)
		//{

		//}
	};

	TEST_CLASS(DateUnitTests)
	{
	public:
		TEST_METHOD(ConstructorValidDates)
		{
			std::vector<const char*> dateString =			{ "03_03_2023 15:53:2",		"4_15_2023 15:53:2",	   "3_7_2023 15:53:2",    "3_3_2023 12:53:2",    "3_3_2023 8:2:2",		"3_3_2023 15:3:2" };
			std::vector<std::string> expectedDateString =	{ "03_03_2023 15:53:02",	"04_15_2023 15:53:02", "03_07_2023 15:53:02", "03_03_2023 12:53:02", "03_03_2023 08:02:02",	"03_03_2023 15:03:02" };
			for (int i = 0; i < dateString.size(); i++)
				Assert::AreEqual(expectedDateString[i], std::string(Date(dateString[i])));
		}
		TEST_METHOD(ConstructorInvalidDates)
		{
			std::vector<const char*> dateString = { "13_3333_2023 15:53:2", "4_344_2023 15:53:2", "3_733_2023 1544:53:2", "3_3_2023 12:5443:2", "3_333_20323 15:333:2" };
			for (int i = 0; i < dateString.size(); i++)
				Assert::ExpectException<std::invalid_argument>([&]() { Date date(dateString[i]); });
		}
		TEST_METHOD(ConstructorSpecialCase1)
		{
			const char* dateString = "3_3_2023 8:2:339999332";
			std::string expected = "03_03_2023 08:02:33";
			Assert::AreEqual(std::string(Date(dateString)), expected);
		}
		TEST_METHOD(ConstructorSpecialCase2)
		{
			const char* dateString = "3_3_2023 8:2:33doesthiswork:)hiseeyan9999332";
			std::string expected = "03_03_2023 08:02:33";
			Assert::AreEqual(std::string(Date(dateString)), expected);
		}
		TEST_METHOD(DefaultConstructor)
		{
			std::string expected = "01_01_1900 00:00:00";
			Assert::AreEqual(std::string(Date()), expected);
		}
		TEST_METHOD(SizeTests)
		{
			std::vector<const char*> dateString = { "03_03_2023 15:53:2",		"4_15_2023 15:53:2",	   "3_7_2023 15:53:2",    "3_3_2023 12:53:2",    "3_3_2023 8:2:2",		"3_3_2023 15:3:2" };
			size_t ExpectedSize = 19;
			for (int i = 0; i < dateString.size(); i++) 
				Assert::AreEqual( Date(dateString[i]).size(), ExpectedSize);
		}
		TEST_METHOD(ValidateDataTests)
		{
			std::vector<const char*> dateString = { "03_03_2023 15:53:2",		"4_15_2023 15:53:2",	   "3_7_2023 15:53:2",    "3_3_2023 12:53:2",    "3_3_2023 8:2:2",		"3_3_2023 15:3:2" };
			for (int i = 0; i < dateString.size(); i++)
				Assert::AreEqual(Date(dateString[i]).validateDate(), true);
		}
	};
}
