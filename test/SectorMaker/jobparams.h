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
  bool        test() const;
  bool        efficiencies() const;
  std::string inputfile() const;
  std::string testfile() const;
  std::string outfile() const;
  int         eta() const;
  int         phi() const;
  int         oeta() const;
  int         ophi() const;
  int         nevt() const;

 private:

  bool         m_rates;   
  bool         m_sectors; 
  bool         m_test;   
  bool         m_efficiencies;    
  std::string  m_inputfile; 
  std::string  m_outfile;
  std::string  m_testfile;  
  int          m_eta;
  int          m_phi;
  int          m_oeta;
  int          m_ophi;
  int          m_nevt;

};

inline std::string jobparams::inputfile() const{
  return m_inputfile;
}

inline std::string jobparams::outfile() const{
  return m_outfile;
}

inline std::string jobparams::testfile() const{
  return m_testfile;
}

inline bool jobparams::rates() const{
  return m_rates;
}

inline bool jobparams::sectors() const{
  return m_sectors;
}

inline bool jobparams::test() const{
  return m_test;
}

inline bool jobparams::efficiencies() const{
  return m_efficiencies;
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

inline int jobparams::nevt() const{
  return m_nevt;
}


#endif
