#!/bin/bash

# 인자: 1) Job index, 2) 이벤트 수, 3) Output 디렉토리
JOBID=$1
NEVENTS=$2
OUTDIR=$3

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

cd /afs/cern.ch/user/j/joshin/public/TrackDetMatchmaker/CMSSW_14_2_0/src
eval `scram runtime -sh`

OUTFILE="root://eosuser.cern.ch/${OUTDIR}/output_${JOBID}.root"

cmsRun TrackDetMatchmaker/condor/test/SingleMuPt1-1000_pythia8_cfi_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_RECOSIM.py \
  maxEvents=${NEVENTS} \
  outputFile=${OUTFILE}
