#include "stdafx.h"
#include "CppUnitTest.h"

#undef max
#undef min

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DACoreTest
{
	/*
		NOTE: don't 'Run All' as the core isn't thread safe yet.....
	*/

	static const char *TOWER_DATA_DIR = ".\\data\\towers\\";
	static const char *HISTORIAN_DATA_DIR = ".\\data\\historians\\";

	TEST_CLASS(AuxUnitTest)
	{
	private:
		static IManager*	mMgr;

	public:
		
		static void resetDataDir(){
			std::string current = boost::filesystem::current_path().string();

			if (boost::filesystem::exists(current + HISTORIAN_DATA_DIR)){
				boost::filesystem::remove_all(current + HISTORIAN_DATA_DIR);
				
			}
			else{
				boost::filesystem::create_directories(current + HISTORIAN_DATA_DIR);
			}

			if (boost::filesystem::exists(current + TOWER_DATA_DIR)){
				boost::filesystem::remove_all(current + TOWER_DATA_DIR);
				
			}
			else{
				boost::filesystem::create_directories(current + TOWER_DATA_DIR);
			}
		}

		static int countFilesInFolder(const char* folder){
			int count = 0;
			boost::filesystem::path dataDir(folder);
			boost::filesystem::directory_iterator end_iter;
			if (boost::filesystem::exists(folder) && boost::filesystem::is_directory(folder))
			{
				for (boost::filesystem::directory_iterator dir_iter(dataDir); dir_iter != end_iter; ++dir_iter)
				{
					if (boost::filesystem::is_regular_file(dir_iter->status()))
					{
						count++;
					}
				}
			}
			return count;
		}

		TEST_CLASS_INITIALIZE(DACoreInit){

			mMgr = getManager();

		}

		TEST_METHOD(SerializationTest1)		// this one creates a historian and then deletes it - checks that the json file is gone
		{
			//Logger::WriteMessage(boost::filesystem::current_path().c_str());

			resetDataDir();
			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 0);

			HistorianId hid1 = mMgr->createHistorian();

			mMgr->saveHistorian(hid1);

			mMgr->deleteHistorian(hid1);

			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 0);

		}

		TEST_METHOD(SerializationTest2)		// this one creates a historian with a tower, and 
											// then deletes the tower and historian - checks that the json files are gone
		{
			//Logger::WriteMessage(boost::filesystem::current_path().c_str());

			resetDataDir();
			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 0);

			HistorianId hid1 = mMgr->createHistorian();

			TowerId id1 = mMgr->createTower(hid1, ITower::Type::DTPRIME);
			mMgr->getTower(id1)->setName("Acid Tower");
			BOTTOMS_COMPOSITION(mMgr->getTower(id1))->setHeavyKey("Water");
			BOTTOMS_COMPOSITION(mMgr->getTower(id1))->setLightKey("Acid");

			mMgr->saveHistorian(hid1);
			mMgr->saveTower(id1);

			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 1);
			Assert::AreEqual(countFilesInFolder(TOWER_DATA_DIR), 1);

			mMgr->removeTowerFromHistorian(hid1, id1);
			mMgr->deleteTower(hid1, id1);
			mMgr->saveHistorian(hid1);
			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 1);
			Assert::AreEqual(countFilesInFolder(TOWER_DATA_DIR), 0);

			mMgr->deleteHistorian(hid1);

			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 0);
			Assert::AreEqual(countFilesInFolder(TOWER_DATA_DIR), 0);
		}

		TEST_METHOD(SerializationTestAddDeleteAll)
		{
			//Logger::WriteMessage(boost::filesystem::current_path().c_str());

			resetDataDir();

			// create 1 named (base) historian:
			HistorianId hid1 = mMgr->createHistorian();
			mMgr->getHistorian(hid1)->setType(IHistorian::Type::PI);

			// create 3 named DTPRIME towers:
			TowerId id1 = mMgr->createTower(hid1, ITower::Type::DTPRIME);
			mMgr->getTower(id1)->setName("Acid Tower");
			BOTTOMS_COMPOSITION(mMgr->getTower(id1))->setHeavyKey("Water");
			BOTTOMS_COMPOSITION(mMgr->getTower(id1))->setLightKey("Acid");

			TowerId id2 = mMgr->createTower(hid1, ITower::Type::DTPRIME);
			mMgr->getTower(id2)->setName("Benzene Tower");
			BOTTOMS_COMPOSITION(mMgr->getTower(id2))->setHeavyKey("Vodka");
			BOTTOMS_COMPOSITION(mMgr->getTower(id2))->setLightKey("Acid");

			TowerId id3 = mMgr->createTower(hid1, ITower::Type::DTPRIME);
			mMgr->getTower(id3)->setName("Butane Tower");
			BOTTOMS_COMPOSITION(mMgr->getTower(id3))->setHeavyKey("Gin");
			BOTTOMS_COMPOSITION(mMgr->getTower(id3))->setLightKey("Acid");

			mMgr->saveHistorians();
			mMgr->saveTowers();

			// now delete everything in the manager...
			mMgr->removeTowerFromHistorian(hid1, id1);
			mMgr->deleteTower(hid1, id1);

			mMgr->removeTowerFromHistorian(hid1, id2);
			mMgr->deleteTower(hid1, id2);

			mMgr->removeTowerFromHistorian(hid1, id3);
			mMgr->deleteTower(hid1, id3);

			mMgr->deleteHistorian(hid1);

			std::vector<TowerId> tids;
			mMgr->enumerateTowers(tids);
			Assert::AreEqual((int)tids.size(), 0);

			std::vector<HistorianId> hids;
			mMgr->enumerateHistorians(hids);
			Assert::AreEqual((int)hids.size(), 0);

		}

		TEST_METHOD(SerializationTest3)		// serialize an INVALID_DBL and read it back in - verify it is the same
		{
			//Logger::WriteMessage(boost::filesystem::current_path().c_str());

			resetDataDir();
			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 0);
			Assert::AreEqual(countFilesInFolder(TOWER_DATA_DIR), 0);

			HistorianId hid1 = mMgr->createHistorian();
			TowerId tid1 = mMgr->createTower(hid1, ITower::Type::DISTILLATION);

			Assert::AreEqual(countFilesInFolder(HISTORIAN_DATA_DIR), 1);
			Assert::AreEqual(countFilesInFolder(TOWER_DATA_DIR), 1);

			//at this point ITowerConfig.IControlLimits should contain INVALID_DBL's ... and should be written out to JSON...
			//note: INVALID_DBL = 1.797693134862316e+308
			Assert::AreEqual(mMgr->getTower(tid1)->getConfig()->getControlLimits()->getHeavyKeyMin(), IValidate::INVALID_DBL);

			// now lets reload the tower and see what comes back.....(id's should be the same)
			//mMgr->loadTowers();

			//double dVal = mMgr->getTower(tid1)->getConfig()->getControlLimits()->getHeavyKeyMin();

			//Assert::IsTrue(IValidate::isInvalidDbl(dVal));

			//dVal = dVal - 1.0;	// IValidate::INVALID_DBL-1 should be valid.....

			//Assert::IsFalse(IValidate::isInvalidDbl(dVal));


		}





		// LAST TEST - generates dummy JSON data for web app
		TEST_METHOD(SerializationTestGenerateHistorian)
		{
			//Logger::WriteMessage(boost::filesystem::current_path().c_str());

			resetDataDir();

			// create 1 named (base) historian:
			HistorianId hid1 = mMgr->createHistorian();
			mMgr->getHistorian(hid1)->setType(IHistorian::Type::PI);

			// create 3 named DTPRIME towers:
			TowerId id1 = mMgr->createTower(hid1, ITower::Type::DTPRIME);
			mMgr->getTower(id1)->setName("Acid Tower");
			BOTTOMS_COMPOSITION(mMgr->getTower(id1))->setHeavyKey("Water");
			BOTTOMS_COMPOSITION(mMgr->getTower(id1))->setLightKey("Acid");

			TowerId id2 = mMgr->createTower(hid1, ITower::Type::DTPRIME);
			mMgr->getTower(id2)->setName("Benzene Tower");
			BOTTOMS_COMPOSITION(mMgr->getTower(id2))->setHeavyKey("Vodka");
			BOTTOMS_COMPOSITION(mMgr->getTower(id2))->setLightKey("Acid");

			TowerId id3 = mMgr->createTower(hid1, ITower::Type::DTPRIME);
			mMgr->getTower(id3)->setName("Butane Tower");
			BOTTOMS_COMPOSITION(mMgr->getTower(id3))->setHeavyKey("Gin");
			BOTTOMS_COMPOSITION(mMgr->getTower(id3))->setLightKey("Acid");

			mMgr->saveTowers();

		}

	};
	IManager*	AuxUnitTest::mMgr;
}