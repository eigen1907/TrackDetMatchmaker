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

  edm::ESGetToken<DTGeometry, MuonGeometryRecord> m_dt_geom_token;
  edm::ESGetToken<CSCGeometry, MuonGeometryRecord> m_csc_geom_token;
  edm::ESGetToken<RPCGeometry, MuonGeometryRecord> m_rpc_geom_token;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> m_gem_geom_token;
  
  const std::string m_output_file_name;
    
  const std::string m_header = "raw_id,sub_det";
  const char m_delimeter = ',';
};

RawIdDumper::RawIdDumper(const edm::ParameterSet& pset):
  m_dt_geom_token(esConsumes<DTGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  m_csc_geom_token(esConsumes<CSCGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  m_rpc_geom_token(esConsumes<RPCGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  m_gem_geom_token(esConsumes<GEMGeometry, MuonGeometryRecord, edm::Transition::BeginRun>()),
  m_output_file_name(pset.getUntrackedParameter<std::string>("outputFileName")) {}


void RawIdDumper::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setAllowAnything();
  descriptions.addWithDefaultLabel(desc);
}


void RawIdDumper::beginRun(const edm::Run& run, const edm::EventSetup& eventsetup) {
  const auto& dtGeom = eventsetup.getData(m_dt_geom_token);
  const auto& cscGeom = eventsetup.getData(m_csc_geom_token);
  const auto& rpcGeom = eventsetup.getData(m_rpc_geom_token);
  const auto& gemGeom = eventsetup.getData(m_gem_geom_token);

  std::vector<uint32_t> dt_ids;
  std::vector<uint32_t> csc_ids;
  std::vector<uint32_t> rpc_ids;
  std::vector<uint32_t> gem_ids;
  
  std::vector<uint32_t> all_ids;
  std::vector<std::string> subDets;


  for ( const DTLayer* dtLayer : dtGeom.layers() ) {
    dt_ids.push_back(dtLayer->geographicalId().rawId());
    subDets.push_back("DT");
  }

  for ( const CSCLayer* cscLayer : cscGeom.layers() ) {
    csc_ids.push_back(cscLayer->geographicalId().rawId());
    subDets.push_back("CSC");
  }
  
  for ( const RPCRoll* rpcRoll : rpcGeom.rolls() ) {
    rpc_ids.push_back(rpcRoll->geographicalId().rawId());
    subDets.push_back("RPC");
  }

  for ( const GEMEtaPartition* gemEtaPartition : gemGeom.etaPartitions() ) {
    gem_ids.push_back(gemEtaPartition->geographicalId().rawId());
    subDets.push_back("GEM");
  }

  sort(dt_ids.begin(), dt_ids.end());
  sort(csc_ids.begin(), csc_ids.end());
  sort(rpc_ids.begin(), rpc_ids.end());
  sort(gem_ids.begin(), gem_ids.end());

  all_ids.insert(all_ids.end(), dt_ids.begin(), dt_ids.end());
  all_ids.insert(all_ids.end(), csc_ids.begin(), csc_ids.end());
  all_ids.insert(all_ids.end(), rpc_ids.begin(), rpc_ids.end());
  all_ids.insert(all_ids.end(), gem_ids.begin(), gem_ids.end());

  std::ofstream fout(m_output_file_name);
  fout << m_header << std::endl;

  if (all_ids.size() != subDets.size()) return;
  for (size_t idx = 0; idx < all_ids.size(); ++idx) {
    fout << all_ids[idx] << m_delimeter
         << subDets[idx] << endl;
  } 
  return;

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RawIdDumper);