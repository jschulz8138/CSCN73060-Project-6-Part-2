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
		TEST_METHOD(SendDataBoundaryTests)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 10, TelemetryData(Date("03_05_2022 15:44:2"), 25, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 12, TelemetryData(Date("09_06_2022 13:38:2"), 5555353, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 14, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 12, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));

			for (int i = 0; i < pkts.size(); i++) { Assert::AreEqual((int)ProtocolFlag::SENDDATA, (int)pkts[i]->getFlag()); }

			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_07_2022 14:27:2"), -3, GALLONS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_07_2022 14:27:2"), -100000, POUNDS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_07_2022 14:27:2"), -3043023, GALLONS)); });

		}
		TEST_METHOD(PacketFlagTests)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 10, TelemetryData(Date("03_05_2022 15:44:2"), 25, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 12, TelemetryData(Date("09_06_2022 13:38:2"), 5555353, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 14, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 12, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 14, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 12, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 14, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 12, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 14, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 12, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 12, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));

			std::vector<ProtocolFlag> flags = { GENERATEID, GENERATEID, GENERATEID, SENDDATA, SENDDATA, SENDDATA, ENDCOMMUNICATION, ENDCOMMUNICATION, ENDCOMMUNICATION, ACK, ACK, ACK };
			for (int i = 0; i < pkts.size(); i++) { Assert::AreEqual((int)flags[i], (int)pkts[i]->getFlag()); }
		}
		TEST_METHOD(PacketIdTests)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 10, TelemetryData(Date("03_05_2022 15:44:2"), 25, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 12, TelemetryData(Date("09_06_2022 13:38:2"), 5555353, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 14, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 15, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 16, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 17, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 18, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 19, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 20, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 21, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));

			//ACK packets will always have the ID be -1?
			std::vector<int> flags = { 10, 11, 12, 13, 14, 15, 16, 17, 18, -1, -1, -1 };
			for (int i = 0; i < pkts.size(); i++) { Assert::AreEqual(flags[i], pkts[i]->getId()); }
		}

		TEST_METHOD(PacketSizeTests)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 10, TelemetryData(Date("03_05_2022 15:44:2"), 25, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 12, TelemetryData(Date("09_06_2022 13:38:2"), 5555353, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 14, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 15, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 16, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 17, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 18, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 19, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 20, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 21, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));

			std::vector<int> flags = { 8, 8, 8, 43, 43, 43, 8, 8, 8, 4, 4, 4 };
			for (int i = 0; i < pkts.size(); i++) { Assert::AreEqual(flags[i], (int)pkts[i]->size()); }
		}
		TEST_METHOD(PacketTelemetryData)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 10, TelemetryData(Date("03_05_2022 15:44:2"), 25, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 12, TelemetryData(Date("09_06_2022 13:38:2"), 5555353, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 14, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 15, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 16, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 17, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 18, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 19, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 20, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 21, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));

			for (int i = 0; i < pkts.size(); i++) {  void* ptr = &pkts[i]->getTelemetryData(); Assert::IsNotNull(ptr);  }
		}
		TEST_METHOD(PacketValidateTests)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 10, TelemetryData(Date("03_05_2022 15:44:2"), 25, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 12, TelemetryData(Date("09_06_2022 13:38:2"), 5555353, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 14, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 15, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 16, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 17, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 18, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 19, TelemetryData(Date("03_03_2021 15:26:2"), 633443, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 20, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK, 21, TelemetryData(Date("03_03_2021 15:26:2"), 0, POUNDS)));

			for (int i = 0; i < pkts.size(); i++) { Assert::IsTrue(pkts[i]->validateData()); }

			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_0732fsd_2022 14:27:2"), -3, GALLONS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_07fsd_2022 14:27:2"), -3, GALLONS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("13_07_2022 14:27:2"), -3, GALLONS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_76_2022 14:27:2"), -3, GALLONS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_07_999022 14:27:2"), -3, GALLONS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_07_2022 66:27:2"), -3, GALLONS)); });
			Assert::ExpectException<std::invalid_argument>([&]() { PacketFactory::create(ProtocolFlag::SENDDATA, 13, TelemetryData(Date("08_07_2022 16:6657:2"), -3, GALLONS)); });
		}
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
