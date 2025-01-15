import FWCore.ParameterSet.Config as cms


from Configuration.Eras.Era_Run3_2024_cff import Run3_2024
process = cms.Process("RawIdDumpProcess", Run3_2024)

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')

######################################################################################################
### From autoCond at https://github.com/cms-sw/cmssw/blob/master/Configuration/AlCa/python/autoCond.py
######################################################################################################
#process.load("Configuration.StandardSequences.GeometryDB_cff")
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#from Configuration.AlCa.autoCond import autoCond
#process.GlobalTag.globaltag = autoCond['phase1_2024_realistic']
#process.GlobalTag.globaltag = autoCond['run3_data']

######################################################################################################
### From specific global tag at https://cms-conddb.cern.ch/cmsDbBrowser/index/Prod
######################################################################################################
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2024_realistic', '')


######################################################################################################
### From specific xml file at https://github.com/cms-sw/cmssw/tree/master/Configuration/Geometry/python
######################################################################################################
#process.load('Configuration.Geometry.GeometryExtended2024Reco_cff')
#process.load('Configuration.Geometry.GeometryDD4hepExtended2024Reco_cff')

######################################################################################################
### The source data defines the time zone of the Geometry that the tag will load.
### Consider the payload of the tag (e.g. https://cms-conddb.cern.ch/cmsDbBrowser/list/Prod/tags/RecoIdealGeometry_RPC_v3_hlt)
######################################################################################################
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:step2.root') ## if you want to use local root file
    #fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/data/Run2024D/Muon0/AOD/PromptReco-v1/000/380/517/00000/1e5bd812-ec52-4553-8475-675c67c1e865.root')
    fileNames = cms.untracked.vstring('file:/eos/user/j/joshin/TrackDetMatchmaker/RECOSIM_2024/output_0.root')
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.DetUnitRawIdDumper = cms.EDAnalyzer("DetUnitRawIdDumper",
    outputFileName = cms.untracked.string("det_unit_raw_id.csv"),
)

process.p = cms.Path(process.DetUnitRawIdDumper)