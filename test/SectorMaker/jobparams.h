#ifndef jobparams_H
#define jobparams_H

#include <string>
#include <cstdio>
#include <tclap/CmdLine.h>
using namespace TCLAP;

class jobparams{

 public:

  /** constructer reading from standard arg */
  jobparams(int argc, char** argv);

  /** default constructer */
  jobparams(){}

  /** copy constructer */
  jobparams(const jobparams& tparams); 

  /** destructer */
  ~jobparams(){}

  /** return value */

  bool        rates() const;
  bool        sectors() const;
  std::string inputfile() const;
  std::string outfile() const;
  int         eta() const;
  int         phi() const;
  int         oeta() const;
  int         ophi() const;

 private:

  bool         m_rates;   
  bool         m_sectors;   
  std::string  m_inputfile; 
  std::string  m_outfile; 
  int          m_eta;
  int          m_phi;
  int          m_oeta;
  int          m_ophi;

};

inline std::string jobparams::inputfile() const{
  return m_inputfile;
}

inline std::string jobparams::outfile() const{
  return m_outfile;
}

inline bool jobparams::rates() const{
  return m_rates;
}

inline bool jobparams::sectors() const{
  return m_sectors;
}

inline int jobparams::eta() const{
  return m_eta;
}

inline int jobparams::phi() const{
  return m_phi;
}

inline int jobparams::oeta() const{
  return m_oeta;
}

inline int jobparams::ophi() const{
  return m_ophi;
}



#endif
