// from HepPDT src/HepPID/ParticleName.cc
// http://lcgapp.cern.ch/project/simu/HepPDT/HepPDT.3.04.01/html/ParticleName_8cc-source.html

const char* pdgName(int id) {
  switch (id) {
    case     1:  return "d";
    case    -1:  return "d~";
    case     2:  return "u";
    case    -2:  return "u~";
    case     3:  return "s";
    case    -3:  return "s~";
    case     4:  return "c";
    case    -4:  return "c~";
    case     5:  return "b";
    case    -5:  return "b~";
    case     6:  return "t";
    case    -6:  return "t~";
    case    11:  return "e^-";
    case   -11:  return "e^+";
    case    12:  return "nu_e";
    case   -12:  return "nu_e~";
    case    13:  return "mu^-";
    case   -13:  return "mu^+";
    case    14:  return "nu_mu";
    case   -14:  return "nu_mu~";
    case    15:  return "tau^-";
    case   -15:  return "tau^+";
    case    16:  return "nu_tau";
    case   -16:  return "nu_tau~";
    case    21:  return "g";
    case    22:  return "gamma";
    case    23:  return "Z^0";
    case    24:  return "W^+";
    case   -24:  return "W^-";
    case    25:  return "H_1^0";
    case   111:  return "pi^0";
    case   130:  return "K_L^0";
    case   310:  return "K_S^0";
    case   211:  return "pi^+";
    case  -211:  return "pi^-";
    case   321:  return "K^+";
    case  -321:  return "K^-";
    case  2112:  return "n^0";
    case -2112:  return "n~^0";
    case  2212:  return "p^+";
    case -2212:  return "p~^-";
    case  3122:  return "Lambda^0";
    case -3122:  return "Lambda~^0";
    case  3222:  return "Sigma^+";
    case -3222:  return "Sigma~^-";

    default:
      static char buffer[32];
      sprintf(buffer, "id:%d", id);
      return buffer;
  }
}
