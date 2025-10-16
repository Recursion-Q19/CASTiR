
#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4NistManager.hh"
//#include "PhysicalConstants.h"
//#include "SystemOfUnits.h"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"

//#include <vector>

class Fiber;
class LightAbsorber;
class CPID_info;
class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4NistManager;
class G4UserLimits;
class G4VisAttributes;

//---------------------------------------------------------------------------
class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		DetectorConstruction(G4double worldSize=0);
		~DetectorConstruction();
  
	public:
		virtual G4VPhysicalVolume* Construct();
		G4double GetWorldFullLength()      {return fWorldLength;}
  
		const G4LogicalVolume* GetWorldLVolume()  const { return logicWorld;}
	private:    
	 	void ConstructMaterials();
	 	void VisualizationAttributes();
	  	void WriteCASTOR_LUT();
	  	void WriteSTIR_Crystal_Map();

	  	G4Box*			solidWorld;        
	  	G4LogicalVolume* 	logicWorld;        
	  	G4VPhysicalVolume* 	physiWorld;        
	 
		G4Tubs*			phantom;
		G4LogicalVolume*     	logicPh;
		G4VPhysicalVolume*   	physPh;

		G4Tubs*			fillableChamber;
		G4LogicalVolume*	logicfCham;
		G4VPhysicalVolume*	physfCham;

		G4Tubs*			fillableLid;        
		G4LogicalVolume*	logicfLid;       
		G4VPhysicalVolume*	physfLid; 

		G4Tubs*			coldSubChamberOuter;
		G4LogicalVolume*	logicColdSubChamOuterPositive;
		G4VPhysicalVolume*	physColdSubChamOuterPositive;  

		G4Tubs*			coldSubChamberInner;
		G4LogicalVolume*	logicColdSubChamInnerPositive;
		G4VPhysicalVolume* 	physColdSubChamInnerPositive; 

		G4LogicalVolume*	logicColdSubChamOuterNegative;
		G4VPhysicalVolume*	physColdSubChamOuterNegative;  

		G4LogicalVolume*	logicColdSubChamInnerNegative;
		G4VPhysicalVolume* 	physColdSubChamInnerNegative; 

		G4Tubs*			fillableRod[5];
		G4LogicalVolume*	logicRod[5];
		G4VPhysicalVolume*	physRod[5];


		G4Box* 			solidDet;
		G4LogicalVolume* 	logicDet[8];
		G4VPhysicalVolume* 	physiDet[8];

		G4Box* 			solidCrystal;
		G4LogicalVolume* 	logicCrystal;
		G4VPhysicalVolume* 	physiCrystal[2450];

		G4Box*			shield_separator;
		G4LogicalVolume*	logicShield;
		G4VPhysicalVolume*	physiShield;

		G4NistManager* 		NISTManager;
		G4Material* 		LYSO;
		G4Material* 		teflon;
		G4Material* 		pmma;
		G4Material* 		water;
		G4Material* 		air;
		G4Material* 		vacuum;
		G4Material* 		tungsten;

		G4VisAttributes*	World_VisAtt;
		G4VisAttributes* 	VisAttSensor;
		G4VisAttributes*	VisAttColdSubChamOuterPositive;  
		G4VisAttributes*	VisAttColdSubChamOuterNegative; 
		G4VisAttributes*	VisAttColdSubChamInnerPositive;  
		G4VisAttributes*	VisAttColdSubChamInnerNegative; 
		G4VisAttributes* 	VisAttfCham;  
		G4VisAttributes* 	VisAttPhantom;


		G4double	fWorldLength;      // Full length of the world volume 
		G4double	field_size;
		G4double 	ringRadius;
		G4double	crystal_to_crystal_gap;
		G4double 	no_detection_len;
		G4double	det_w;
		G4double	det_h;
		G4double	det_l;
		G4double	crystal_w;
		G4double	crystal_h;
		G4double 	actual_det_w;
		G4double 	actual_det_l;

		G4int 		MaxNumDetectors;
		G4int 		numDetectorsPerRing;
		G4int 		numCrystals1D_x;
		G4int 		numCrystals1D_z;

  
		G4UserLimits*	stepLimit;	// adjusting max. step size for 
                                        	// more detailed simulation of 
                                        	// the energy deposition
};


//-----------------------------------------------------------------------------
#endif
