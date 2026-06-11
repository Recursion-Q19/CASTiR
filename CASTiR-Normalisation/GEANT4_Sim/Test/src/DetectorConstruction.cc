#include "DetectorConstruction.hh"

#include "MCEvent.hh"
#include "DataManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"

#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include <vector>
#include <algorithm>

#include <fstream>

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Constructor
//===============================================================================================================

DetectorConstruction::DetectorConstruction(G4double worldSize)
{
	NISTManager 	= G4NistManager::Instance();

	fWorldLength 	= worldSize;
	field_size 		= 40.455*cm;
	stepLimit 		= 0;
	
	solidWorld 		= 0;
	logicWorld 		= 0; 
	physiWorld 		= 0;

	/*
	pmmaBox	= 0;
	logicPmmaBox	= 0;
	physicsPmmaBox	= 0;
	*/
	
	r_in 			= 71*mm;
	r_out 			= 72.5*mm;
	solidAnnulus 	= 0;
	logicAnnulus 	= 0;
	physicsAnnulus 	= 0;
	
	World_VisAtt 	= 0;
	VisAttPhantom 	= 0;
	VisAttSensor 	= 0;  
 
	MaxNumDetectors 	= 96;
	numDetectorsPerRing = 8;

	if(numDetectorsPerRing>MaxNumDetectors)
	{
		G4cerr << __FILE__ << " : " << __LINE__<< ": number of detector modules cannot exceed 96" << G4endl;
		exit(-1);
	}

	crystal_to_crystal_gap 	= 0.1*mm;
	numCrystals1D_x	= 35;
	numCrystals1D_z = 70;
	crystal_w 		= 1.2*mm;
	crystal_h 		= 13.0*mm;
	det_w 			= 60.0*mm;
	det_l 			= 120.0*mm;
	det_h 			= crystal_h + crystal_to_crystal_gap;
	actual_det_w 	= (crystal_w + crystal_to_crystal_gap)*numCrystals1D_x;
	no_detection_len= det_w - actual_det_w; 
	actual_det_l 	= (crystal_w + crystal_to_crystal_gap)*numCrystals1D_z + no_detection_len;
 	ringRadius 		= 73.*mm;

  //-----------------------------------------------------------------;
  //DataManager* dataManager = DataManager::GetInstance();
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Destructor
//===============================================================================================================

 
DetectorConstruction::~DetectorConstruction()
{

	if(VisAttPhantom!=0) {delete VisAttPhantom; VisAttPhantom = 0;}
	if(VisAttSensor !=0) {delete VisAttSensor; VisAttSensor   = 0;}  
	if(World_VisAtt !=0) {delete World_VisAtt; World_VisAtt   = 0;}
	if(VisAttActive !=0) {delete VisAttActive; VisAttActive   = 0;}

	if(solidActive != 0) {delete solidActive; solidActive = 0;}
	if(logicActive != 0) {delete logicActive; logicActive = 0;}
	if(physicsActive != 0) {delete physicsActive; physicsActive = 0;}

	if(solidAnnulus != 0) {delete solidAnnulus; solidAnnulus = 0;}
	if(logicAnnulus != 0) {delete logicAnnulus; logicAnnulus = 0;}
	if(physicsAnnulus != 0) {delete physicsAnnulus; physicsAnnulus = 0;}
	
	// These should be deleted before detector modules as otherwise we get invalid pointer error (Since the parent volume is deleted earlier). The problem happens with physiShield which is the that is inside logicDet[]. Same for the rest.
	if(shield_separator != 0) {delete shield_separator; shield_separator = 0;}
	if(logicShield != 0) {delete logicShield; logicShield = 0;}
	if(physiShield != 0) {delete physiShield; physiShield = 0;}
	
	if(shield_separator_side != 0) {delete shield_separator_side; shield_separator_side = 0;}
	if(logicShield_side != 0) {delete logicShield_side; logicShield_side = 0;}
	if(physiShield_side_right != 0) {delete physiShield_side_right; physiShield_side_right = 0;}
	if(physiShield_side_left != 0) {delete physiShield_side_left; physiShield_side_left = 0;}
	
	if(solidCrystal != 0) {delete solidCrystal; solidCrystal = 0;}
	if(logicCrystal != 0) {delete logicCrystal; logicCrystal = 0;}
	for(G4int copyNo=0; copyNo<(numCrystals1D_x*numCrystals1D_z); copyNo++)
		{	if(physiCrystal[copyNo] != 0) {delete physiCrystal[copyNo]; physiCrystal[copyNo] = 0;}}
	
	if(solidDet != 0) {delete solidDet; solidDet = 0;}
	for(G4int copyNo=0; copyNo<numDetectorsPerRing; copyNo++)
		{	if(logicDet[copyNo] != 0) {delete logicDet[copyNo]; logicDet[copyNo] = 0;}
			if(physiDet[copyNo] != 0) {delete physiDet[copyNo]; physiDet[copyNo] = 0;}
		}

	if(solidWorld!=0) {delete solidWorld; solidWorld = 0;}
	if(logicWorld!=0) {delete logicWorld; logicWorld = 0;}
	if(physiWorld!=0) {delete physiWorld; physiWorld = 0;}
	
	if(LYSO!=0) {delete LYSO; LYSO =0;}

	if(stepLimit!=0) {delete stepLimit; stepLimit =0;}

}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Construct method
//===============================================================================================================

 
G4VPhysicalVolume* DetectorConstruction::Construct()
{  
	//------------------------------
	// Construct Materials
	//------------------------------
	ConstructMaterials();

	//------------------------------ 
	// World
	//------------------------------ 
	G4GeometryManager::GetInstance()-> SetWorldMaximumExtent(fWorldLength);
	G4cout << "Computed tolerance = " << G4GeometryTolerance::GetInstance() -> GetSurfaceTolerance()/mm << " mm" << G4endl;

	G4double HalfWorldLength = 0.5*fWorldLength;

	solidWorld = new G4Box("sWorld",HalfWorldLength,HalfWorldLength,HalfWorldLength);
	logicWorld = new G4LogicalVolume(solidWorld, vacuum, "lWorld", 0, 0, 0);
	physiWorld = new G4PVPlacement(0, G4ThreeVector(0.*mm,0.*mm,0.*mm), logicWorld, "pvWorld", 0, false, 0);   
	//G4cout<<"world center position:  "<<physiWorld->GetObjectTranslation()<<G4endl; 


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Source Distribution
//===============================================================================================================


	solidAnnulus 	= new G4Tubs("sAnnulus", r_in, r_out, ((actual_det_l + 4)/2.)*mm, 67.*deg, 46.*deg); //+4 for surrounding the active area
	logicAnnulus 	= new G4LogicalVolume(solidAnnulus, pmma, "lAnnulus", 0, 0, 0);
	physicsAnnulus 	= new G4PVPlacement(0, G4ThreeVector(0.*mm, 0.*mm, 0.*mm), logicAnnulus, "pvAnnulus", logicWorld, false, 0);   

	G4cout << "Actual Active Area Length:" << (actual_det_l + 2.) << G4endl;

	solidActive 	= new G4Tubs("sActive", r_in + .5*mm, r_out - .5*mm, ((actual_det_l + 2.)/2.)*mm, 67.5*deg, 45.*deg); // +2mm for active area to cover the whole detector
	logicActive 	= new G4LogicalVolume(solidActive, water, "lActiveArea", 0, 0, 0);
	physicsActive 	= new G4PVPlacement(0, G4ThreeVector(0.*mm, 0.*mm, 0.*mm), logicActive, "pvActive", logicAnnulus, false, 0);  
	
	//G4cout << "Radiiiiiiiiiiiiiiii: " << ((actual_det_l - 0.4)/2.) << "	Radiiiiiiiiiiiiiiiiiiiii: " << ((actual_det_l + 4)/2.) << " actual_det_l: " << actual_det_l << G4endl; 
/*
	pmmaBox = new G4Box("pmmaBox", .6*mm + det_w/2., 2.5 * mm, 2.1*mm + actual_det_l/2.);
	logicPmmaBox = new G4LogicalVolume(pmmaBox, pmma, "lPmmaBox", 0, 0, 0);
	physicsPmmaBox = new G4PVPlacement(0, G4ThreeVector(0.*mm, (ringRadius-3.5) * mm, 0.*mm), logicPmmaBox, "pvPmmaBox", logicWorld, false, 0);  
	
	solidActive = new G4Box("sActive", det_w/2. - 1.4*mm, .5 * mm, .1*mm + actual_det_l/2.); 
	logicActive = new G4LogicalVolume(solidActive, water, "lActiveArea", 0, 0, 0);
	physicsActive = new G4PVPlacement(0, G4ThreeVector(0.*mm, 0.*mm, 0.*mm), logicActive, "pvActive", logicPmmaBox, false, 0);    
	

	logicPmmaBox11 = new G4LogicalVolume(pmmaBox, pmma, "lPmmaBox", 0, 0, 0);
	G4ThreeVector pos11 = G4ThreeVector(0.*mm, (ringRadius-3.5) * mm, 0.*mm);
	
	G4double angular_shift11 = twopi/numDetectorsPerRing;	
		
	G4RotationMatrix rot11; 
	rot11.rotateZ(-angular_shift11*rad);
	pos11.rotateZ(-angular_shift11*rad);
	G4Transform3D TransformDet11 = G4Transform3D(rot11, pos11);

	G4VPhysicalVolume* physicsPmmaBox11 = new G4PVPlacement (TransformDet11, logicPmmaBox11, "physicsPmmaBox11", logicWorld, false, 0, false);
	G4VPhysicalVolume* physicsActive11 = new G4PVPlacement(0, G4ThreeVector(0.*mm, 0.*mm, 0.*mm), logicActive, "pvActive", logicPmmaBox11, false, 0);
*/	

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//==========================================================================================================================
	//Scanner Geometry
//==========================================================================================================================
	solidDet = new G4Box("sDet", det_w/2., det_h/2., det_l/2.);

	G4double angular_shift = twopi/numDetectorsPerRing;

	for(G4int copyNo=0; copyNo<numDetectorsPerRing; copyNo++)
	{  
		G4String name = "lDet_";
		std::ostringstream tmp_str;
		tmp_str << copyNo;
		name+=tmp_str.str();
 
		logicDet[copyNo] = new G4LogicalVolume(solidDet, teflon, name, 0, 0, 0);
	  
		name = "det_";
		name+=tmp_str.str();
	  
		G4ThreeVector pos = G4ThreeVector(0.*mm, ringRadius + crystal_h/2. - crystal_to_crystal_gap/2., 0.*mm); 
		
		G4RotationMatrix rot; 

		rot.rotateZ(copyNo*angular_shift*rad);
		pos.rotateZ(copyNo*angular_shift*rad);

		G4Transform3D TransformDet = G4Transform3D(rot, pos);

		physiDet[copyNo] = new G4PVPlacement (TransformDet, logicDet[copyNo], name, logicWorld, false, copyNo, false);
	}

	shield_separator = new G4Box("sep", actual_det_w/2., det_h/2., no_detection_len/2.);
	logicShield = new G4LogicalVolume(shield_separator, tungsten, "lShield", 0, 0, 0);
	
	shield_separator_side = new G4Box("sep_side", no_detection_len/4., det_h/2., actual_det_l/2.);
	logicShield_side = new G4LogicalVolume(shield_separator_side, tungsten, "lShield_side", 0, 0, 0);

	for(G4int copyNo=0; copyNo<numDetectorsPerRing; copyNo++)
	{   
		G4ThreeVector pos = G4ThreeVector(0.*mm, 0.*mm, 0.*mm); 
		physiShield = new G4PVPlacement (0, pos, logicShield, "pShield", logicDet[copyNo], false, copyNo, true);

		G4ThreeVector pos_side_right = G4ThreeVector((det_w/2. - no_detection_len/4.)*mm, 0.*mm, 0.*mm); 		
		physiShield_side_right = new G4PVPlacement (0, pos_side_right, logicShield_side, "pShield_side_right", logicDet[copyNo], false, copyNo, true);
		
		G4ThreeVector pos_side_left = G4ThreeVector((-det_w/2. + no_detection_len/4.)*mm, 0.*mm, 0.*mm); 		
		physiShield_side_left = new G4PVPlacement (0, pos_side_left, logicShield_side, "pShield_side_left", logicDet[copyNo], false, copyNo, true);
	}

	solidCrystal = new G4Box("sCrystal", crystal_w/2., crystal_h/2., crystal_w/2.);
	logicCrystal = new G4LogicalVolume(solidCrystal, LYSO, "lCrystal", 0, 0, 0);

	G4int numCrystals = numCrystals1D_x * numCrystals1D_z;
	G4int b;

	for(G4int cn = 0; cn < numDetectorsPerRing; cn++)
	{
		for(G4int copyNo=0; copyNo<numCrystals; copyNo++)
		{
			G4int i = copyNo % numCrystals1D_x;
			G4int j = copyNo / numCrystals1D_x;
			
			b = (int)(j > 34);	
		
			G4double x = actual_det_w/2. - (0.5+i)*crystal_to_crystal_gap - (1/2. + i)*crystal_w;
			G4double y = 0.5*(crystal_to_crystal_gap);
			G4double z = -(actual_det_l/2.) + (1/2. + j)*(crystal_to_crystal_gap + crystal_w) + no_detection_len*b;

			G4ThreeVector pos = G4ThreeVector(x, y, z);
	   		
			std::ostringstream tmp_str;
 	      
			tmp_str << "pvCrystal_" << copyNo;
			G4String pv_name = tmp_str.str();
  	  
			physiCrystal[copyNo] = new G4PVPlacement(0, pos, logicCrystal, pv_name, logicDet[cn], false, copyNo, false);
		}
	}

	WriteCASTOR_LUT();
	WriteSTIR_Crystal_Map();
  
//--------- Visualization attributes -------------------------------

	VisualizationAttributes();

//--------- example of User Limits -------------------------------

// below is an example of how to set tracking constraints in a given
// logical volume(see also in PhysicsList how to setup the processes
// G4StepLimiter or G4UserSpecialCuts).
  
// Sets a max Step length in the tracker region, with G4StepLimiter
//
// G4double maxStep = 0.5*cm;
// stepLimit = new G4UserLimits(maxStep);
// logicWorld->SetUserLimits(stepLimit);
 
// Set additional contraints on the track, with G4UserSpecialCuts
//
// G4double maxLength = 2*fTrackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
// logicTracker->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,
//                                               minEkin));
  
	return physiWorld;
}
 
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Material Construction
//===============================================================================================================

void DetectorConstruction::ConstructMaterials()
{
	NISTManager=G4NistManager::Instance();

	vacuum 	= NISTManager->FindOrBuildMaterial("G4_Galactic");
	tungsten= NISTManager->FindOrBuildMaterial("G4_W");
	water 	= NISTManager->FindOrBuildMaterial("G4_WATER");
	air 	= NISTManager->FindOrBuildMaterial("G4_AIR");
	teflon 	= NISTManager->FindOrBuildMaterial("G4_TEFLON");
	pmma  	= NISTManager->FindOrBuildMaterial("G4_PLEXIGLASS"); //Default 1.19 g/cm3
	LYSO 	= new G4Material("LYSO", 7.2*g/cm3, 5, kStateSolid);
	LYSO->AddElement(NISTManager->FindOrBuildElement("Lu"), 71.43*perCent);
	LYSO->AddElement(NISTManager->FindOrBuildElement("Y"), 4.03*perCent);
	LYSO->AddElement(NISTManager->FindOrBuildElement("Si"), 6.37*perCent);
	LYSO->AddElement(NISTManager->FindOrBuildElement("O"), 18.14*perCent);
	LYSO->AddElement(NISTManager->FindOrBuildElement("Ce"), 0.02*perCent); // cooke2000
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Visualization
//===============================================================================================================

void DetectorConstruction::VisualizationAttributes()
{
	World_VisAtt = new G4VisAttributes();
	World_VisAtt->SetForceWireframe(true);  
	// logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
	logicWorld->SetVisAttributes(World_VisAtt);

	VisAttActive = new G4VisAttributes(true, G4Colour(0.25, 0.75, 1., 0.4));
	//VisAttActive->SetForceWireframe(true);
	logicAnnulus->SetVisAttributes(VisAttActive);
	//logicPmmaBox11->SetVisAttributes(VisAttActive);

	VisAttPhantom = new G4VisAttributes(true, G4Colour(0.1, 0.5, 0.5, 1.0));
	//VisAttPhantom->SetForceWireframe(true);
	logicShield_side->SetVisAttributes(VisAttPhantom);

	VisAttSensor = new G4VisAttributes(true, G4Colour(0.5, 0.5, 0.5, 0.4));
	VisAttSensor->SetForceWireframe(true);

	for(G4int copyNo=0; copyNo<numDetectorsPerRing; copyNo++)
	{  
		logicDet[copyNo]->SetVisAttributes(VisAttSensor);
		//logicDet[copyNo]->SetVisAttributes(VisAttSensor);
	}
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//STIR Crystal Map Description
//===============================================================================================================

void DetectorConstruction::WriteSTIR_Crystal_Map()
{
	G4double  angular_shift = twopi/numDetectorsPerRing;
	G4int b;
  
	std::fstream Crystal_Map;
	Crystal_Map.open("MySTIR_Simulation_crystal_map.txt", std::ios::out);

	for(G4int j=0; j<numCrystals1D_z; j++)
	{
		for(G4int cn=0; cn<numDetectorsPerRing; cn++)
		{ 
			for(G4int i=0; i<numCrystals1D_x; i++)
			{
				b = (int)(j > 34);	
		
				G4double x = actual_det_w/2. - (0.5 + i) * (crystal_to_crystal_gap + crystal_w);
				G4double y = 0;
				G4double z = -actual_det_l/2. + (1/2. + j) * (crystal_to_crystal_gap + crystal_w) + (no_detection_len * b);

				G4ThreeVector pos = G4ThreeVector(x, y, z);
				
				G4ThreeVector pos_det = G4ThreeVector(0.*mm, ringRadius, 0.*mm) + pos;

				pos_det.rotateZ(cn * angular_shift*rad);
	  
				int tmp1 = j;	 		  

				int tmp2 = int(cn*numCrystals1D_x + i);
		  
				float tmp3 = float(pos_det.x()/mm);
		  
				float tmp4 = float(pos_det.y()/mm);
	  
				float tmp5 = float(pos_det.z()/mm);

				Crystal_Map << tmp1 << "\t" << tmp2 << "\t" << 0 << "\t" << tmp3 << "\t" << tmp4 << "\t" << tmp5 << G4endl;
			}
		}
	}	

  Crystal_Map.close();
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//CASToR Look Up Table (LUT) Construction
//===============================================================================================================

void DetectorConstruction::WriteCASTOR_LUT()
{
	G4double  angular_shift = twopi/numDetectorsPerRing;
	G4int b;
	  
	std::fstream lut_table;
	lut_table.open("PET_URNIDDL_PHYTOPET.lut", std::ios::out | std::ios::binary);

	for(G4int j=0; j<numCrystals1D_z; j++)
	{
		for(G4int cn=0; cn<numDetectorsPerRing; cn++)
		{ 
			for(G4int i=0; i<numCrystals1D_x; i++)
			{
				b = (int)(j > 34);	
		
				G4double x = actual_det_w/2. - (0.5 + i)* (crystal_to_crystal_gap + crystal_w);
				G4double y = crystal_h/2.;
				G4double z = -actual_det_l/2. + (1/2. + j)*(crystal_to_crystal_gap + crystal_w) + (no_detection_len * b);

				G4ThreeVector pos = G4ThreeVector(x, y, z);
		  
				G4ThreeVector pos_det = G4ThreeVector(0.*mm, ringRadius, 0.*mm) + pos;
				pos_det.rotateZ(cn*angular_shift*rad);
		  
				G4ThreeVector vect(0, 1, 0);
				vect.rotateZ(cn*angular_shift*rad);
		  
		      
				float tmp = float(pos_det.x()/mm);
				lut_table.write((char *)&tmp, sizeof(float));
				tmp = float(pos_det.y()/mm);
				lut_table.write((char *)&tmp, sizeof(float));
				tmp = float(pos_det.z()/mm);
				lut_table.write((char *)&tmp, sizeof(float));
				tmp = float(vect.x());
				lut_table.write((char *)&tmp, sizeof(float));
				tmp = float(vect.y());
				lut_table.write((char *)&tmp, sizeof(float));
				tmp = float(vect.z());
				lut_table.write((char *)&tmp, sizeof(float));	 
			}
		}
	}
	lut_table.close();
}
