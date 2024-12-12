#!/bin/bash

# 인자: 1) Job index, 2) 이전 단계 output root 디렉토리 3) output csv 디렉토리
JOBID=$1
INDIR=$2
OUTDIR=$3

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

cd /afs/cern.ch/user/j/joshin/public/TrackDetMatchmaker/CMSSW_14_2_0/src
eval `scram runtime -sh`

INFILE="root://eosuser.cern.ch/${INDIR}/output_${JOBID}.root"
OUTFILE="root://eosuser.cern.ch/${OUTDIR}/output_${JOBID}.csv"

cmsRun TrackDetMatchmaker/Matches/test/trackDetMatchesProducer_cfg.py \
  maxEvents=-1 \
  inputFiles=${INFILE} \
  outputFile=${OUTFILE}
