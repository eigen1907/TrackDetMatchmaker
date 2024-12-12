import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing('analysis')

# 기본값 설정 (필요하다면 수정)
options.outputFile = 'output.csv'
options.maxEvents = -1

options.parseArguments()

process = cms.Process("TrackDetMatchesProducer")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

process.TrackDetMatches = cms.EDAnalyzer("TrackDetMatches",
    muonsTag = cms.untracked.InputTag("muons"),
    outputFileName = cms.untracked.string(options.outputFile)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False),
    allowUnscheduled = cms.untracked.bool(True),
)

process.p = cms.Path(process.TrackDetMatches)
