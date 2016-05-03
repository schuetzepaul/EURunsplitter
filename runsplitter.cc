/*
 * Author: Paul Schuetze
 *
 * Purpose: Splitting .raw files from eudaq into several files
 *
 *
 *
 */


#include "eudaq/FileReader.hh"
#include "FileWriterNative.hh"

#include <iostream>
#include <string>


using namespace std;
using namespace eudaq;

void usage(){

  cout << "Usage:\t\t ./runsplitter  folder runnr begin_new_runnr [evts/run]" << endl;
  exit(0);

}



int main( int argc, char* argv[] ){
  
  if(argc < 4) usage();

  // Init reader
  
  string origRunnr = argv[2];
  
  string folder = argv[1];
  char buffer[100];
  

  if(folder.at(folder.size()-1) == '/'){
    sprintf(buffer, "%srun$6R$X", folder.c_str());
  }else{
    sprintf(buffer, "%s/run$6R$X", folder.c_str());
  }
  
  string filename(buffer);
  
  FileReader * reader;
  reader = new FileReader(origRunnr, filename);
  
  
  // Init writer
  
  unsigned firstNewRunnr = atoi(argv[3]);
  unsigned currentRunnr = firstNewRunnr;

  FileWriterNative * writer;
  writer = new FileWriterNative("");

  writer->SetPattern(filename);
  
  // Opening first new file
  
  writer->StartRun(firstNewRunnr);

  int evtsPerFile;
  if(argc > 4){
    evtsPerFile = atoi(argv[4]);
  }else{
    evtsPerFile = 500000;
  }

  // Start Processing

  DetectorEvent boreevent = reader->GetDetectorEvent();

  if(!boreevent.IsBORE()){
    cout << "First event is not BORE. Damn it!" << endl;
    exit(1);
  }

  writer->WriteEvent(boreevent);
  
  DetectorEvent tempevent = boreevent;
  if(!reader->NextEvent()){
    cout << "No second event found. Exit." << endl;
    exit(1);
  };
  
  unsigned evtnr = 1;

  do{
    
    if(!(evtnr%evtsPerFile)){
      // Start new file
      currentRunnr++;
      
      cout << "\nStarting new file: Run Nr.: " << currentRunnr << endl << endl;
      
      writer->StartRun(currentRunnr);
      
      // Write old BOREevent first
      writer->WriteEvent(boreevent);
    }
    
    tempevent = reader->GetDetectorEvent();
    writer->WriteEvent(tempevent);
    
    if(!(evtnr%5000))cout << "Writing out event Nr. " << evtnr << " to Runnr. " << currentRunnr << endl;

    evtnr++;

  }while(reader->NextEvent());

  //delete reader;
  //delete writer;
  

  return 0;

}
