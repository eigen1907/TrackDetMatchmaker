cmsDriver.py SingleMuPt1_pythia8_cfi \
-s GEN,SIM \
--datatier GEN-SIM \
--era Run3_2024 \
--conditions auto:phase1_2024_realistic \
--beamspot DBrealistic \
--geometry DB:Extended \
--eventcontent FEVTDEBUG \
--relval 25000,100 \
-n 10 \
--fileout file:step1.root

cmsDriver.py step2 \
-s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2024 \
--datatier GEN-SIM-DIGI-RAW \ 
--era Run3_2024 \
--conditions auto:phase1_2024_realistic \
--geometry DB:Extended \
--eventcontent FEVTDEBUGHLT \
-n 10 \
--filein file:step1.root \
--fileout file:step2.root

cmsDriver.py step3 \
-s RAW2DIGI,L1Reco,RECO,RECOSIM \
--datatier GEN-SIM-RECO \
--era Run3_2024 \
--conditions auto:phase1_2024_realistic \
--geometry DB:Extended \
--eventcontent RECOSIM \
-n 10 \
--filein file:step2.root \
--fileout file:step3.root