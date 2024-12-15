#!/bin/bash

JOBID=$1
OUTDIR=$2

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

cd /afs/cern.ch/user/j/joshin/public/TrackDetMatchmaker/CMSSW_14_2_0/src
eval `scram runtime -sh`

OUTFILE="root://eosuser.cern.ch/${OUTDIR}/output_${JOBID}.root"

cmsRun ${CMSSW_BASE}/src/TrackDetMatchmaker/Matches/test/SingleMuPt1-1000_pythia8_cfi_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_RECOSIM.py \
  outputFile=${OUTFILE}
