#include "verrouSynchroLib.h"
#define VERROU_SYNCHRO_INCLUDE
#include "verrou.h"
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>


bool synchroDebug=false;
std::string outputPrefix("==verrouSynchroLib.so== ");


bool generateTrace;
std::ofstream outputTraceFile;

bool generateTraceFP;
std::ofstream outputTraceFPFile;
std::string strNameOld("init unamed synchro");
int indexOld=0;
int countOld=0;

bool activeSynchro;
bool inactiveSynchro;
typedef std::vector< std::pair<std::string , int> >  storeSynchroType;
storeSynchroType activeTrace;
storeSynchroType inactiveTrace;



/*Private function prototype*/
void readDebug();

void verrou_synchro_generate_trace_init();
void verrou_synchro_generate_trace(const std::string& strName, int index);
void verrou_synchro_generate_trace_finalyze();

void verrou_synchro_generate_trace_fp_init();
void verrou_synchro_generate_trace_fp(const std::string& strName, int index);
void verrou_synchro_generate_trace_fp_finalyze();


void verrou_synchro_activation_init();
void verrou_synchro_activation(const std::string& strName, int index);
void verrou_synchro_activation_finalyze();

/*Public function*/
void verrou_synchro_init(){
  readDebug();
  if(synchroDebug){
    std::cerr << outputPrefix<<"synchro_init" <<std::endl;
    std::cerr << outputPrefix<<"\tfp     instr: " << VERROU_COUNT_FP_INSTRUMENTED<<std::endl;
    std::cerr << outputPrefix<<"\tfp not instr: " << VERROU_COUNT_FP_NOT_INSTRUMENTED<<std::endl;
  }
  verrou_synchro_activation_init();
  verrou_synchro_generate_trace_init();
  verrou_synchro_generate_trace_fp_init();

  verrou_synchro(strNameOld.c_str(),0);
}


void verrou_synchro(char const * const name, int index){
  std::string strName(name);
  if(synchroDebug){
    std::cerr << outputPrefix<<"synchro: " <<name<< ":"<<index<<std::endl;
    std::cerr << outputPrefix<<"\tfp     instr: " << VERROU_COUNT_FP_INSTRUMENTED<<std::endl;
    std::cerr << outputPrefix<<"\tfp not instr: " << VERROU_COUNT_FP_NOT_INSTRUMENTED<<std::endl;
  }
  verrou_synchro_generate_trace(strName,index);
  verrou_synchro_generate_trace_fp(strName,index);
  verrou_synchro_activation(strName,index);
}

void verrou_synchro_finalyze(){
  verrou_synchro_generate_trace_finalyze();
  verrou_synchro_generate_trace_fp_finalyze();
  verrou_synchro_activation_finalyze();
}


//Private function

void readDebug()
  {//block debug
    char* debugStr;
    debugStr = getenv ("DEBUG_PRINT_SYNCHRO");
    if(debugStr==NULL){
      synchroDebug=false;
    }else{
      std::cout<< outputPrefix<<"Debug activated"<<std::endl;
      std::cerr<< outputPrefix<<"Debug activated"<<std::endl;
      synchroDebug=true;
    }
  }



void verrou_synchro_generate_trace_init()
{//block GENERATE SYNCHRO LIST
  char* fileNameList = getenv ("GENERATE_SYNCHRO_LIST");
  if (fileNameList==NULL){
      generateTrace=false;
  }else{
    std::cerr<< outputPrefix<<"Generate synchro list : init"<<std::endl;
    generateTrace=true;
    outputTraceFile.open(fileNameList, std::ios::out);
  }
}//fin block GENERATE SYNCHRO LIST


void verrou_synchro_generate_trace(const std::string& strName, int index){
  if(generateTrace){
    outputTraceFile << strName <<"\t"<<index <<std::endl;
    if(synchroDebug){
      std::cerr <<outputPrefix<<"trace generation : "  << strName <<"\t"<<index <<std::endl;
    }
  }
}

void verrou_synchro_generate_trace_finalyze(){
  if(generateTrace){
    outputTraceFile.close();
  }
}



void verrou_synchro_generate_trace_fp_init(){
  char* fileNameFPList = getenv ("GENERATE_SYNCHRO_FP_LIST");
  if (fileNameFPList==NULL){
    generateTraceFP=false;
  }else{
    std::cerr<< outputPrefix <<"Generate synchro FP list : init"<<std::endl;
    generateTraceFP=true;
    outputTraceFPFile.open(fileNameFPList, std::ios::out);
  }
}

void verrou_synchro_generate_trace_fp(const std::string& strName, int index){
   if(generateTraceFP){
    long int count=VERROU_COUNT_FP_INSTRUMENTED;
    if(countOld!=count){
      outputTraceFPFile << strNameOld <<"\t"<<indexOld <<std::endl;
      if(synchroDebug){
	std::cerr <<outputPrefix<<"trace FP generation : "  << strNameOld <<"\t"<<indexOld <<std::endl;
      }
    }
    strNameOld=strName;
    indexOld=index;
    countOld=count;
   }
}


void verrou_synchro_generate_trace_fp_finalyze(){
  if(generateTraceFP){
    long int count=VERROU_COUNT_FP_INSTRUMENTED;
    if(countOld!=count){
      outputTraceFPFile << strNameOld <<"\t"<<indexOld <<std::endl;
      if(synchroDebug){
	std::cerr <<outputPrefix<<"trace FP generation : "  << strNameOld <<"\t"<<indexOld <<std::endl;
      }
    }

    outputTraceFPFile.close();
  }
}


void fileToActiveInactiveStore(std::ifstream& input, storeSynchroType& store){
    std::string strLine;
    while (std::getline(input, strLine)){
      if(strLine[0]=='#'){
	continue;
      }
      std::stringstream sstream(strLine);
      std::string keyStr;
      getline(sstream,keyStr,'\t');
      int index;
      sstream >> index;

      std::pair<std::string,int> key(keyStr,index);
      if(std::find(store.begin(), store.end(), key)==store.end()){
	store.push_back(key);
      }
      if(synchroDebug){
	std::cerr << outputPrefix<<"loading : " <<keyStr<< ":"<<index<<std::endl;
      }
    }
}

void verrou_synchro_activation_init(){
  char* fileNameList;
  fileNameList = getenv ("SYNCHRO_LIST");
  if (fileNameList==NULL){
    activeSynchro=false;
  }else{
    long int count=VERROU_COUNT_FP_INSTRUMENTED;
    if(count !=0){
      std::cerr << outputPrefix << "error : fp operation before verrou_synchro_init call" <<std::endl;
      std::cerr << outputPrefix << "advice : move verrou_synchro call or use --instr-atstart=no" <<std::endl;
    }

    activeSynchro=true;
    std::cerr<< outputPrefix<<"Loading synchro list"<<std::endl;

    std::ifstream inputSynchro(fileNameList, std::ios::in);

    if(inputSynchro.fail()){
      std::cerr<< outputPrefix<<"Fail loading file :"<< fileNameList<<std::endl;
    }
    fileToActiveInactiveStore(inputSynchro, activeTrace);

  }
  if(activeSynchro){
    fileNameList = getenv ("SYNCHRO_EXCLUDE_LIST");
    if (fileNameList==NULL){
      inactiveSynchro=false;
    }else{
      inactiveSynchro=true;
      std::cerr<< outputPrefix<<"Loading exclude synchro list"<<std::endl;
      std::ifstream inputSynchro(fileNameList, std::ios::in);
      if(inputSynchro.fail()){
	std::cerr<< outputPrefix<<"Fail loading file :"<< fileNameList<<std::endl;
      }
      fileToActiveInactiveStore(inputSynchro, inactiveTrace);
    }
  }
}


void verrou_synchro_activation(const std::string& strName, int index){
  /*Activation*/
  if(activeSynchro){
    std::pair<std::string, int> key(strName, index);
    bool isActivePair=!(std::find(activeTrace.begin(), activeTrace.end(), key)==activeTrace.end() );
    bool isInactivePair=!(std::find(inactiveTrace.begin(), inactiveTrace.end(), key)==inactiveTrace.end() );
    if(isInactivePair && isActivePair){
       if(synchroDebug){
	std::cerr << outputPrefix<< strName << " " << index << " : coherence probleme detected" << std::endl;
      }

    }
    if(isActivePair){
      VERROU_START_INSTRUMENTATION;
      if(synchroDebug){
	std::cerr << outputPrefix<< strName << " " << index << " : activated" << std::endl;
      }
    }
    if((!inactiveSynchro&&(!isActivePair)) || (inactiveSynchro&&isActivePair)){
      VERROU_STOP_INSTRUMENTATION;
      if(synchroDebug){
	std::cerr << outputPrefix<< strName << " " << index << " : deactivated" << std::endl;
      }
    }
  }
}

void verrou_synchro_activation_finalyze(){
  VERROU_STOP_INSTRUMENTATION;
  if(synchroDebug){
    std::cerr << outputPrefix<< "Finalyse : deactivated" << std::endl;
  }
}
