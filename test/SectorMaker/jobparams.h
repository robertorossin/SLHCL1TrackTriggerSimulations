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

  bool        dbg() const;
  std::string option() const;
  std::string inputfile() const;
  std::string testfile() const;
  std::string outfile() const;
  std::string pattfile() const;
  int         eta() const;
  int         phi() const;
  int         oeta() const;
  int         ophi() const;
  int         nevt() const;

 private:

  bool         m_dbg;   
  std::string  m_opt;  
  std::string  m_inputfile; 
  std::string  m_outfile;
  std::string  m_testfile; 
  std::string  m_pattfile;  
  int          m_eta;
  int          m_phi;
  int          m_oeta;
  int          m_ophi;
  int          m_nevt;

};

inline std::string jobparams::option() const{
  return m_opt;
}

inline std::string jobparams::inputfile() const{
  return m_inputfile;
}

inline std::string jobparams::outfile() const{
  return m_outfile;
}

inline std::string jobparams::testfile() const{
  return m_testfile;
}

inline std::string jobparams::pattfile() const{
  return m_pattfile;
}

inline bool jobparams::dbg() const{
  return m_dbg;
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
