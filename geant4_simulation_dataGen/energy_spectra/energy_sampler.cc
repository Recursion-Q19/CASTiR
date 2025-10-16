
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
#include "TFile.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TRandom3.h"


void energy_sampler();

int main()
{
  energy_sampler();
  return 0;
}

void energy_sampler()
{
  TFile *f = new TFile("test_root.root", "RECREATE");
  TGraph * gr = new TGraph();
  gr->SetMarkerColor(2);
  gr->SetMarkerStyle(5);
  gr->SetMarkerSize(1.2);  

  // particle energy sampler
  double              particleEnergy=0.;
  double              integral=0.;    //used by generator internally

  vector<double> energyV;       //used by generator internally
  vector<double> intensityV;    //used by generator internally
  vector<double> intensityVCI;  //used by generator internally
  std::pair<vector<double>::iterator, vector<double>::iterator> position;

  TRandom3 * rand_num = new TRandom3();
  rand_num->SetSeed(time(NULL));

  energyV.clear();     
  intensityV.clear();  
  intensityVCI.clear();

  ifstream spectrum_file;
  spectrum_file.open("pinacle_6MV.txt");

  double energy=0., intensity=0.;
  if(!spectrum_file.good())    
    { 
      cerr<<endl<<__FILE__<<": "<<__LINE__
	  <<"failed to open spectrum file : \"6MV_baker_1997.txt\""<<endl<<endl;
    }
  else
    {
      while(!spectrum_file.eof())
  	{
  	  spectrum_file>>energy>>intensity;
	  if(spectrum_file.eof())break;
  	  energyV.push_back(energy);
	  intensityV.push_back(intensity);
	}
    }

  cout<<energyV.size()<<" "<<intensityV.size()<<endl;

  for(int i=0; i<intensityV.size(); i++)
    {
      integral += 0.5*(intensityV[i] + intensityV[i-1])*(energyV[i]-energyV[i-1]);
    }

  double CIvalue = 0;
  for(int i=0; i<intensityV.size(); i++)
    {
      CIvalue   += 0.5*(intensityV[i] + intensityV[i-1])*(energyV[i]-energyV[i-1])/integral;
      intensityVCI.push_back(CIvalue);
      cout<<i<<" "<<energyV[i]<<" "<<intensityV[i]<<" "<<intensityVCI[i]<<endl;
    }

  TH1F * histo = new TH1F("histo", "; E_{#gamma} (MeV);", 8500, -0.5, 16.5);
  histo->SetDirectory(f);
  for(int i=0; i<100000; i++)
    {
      double r1 =rand_num->Rndm();
      position = equal_range(intensityVCI.begin(), intensityVCI.end(), r1);
  
      int j = position.first - intensityVCI.begin();

      double  particleEnergy = energyV[j-1];
      if(r1 > intensityVCI[j-1]) 
      	{
	  //XS cout<<intensityVCI[j-1]<<"  "<<intensityVCI[j]<<endl;
      	  particleEnergy+= (energyV[j]-energyV[j-1])*(r1-intensityVCI[j-1])/(intensityVCI[j]-intensityVCI[j-1]);
      	}
      histo->Fill(particleEnergy);
    }
    f->Write();
    f->Close();
    //  gr->Draw("ap");
}



