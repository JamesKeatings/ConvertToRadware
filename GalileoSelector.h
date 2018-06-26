//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May 30 10:53:42 2015 by ROOT version 5.34/23
// from TTree GalileoTree/GalileoTree
// found on file: test_r1002.root
//////////////////////////////////////////////////////////

#ifndef GalileoSelector_h
#define GalileoSelector_h

#define TATRO
#define BETA_SCAN

#include <map>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <utility>
#include <fstream>
#include <limits>
#include <csignal>
#include <ctime>
#include <TEntryList.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TVector3.h>
#include <TH2.h>
#include <TH1.h>
#include <TF1.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TRandom.h>
#include <TCutG.h>
#include <TList.h>
#include <TKey.h>
#include <TSysEvtHandler.h>
#include <TSystem.h>
#include <TApplication.h>
#include "EvapKinematicsCalculator.h"
#include "TGermaniumAnalysis.h"
#include "TEuclidesAnalysis.h"
#include "TNeutronWallAnalysis.h"
#include "TNeutronWallDigitalAnalysis.h"
#include "TLaBrAnalysis.h"
#include "TTraceAnalysis.h"
#include "TSpiderAnalysis.h"


//Input classes
#include "GalileoEvent.h"
#include "AncillaryEvent.h"
#include "EuclidesEvent.h"
#include "NeutronWallDigital.h"
#include "LaBrEvent.h"
#include "TraceEvent.h"
#include "SpiderEvent.h"
#include "DetectorPresent.h"


//Ouput classes
#include "GalTreeEvent.h"
#include "EuclTreeEvent.h"
#include "NWTreeEvent.h"
#include "LaBrTreeEvent.h"
#include "TACTreeEvent.h"
#include "TraceTreeEvent.h"
#include "SpiderTreeEvent.h"

// Header file for the classes stored in the TTree if any.
//#include "/home/daq/root-v5/src/cint/cint/lib/prec_stl/utility"

// Fixed size dimensions of array or collections stored in the TTree if any.
//const Int_t kMaxAncillaryEvents = 200;
//const Int_t kMaxEuclidesEvents = 64;
//const Int_t kMaxGalileoEvents = 32;

//class TNeutronWallAnalysis;




class GalileoSelector : public TSelector// ,/* public TGermaniumAnalysis, public TEuclidesAnalysis,*/ public TNeutronWallAnalysis {
{ public :

  TTreeReader     fReader;  //!the tree reader
  TTree          *fChain=0;   //!pointer to the analyzed TTree or TChain

  // Event TimeStamp
  TTreeReaderValue<double> EventTimeStamp = {fReader, "EventTS.EventTimeStamp"};

  // GALILEO BRANCHES
  TTreeReaderArray<GalileoEvent> *galileo_event = NULL;

  // EUCLIDES BRANCHES
  TTreeReaderArray<EuclidesEvent> *euclides_event  = NULL;//{fReader, ""};

  // NeutronWall Digital BRANCHES
  TTreeReaderArray<NeutronWallDigital> *nwdigi_event  = NULL;//{fReader, ""};

  // NEUTRONWALL VME BRANCHES
  TTreeReaderValue<AncillaryEvent> *AGAVA_event  = NULL;//{fReader, ""};

  // LABR BRANCHES
  TTreeReaderArray<LaBrEvent> *labr_event  = NULL ;//{fReader, ""};

  // TRACE BRANCHES
  TTreeReaderArray<TraceEvent> *trace_event  = NULL ;//{fReader, ""};
  
  // SPIDER BRANCHES
  TTreeReaderArray<SpiderEvent> *spider_event  = NULL ;//{fReader, ""};

 GalileoSelector(TTree * /*tree*/ =0) : nb_entries(0){}
  //Default TSelector functions
  virtual ~GalileoSelector() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  //Home made
  virtual Long64_t GetEntries() { return fChain ? fChain->GetEntries() : -10;}

  virtual void    Read_NW_LookUpTable(); 
  virtual void    Read_NW_Digital_LookUpTable(); 
  virtual void    Read_NW_clean_2n_limits();
  virtual void    Read_TAC_LookUpTable(); 
  virtual void    Read_EUCLIDES_LookUpTable();
  virtual void    Read_GALILEO_LookUpTable();
  virtual void    Read_LABR_LookUpTable();
  virtual void    Read_TRACE_LookUpTable();
  virtual void    Read_SPIDER_LookUpTable();


  virtual float   CalibDet(float,int,int,int);
  //  virtual TCutG*  FindNWCut(int detNb);
  virtual Int_t   FindNWDetector(int channel);
  virtual bool    FindTACModule(int channel);

  virtual Bool_t  TreatGalileoSingle();
  //  virtual void    Create_GALILEO_gg();
  virtual Bool_t  TreatEuclidesSingle(int);
  virtual Bool_t  TreatNeutronWallSingle();
  virtual Bool_t  TreatNeutronWallSingleDigital();
  virtual Bool_t  TreatLaBrSingle();
  virtual Bool_t  TreatTraceSingle();
  virtual Bool_t  TreatSpiderSingle();
  
  virtual Bool_t  TreatGalileoSpiderCoinc();
  virtual Bool_t  TreatGalileoNWCoinc();
  virtual void    TreatGalileoNWDigiCoinc();
  virtual Bool_t  TreatGalileoEuclidesCoinc();
  // LookUpTable vectors
   
  std::map<unsigned int, NeutronWallDetector*> LUT_NW       ;
  std::map<unsigned int, NeutronWallDigitalDetector> LUT_NWDIGI   ;
  std::map<unsigned int, TacModule*          > LUT_TAC      ;          
  std::map<unsigned int, EuclidesDetector    > LUT_EUCLIDES ;
  std::map<unsigned int, GalileoDetector     > LUT_GALILEO  ;
  std::map<unsigned int, LaBrDetector        > LUT_LABR     ;
  std::map<unsigned int, SpiderDetector      > LUT_SPIDER   ;
  std::map<unsigned int, TraceDetector       > LUT_TRACE    ;
  unsigned long long first_time_stamp;
  Long64_t nb_entries;
  TObjArray *toks;
  TF1 *time_extrapol;
  TF1 *time_extrapol_spider;
  Float_t beta;
  int aCompound;
  int zCompound;
  std::map<UInt_t,UInt_t> num_particle;
  int numEuclides;
  int numEuclidesRandom;
  int pid;
  std::map<int,TEntryList*> list_entries;
  std::map<int,TEntryList*>::iterator it_le;
  // TH2F* hEuclides_theta_E_alpha;
  // TH2F* hEuclides_theta_E_proton;
  // std::map<UInt_t, TH1I*> hEuclides_mult_particle;
  // std::map<UInt_t, TH2F*> hEuclides_E_theta;
  // TH1F* hCoincidences_pid;
  // TH2F* hEuclides_theta_phi_pattern;
  // //A map of PID with a pair of a histogram (single an matrix) to be filled by the list read in by the Selector at the begining
  // std::map<int, std::pair<TH1F*, TH2*> > spectraPID;
  // std::map<int, std::pair<TH1F*, TH2*> > spectraPIDNewDC;
  TFile *output;
  TTree *outputTree;
  std::vector<GalTreeEvent>     *GalEvent;
  std::vector<EuclTreeEvent>    *EuclEvent;
  std::vector<NWTreeEvent>      *NWEvent;
  std::vector<LaBrTreeEvent>    *LabrEvent;
  std::vector<TACTreeEvent>     *TACEvent;
  std::vector<TraceTreeEvent>   *TraceEvents;
  std::vector<SpiderTreeEvent>  *SpiderEvents;

  GalTreeEvent    galileo_event_tree ;
  EuclTreeEvent   euclides_event_tree;
  NWTreeEvent     nw_event_tree      ;
  LaBrTreeEvent   labr_event_tree    ;
  TACTreeEvent    tac_event_tree     ;
  TraceTreeEvent  trace_event_tree   ;
  SpiderTreeEvent spider_event_tree   ;
   
  int fMultProton ;
  int fMultAlpha  ;
  int fMultAlphaDE;
  int fMultNeutron;
  int fMultSpider ;
  // TH1D* hTSDff;
  std::map<std::pair<int,int>,TH2F*> mGammaGamma_ringwise;
  std::map<std::pair<int,int>,TH2F*> mGammaGamma_ringwise_alpha;
  std::map<std::pair<int,int>,TH2F*> mGammaGamma_ringwise_neutron;
  std::map<int,TH1F*> hGamma_ringwise;
  //  std::map<int,TH1F*> hGamma_ringwise_alpha;


  std::map<UInt_t,UInt_t> previous_gpart_coinc;
  TH2F *mGammaGamma;
  TH2F *mGammaGammaDC;
  TH2F *mGammaGammaDC_kine;
  UShort_t NW_ids_with_the_clean_neutrons[45];  
  int multiplicity_EUCLIDES;
  int multiplicity_GALILEO;
  int multiplicity_NW;
  int multiplicity_SPIDER;
  //Everything for kinematical reconstuction
  std::map<UInt_t, std::string> particle_name;
  std::map<UInt_t, int>         particle_offset;
  std::map<std::string,UInt_t>  particle_id;


  //Matrices for time alignment
  TH2F *mTimeDiff_Ge0_Ge_dom   ; //Ge0         - each domain of GALILEO
  TH2F *mTimeDiff_Eu_EDe_dom   ; //Time difference between E and DE for each euclides domain
  TH2F *mTimeDiff_Ge_Eu_dom    ; //All Galileo - each domain of EUCLIDES
  TH2F *mTimeDiff_Ge_Spider_dom; //All Galileo - each domain of SPIDER
  TH2F *mTimeDiff_Ge_Trace_dom ; //All Galileo - each domain of TRACE
  TH2F *mTimeDiff_Ge_LaBr_dom  ; //All Galileo - each domain of LaBr array
  
  //Spectra for time gates
  //
  TH1F *hTimeDiff_Ge_Ge;     // Galileo  - Galileo
  TH1F *hTimeDiff_Ge_Eu;     // Galileo  - Euclides
  TH1F *hTimeDiff_Eu_Eu;     // Euclides - Euclides 
  TH1F *hTimeDiff_Ge_NW;     // Galileo  - NeutronWall
  TH1F *hTimeDiff_Ge_Spider; // Galileo  - Spider
  TH1F *hTimeDiff_Ge_Trace ; // Galileo  - Trace
  TH1F *hTimeDiff_Ge_LaBr  ; // Galileo  - LaBr3
  

  // Time spectra for each single detectors with respect to the
  // event starting the merged event (T=0)
  TH1F *hTimeGalileoSingle;        // Galileo
  TH1F *hTimeEuclidesSingle;       // Euclides
  TH1F *hTimeNeutronWallSingle;    // NeutronWall
  TH1F *hTimeSpiderSingle;         // Spider
  TH1F *hTimeTraceSingle;          // Trace
  TH1F *hTimeLaBrSingle;           // LaBr
  
 
  // Charged particle identification gate
  EvapKinematicsCalculator* evapCalc;
  std::map<std::string, TCutG*>::iterator it_part;  


  // Individual detector data containers
  std::vector<GalileoData>  galileoData;  //  Galileo
  std::vector<EuclidesData> euclidesData; //  Euclides
  std::vector<LaBrData>     labrData;	  //  LaBr
  std::vector<SpiderData>   spiderData;	  //  Spider
  std::vector<TraceData>    traceData;    //  Trace
  std::map<UInt_t, NeutronWallDetectorData> nw_indi_data;         // NeutronWall VME
  std::map<UInt_t, NeutronWallDetectorDigiData> nw_indi_datadigi; // NeutronWall Digital 

  GalileoData  tmp_geData     ;
  EuclidesData tmp_siData     ;
  LaBrData     tmp_LaData     ;
  SpiderData   tmp_spiderData ;
  TraceData    tmp_traceData  ;
  NeutronWallData     *nwData    ;  
  NeutronWallDigiData *nwDataDigi;  

  std::map<Int_t,Int_t> mult_sigNW ;
  
  std::vector<short> NW_fired_with_neutron;
  std::clock_t start;
  double duration;
  // Boolean for autoconfiguration of the selector
  bool has_galileo;
  bool has_euclides;
  bool has_neutronwall;
  bool has_neutronwall_digital;
  bool has_labr;
  bool has_spider;
  bool has_trace;
  std::vector<anAncillaryData> tmp_ADC;
  
  ClassDef(GalileoSelector,0);
};

#endif

#ifdef GalileoSelector_cxx
void GalileoSelector::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  if(!tree) return;
  std::cout << "Checking the detectors present in the data: " << std::endl;
  TList *in = (TList*)tree->GetUserInfo();
  TObjLink *lnk = in->FirstLink();
  DetectorPresent *det_pres = (DetectorPresent*)lnk->GetObject();
  std::cout << det_pres->GetSize() << std::endl;
  has_galileo=det_pres->At(0);
  has_euclides=det_pres->At(1);
  has_neutronwall=det_pres->At(2);
  has_neutronwall_digital=det_pres->At(3);
  has_labr=det_pres->At(4);
  has_spider=det_pres->At(5);
  has_trace=det_pres->At(6);
  unsigned long long file_first_ts= det_pres->GetFTS();
  std::cout << "File first time stamp: " << file_first_ts << std::endl;;
  std::cout << "The tree contains data coming from: " << std::endl;
  if(has_galileo){
    std::cout << "\t GALILEO " << std::endl;
    Read_GALILEO_LookUpTable();
    //    Create_GALILEO_gg();
    // GALILEO BRANCHES
    galileo_event = new TTreeReaderArray<GalileoEvent>(fReader, "GalileoEvents");
  }
  if(has_euclides) {
    std::cout << "\t EUCLIDES" << std::endl;
    Read_EUCLIDES_LookUpTable();
    // EUCLIDES BRANCHES
    euclides_event  = new TTreeReaderArray<EuclidesEvent>(fReader, "EuclidesEvents");
  }
  if(has_neutronwall){
    std::cout << "\t NEUTRON WALL" << std::endl;
    Read_NW_LookUpTable();
    Read_NW_clean_2n_limits();
    Read_TAC_LookUpTable();
    // NEUTRONWALL VME BRANCHES
    AGAVA_event  = new TTreeReaderValue<AncillaryEvent>(fReader, "AncillaryEvents");
  }
  if(has_neutronwall_digital) {
    Read_NW_Digital_LookUpTable();
    Read_NW_clean_2n_limits();
    std::cout << "\t NEUTRON WALL DIGITAL" << std::endl;
    nwdigi_event  = new TTreeReaderArray<NeutronWallDigital>(fReader, "NeutronWallEvents");
  }
  if(has_labr){
    std::cout << "\t LABR" << std::endl;
    Read_LABR_LookUpTable();
    labr_event  = new TTreeReaderArray<LaBrEvent>(fReader, "LaBrEvents");
  }
  if(has_spider){
    std::cout << "\t SPIDER" << std::endl;
    Read_SPIDER_LookUpTable();
    spider_event  = new TTreeReaderArray<SpiderEvent>(fReader, "SpiderEvents");
  }
  if(has_trace){
    std::cout << "\t TRACE" << std::endl;
    Read_TRACE_LookUpTable();
    trace_event  = new TTreeReaderArray<TraceEvent>(fReader, "TraceEvents");
  }
  //  std::exit(10);
  fReader.SetTree(tree);

  output->cd();
  outputTree = new TTree("SelectedGalileo","SelectedGalileo");
  outputTree->Branch("fTEventTS",&EventTimeStamp,"TimeStamp/D");
  if(has_euclides || has_neutronwall || has_neutronwall_digital)
    outputTree->Branch("fPID",&pid,"Particle IDentification/I");
  if(has_galileo){
    GalEvent= new std::vector<GalTreeEvent>;
    outputTree->Branch("GalileoEvents",&GalEvent);
  }
  if(has_euclides){
    outputTree->Branch("fMultProton" ,&fMultProton ,"Proton multiplicity/I");
    outputTree->Branch("fMultAlpha"  ,&fMultAlpha  ,"Alpha multiplicity/I");
    outputTree->Branch("fMultAlphaDE",&fMultAlphaDE,"Stopped Alpha multiplicity/I");
    EuclEvent=new std::vector<EuclTreeEvent>;
    outputTree->Branch("EuclidesEvents",&EuclEvent);
  }
  if(has_neutronwall){
    NWEvent = new std::vector<NWTreeEvent>;
    TACEvent = new std::vector<TACTreeEvent>;
    outputTree->Branch("fMultNeutron",&fMultNeutron,"Neutron multiplicity/I");
    outputTree->Branch("TACEvents",&TACEvent);
    outputTree->Branch("NeutronWallEvents",&NWEvent);
  }
  if(has_neutronwall_digital){
    if(!has_neutronwall){
      NWEvent = new std::vector<NWTreeEvent>;
      outputTree->Branch("NeutronWallEvents",&NWEvent);
      outputTree->Branch("fMultNeutron",&fMultNeutron,"Neutron multiplicity/I");
    }
  }
  if(has_labr){
    LabrEvent = new std::vector<LaBrTreeEvent>;
    outputTree->Branch("LaBrEvents",&LabrEvent);
  }
  if(has_spider){
    SpiderEvents = new std::vector<SpiderTreeEvent>;
    outputTree->Branch("SpiderEvents",&SpiderEvents);
    outputTree->Branch("fMultSpider",&fMultSpider,"SPIDER Multiplicity/I");
  }
  if(has_trace){
    TraceEvents = new std::vector<TraceTreeEvent>;
    outputTree->Branch("TraceEvents",&TraceEvents);
  }

}

Bool_t GalileoSelector::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}







#endif // #ifdef GalileoSelector_cxx
