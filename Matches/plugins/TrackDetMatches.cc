#include <memory>
#include <fstream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonChamberMatch.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "TTree.h"
#include <vector>

class TrackDetMatches : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit TrackDetMatches(const edm::ParameterSet&);
  ~TrackDetMatches() = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  edm::EDGetTokenT<reco::MuonCollection> muonsToken_;
  std::string outputFileName_;
  
  const std::string header_ = "pt,eta,phi,"
                              "rawId0,rawId1,rawId2,rawId3,rawId4,rawId5,rawId6,rawId7,"
                              "rawId8,rawId9,rawId10,rawId11,rawId12,rawId13,rawId14,"
                              "rawId15,rawId16,rawId17,rawId18,rawId19,rawId20,rawId21,"
                              "rawId22,rawId23,rawId24,rawId25,rawId26,rawId27,rawId28,"
                              "rawId29,rawId30,rawId31";
  const char delimeter_ = ',';
};


TrackDetMatches::TrackDetMatches(const edm::ParameterSet& iConfig): 
  muonsToken_(consumes<reco::MuonCollection>(iConfig.getUntrackedParameter<edm::InputTag>("muonsTag"))),
  outputFileName_(iConfig.getUntrackedParameter<std::string>("outputFileName")) {
}

void TrackDetMatches::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("muonsTag", edm::InputTag("muons"));
  desc.addUntracked<std::string>("outputFileName", "output.csv");
  descriptions.add("TrackDetMatches", desc);
}

void TrackDetMatches::beginJob() {
  std::ofstream fout(outputFileName_);
  fout << header_ << std::endl;
}

void TrackDetMatches::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::ofstream fout(outputFileName_, std::ios::app);

  for (const auto& muon : iEvent.get(muonsToken_)) {    
    if (muon.isTrackerMuon()) {
      const auto innerTrack = muon.innerTrack();

      fout << innerTrack->pt()  << delimeter_ 
           << innerTrack->eta() << delimeter_
           << innerTrack->phi() << delimeter_;
        
      const auto muonMatches = muon.matches();
      for (const auto& chamberMatch: muonMatches) {
        fout << chamberMatch.id.rawId() << delimeter_;
      }

      for (int i = 0; i < 32 - (int)muonMatches.size(); i++) {
        fout << 0 << delimeter_;
      }

      fout << std::endl;
    }
  }
}

DEFINE_FWK_MODULE(TrackDetMatches);
