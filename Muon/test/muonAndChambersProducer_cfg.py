import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonChamberMatchProcess")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1) )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("file:step3.root")
)

process.MuonChambers = cms.EDAnalyzer("MuonChambers")

process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False),
    allowUnscheduled = cms.untracked.bool(True),
)

process.p = cms.Path(process.MuonChambers)
