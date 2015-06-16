#ifndef SimpleFits_Logger_h
#define SimpleFits_Logger_h

#include <string.h>
#include <cstdio>
#include <iostream> 
#include <ostream>
#include <map>
#include "TString.h"

class Logger {
 public:
   enum level{Fatal=0,Error=1,Warning=2,Info=3,Verbose=4,Debug=5};
  static Logger* Instance(){if(instance==NULL) instance=new Logger(); return instance;}

  // set output stream 
  void Set_cout(){s=&std::cout;}
  void Set_cerr(){s=&std::cerr;}
  void Set_stream(std::ostream *stream){s=stream;}

  // switch level for one individual file
  void setLevelForClass(TString file, level l){
	  if (file.Contains(".")) stripFile(file);
	  individualFileLevels.insert(std::pair<TString, level>(file, l));
	  useIndividualFileLevels = true;
  }

  // Manipulate output levels
  void SetLevel(level _l){l=_l;}
  level Level(TString file = ""){
	  if (useIndividualFileLevels){
		  stripFile(file);
		  std::map<TString, level>::iterator it = individualFileLevels.find(file);
		  if (it != individualFileLevels.end()) return it->second;
	  }
	  return l;
  }
  std::ostream& Stream(){return (*s);}

  static int levelColor(level l){
	  if (l == Fatal)	return 41; // red background
	  if (l == Error)	return 43; // yellow background
	  if (l == Warning)	return 31; // red font
	  if (l == Info)	return 34; // blue font
	  if (l == Verbose)	return 0; // nothing
	  if (l == Debug)	return 0; // nothing
	  return 30;
  }

 private:
  Logger():
	  l(Verbose),
	  useIndividualFileLevels(false){
	  Set_cout();
  }
  virtual ~Logger(){};
  
  static Logger *instance;
  level l;
  bool useIndividualFileLevels;
  std::map<TString, level> individualFileLevels;
  std::ostream *s;

  // remove extension (like .h or .cxx) and preceding folders from file name
  void stripFile(TString& file){
	  Ssiz_t lastDot = file.Last('.');
	  Ssiz_t lastSlash = file.Last('/');
	  if (lastDot != -1)	file.Remove(lastDot);
	  if (lastSlash != -1)	file.Remove(0, lastSlash+1);
  }
};

#define Logger(level) \
  if(Logger::Instance()->Level(__FILE__) >= level) \
    Logger::Instance()->Stream() << "\033[1;" << Logger::levelColor(level) << "m" << #level << "\033[0m" << "[" << __FILE__ << " " <<  __func__ << "(..) l. " << __LINE__ << "] - "

#endif

