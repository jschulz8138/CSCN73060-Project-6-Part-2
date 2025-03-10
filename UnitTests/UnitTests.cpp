#include "pch.h"
#include "CppUnitTest.h"
#include "../Shared/PacketFactory.h"
#include "../Shared/Date.h"
#include "../Shared/TelemetryData.h"
#include "../Shared/Packet.h"
#include "../Shared/FuelType.h"
#include "../Shared/ProtocolFlag.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(DateUnitTests)
	{
	public:
		TEST_METHOD(ConstructorValidDates)
		{
			std::vector<const char*> dateString =			{ "3_3_2023 15:53:2",		"4_3_2023 15:53:2",		"3_7_2023 15:53:2", "3_3_2023 12:53:2", "3_3_2023 8:2:2",						"3_3_2023 15:3:2" };
			std::vector<std::string> expectedDateString =	{ "03_03_2023 15:53:02",	"04_03_2023 15:53:02", "03_07_2023 15:53:02", "03_03_2023 12:53:02", "03_03_2023 08:02:02",	"03_03_2023 15:03:02" };
			for (int i = 0; i < dateString.size(); i++)
				Assert::AreEqual(std::string(Date(dateString[i])), expectedDateString[i]);
		}
		TEST_METHOD(ConstructorInvalidDates)
		{
			std::vector<const char*> dateString = { "13_3333_2023 15:53:2", "4_344_2023 15:53:2", "3_733_2023 1544:53:2", "3_3_2023 12:5443:2", "3_333_20323 15:333:2" };
			std::vector<Date> dateVector;
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
		
		/*TEST_METHOD(ConstructorInvalidDate1)
		{
			const char* dateString = "12_12_2022 25:21:28";
			std::string expectedDate = "12_12_2022 25:21:28";
			Assert::ExpectException<std::invalid_argument>([&]() { Date date(dateString); });
		}
		TEST_METHOD(ConstructorInvalidDate2)
		{
			const char* dateString = "16_16_2022 22:21:28";
			std::string expectedDate = "16_16_2022 22:21:28";
			Assert::ExpectException<std::invalid_argument>([&]() { Date date(dateString); });
		}*/
	};
}
