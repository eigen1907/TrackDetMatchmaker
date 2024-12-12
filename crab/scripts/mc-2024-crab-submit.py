#!/usr/bin/env python3
import argparse
import getpass
import warnings
from CRABClient.ClientExceptions import ClientException
from CRABClient.UserUtilities import config as CrabConfig
from CRABAPI.RawCommand import crabCommand

def submit(config):
    try:
        crabCommand('submit', config=config)
    except ClientException as error:
        warnings.warn(f"Crab submission failed: {error}")

def create_config_step1(user, name, storage_site, pset):
    """
    Step1: GEN-SIM production
    No inputDataset, PrivateMC mode.
    """
    config = CrabConfig()
    config.General.requestName = f"{name}_step1_GEN-SIM"
    config.General.transferLogs = False
    config.General.transferOutputs = True

    config.JobType.pluginName = 'PrivateMC'
    config.JobType.psetName = pset
    config.JobType.maxMemoryMB = 2500
    config.JobType.numCores = 1

    config.Data.publication = False
    config.Data.outputPrimaryDataset = 'Run3_2024_SinglemuPt1-1000'
    config.Data.splitting = 'EventBased'
    config.Data.unitsPerJob = 500  # events per job
    config.Data.totalUnits = config.Data.unitsPerJob * 100
    config.Data.outLFNDirBase = f'/store/user/{user}/{name}'
    config.Data.outputDatasetTag = 'GENSIM'
    config.Site.storageSite = storage_site
    return config

def create_config_step2(user, name, storage_site, pset, input_dataset):
    """
    Step2: DIGI,L1,DIGI2RAW,HLT
    inputDataset = step1's output dataset
    """
    config = CrabConfig()
    config.General.requestName = f"{name}_step2_DIGI-RAW"
    config.General.transferLogs = False
    config.General.transferOutputs = True

    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = pset
    config.JobType.maxMemoryMB = 2500

    config.Data.inputDataset = input_dataset
    config.Data.splitting = 'Automatic'
    config.Data.outLFNDirBase = f'/store/user/{user}/{name}'
    config.Data.outputDatasetTag = 'DIGI-RAW'
    config.Data.publication = False

    config.Site.storageSite = storage_site
    return config

def create_config_step3(user, name, storage_site, pset, input_dataset):
    """
    Step3: RAW2DIGI, L1Reco, RECO, RECOSIM
    inputDataset = step2's output dataset
    """
    config = CrabConfig()
    config.General.requestName = f"{name}_step3_RECO"
    config.General.transferLogs = False
    config.General.transferOutputs = True

    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = pset
    config.JobType.maxMemoryMB = 2500

    config.Data.inputDataset = input_dataset
    config.Data.splitting = 'Automatic'
    config.Data.outLFNDirBase = f'/store/user/{user}/{name}'
    config.Data.outputDatasetTag = 'RECO'
    config.Data.publication = False

    config.Site.storageSite = storage_site
    return config


def main():
    parser = argparse.ArgumentParser(description="Submit three-step CRAB jobs.")
    parser.add_argument('--step', type=int, required=True, help='Which step to submit? 1,2, or 3')
    parser.add_argument('--user', default=getpass.getuser(), help='User name')
    parser.add_argument('--name', default='MC_Run3_2024', help='Project name')
    parser.add_argument('--storageSite', default='T3_CH_CERNBOX', help='Site to store output')
    parser.add_argument('--psetStep1', default='path/to/step1_cfg.py', help='Path to step1 cfg')
    parser.add_argument('--psetStep2', default='path/to/step2_cfg.py', help='Path to step2 cfg')
    parser.add_argument('--psetStep3', default='path/to/step3_cfg.py', help='Path to step3 cfg')
    parser.add_argument('--inputDatasetStep2', default=None, help='Input dataset for step2 (output of step1)')
    parser.add_argument('--inputDatasetStep3', default=None, help='Input dataset for step3 (output of step2)')

    args = parser.parse_args()

    if args.step == 1:
        # Step1 Submit
        config = create_config_step1(args.user, args.name, args.storageSite, args.psetStep1)
        submit(config)
    elif args.step == 2:
        if not args.inputDatasetStep2:
            raise ValueError("For step2, you must provide --inputDatasetStep2 which is the output of step1.")
        config = create_config_step2(args.user, args.name, args.storageSite, args.psetStep2, args.inputDatasetStep2)
        submit(config)
    elif args.step == 3:
        if not args.inputDatasetStep3:
            raise ValueError("For step3, you must provide --inputDatasetStep3 which is the output of step2.")
        config = create_config_step3(args.user, args.name, args.storageSite, args.psetStep3, args.inputDatasetStep3)
        submit(config)
    else:
        raise ValueError("Invalid step number. Must be 1, 2, or 3.")

if __name__ == '__main__':
    main()
