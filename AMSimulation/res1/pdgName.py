#!/usr/bin/env python

class pdgName:
    '''
    Convert a pdgId to a string name
    Taken from HepPDT src/HepPID/ParticleName.cc
    http://lcgapp.cern.ch/project/simu/HepPDT/HepPDT.3.04.01/html/ParticleName_8cc-source.html
    '''

    def __init__(self):
        self.namemap = {
              1:  "d",
             -1:  "d~",
              2:  "u",
             -2:  "u~",
              3:  "s",
             -3:  "s~",
              4:  "c",
             -4:  "c~",
              5:  "b",
             -5:  "b~",
              6:  "t",
             -6:  "t~",
             11:  "e^-",
            -11:  "e^+",
             12:  "nu_e",
            -12:  "nu_e~",
             13:  "mu^-",
            -13:  "mu^+",
             14:  "nu_mu",
            -14:  "nu_mu~",
             15:  "tau^-",
            -15:  "tau^+",
             16:  "nu_tau",
            -16:  "nu_tau~",
             21:  "g",
             22:  "gamma",
             23:  "Z^0",
             24:  "W^+",
            -24:  "W^-",
             25:  "H_1^0",
            111:  "pi^0",
            130:  "K_L^0",
            310:  "K_S^0",
            211:  "pi^+",
           -211:  "pi^-",
            321:  "K^+",
           -321:  "K^-",
            323:  "K*(892)^+",
           -323:  "K*(892)^-",
            411:  "D^+",
           -411:  "D^-",
            521:  "B^+",
           -521:  "B^-",
           2112:  "n^0",
          -2112:  "n~^0",
           2212:  "p^+",
          -2212:  "p~^-",
           3122:  "Lambda^0",
          -3122:  "Lambda~^0",
           3112:  "Sigma^-",
          -3112:  "Sigma~^+",
           3222:  "Sigma^+",
          -3222:  "Sigma~^-",
           3312:  "Xi^-",
          -3312:  "Xi~^+",
           3334:  "Omega^-",
          -3334:  "Omega~^+",
        }

    def get(self, pdgId):
        return self.namemap[pdgId]

