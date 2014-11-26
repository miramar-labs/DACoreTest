#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DACoreTest
{
	TEST_CLASS(AuxUnitTest)
	{
	private:
		static IManager*	mMgr;

	public:
		
		static void resetDataDir(){
			std::string current = boost::filesystem::current_path().string();

			boost::filesystem::remove_all(current + "\\data\\historians\\");
			boost::filesystem::create_directories(current + "\\data\\historians\\");

			boost::filesystem::remove_all(current + "\\data\\towers\\");
			boost::filesystem::create_directories(current + "\\data\\towers\\");
		}

		TEST_CLASS_INITIALIZE(DACoreInit){

			mMgr = getManager();

		}

		TEST_METHOD(SerializationTest)
		{
			Logger::WriteMessage(boost::filesystem::current_path().c_str());

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

			// now load them back in from json....
			mMgr->loadHistorians();
			mMgr->loadTowers();

			mMgr->enumerateHistorians(hids);
			Assert::AreEqual((int)hids.size(), 1);

			mMgr->enumerateTowers(tids);
			Assert::AreEqual((int)tids.size(), 3);
		}

	};
	IManager*	AuxUnitTest::mMgr;
}