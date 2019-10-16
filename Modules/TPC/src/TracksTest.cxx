// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   TracksTest.cxx
/// \author Stefan Heckel
///

// root includes
#include <TCanvas.h>
#include <TH1.h>

// O2 includes
#include "Framework/ProcessingContext.h"

// QC includes
#include "QualityControl/QcInfoLogger.h"
#include "TPC/TracksTest.h"

namespace o2::quality_control_modules::tpc
{

TracksTest::TracksTest() : TaskInterface() {}

TracksTest::~TracksTest()
{
}

void TracksTest::initialize(o2::framework::InitContext& /*ctx*/)
{
  QcInfoLogger::GetInstance() << "initialize TPC TracksTest QC task" << AliceO2::InfoLogger::InfoLogger::endm;

  mQCTracksTest.initializeHistograms();

  for (auto& hist : mQCTracksTest.getHistograms1D()) {
    getObjectsManager()->startPublishing(&hist);
    getObjectsManager()->addMetadata(hist.GetName(), "custom", "34");
  }
}

void TracksTest::startOfActivity(Activity& /*activity*/)
{
  QcInfoLogger::GetInstance() << "startOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
  mQCTracksTest.resetHistograms();
}

void TracksTest::startOfCycle()
{
  QcInfoLogger::GetInstance() << "startOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void TracksTest::monitorData(o2::framework::ProcessingContext& ctx)
{
  using TrackType = std::vector<o2::tpc::TrackTPC>;
  auto tracks = ctx.inputs().get<TrackType>("tpc-sampled-tracks");
  QcInfoLogger::GetInstance() << "monitorData: " << tracks.size() << AliceO2::InfoLogger::InfoLogger::endm;

  for (auto const& track : tracks) {
    mQCTracksTest.processTrack(track);
    //const auto p = track.getP();
    //const auto dEdx = track.getdEdx().dEdxTotTPC;
    //printf("p: dEdx = %.2f: %.2f\n", p, dEdx);
  }
}

void TracksTest::endOfCycle()
{
  QcInfoLogger::GetInstance() << "endOfCycle" << AliceO2::InfoLogger::InfoLogger::endm;
}

void TracksTest::endOfActivity(Activity& /*activity*/)
{
  QcInfoLogger::GetInstance() << "endOfActivity" << AliceO2::InfoLogger::InfoLogger::endm;
}

void TracksTest::reset()
{
  // clean all the monitor objects here

  QcInfoLogger::GetInstance() << "Resetting the histogram" << AliceO2::InfoLogger::InfoLogger::endm;
  mQCTracksTest.resetHistograms();
}

} // namespace o2::quality_control_modules::tpc
