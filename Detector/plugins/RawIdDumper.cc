#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/ESGetToken.h"

#include "DataFormats/Common/interface/Handle.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

class RawIdDumper : public edm::one::EDAnalyzer<edm::one::WatchRuns> {
public:
  explicit RawIdDumper(const edm::ParameterSet&);
  ~RawIdDumper() = default;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override {};
  void endRun(const edm::Run&, const edm::EventSetup&) override {};

  edm::ESGetToken<DTGeometry, MuonGeometryRecord> DTGeomToken_;
  edm::ESGetToken<CSCGeometry, MuonGeometryRecord> CSCGeomToken_;
  edm::ESGetToken<RPCGeometry, MuonGeometryRecord> RPCGeomToken_;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> GEMGeomToken_;
  
  const std::string outputFileName_;
    
  const std::string header_ = "det_raw_id,sub_det";
  const char delimeter_ = ',';
};

RawIdDumper::RawIdDumper(const edm::ParameterSet& pset):
  DTGeomToken_(esConsumes<DTGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  CSCGeomToken_(esConsumes<CSCGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  RPCGeomToken_(esConsumes<RPCGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  GEMGeomToken_(esConsumes<GEMGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  outputFileName_(pset.getUntrackedParameter<std::string>("outputFileName")) {}


void RawIdDumper::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setAllowAnything();
  descriptions.addWithDefaultLabel(desc);
}


void RawIdDumper::beginRun(const edm::Run& run, const edm::EventSetup& eventsetup) {
  const auto& DTGeom = eventsetup.getData(DTGeomToken_);
  const auto& CSCGeom = eventsetup.getData(CSCGeomToken_);
  const auto& RPCGeom = eventsetup.getData(RPCGeomToken_);
  const auto& GEMGeom = eventsetup.getData(GEMGeomToken_);

  std::vector<uint32_t> DTRawIds;
  std::vector<uint32_t> CSCRawIds;
  std::vector<uint32_t> RPCRawIds;
  std::vector<uint32_t> GEMRawIds;
  
  std::vector<uint32_t> muonsystemRawIds;
  std::vector<std::string> detType;


  for ( const auto DTDet : DTGeom.dets() ) {
    DTRawIds.push_back(DTDet->geographicalId().rawId());
    detType.push_back("DT");
  }

  for ( const auto CSCDet : CSCGeom.dets() ) {
    CSCRawIds.push_back(CSCDet->geographicalId().rawId());
    detType.push_back("CSC");
  }
  
  for ( const auto RPCDet : RPCGeom.dets() ) {
    RPCRawIds.push_back(RPCDet->geographicalId().rawId());
    detType.push_back("RPC");
  }

  for ( const auto GEMDet : GEMGeom.dets() ) {
    GEMRawIds.push_back(GEMDet->geographicalId().rawId());
    detType.push_back("GEM");
  }

  sort(DTRawIds.begin(), DTRawIds.end());
  sort(CSCRawIds.begin(), CSCRawIds.end());
  sort(RPCRawIds.begin(), RPCRawIds.end());
  sort(GEMRawIds.begin(), GEMRawIds.end());

  muonsystemRawIds.insert(muonsystemRawIds.end(), DTRawIds.begin(), DTRawIds.end());
  muonsystemRawIds.insert(muonsystemRawIds.end(), CSCRawIds.begin(), CSCRawIds.end());
  muonsystemRawIds.insert(muonsystemRawIds.end(), RPCRawIds.begin(), RPCRawIds.end());
  muonsystemRawIds.insert(muonsystemRawIds.end(), GEMRawIds.begin(), GEMRawIds.end());

  std::ofstream fout(outputFileName_);
  fout << header_ << std::endl;

  if (muonsystemRawIds.size() != detType.size()) return;
  for (size_t idx = 0; idx < muonsystemRawIds.size(); ++idx) {
    fout << muonsystemRawIds[idx] << delimeter_
         << detType[idx] << endl;
  } 
  return;

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RawIdDumper);