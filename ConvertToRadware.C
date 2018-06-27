/*
	ConvertToRadware.C
	Written by James Keatings (James.Keatings@uws.ac.uk)
	
	Converts Galileo + Euclides + NW + TAC root data files
	to incub8r format.

	Usage:
		root -l
		.L ConvertToRadware.C
		ConvertToRadware("inputfile.root")
*/

#include "/nucleardata1/jk/Ra/rootselectors/Selector/GalileoSelector.h"
#include "/nucleardata1/jk/Ra/rootselectors/Selector/GalTreeEvent.h"
#include "/nucleardata1/jk/Ra/rootselectors/Selector/EuclTreeEvent.h"
#include "/nucleardata1/jk/Ra/rootselectors/Selector/TACTreeEvent.h"
#include "/nucleardata1/jk/Ra/rootselectors/Selector/NWTreeEvent.h"

//32768
const int BLOCK_SIZE = 32768*8/16;	//32KB in 16b blocks (8b = 1B)
void ConvertToRadware2(int file){

	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "		Running ConvertToRadware.C			" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;

	//OPENING FILES
	TFile *infile= new TFile(Form("/nucleardata1/jk/Ra/rootselectors/Selector/run_%i_selector.root",file),"READ");
	if(infile->IsOpen()) cout << "Successfully opened input file run_" << file << "_selector.root" << endl;
	else { cerr << "Error opening input file run_" << file << "_selector.root. Cannot continue!" << endl; return; }
	ofstream outfile;
	outfile.open(Form("/nucleardata1/jk/Ra/radwarefiles/%i.dat",file));
	if(outfile.is_open()) cout << "Successfully opened output file "<< file << ".dat" << endl;
	else{ cerr << "Error opening output file "<< file << ".dat. Cannot continue!" << endl; return; }

	
	//GETENTRIES AND INITIALISE TREEREADER
	TTree *tree = (TTree*)infile->Get("SelectedGalileo");
	Long64_t entries= tree->GetEntries();
	cout << "Successfully copied tree, number of entries = " << entries << endl;
	TTreeReader reader("SelectedGalileo");

	//TTreeReaderValue<Double_t> EventTS = {reader, "fTEventTS"};
	/*TTreeReaderValue<Int_t> fMultProton(reader,"fMultProton");
	TTreeReaderValue<Int_t> fMultAlpha(reader,"fMultAlpha");
	TTreeReaderValue<Int_t> fMultAlphaDE(reader,"fMultAlphaDE");
	TTreeReaderValue<Int_t> fMultNeutron(reader,"fMultNeutron");*/

	Int_t fMultProton, fMultAlpha, fMultAlphaDE, fMultNeutron;
	tree->SetBranchAddress("fMultProton",&fMultProton);
	tree->SetBranchAddress("fMultAlpha",&fMultAlpha);
	tree->SetBranchAddress("fMultAlphaDE",&fMultAlphaDE);
	tree->SetBranchAddress("fMultNeutron",&fMultNeutron);

	//MAPPING GALILEO EVENTS
	TTreeReaderValue<std::vector<GalTreeEvent>> GalileoEvents(reader,"GalileoEvents");
	TTreeReaderArray<Float_t> GalileoEvents_fEnergy(reader,"GalileoEvents.fEnergy");
	TTreeReaderArray<Float_t> GalileoEvents_fEnergyDC(reader,"GalileoEvents.fEnergyDC");
	TTreeReaderArray<Float_t> GalileoEvents_fEnergyDC_kine(reader,"GalileoEvents.fEnergyDC_kine");
	TTreeReaderArray<Float_t> GalileoEvents_fTheta(reader,"GalileoEvents.fTheta");
	TTreeReaderArray<Float_t> GalileoEvents_fTheta_kine(reader,"GalileoEvents.fTheta_kine");
	TTreeReaderArray<Float_t> GalileoEvents_fTimeDiff(reader,"GalileoEvents.fTimeDiff");
	TTreeReaderArray<Int_t> GalileoEvents_fChannel(reader,"GalileoEvents.fChannel");
	TTreeReaderArray<UInt_t> GalileoEvents_fTimeStamp(reader,"GalileoEvents.fTimeStamp");
	TTreeReaderArray<Int_t> GalileoEvents_fRing(reader,"GalileoEvents.fRing");
	TTreeReaderArray<Int_t> GalileoEvents_fPartCoinc(reader,"GalileoEvents.fPartCoinc");
	TTreeReaderArray<Int_t> GalileoEvents_fGammaCoinc(reader,"GalileoEvents.fGammaCoinc");

	//MAPPING EUCLIDES EVENTS
	TTreeReaderValue<std::vector<EuclTreeEvent>> EuclidesEvents(reader,"EuclidesEvents");	
	TTreeReaderArray<Float_t> EuclidesEvents_fE(reader,"EuclidesEvents.fE");
	TTreeReaderArray<Float_t> EuclidesEvents_fDE(reader,"EuclidesEvents.fDE");
	TTreeReaderArray<Float_t> EuclidesEvents_fTimeDiff(reader,"EuclidesEvents.fTimeDiff");
	TTreeReaderArray<Int_t> EuclidesEvents_fChannel(reader,"EuclidesEvents.fChannel");
	TTreeReaderArray<UInt_t> EuclidesEvents_fTimeStamp(reader,"EuclidesEvents.fTimeStamp");

	//MAPPING TAC EVENTS
	TTreeReaderValue<std::vector<TACTreeEvent>> TACEvents(reader,"TACEvents");	
	TTreeReaderArray<Float_t> TACEvents_fValue(reader,"TACEvents.fValue");
	TTreeReaderArray<Int_t> TACEvents_fChannel(reader,"TACEvents.fChannel");
	TTreeReaderArray<UInt_t> TACEvents_fTimeStamp(reader,"TACEvents.fTimeStamp");

	//MAPPING NEUTRON WALL EVENTS
	TTreeReaderValue<std::vector<NWTreeEvent>> NeutronWallEvents(reader,"NeutronWallEvents");
	TTreeReaderArray<Float_t> NeutronWallEvents_fZCO(reader,"NeutronWallEvents.fZCO");
	TTreeReaderArray<Float_t> NeutronWallEvents_fTOF(reader,"NeutronWallEvents.fTOF");
	TTreeReaderArray<Float_t> NeutronWallEvents_fQVC(reader,"NeutronWallEvents.fQVC");
	TTreeReaderArray<Float_t> NeutronWallEvents_fTRF(reader,"NeutronWallEvents.fTRF");
	TTreeReaderArray<UInt_t> NeutronWallEvents_fChannel(reader,"NeutronWallEvents.fChannel");
	TTreeReaderArray<UInt_t> NeutronWallEvents_fTimeStamp(reader,"NeutronWallEvents.fTimeStamp");
	TTreeReaderArray<Float_t> NeutronWallEvents_fTimeDiff(reader,"NeutronWallEvents.fTimeDiff");

	int size = 0; 
	int buffer_size = 0;
	int zero = 0;
	int mult = 0;
	int freq = 2000;
	int event_separator = 65535;
	int tmp;

	//LOOP OVER ALL TREES
	cout << "\nBeginning to process events....." << endl;
	while(reader.Next()){
		tree->GetEntry(reader.GetCurrentEntry());
		if(reader.GetCurrentEntry()%freq == 0){
			cout << 100*reader.GetCurrentEntry()/entries << "\% finished" << "	" << "Processing " << reader.GetCurrentEntry() << "/" << entries << "\r";

		}
		//GET SIZE OF EVENT*
		size = 0;
		size = 8 + GalileoEvents_fEnergy.GetSize()*3 + EuclidesEvents_fE.GetSize()*4 + NeutronWallEvents_fZCO.GetSize()*4;
		//CHECK BUFFER WILL FIT IN BLOC
		if(buffer_size + size <= BLOCK_SIZE){
			//cout << "size = " << size << "	buffer_size = " << buffer_size << endl;
			buffer_size+=size;
			//START NEW EVENT
			//outfile.write((char*)&"ffff",sizeof(short int));
			outfile.write((char*)&event_separator,sizeof(short int));

			//EVENT LENGTH & GE MULTIPLICITY
			outfile.write((char*)&size,sizeof(short int));
			mult = GalileoEvents_fEnergy.GetSize();
			outfile.write((char*)&mult,sizeof(short int));			
			//GE ID, GE E, & GE T FOR EACH GAMMA
			for (int i = 0; i < GalileoEvents_fEnergy.GetSize(); i++){
				//cout << GalileoEvents_fEnergyDC[i] << endl;
				outfile.write((char*)&GalileoEvents_fChannel[i],sizeof(short int));
				tmp = (int)rint(4*GalileoEvents_fEnergyDC[i]);
				//cout << tmp << endl;
				outfile.write((char*)&tmp,sizeof(short int));
				outfile.write((char*)&GalileoEvents_fTimeStamp[i],sizeof(short int));
			}

			//EUCLIDES MUTLIPLICITY
			mult = EuclidesEvents_fE.GetSize();
			outfile.write((char*)&mult,sizeof(short int));
			outfile.write((char*)&fMultProton,sizeof(short int));
			outfile.write((char*)&fMultAlpha,sizeof(short int));

			//SI ID, SI E, SI dE, SI T, & PID FOR EACH PARTICLE
			for(int i = 0; i < EuclidesEvents_fE.GetSize(); i++){
				outfile.write((char*)&EuclidesEvents_fChannel[i],sizeof(short int));
				tmp = (int)rint(EuclidesEvents_fE[i]);
				outfile.write((char*)&tmp,sizeof(short int));
				tmp = (int)rint(EuclidesEvents_fDE[i]);
				outfile.write((char*)&tmp,sizeof(short int));
				outfile.write((char*)&EuclidesEvents_fTimeStamp[i],sizeof(short int));
			}
	
			//NW MUTLIPLICITY
			mult = NeutronWallEvents_fZCO.GetSize();
			outfile.write((char*)&mult,sizeof(short int));
			outfile.write((char*)&fMultNeutron,sizeof(short int));

			//NW E, NW ZCO, & GOOD/BAD FLAG FOR EACH NEUTRON
			for(int i = 0; i < NeutronWallEvents_fZCO.GetSize(); i++){
				tmp = (int)rint(NeutronWallEvents_fQVC[i]);
				outfile.write((char*)&tmp,sizeof(short int));
				tmp = (int)rint(NeutronWallEvents_fZCO[i]);
				outfile.write((char*)&tmp,sizeof(short int));
				tmp = (int)rint(NeutronWallEvents_fChannel[i]);
				outfile.write((char*)&tmp,sizeof(short int));
				tmp = (int)rint(NeutronWallEvents_fTOF[i]);
				outfile.write((char*)&tmp,sizeof(short int));
				//if(NeutronWallEvents_fQVC[i] > NeutronWallEvents_fZCO[i]) outfile.write((char*)&"1",sizeof(short int));
				//else outfile.write((char*)&"0",sizeof(short int));
			}
			//RESET SIZE

		//IF NO ROOM IN BLOCK FOR NEXT EVENT END EVENT AND FILL BLANK SPACE WITH 0000
		} else{
			outfile.write((char*)&event_separator,sizeof(short int));
			buffer_size++;
			for(int i = buffer_size; i < BLOCK_SIZE; i++){
				outfile.write((char*)&zero,sizeof(short int));
			}
			//RESET BUFFER_SIZE
			buffer_size = 0;
		}
		if (buffer_size == BLOCK_SIZE) buffer_size = 0;
	}
	
	cout << "\n\nFinished processing run_" << file << "_selector.root\nResults have been written to "<< file << ".dat" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	
	return;
}
