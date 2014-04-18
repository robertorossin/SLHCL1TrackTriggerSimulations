#########################
#
# Configuration file for fast pattern recognition
# using a pattern bank
#
# Instruction to run this script are provided on this page:
#
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto
#
# Look at STEP V
#
# Author: S.Viret (viret@in2p3.fr)
# Date        : 04/09/2013
#
# Script tested with release CMSSW_6_1_2_SLHC6_patch1
#
#########################

import FWCore.ParameterSet.Config as cms

process = cms.Process("PReco")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1) # Don't change this!!!!!!
)

process.source = cms.Source("EmptySource")

process.preco = cms.EDAnalyzer('PatternExtractor',
                               inputRootFile = cms.string('root://lyogrid06.in2p3.fr//dpm/in2p3.fr/home/cms/data/store/user/sviret/SLHC/GEN/612_SLHC6_MU/MU_612_SLHC6.root'),
                               inputBankFile = cms.string('/afs/cern.ch/work/s/sviret/testarea/PatternBanks/BE_5D/Eta7_Phi8/ss64_cov40/612_SLHC6_MUBANK_lowmidhig_sec37_ss64_cov40.pbk'),
                               outputRootFile= cms.string('my_PR_output.root'),
                               n_events      = cms.untracked.int32(5000),
                               skip_events   = cms.untracked.int32(0),
                               keepID        = cms.untracked.int32(0),
                               threshold     = cms.int32(5))


process.p = cms.Path(process.preco)
