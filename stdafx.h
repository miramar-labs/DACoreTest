// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

//BOOST
#include <boost/signals2.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/local_time/local_time.hpp>

// DACore :
#define UNIT_TESTING

#include "IManager.h"
#include "IOverrides.h"
#include "IFeeds.h"
#include "ITowers.h"
#include "IHistorian.h"
#include "ITower.h"
#include "ITowerData.h"
#include "IInstrumentsData.h"
#include "IComposition.h"
#include "IAdvice.h"
#include "ITowerConfig.h"
#include "IOverhead.h"
#include "IBottoms.h"
#include "IFeed.h"
#include "IHeatingMediumFeed.h"
#include "IExternalReflux.h"
#include "IReboiler.h"
#include "IValidate.h"
#include "IControlLimits.h"

using namespace dacore;


