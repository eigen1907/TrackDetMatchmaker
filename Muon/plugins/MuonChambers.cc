#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonChamberMatch.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include <vector>

using namespace std;

class MuonChambers : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit MuonChambers(const edm::ParameterSet&);
  ~MuonChambers() = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  edm::EDGetTokenT<reco::MuonCollection> muonsToken_;

  const std::string outputFileName_;  
  const std::string header_ = "muon_pt,muon_eta,muon_phi,track_pt,track_eta,track_phi,"
                              "rawId0,rawId1,rawId2,rawId3,rawId4,rawId5,rawId6,rawId7,"
                              "rawId8,rawId9,rawId10,rawId11,rawId12,rawId13,rawId14,rawId15,"
                              "rawId16,rawId17,rawId18,rawId19,rawId20,rawId21,rawId22,rawId23"
                              "rawId24,rawId25,rawId26,rawId27,rawId28,rawId29,rawId30,rawId31";
  const char delimeter_ = ',';
};


MuonChambers::MuonChambers(const edm::ParameterSet& iConfig): 
  muonsToken_(consumes<reco::MuonCollection>(iConfig.getUntrackedParameter<edm::InputTag>("muonsTag"))),
  outputFileName_(iConfig.getUntrackedParameter<std::string>("outputFileName")) {
}

void MuonChambers::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.addUntracked<edm::InputTag>("muonsTag", edm::InputTag("muons"));
  desc.addUntracked<std::string>("outputFileName", "output.csv");
  descriptions.add("MuonChambers", desc);
}

void MuonChambers::beginJob() {
  std::ofstream fout(outputFileName_);
  fout << header_ << std::endl;
}
void MuonChambers::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::ofstream fout(outputFileName_, std::ios::app);

  for (const auto& muon: iEvent.get(muonsToken_)) {    
    if (muon.isTrackerMuon()) {
      const auto innerTrack = muon.innerTrack();

      fout << muon.pt()            << delimeter_ 
           << muon.eta()           << delimeter_ 
           << muon.phi()           << delimeter_ 
           << innerTrack->pt()     << delimeter_ 
           << innerTrack->eta()    << delimeter_
           << innerTrack->phi()    << delimeter_;
        
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
  return;
}

DEFINE_FWK_MODULE(MuonChambers);
