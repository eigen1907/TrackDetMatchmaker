#!/bin/bash

JOBID=$1
INDIR=$2
OUTDIR=$3

export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

cd /afs/cern.ch/user/j/joshin/public/TrackDetMatchmaker/CMSSW_14_2_0/src
eval `scram runtime -sh`

INFILE="root://eosuser.cern.ch/${INDIR}/output_${JOBID}.root"
OUTFILE="root://eosuser.cern.ch/${OUTDIR}/output_${JOBID}.csv"

cmsRun ${CMSSW_BASE}/src/TrackDetMatchmaker/Matches/test/trackDetMatchesProducer_cfg.py \
  inputFiles=${INFILE} \
  outputFileName=/tmp/${USER}_output_${JOBID}.csv

xrdcp /tmp/${USER}_output_${JOBID}.csv ${OUTFILE}
rm /tmp/${USER}_output_${JOBID}.csv
