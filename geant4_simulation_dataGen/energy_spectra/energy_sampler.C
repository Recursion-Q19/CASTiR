
using namespace std;

#include <vector>

void energy_sampler();

int main()
{
  energy_sampler();
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
  double              CIntegral=0.;   //used by generator internally

  vector<double> energyV;       //used by generator internally
  vector<double> intensityV;    //used by generator internally
  vector<double> intensityVCI;  //used by generator internally
  std::pair<vector<double>::iterator, vector<double>::iterator> position;

  energyV.clear();     
  intensityV.clear();  
  intensityVCI.clear();

  ifstream spectrum_file;
  spectrum_file.open("6MV_baker_1997.txt");

  if(!spectrum_file.good())
    cerr<<"failed to open spectrum file... EXITING...!!!"<<endl;
  
  double energy=0., intensity=0.;
  if(!spectrum_file.good())    
    { 
      G4cerr<<G4endl<<__FILE__<<": "<<__LINE__
  	    <<"failed to open spectrum file : \""<<name<<"\""<<G4endl<<G4endl;
    }
  else
    {
      while(!spectrum_file.eof())
  	{
  	  spectrum_file>>energy>>intensity;
	  if(spectrum_file.eof())break;
  	  energyV.push_back(energy*MeV);
	  intensityV.push_back(intensity);
	  //integral += intensity;
  	}
    }

  for(int i=0; i<energyV.size(); i++)
    {
      integral += 0.5*(intensityV[i] + intensityV[i-1])*(energyV[i]-energyV[i-1]);
    }

  double CIvalue = 0;
  for(int i=0; i<energyV.size(); i++)
    {
      // CIvalue   += intensityV[i]/integral;
      CIvalue   += 0.5*(intensityV[i] + intensityV[i-1])*(energyV[i]-energyV[i-1])/integral;
      CIntegral  = CIvalue; 
      intensityVCI.push_back(CIvalue);
      G4cout<<i<<" "<<energyV[i]<<" "<<intensityV[i]<<" "<<intensityVCI[i]<<G4endl;

    }

  TH1F * histo = new TH1F("histo", "; E_{#gamma} (MeV);" 3500, -0.5, 6.5);
  for(int i=0; i<10000; i++)
    {
      position = std::equal_range(intensityVCI.begin(), intensityVCI.end(), probability);
  
      int i = position.first - intensityVCI.begin();
      double  particleEnergy = 0.5*((energyV[i+1]-energyV[i-1])*G4UniformRand() + energyV[i] + energyV[i-1]);
      histo->Fill(particleEnergy);
    }

  histo->Draw();

  //  gr->Draw("ap");
}
