#include "pch.h"
#include "CppUnitTest.h"
#include "../Shared/PacketFactory.h"
#include "../Shared/Date.h"
#include "../Shared/TelemetryData.h"
#include "../Shared/Packet.h"
#include "../Shared/FuelType.h"
#include "../Shared/ProtocolFlag.h"
#include "../Client/Plane.h"
#include <iostream>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{
	

	TEST_CLASS(PacketSerializeTests)
	{
	public:
		TEST_METHOD(SerializePacketsTests)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 10));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 12));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK));

			// Creating hardcoded expected vector<char> elements that should be whats getting generated by the SerializeData() function
			// the vector<char> that's generated from the SerializeData() function is what is sent over the network
			std::vector<std::vector<char>> expected;
			expected.push_back({ 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00 }); // GenerateID Serialized Packet structure

			expected.push_back({ 0x01, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
								0x32, 0x5F, 0x30, 0x34, 0x5F, 0x32, 0x30, 0x32, 0x35, 0x20, 0x31, 0x32, 0x3A, 0x33, 0x35, 0x3A, 0x30,
								0x36, 0x00, 0x00, 0x54, 0x42, 0x00, 0x00, 0x00, 0x00}); // SendData Serialized Packet structure

			expected.push_back({ 0x02, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00 }); // EndCommunication Serialized Packet structure

			expected.push_back({ 0x03, 0x00, 0x00, 0x00 }); // Ack Serialized Packet structure

			// Loop through the packets and compare serialized data
			for (int i = 0; i < pkts.size(); i++)
			{
				// Check if sizes of vectors match first (making sure full packet size is correct first)
				Assert::AreEqual(expected[i].size(), pkts[i]->SerializeData().size(), L"Sizes do not match.");
				Assert::IsTrue(expected[i] == pkts[i]->SerializeData());
			}
		}

		TEST_METHOD(DeserializePacketsTests)
		{
			std::vector<std::unique_ptr<Packet>> pkts;
			pkts.push_back(PacketFactory::create(ProtocolFlag::GENERATEID, 10));
			pkts.push_back(PacketFactory::create(ProtocolFlag::SENDDATA, 11, TelemetryData(Date("02_04_2025 12:35:6"), 53, GALLONS)));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, 12));
			pkts.push_back(PacketFactory::create(ProtocolFlag::ACK));

			// CSerializing the data and pushing it into a vector, to then be deserialized later into a new "recievedPkts" vector
			// and checked with the original "pkts" vector to make sure the packets we serialized and deserialized properly
			std::vector<std::vector<char>> serializedData;
			serializedData.push_back(pkts[0]->SerializeData());
			serializedData.push_back(pkts[1]->SerializeData());
			serializedData.push_back(pkts[2]->SerializeData());
			serializedData.push_back(pkts[3]->SerializeData());


			std::vector<std::unique_ptr<Packet>> receivedPkts;
			receivedPkts.push_back(PacketFactory::create(serializedData[0]));
			receivedPkts.push_back(PacketFactory::create(serializedData[1]));
			receivedPkts.push_back(PacketFactory::create(serializedData[2]));
			receivedPkts.push_back(PacketFactory::create(serializedData[3]));

			// Assert the "received" packets
			
			// GenerateID Packet
			Assert::IsTrue(pkts[0]->getFlag() == receivedPkts[0]->getFlag());
			Assert::IsTrue(pkts[0]->getId() == receivedPkts[0]->getId());

			// SendData Packet
			Assert::IsTrue(pkts[1]->getFlag() == receivedPkts[1]->getFlag());
			Assert::IsTrue(pkts[1]->getId() == receivedPkts[1]->getId());
			Assert::IsTrue(pkts[1]->getTelemetryData().size() == receivedPkts[1]->getTelemetryData().size());

			// EndCommunication Packet
			Assert::IsTrue(pkts[2]->getFlag() == receivedPkts[2]->getFlag());
			Assert::IsTrue(pkts[2]->getId() == receivedPkts[2]->getId());

			// Ack Packet
			Assert::IsTrue(pkts[3]->getFlag() == receivedPkts[3]->getFlag());
		}
	};

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

			for (int i = 0; i < pkts.size(); i++) {	Assert::IsTrue(pkts[i]->validateData());  }
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

	TEST_CLASS(PlaneUnitTests)
	{
	public:
		// Verifies if every file opens correctly
		TEST_METHOD(OpenFuelDataFile_ValidFiles)
		{
			// Setup
			Plane plane;
			bool result = false;

			// Run test
			// test all files that are registered in the Plane header
			for (int i = 0; i < NUM_OF_PLANE_FILES; i++)
			{
				result = plane.OpenFuelDataFile(plane[i]);

				// Verify
				Assert::IsTrue(result);

				// Clean-up
				plane.CloseFuelDataFile();
			}
		}

		// Verifies that files not listed are not openable
		TEST_METHOD(OpenFuelDataFile_InvalidFile)
		{
			// Setup
			Plane plane;
			bool result = true;

			// Run Test
			result = plane.OpenFuelDataFile("fakeTelemetryFile.txt");

			// Verify
			Assert::IsFalse(result);
		}

		// Verifies that a file reading returns valid readings
		// This does not ensure that it is reading it correctly
		TEST_METHOD(GetNextFuelData_ValidReading)
		{
			// Setup
			Plane plane;
			TelemetryData data;

			// Run Test
			// Iterate through every file to verify
			for (int i = 0; i < NUM_OF_PLANE_FILES; i++) {
				plane.OpenFuelDataFile(plane[i]);

				// Ensure Iterate through every line
				while (plane.GetNextFuelData()) {
					data = plane.GetTelemetry();
					// Assert
					// Ensure telemetry from file is valid
					Assert::IsTrue(data.validateTeletryData());
				}
			}
		}

		// Verifies that if a file is not opened it cannot be read
		TEST_METHOD(GetNextFuelData_NotRead)
		{
			// Setup
			Plane plane;
			bool result = true;

			// Run Test
			// try to read next line in unopened file
			result = plane.GetNextFuelData();

			// Assert
			Assert::IsFalse(result, L"Should fail when file is not open");
		}

		// Verify that if a file is closed data cannot be retrieved
		// AKA it's actually really closed
		TEST_METHOD(CloseFuelDataFile_CorrectClosing)
		{
			// Setup
			Plane plane;
			bool result;

			// Run test
			// Open a file then close it
			plane.OpenFuelDataFile(plane[0]);
			plane.CloseFuelDataFile();

			// Verify
			// Try to read file's next line.
			// It should fail
			result = plane.GetNextFuelData();
			Assert::IsFalse(result);
		}

		// Validates that the first and last telemetry data are parsed correctly
		TEST_METHOD(FullPlane_ValidateFirstLastAndCount)
		{
			// Setup
			Plane plane;
			plane.OpenFuelDataFile("katl-kefd-B737-700.txt");
			int expectedLineCount = 8565;	// expected number of lines for katl-kefd-B737-700.txt

			// First expected telemetry return
			std::string expectedFirstDate = "03_03_2023 14:53:21";
			float expectedFirstFuel = 4564.466309f;
			//TelemetryData expectedFirstTemetry(Date(expectedFirstDate), expectedFirstFuel, FuelType::GALLONS);

			// Last expected telemetry return
			std::string expectedLastDate = "03_03_2023 17:48:29";
			float expectedLastFuel = 2672.010254f;
			//TelemetryData expectedLastTelemetry(Date(expectedLastDate), expectedLastFuel, FuelType::GALLONS);

			// Run Test
			plane.GetNextFuelData();
			TelemetryData firstTelemetry = plane.GetTelemetry();

			// Verify first telemetry is correct
			std::string firstDateString = firstTelemetry.getDate();
			Assert::AreEqual(expectedFirstDate, firstDateString);
			Assert::AreEqual(expectedFirstFuel, firstTelemetry.getFuel());

			// iterate through all lines while counting them
			int lineCount = 1; //starts at 1 since th first line was already read
			TelemetryData lastTelemetry;
			while (plane.GetNextFuelData()) {
				lastTelemetry = plane.GetTelemetry();
				lineCount++;
			}

			// Verify first telemetry is correct
			std::string lastDateString = lastTelemetry.getDate();
			Assert::AreEqual(expectedLastDate, lastDateString);
			Assert::AreEqual(expectedLastFuel, lastTelemetry.getFuel());

			// Verify the correct amount of lines were read
			// 8565 is the number of lines
			Assert::AreEqual(expectedLineCount, lineCount);

			// Clean-up
			plane.CloseFuelDataFile();
		}
	};
}
