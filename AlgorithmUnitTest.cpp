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

			//TODO: cleanup mHistPI;
		}

		TEST_METHOD_INITIALIZE(TestInit){
			mTowerDTPRIME = mMgr->createTower(mHistPI, ITower::DTPRIME);
		}

		TEST_METHOD_CLEANUP(TestCleanup){

			mMgr->removeTowerFromHistorian(mHistPI, mTowerDTPRIME);

			mMgr->deleteTower(mHistPI, mTowerDTPRIME);
		}

		TEST_METHOD(TestScenarioIncomplete)
		{
			mMgr->getTower(mTowerDTPRIME)->getConfig()->setIsIncomplete(true);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), INCOMPLETE_CONFIGURATION.c_str());

			delete advice;
		}

		TEST_METHOD(TestScenarioInactive){

			mMgr->getTower(mTowerDTPRIME)->getConfig()->setIsInactive(true);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), TOWER_INACTIVE.c_str());

			delete advice;

		}

		TEST_METHOD(TestScenarioOverheadCompUnstable){

			IInstrumentsData* idata = mMgr->getTower(mTowerDTPRIME)->getData()->getPrimaryInstruments();

			idata->setHeavyKeyCurrent(121.7);

			idata->setLightKeyCurrent(2.3);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), OVH_COMP_UNSTABLE.c_str());

			delete advice;

		}

		TEST_METHOD(TestScenarioBottomsCompUnstable){

			IInstrumentsData* idata = mMgr->getTower(mTowerDTPRIME)->getData()->getPrimaryInstruments();

			idata->setHeavyKeyCurrent(92.15);

			idata->setLightKeyCurrent(2.10);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), BOTTOMS_COMP_UNSTABLE.c_str());

			delete advice;

		}

		TEST_METHOD(TestScenarioBothCompUnstable){

			IInstrumentsData* idata = mMgr->getTower(mTowerDTPRIME)->getData()->getPrimaryInstruments();

			idata->setHeavyKeyCurrent(121.7);

			idata->setLightKeyCurrent(2.10);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			Assert::AreEqual(advice->getPrimaryMessage().c_str(), OVH_AND_BOTTOMS_COMP_UNSTABLE.c_str());

			delete advice;

		}

		TEST_METHOD(TestScenario1){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setHeavyKeyCurrent(99.5);
			instruments->setTowerDeltaPressureOrTemperature(3.48);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario1ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Consider trying: Decrease the heating medium/Feed Ratio and Decrease Reflux Flow");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_BOTH_REBOIL_AND_REFLUX);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()==NULL);
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

				Logger::WriteMessage("**** PASS: testScenario1");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario1");

			delete advice;
		}

		TEST_METHOD(TestScenario2){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setHeavyKeyCurrent(98.5);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario2ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Increase heating medium FIC and Increase Reflux Flow");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::INCREASE_BOTH_REBOIL_AND_REFLUX);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==);

				Logger::WriteMessage("**** PASS: testScenario2");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario2");

			delete advice;
		}

		TEST_METHOD(TestScenario3){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setHeavyKeyCurrent(93.17);
			instruments->setLightKeyCurrent(2.3);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario3ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Decrease the heating medium/Feed Ratio and Decrease Reflux Flow");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_BOTH_REBOIL_AND_REFLUX);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

				Logger::WriteMessage("**** PASS: testScenario3");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario3");

			delete advice;
		}

		TEST_METHOD(TestScenario4){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setHeavyKeyCurrent(98.5);
			instruments->setLightKeyCurrent(2.6);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario4ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Decrease the heating medium/Feed Ratio");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REBOIL);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

				Logger::WriteMessage("**** PASS: testScenario4");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario4");

			delete advice;
		}

		TEST_METHOD(TestScenario5){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setHeavyKeyCurrent(99.5);
			instruments->setLightKeyCurrent(2.68);
			instruments->setTowerDeltaPressureOrTemperature(1.9);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario5ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Increase Reflux Flow");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::INCREASE_REFLUX);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

				Logger::WriteMessage("**** PASS: testScenario5");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario5");

			delete advice;
		}

		TEST_METHOD(TestScenario6){

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario6ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Decrease Reflux Flow");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REFLUX);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

				Logger::WriteMessage("**** PASS: testScenario6");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario6");

			delete advice;
		}

		TEST_METHOD(TestScenario7){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setTowerDeltaPressureOrTemperature(1.1);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario7ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Increase heating medium FIC");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::INCREASE_REBOIL);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::);

				Logger::WriteMessage("**** PASS: testScenario7");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario7");

			delete advice;
		}

		TEST_METHOD(TestScenario8){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setHeavyKeyCurrent(92.15);
			instruments->setLightKeyCurrent(2.70);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario8ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Decrease Reflux Flow");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REFLUX);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

				Logger::WriteMessage("**** PASS: testScenario8");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario8");

			delete advice;
		}

		TEST_METHOD(TestScenario9a){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setHeavyKeyCurrent(95.7);
			instruments->setLightKeyCurrent(2.3);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario9ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Decrease the heating medium/Feed Ratio");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REBOIL);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

				Logger::WriteMessage("**** PASS: testScenario9a");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario9a");

			delete advice;

		}

		TEST_METHOD(TestScenario9b){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			mMgr->getTower(mTowerDTPRIME)->getConfig()->getBottoms()->getComposition()->setReportMethod(IComposition::ReportMethod::ASSAY);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == Scenario9ID){

				Assert::AreEqual(advice->getPrimaryMessage().c_str(),  "Decrease the heating medium/Feed Ratio");
				Assert::AreEqual(advice->getPrimaryMessageType(), IAdvice::MessageType::DECREASE_REBOIL);

				Assert::AreEqual(advice->getSeverity(), IAdvice::Severity::RED);

				//TODO Assert::AreEqual(advice->getSecondaryMessage()=="ERROR- Delta Pressure is reading too low for current load on column, negatively impacting some decisions.");
				//TODO Assert::AreEqual(advice->getSecondaryMessageType()==IAdvice::MessageType::INVALID);

				Logger::WriteMessage("**** PASS: testScenario9b");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenario9b");

			delete advice;

		}

		TEST_METHOD(TestScenarioAdviceForPressure){

		}

		TEST_METHOD(TestScenarioNoAdvice){

			ITowerData* tdata = mMgr->getTower(mTowerDTPRIME)->getData();

			IInstrumentsData* instruments = tdata->getPrimaryInstruments();
			instruments->setLightKeyCurrent(2.6);

			IAdvice* advice = mMgr->getAdvice(mTowerDTPRIME);

			Logger::WriteMessage(advice->getPrimaryMessage().c_str());

			if (advice->getAlgorithmId() == ScenarioNoAdviceID){

				Logger::WriteMessage("**** PASS: testScenarioNoAdvice");
			}
			else
				Logger::WriteMessage("**** FAIL: testScenarioNoAdvice");

			delete advice;
		}

	};
	IManager*	AlgorithmUnitTest::mMgr;
	HistorianId	AlgorithmUnitTest::mHistPI;
	TowerId		AlgorithmUnitTest::mTowerDTPRIME;
}