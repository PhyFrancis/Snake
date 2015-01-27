#ifndef  _UTILITY_H_
#define  _UTILITY_H_

#include <string>
#include <fstream>

inline
bool ReadFile(const char* pFileName, std::string& outFile) {
  std::ifstream f(pFileName);
    
  bool ret = false;
    
  if (f.is_open()) {
    std::string line;
    while (getline(f, line)) {
      outFile.append(line);
      outFile.append("\n");
    }
        
    f.close();
        
    ret = true;
  }
    
  return ret;
}

#endif   // _UTILITY_H_
