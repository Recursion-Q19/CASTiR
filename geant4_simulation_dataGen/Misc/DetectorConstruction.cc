#include "DetectorConstruction.hh"

#include "MCEvent.hh"
#include "DataManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
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

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Constructor
//===============================================================================================================

 
DetectorConstruction::DetectorConstruction(G4double worldSize)
{
	NISTManager 	= G4NistManager::Instance();

	fWorldLength 	= worldSize;
	field_size 	= 40.455*cm;
	stepLimit 	= 0;
	
	solidWorld 	= 0;
	logicWorld 	= 0; 
	physiWorld 	= 0;

	phantom 	= 0;
	logicPh 	= 0;
  	physPh		= 0;

	fillableLid 	= 0;
	logicfLid 	= 0;
  	physfLid 	= 0;

	fillableChamber = 0;
	logicfCham 	= 0;
  	physfCham 	= 0;

	coldSubChamberOuter 		= 0;
	logicColdSubChamOuterPositive 	= 0;
  	physColdSubChamOuterPositive 	= 0;

	coldSubChamberInner 		= 0;
	logicColdSubChamInnerPositive 	= 0;
  	physColdSubChamInnerPositive 	= 0;

	logicColdSubChamOuterNegative 	= 0;
  	physColdSubChamOuterNegative 	= 0;
	logicColdSubChamInnerNegative 	= 0;
  	physColdSubChamInnerNegative 	= 0;

	for(G4int cn=0; cn<5; cn++)
	{
		fillableRod[cn] = 0;
		logicRod[cn] 	= 0;
	  	physRod[cn] 	= 0;	
	}

	World_VisAtt 	= 0;
	VisAttPhantom 	= 0;
	VisAttfCham 	= 0;
	VisAttSensor 	= 0;  
 
	MaxNumDetectors 	= 96;
	numDetectorsPerRing 	= 8;

	if(numDetectorsPerRing>MaxNumDetectors)
	{
		G4cerr << __FILE__ << " : " << __LINE__<< ": number of detector modules cannot exceed 96" << G4endl;
		exit(-1);
	}

	crystal_to_crystal_gap 	= 0.1*mm;
	numCrystals1D_x	= 35;
	numCrystals1D_z = 70;
	crystal_w 	= 1.2*mm;
	crystal_h 	= 13.0*mm;
	det_w 		= 60.0*mm;
	det_l 		= 120.0*mm;
	det_h 		= crystal_h + crystal_to_crystal_gap;
	actual_det_w 	= (crystal_w + crystal_to_crystal_gap)*numCrystals1D_x;
	no_detection_len= det_w - actual_det_w; 
	actual_det_l 	= (crystal_w + crystal_to_crystal_gap)*numCrystals1D_z + no_detection_len;
 	ringRadius 	= 73.*mm;

  //-----------------------------------------------------------------;
  //DataManager* dataManager = DataManager::GetInstance();
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//Destructor
//===============================================================================================================

 
DetectorConstruction::~DetectorConstruction()
{
	if(VisAttfCham!=0){delete VisAttfCham; VisAttfCham = 0;}
	if(VisAttPhantom!=0) {delete VisAttPhantom; VisAttPhantom = 0;}
	if(VisAttSensor!=0) {delete VisAttSensor; VisAttSensor = 0;}  
	if(World_VisAtt!=0) {delete World_VisAtt; World_VisAtt = 0;}

	if(fillableChamber!=0){delete fillableChamber; fillableChamber = 0;}
	if(logicfCham!=0){delete logicfCham; logicfCham = 0;}
	if(physfCham!=0){delete physfCham; physfCham = 0;}

	if(fillableLid!=0){delete fillableLid; fillableLid = 0;}
	if(logicfLid!=0){delete logicfLid; logicfLid = 0;}
	if(physfLid!=0){delete physfLid; physfLid = 0;}

	if(phantom != 0){delete phantom; phantom = 0;}
	if(logicPh != 0){delete logicPh; logicPh = 0;}
	if(physPh != 0){delete logicPh; physPh = 0;}
  
	if(solidWorld!=0){delete solidWorld; solidWorld = 0;}
	if(logicWorld!=0){delete logicWorld; logicWorld = 0;}
	if(physiWorld!=0){delete physiWorld; physiWorld = 0;}

	if(stepLimit!=0){delete stepLimit; stepLimit =0;}
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
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
	G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fWorldLength);
	G4cout << "Computed tolerance = " << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm << " mm" << G4endl;

	G4double HalfWorldLength = 0.5*fWorldLength;

	solidWorld = new G4Box("sWorld",HalfWorldLength,HalfWorldLength,HalfWorldLength);
	logicWorld = new G4LogicalVolume(solidWorld, vacuum, "lWorld", 0, 0, 0);
	physiWorld = new G4PVPlacement(0, G4ThreeVector(0.*mm,0.*mm,0.*mm), logicWorld, "pvWorld", 0, false, 0);   
	//G4cout<<"world center position:  "<<physiWorld->GetObjectTranslation()<<G4endl; 


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//===============================================================================================================
	//NEMA Small Animal Phantom Geometry
//===============================================================================================================

	phantom = new G4Tubs("phantom", 0, 16.75*mm, 31.5*mm, 0, 360*deg);
	logicPh = new G4LogicalVolume(phantom, pmma, "logicPh", 0, 0, 0);
  	physPh = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.*mm), logicPh, "physPh", logicWorld, false, true);

	//Fillable Cylindrical Chamber
	fillableLid = new G4Tubs("fiLid", 0, 12.*mm, 1.5*mm, 0, 360*deg);
	logicfLid = new G4LogicalVolume(fillableLid, water, "logicfLid", 0, 0, 0);
  	physfLid = new G4PVPlacement(0, G4ThreeVector(0., 0., 25.*mm), logicfLid, "physfLid", logicPh, false, true);

	//Fillable Cylindrical Chamber
	fillableChamber = new G4Tubs("fiChamber", 0, 15.*mm, 15.*mm, 0, 360*deg);
	logicfCham = new G4LogicalVolume(fillableChamber, water, "logicfCham", 0, 0, 0);
  	physfCham = new G4PVPlacement(0, G4ThreeVector(0., 0., -11.5*mm), logicfCham, "physfCham", logicPh, false, true);


	
	//Two Cold Cylindrical Sub-Chamber
	coldSubChamberOuter = new G4Tubs("coldSubChamberOuter", 0, 5.*mm, 7.5*mm, 0, 360*deg);
	logicColdSubChamOuterPositive = new G4LogicalVolume(coldSubChamberOuter, pmma, "logicColdSubChamOuterPositive", 0, 0, 0);
  	physColdSubChamOuterPositive = new G4PVPlacement(0, G4ThreeVector(7.5*mm, 0., -7.5*mm), logicColdSubChamOuterPositive, "physColdSubChamOuterPositive", logicfCham, false, true);

	//=================
	coldSubChamberInner = new G4Tubs("coldSubChamberInner", 0, 4.*mm, 7.*mm, 0, 360*deg);
	logicColdSubChamInnerPositive = new G4LogicalVolume(coldSubChamberInner, water, "logicColdSubCham2Outer", 0, 0, 0);
  	physColdSubChamInnerPositive = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5*mm), logicColdSubChamInnerPositive, "physColdSubCham1InnerPositive", logicColdSubChamOuterPositive, false, true);


	//==================
	logicColdSubChamOuterNegative = new G4LogicalVolume(coldSubChamberOuter, pmma, "logicColdSubChamOuterNegative", 0, 0, 0);
  	physColdSubChamOuterNegative = new G4PVPlacement(0, G4ThreeVector(-7.5*mm, 0., -7.5*mm), logicColdSubChamOuterNegative, "physColdSubCham2Outer", logicfCham, false, true);

	//==================
	logicColdSubChamInnerNegative = new G4LogicalVolume(coldSubChamberInner, air, "logicColdSubCham2Outer", 0, 0, 0);
  	physColdSubChamInnerNegative = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5*mm), logicColdSubChamInnerNegative, "physColdSubChamInnerNegative", logicColdSubChamOuterNegative, false, true);


////////////////////////////////////////////////////

	G4double lid_angular_shift = twopi/5;
	for(G4int cn=0; cn<5; cn++)
	{
		G4String nameShape = "SRod_";
		std::ostringstream tmp_str;
		tmp_str << cn;
		nameShape+=tmp_str.str();

		fillableRod[cn] = new G4Tubs("nameShape", 0, (0.5*(cn+1))*mm, 10.*mm, 0, 360*deg);

		G4String nameLog = "lRod_";
		tmp_str << cn;
		nameLog+=tmp_str.str();

		logicRod[cn] = new G4LogicalVolume(fillableRod[cn], water, nameLog, 0, 0, 0);
	  
		G4String namePlace = "det_";
		namePlace+=tmp_str.str();
	  
		G4ThreeVector pos = G4ThreeVector(7.*mm, 0.*mm, 13.5*mm); 
		G4RotationMatrix rot; 
		rot.rotateZ(cn*lid_angular_shift*rad);
		pos.rotateZ(cn*lid_angular_shift*rad);
		G4Transform3D TransformDet = G4Transform3D(rot, pos);

		physRod[cn] = new G4PVPlacement (TransformDet, logicRod[cn], namePlace, logicPh, true, cn);
	
	}
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

	for(G4int copyNo=0; copyNo<numDetectorsPerRing; copyNo++)
	{   
		G4ThreeVector pos = G4ThreeVector(0.*mm, 0.*mm, 0.*mm); 
		physiShield = new G4PVPlacement (0, pos, logicShield, "pShield", logicDet[copyNo], false, copyNo, true);
	}

	solidCrystal = new G4Box("sCrystal", crystal_w/2., crystal_h/2., crystal_w/2.);
	logicCrystal = new G4LogicalVolume(solidCrystal, LYSO, "lCrystal", 0, 0, 0);

	G4int numCrystals = numCrystals1D_x*numCrystals1D_z;
	G4int b;

	for(G4int cn=0; cn<numDetectorsPerRing; cn++)
	{
		for(G4int copyNo=0; copyNo<numCrystals; copyNo++)
		{
			G4int i = copyNo%numCrystals1D_x;
			G4int j = copyNo/numCrystals1D_x;
			
			b = (int)(j > 34);	
		
			G4double x = actual_det_w/2. - (0.5+i)*crystal_to_crystal_gap - (1/2. + i)*crystal_w;
			G4double y = 0.5*(crystal_to_crystal_gap);
			G4double z = -(actual_det_l/2.) + (1/2. + j)*(crystal_to_crystal_gap + crystal_w) + no_detection_len*b;

			G4ThreeVector pos = G4ThreeVector(x, y, z);
	   		
			std::ostringstream tmp_str;
 	      
			tmp_str<<"pvCrystal_"<< copyNo;
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

	VisAttSensor = new G4VisAttributes(true, G4Colour(0.5, 0.5, 0.5, 0.4));
	VisAttSensor->SetForceWireframe(true);

	for(G4int copyNo=0; copyNo<numDetectorsPerRing; copyNo++)
	{  
		logicDet[copyNo]->SetVisAttributes(VisAttSensor);
		//logicDet[copyNo]->SetVisAttributes(VisAttSensor);
	}

	VisAttPhantom = new G4VisAttributes(true, G4Colour::Blue());
	VisAttPhantom->SetForceWireframe(true);
	logicPh->SetVisAttributes(VisAttPhantom);
		
	VisAttfCham = new G4VisAttributes(true, G4Colour::Green());
	VisAttfCham->SetForceWireframe(true);

	for(G4int cn=0; cn<5; cn++)
	{
		logicRod[cn]->SetVisAttributes(VisAttfCham);
	}

	//VisAttfCham->SetForceWireframe(true);
	logicfCham->SetVisAttributes(VisAttfCham);
	logicfLid->SetVisAttributes(VisAttfCham);

	VisAttColdSubChamOuterPositive = new G4VisAttributes(true, G4Colour::Grey());
	VisAttColdSubChamOuterPositive->SetForceWireframe(true);
	logicColdSubChamOuterPositive->SetVisAttributes(VisAttColdSubChamOuterPositive);

	VisAttColdSubChamOuterNegative = new G4VisAttributes(true, G4Colour::Cyan());
	VisAttColdSubChamOuterNegative->SetForceWireframe(true);
	logicColdSubChamOuterNegative->SetVisAttributes(VisAttColdSubChamOuterNegative);

	VisAttColdSubChamInnerPositive = new G4VisAttributes(true, G4Colour::Yellow());
	//VisAttColdSubCham1Outer->SetForceWireframe(true);
	logicColdSubChamInnerPositive->SetVisAttributes(VisAttColdSubChamInnerPositive);

	VisAttColdSubChamInnerNegative = new G4VisAttributes(true, G4Colour::Red());
	//VisAttColdSubCham2Outer->SetForceWireframe(true);
	logicColdSubChamInnerNegative->SetVisAttributes(VisAttColdSubChamInnerNegative);

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
		
				G4double x = actual_det_w/2. - (0.5 + i)* (crystal_to_crystal_gap + crystal_w);
				G4double y = 0;
				G4double z = -actual_det_l/2. + (1/2. + j)*(crystal_to_crystal_gap + crystal_w) + no_detection_len*b;

				G4ThreeVector pos = G4ThreeVector(x, y, z);
				
				G4ThreeVector pos_det = G4ThreeVector(0.*mm, ringRadius, 0.*mm) + pos;

				pos_det.rotateZ(cn*angular_shift*rad);
	  
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
				G4double z = -actual_det_l/2. + (1/2. + j)*(crystal_to_crystal_gap + crystal_w) + no_detection_len*b;

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
