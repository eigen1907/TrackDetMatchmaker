import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing('analysis')

options.register('outputFileName',
                 'file:output.csv', # default output name
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 'Output file name')

options.parseArguments()

process = cms.Process("TrackDetMatchesProducer")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

process.TrackDetMatches = cms.EDAnalyzer("TrackDetMatches",
    muonsTag = cms.untracked.InputTag("muons"),
    outputFileName = cms.untracked.string(options.outputFileName)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False),
    allowUnscheduled = cms.untracked.bool(True),
)

process.p = cms.Path(process.TrackDetMatches)
