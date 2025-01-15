#include <memory>
#include <fstream>
#include <vector>
#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackToTrackMap.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetMatchInfo.h"

#include "TrackingTools/TrackAssociator/interface/DetIdAssociator.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "TrackingTools/Records/interface/DetIdAssociatorRecord.h"

class TrackDetMatches : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit TrackDetMatches(const edm::ParameterSet&);
  ~TrackDetMatches() override {}
  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  const edm::EDGetTokenT<reco::MuonCollection> muonsToken_;
  const edm::ESGetToken<Propagator, TrackingComponentsRecord> propagatorToken_;
  const edm::ESGetToken<DetIdAssociator, DetIdAssociatorRecord> muonDetIdAssocToken_;
  
  std::string outputFileName_;
  TrackDetectorAssociator trackAssociator_;
  TrackAssociatorParameters parameters_;

  std::string header_ = "track_pt,track_eta,track_phi,match_raw_id,assos_raw_id,candi_raw_id";
  char delimeter_ = ',';
  char subDelimeter_ = ' ';
};

TrackDetMatches::TrackDetMatches(const edm::ParameterSet& iConfig)
    : muonsToken_(consumes<reco::MuonCollection>(iConfig.getUntrackedParameter<edm::InputTag>("muonsTag"))),
      propagatorToken_(esConsumes(edm::ESInputTag("", "SteppingHelixPropagatorAny"))),
      muonDetIdAssocToken_(esConsumes(edm::ESInputTag("", "MuonDetIdAssociator"))),
      outputFileName_(iConfig.getUntrackedParameter<std::string>("outputFileName")) {
  const edm::ParameterSet parameters = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
  edm::ConsumesCollector iC = consumesCollector();
  parameters_.loadParameters(parameters, iC);
}

void TrackDetMatches::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("muonsTag", edm::InputTag("muons"));
  desc.addUntracked<std::string>("outputFileName", "output.csv");

  edm::ParameterSetDescription descTrkAsoPar;
  descTrkAsoPar.add<edm::InputTag>("GEMSegmentCollectionLabel", edm::InputTag("gemSegments"));
  descTrkAsoPar.add<edm::InputTag>("ME0SegmentCollectionLabel", edm::InputTag("me0Segments"));
  descTrkAsoPar.add<bool>("useGEM", true);
  descTrkAsoPar.add<bool>("useME0", false);
  descTrkAsoPar.add<bool>("preselectMuonTracks", true);
  descTrkAsoPar.add<edm::InputTag>("RPCHitCollectionLabel", edm::InputTag("rpcRecHits"));
  descTrkAsoPar.add<edm::InputTag>("GEMHitCollectionLabel", edm::InputTag("gemRecHits"));
  descTrkAsoPar.add<edm::InputTag>("ME0HitCollectionLabel", edm::InputTag("me0RecHits"));
  descTrkAsoPar.setAllowAnything();
  desc.add<edm::ParameterSetDescription>("TrackAssociatorParameters", descTrkAsoPar);

  descriptions.add("TrackDetMatches", desc);
}

void TrackDetMatches::beginJob() {
  std::ofstream fout(outputFileName_);
  fout << header_ << std::endl;
}

void TrackDetMatches::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::ofstream fout(outputFileName_, std::ios::app);
  const auto& muons = iEvent.get(muonsToken_);
  auto const& muonDetIdAssociator = iSetup.getData(muonDetIdAssocToken_);
  trackAssociator_.setPropagator(&iSetup.getData(propagatorToken_));

  for (const auto& muon : muons) {
    if (!muon.innerTrack()) continue;

    auto info = trackAssociator_.associate(iEvent, iSetup, *muon.innerTrack(), parameters_);

    fout << muon.innerTrack()->pt() << delimeter_
         << muon.innerTrack()->eta() << delimeter_
         << muon.innerTrack()->phi() << delimeter_;

    const auto muonMatches = muon.matches();
    for (const auto& cm : muonMatches) fout << cm.id.rawId() << subDelimeter_;
    fout << delimeter_;

    for (const auto& ch : info.chambers) fout << ch.id.rawId() << subDelimeter_;
    fout << delimeter_;

    auto& cachedTraj = trackAssociator_.getCachedTrajector();
    auto stateAtHcal = cachedTraj.getStateAtHcal();
    if (!stateAtHcal.isValid()) stateAtHcal = cachedTraj.getOuterState();
    if (!stateAtHcal.isValid()) {
      fout << std::endl;
      continue;
    }

    auto delta = cachedTraj.trajectoryDelta(CachedTrajectory::FullTrajectory);
    auto mapRange = trackAssociator_.getMapRange(delta, parameters_.dRMuonPreselection);
    auto candiMuonChamberIds = muonDetIdAssociator.getDetIdsCloseToAPoint(stateAtHcal.position(), mapRange);

    for (auto did : candiMuonChamberIds) {
      fout << did.rawId() << subDelimeter_;
    }  
    fout << std::endl;
  }
}

DEFINE_FWK_MODULE(TrackDetMatches);
