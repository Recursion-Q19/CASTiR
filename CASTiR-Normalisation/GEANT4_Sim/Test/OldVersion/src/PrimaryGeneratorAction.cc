#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4Poisson.hh"

#include "DataManager.hh"
#include "MCEvent.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"



#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

//----------------------------------------------------------------------------

PrimaryGeneratorAction::PrimaryGeneratorAction():G4VUserPrimaryGeneratorAction()
{
  // default particle
  particleTable = G4ParticleTable::GetParticleTable();
  particle = particleTable->FindParticle("e+");
  
  particleGun = new G4ParticleGun(particle, 1);

  particleDirection = G4ThreeVector(0., 0., 1.);
 
  particleEnergy = 0.*MeV;
  integral       = 0.; 
  
  //Instantiation of messenger
  messenger = new PrimaryGeneratorMessenger(this);

  fixedEnergyFlag  = false;
 
  if(fixedEnergyFlag==false)InitializeSpectrum("./energy_spectra/F18-Blanco-Spectra.txt");

  //Data Management
  dataManager  = DataManager::GetInstance();

	logicalVolumeStore = G4LogicalVolumeStore::GetInstance();
        navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  eventPrimary = new EventPrimary;
}

//-----------------------------------------------------------------------------

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  //delete eventPrimary;
  delete particleGun;
  delete messenger;
}

//-----------------------------------------------------------------------------
void PrimaryGeneratorAction::InitializeSpectrum(G4String name)
{
  if(fixedEnergyFlag == true) return;
  G4double energy=0., intensity=0.;

  energyV.clear();
  intensityV.clear();
  intensityVCI.clear();

  std::ifstream spectrum_file;
  
  spectrum_file.open(name.data());
  
  if(!spectrum_file.good())    
    { 
      G4cerr<<G4endl<<__FILE__<<": "<<__LINE__<<G4endl
  	    <<"failed to open spectrum file : \""<<name<<"\""<<G4endl<<G4endl;
      exit(-1);
    }
  else
    {
      G4cout<<"Initializing X-ray spectrum to: "<<name<<G4endl;
      while(!spectrum_file.eof())
  	{
  	  spectrum_file>>energy>>intensity;
	  if(spectrum_file.eof())break;
  	  energyV.push_back(energy*MeV);
	  intensityV.push_back(intensity);
  	}
    }

  spectrum_file.close();

  for(unsigned int i=0; i<intensityV.size(); i++)
    {
      if(i>0)
	integral += 0.5*(intensityV[i] + intensityV[i-1])*(energyV[i]-energyV[i-1]);
      else if(i==0)
	integral += 0.5*(intensityV[i])*(energyV[i]);
    }

  G4double CIvalue = 0.;

  for(unsigned int i=0; i<intensityV.size(); i++)
    {
      if(i>0)
	CIvalue += 0.5*(intensityV[i] + intensityV[i-1])*(energyV[i]-energyV[i-1])/integral;
      else if (i==0)
	CIvalue += 0.5*(intensityV[i])*(energyV[i])/integral;
      intensityVCI.push_back(CIvalue);
    }
}
//-----------------------------------------------------------------------------
void PrimaryGeneratorAction::SampleEnergy(G4double probability)
{
  //sample energy from initialize spectrum
  position = equal_range(intensityVCI.begin(), intensityVCI.end(), probability);
  G4int j = position.first - intensityVCI.begin();

  particleEnergy = energyV[j-1];

  if(probability > intensityVCI[j-1])
    {
      particleEnergy += (energyV[j]-energyV[j-1])*(probability - intensityVCI[j-1])/(intensityVCI[j]-intensityVCI[j-1]);
    }
}
//-----------------------------------------------------------------------------
void PrimaryGeneratorAction::SampleDirection()
{
  G4double phi = twopi*G4UniformRand();

  G4double cosTheta = G4UniformRand();
  
  particleDirection = G4ThreeVector(0.,0.,1.);
  particleDirection.rotateY(acos(cosTheta));
  particleDirection.rotateZ(phi);
}

//-----------------------------------------------------------------------------

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  if(particle==0) return;

  dataManager->CleanUpEvent();
  dataManager->SetEventNumber(anEvent->GetEventID());
  dataManager->SetEventName("singlePhotonEvent");

  if(fixedEnergyFlag==false)
    { SampleEnergy(G4UniformRand());}

  SampleDirection();
  
  particleGun->SetParticleMomentumDirection(particleDirection);
  particleGun->SetParticleEnergy(particleEnergy);

  eventPrimary->Clear("");
  
  eventPrimary->SetTrackID(0);
  eventPrimary->SetName(particle->GetParticleName());
  eventPrimary->SetMass(particle->GetPDGMass());
  eventPrimary->SetCharge(particle->GetPDGCharge());
  eventPrimary->SetTotalEnergy(particleEnergy/MeV);
  //   eventPrimary->SetLocalTime();
  //   eventPrimary->SetGlobalTime();
  //eventPrimary->SetPolarization(TVector3(fBeamPolarization.x(),
  //   					 fBeamPolarization.y(),
  //					   fBeamPolarization.z())); 
  eventPrimary->SetMomentum(TVector3(particleEnergy*particleDirection.x()/MeV,
  				     particleEnergy*particleDirection.y()/MeV,
  				     particleEnergy*particleDirection.z()/MeV));
  
  //dataManager->AddPrimary(eventPrimary);



	G4double x, y, z;

	x = 57.2 * G4UniformRand();
	y = G4UniformRand();
	z = 105.1 * G4UniformRand(); 
	
	
	particleGun->SetParticlePosition(G4ThreeVector((x-28.6)*mm, (69. + y)*mm, (z-52.55)*mm)); 
		
/*
	G4double x, y, z;
	G4double phi, u, radius;
	G4double r_in = 66;
	G4double r_out = 67;

	phi = (G4UniformRand() / 2) *twopi; // min + (max -min) * rand()
	u = G4UniformRand();

	radius = std::sqrt( (u*(std::pow(r_out, 2) - std::pow(r_in, 2))) + std::pow(r_in, 2) );

	x = radius * std::cos( phi );
	y = radius * std::sin( phi );
	// z = radius * std::cos( theta );
	z = 10. * G4UniformRand();
	
	particleGun->SetParticlePosition(G4ThreeVector(x*mm, y*mm, z*mm));
*/

  particleGun->GeneratePrimaryVertex(anEvent);
}
//==========================================================

