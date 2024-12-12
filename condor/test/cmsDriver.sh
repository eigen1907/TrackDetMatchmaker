cmsDriver.py SingleMuPt1_pythia8_cfi \
-s GEN,SIM,DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2024,RAW2DIGI,L1Reco,RECO,RECOSIM \
--datatier GEN-SIM-RECO \
--era Run3_2024 \
--conditions auto:phase1_2024_realistic \
--geometry DB:Extended \
--eventcontent RECOSIM \
-n 10 \
--fileout file:RECOSIM.root