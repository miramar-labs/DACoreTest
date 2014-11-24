#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft{
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			template<>
			static std::wstring ToString<IAdvice::MessageType>(const IAdvice::MessageType  & mt) {
				switch (mt){
				case IAdvice::MessageType::INCREASE_REFLUX:return L"INCREASE_REFLUX";
				case IAdvice::MessageType::DECREASE_REFLUX:return L"DECREASE_REFLUX";
				case IAdvice::MessageType::INCREASE_REBOIL:return L"INCREASE_REBOIL";
				case IAdvice::MessageType::DECREASE_REBOIL:return L"DECREASE_REBOIL";
				case IAdvice::MessageType::DECREASE_BOTH_REBOIL_AND_REFLUX:return L"DECREASE_BOTH_REBOIL_AND_REFLUX";
				case IAdvice::MessageType::INCREASE_BOTH_REBOIL_AND_REFLUX:return L"INCREASE_BOTH_REBOIL_AND_REFLUX";
				case IAdvice::MessageType::INCREASE_PRESSURE:return L"INCREASE_PRESSURE";
				case IAdvice::MessageType::DECREASE_PRESSURE:return L"DECREASE_PRESSURE";
				default:
					return L"UNKNOWN IAdvice::MessageType";
				}
			}

			template<>
			static std::wstring ToString<IAdvice::Severity>(const IAdvice::Severity  & s) {
				switch (s){
				case IAdvice::Severity::RED:
					return L"RED";
				case IAdvice::Severity::YELLOW:
					return L"YELLOW";
				case IAdvice::Severity::GREEN:
					return L"GREEN";
				case IAdvice::Severity::CLEAR:
					return L"CLEAR";
				default:return L"UNKNOWN IAdvice::Severity";
				}
			}

		}
	}
}

namespace DACoreTest
{
	TEST_CLASS(AlgorithmUnitTest)
	{
	private:
		static IManager*	mMgr;
		static HistorianId	mHistPI;
		static TowerId		mTowerDTPRIME;

		//TODO - FIXME
		const std::string NO_ADVICE = "No advice at this time.";
		const std::string INCOMPLETE_CONFIGURATION = "The tower has an incomplete configuration.";
		const std::string TOWER_INACTIVE = "The tower is not active.";
		const std::string OVH_COMP_UNSTABLE = "Overhead composition unstable. No advice at this time.";
		const std::string BOTTOMS_COMP_UNSTABLE = "Bottoms composition unstable. No advice at this time.";
		const std::string OVH_AND_BOTTOMS_COMP_UNSTABLE = "Overhead and Bottoms compositions unstable. No advice at this time.";
		const std::string DECREASE_BOTH_REBOIL_AND_REFLUX = "Decrease the heating medium/Feed Ratio and Decrease Reflux Flow";
		const std::string INCREASE_BOTH_REBOIL_AND_REFLUX = "Increase heating medium FIC and Increase Reflux flow";
		const std::string DECREASE_REBOIL = "Decrease the heating medium/Feed Ratio";
		const std::string INCREASE_REFLUX = "Increase Reflux flow";
		const std::string DECREASE_REFLUX = "Decrease Reflux Flow";
		const std::string INCREASE_REBOIL = "Increase heating medium FIC";

		//TODO - FIXME
		const std::string ScenarioIncompleteID = "Incomplete";
		const std::string ScenarioInactiveID = "Inactive";
		const std::string ScenarioUnstableID = "Unstable";
		const std::string SecondaryAdviceForPressureID = "TODO";
		const std::string Scenario1ID = "1. High Impurities Both Ends; High Loading";
		const std::string Scenario2ID = "2. High Impurities Both Ends; Low or Normal Loading";
		const std::string Scenario3ID = "3. Impurities Below Minimum Both Ends; Sufficient Loading";
		const std::string Scenario4ID = "4. High Heavies In OH; Lights In Base Less Than 'Almost High'; Sufficient Loading";
		const std::string Scenario5ID = "5. High Heavies In OH; Lights In Base Less Than Max; Load Less Than Min; Or, Base Lights Greater Than Almost High And Load Ok";
		const std::string Scenario6ID = "6. High Lights In Base; Heavies In Overhead Less Than Almost High; Sufficient Loading";
		const std::string Scenario7ID = "7. High Lights In Base; Heavies In Overhead Less Than Max; Load Less Than Min; Or, Heavies in Overhead Greater Than Almost High And Load OK";
		const std::string Scenario8ID = "8. Heavies in Overhead Below Min; Lights in Base Above Min; Sufficient Loading";
		const std::string Scenario9ID = "9. Lights In Base Below Min; Heavies in Overhead Less Than Max; Sufficient Loading";
		const std::string ScenarioNoAdviceID = "No Advice";

	public:

		TEST_CLASS_INITIALIZE(DACoreInit){

			mMgr = getManager();

			mHistPI = mMgr->createHistorian();	// create a default PI historian
		}

		TEST_CLASS_CLEANUP(DACoreCleanup){

			mMgr->deleteHistorian(mHistPI);
		}

		TEST_METHOD_INITIALIZE(TestInit){
			mTowerDTPRIME = mMgr->createTower(mHistPI, ITower::DTPRIME);
		}

		TEST_METHOD_CLEANUP(TestCleanup){

			mMgr->removeTowerFromHistorian(mHistPI, mTowerDTPRIME);

			mMgr->deleteTower(mHistPI, mTowerDTPRIME);
		}

		TEST_METHOD(ScenarioIncomplete){

			CONFIG(mMgr->getTower(mTowerDTPRIME))->setIsIncomplete(true);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), INCOMPLETE_CONFIGURATION.c_str());

			delete advice;
		}

		TEST_METHOD(ScenarioInactive){

			CONFIG(mMgr->getTower(mTowerDTPRIME))->setIsInactive(true);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), TOWER_INACTIVE.c_str());

			delete advice;

		}

		TEST_METHOD(ScenarioOverheadCompUnstable){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(121.7);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.3);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), OVH_COMP_UNSTABLE.c_str());

			delete advice;

		}

		TEST_METHOD(ScenarioBottomsCompUnstable){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(92.15);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.10);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), BOTTOMS_COMP_UNSTABLE.c_str());

			delete advice;

		}

		TEST_METHOD(ScenarioBothCompUnstable){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(121.7);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.10);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), OVH_AND_BOTTOMS_COMP_UNSTABLE.c_str());

			delete advice;

		}

		TEST_METHOD(Scenario1){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(99.5);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setTowerDeltaPressureOrTemperature(3.48);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario1ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), DECREASE_BOTH_REBOIL_AND_REFLUX.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_BOTH_REBOIL_AND_REFLUX);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()==NULL);
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

			delete advice;
		}

		TEST_METHOD(Scenario2){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(98.5);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario2ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), INCREASE_BOTH_REBOIL_AND_REFLUX.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::INCREASE_BOTH_REBOIL_AND_REFLUX);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==);

			delete advice;
		}

		TEST_METHOD(Scenario3){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(93.17);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.3);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario3ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), DECREASE_BOTH_REBOIL_AND_REFLUX.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_BOTH_REBOIL_AND_REFLUX);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

			delete advice;
		}

		TEST_METHOD(Scenario4){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(98.5);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.6);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario4ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(),  DECREASE_REBOIL.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REBOIL);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

			delete advice;
		}

		TEST_METHOD(Scenario5){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(99.5);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.68);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setTowerDeltaPressureOrTemperature(1.9);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario5ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(),  INCREASE_REFLUX.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::INCREASE_REFLUX);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

			delete advice;
		}

		TEST_METHOD(Scenario6){//CHECKME - no params???

			Assert::Fail();	//TODO

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario6ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), DECREASE_REFLUX.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REFLUX);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

			delete advice;
		}

		TEST_METHOD(Scenario7){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setTowerDeltaPressureOrTemperature(1.1);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario7ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), INCREASE_REBOIL.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::INCREASE_REBOIL);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

			delete advice;
		}

		TEST_METHOD(Scenario8){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(92.15);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.70);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario8ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), DECREASE_REFLUX.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REFLUX);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

			delete advice;
		}

		TEST_METHOD(Scenario9a){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setHeavyKeyCurrent(95.7);
			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.3);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario9ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), DECREASE_REBOIL.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REBOIL);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

			delete advice;

		}

		TEST_METHOD(Scenario9b){

			BOTTOMS_COMPOSITION(mMgr->getTower(mTowerDTPRIME))->setReportMethod(IComposition::ReportMethod::ASSAY);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), Scenario9ID);

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), DECREASE_REBOIL.c_str());
			Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REBOIL);

			Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

			//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
			//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

			delete advice;

		}

		TEST_METHOD(ScenarioAdviceForPressure){
			Assert::Fail();	//TODO
		}

		TEST_METHOD(ScenarioNoAdvice){

			DATA(mMgr->getTower(mTowerDTPRIME))->getPrimaryInstruments()->setLightKeyCurrent(2.6);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getAlgorithmId(), ScenarioNoAdviceID);

			delete advice;
		}

	};
	IManager*	AlgorithmUnitTest::mMgr;
	HistorianId	AlgorithmUnitTest::mHistPI;
	TowerId		AlgorithmUnitTest::mTowerDTPRIME;
}