import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from TrackingTools.TrackAssociator.default_cfi import *

TrackAssociatorParameterBlock.TrackAssociatorParameters.preselectMuonTracks = True
TrackAssociatorParameterBlock.TrackAssociatorParameters.useGEM = True
TrackAssociatorParameters.useGEM = True

options = VarParsing('analysis')
options.register(
    'outputFileName',
    'output.csv',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    'Output CSV file name'
)
options.parseArguments()

process = cms.Process("TrackDetMatches")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(options.inputFiles))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff")
process.load("TrackingTools.Configuration.TrackingTools_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Geometry.ForwardGeometry.ForwardGeometry_cfi")
process.es_prefer_zdcHardcode = cms.ESPrefer("ZdcHardcodeGeometryEP","zdcHardcodeGeometryEP")
process.es_prefer_CastorHardcode = cms.ESPrefer("CastorHardcodeGeometryEP")

#process.load("Geometry.ForwardCommonData.castorGeometry_cff")

from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['phase1_2024_realistic']
#process.GlobalTag.globaltag = autoCond['run2_mc']
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2024_realistic', '')

process.TrackDetMatches = cms.EDAnalyzer("TrackDetMatches",
    TrackAssociatorParameterBlock,
    outputFileName = cms.untracked.string(options.outputFileName),
)


process.p = cms.Path(process.TrackDetMatches)

