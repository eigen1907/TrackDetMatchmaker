#!/bin/bash
source /cvmfs/cms.cern.ch/common/crab-setup.sh

#python3 mc-2024-crab-submit.py \
#  --step 1 \
#  --psetStep1 ${CMSSW_BASE}/src/TrackDetMatchmaker/MC/test/SingleMuPt1-1000_pythia8_cfi_GEN_SIM.py
#
python3 mc-2024-crab-submit.py \
  --step 2 \
  --psetStep2 ${CMSSW_BASE}/src/TrackDetMatchmaker/MC/test/step2_DIGI_L1_DIGI2RAW_HLT.py \
  --inputDatasetStep2 "/eos/user/j/joshin/MC_Run3_2024/Run3_2024_SinglemuPt1-1000/GENSIM/241211_115516/0000"

#python3 mc-2024-crab-submit.py \
#  --step 3 \
#  --psetStep3 ${CMSSW_BASE}/src/TrackDetMatchmaker/MC/test/step3_RAW2DIGI_L1Reco_RECO_RECOSIM.py \
#  --inputDatasetStep3 "/store/user/...." 
