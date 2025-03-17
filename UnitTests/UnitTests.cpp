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
			//std::vector<ProtocolFlag> Flags = { GENERATEID };//, SENDDATA, ENDCOMMUNICATION, ACK, GENERATEID, SENDDATA, ENDCOMMUNICATION, ACK };
			//std::vector<int> Ids = { 0 };//, 1, 2, 3, 4, 5, 6, 7 };
			//std::vector<TelemetryData> Data = { 
				//TelemetryData(Date("03_05_2022 15:24:2"), 1, POUNDS), 
				//TelemetryData(Date("02_03_2022 05:53:2"), 2, GALLONS),
				//TelemetryData(Date("08_02_2023 03:32:2"), 3, POUNDS),
				//TelemetryData(Date("11_15_2024 06:11:2"), 4, GALLONS),
				//TelemetryData(Date("12_12_2023 17:21:2"), 5, POUNDS),
				//TelemetryData(Date("01_03_2022 20:02:2"), 6, GALLONS),
				//TelemetryData(Date("04_19_2023 21:34:2"), 7, GALLONS),
				//TelemetryData(Date("06_20_2021 15:48:2"), 8, GALLONS)
			//};
			//std::vector<Packet> Packets;
			//for (int i = 0; i < Flags.size(); i++) {
				//Packets.push_back(PacketFactory::create(Flags[i], Ids[i], Data[i]));
				//Assert::AreSame(PacketFactory::create(Flags[i], Ids[i], Data[i]).getFlag(), Flags[i]);
				//Assert::AreSame(PacketFactory::create(Flags[i], Ids[i], Data[i]).getId(), Ids[i]);
				//Assert::AreSame(PacketFactory::create(Flags[i], Ids[i], Data[i]).getTelemetryData(), Data[i]);
				//Assert::AreEqual(Packets[i].getFlag(), Flags[i]);
				//Assert::AreEqual(Packets[i].getId(), Ids[i]);
			//}
			//for (int i = 0; i < Flags.size(); i++) {
				//Assert::AreSame(PacketFactory::create(Flags[i], Ids[i], Data[i]).getFlag(), Flags[i]);
				//Assert::AreSame(PacketFactory::create(Flags[i], Ids[i], Data[i]).getId(), Ids[i]);
				//Assert::AreSame(PacketFactory::create(Flags[i], Ids[i], Data[i]).getTelemetryData(), Data[i]);
				//Assert::AreEqual(Packets[i].getFlag(), Flags[i]);
				//Assert::AreEqual(Packets[i].getId(), Ids[i]);
			//}
			Packet pkt = PacketFactory::create(SENDDATA, 10, TelemetryData(Date("03_05_2022 15:24:2"), 1, POUNDS));
			Assert::AreSame((int)SENDDATA, (int)pkt.getFlag());


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
