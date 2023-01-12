#ifndef __EIFTOO_COMMON_DEFINE_H_
#define __EIFTOO_COMMON_DEFINE_H_

#include <list>
#include <vector>
#include <string>


using namespace std;

// Type Define
typedef char				S8;
typedef unsigned char		U8;
typedef short				S16;
typedef unsigned short		U16;
typedef long				S32;
typedef unsigned long		U32;
typedef float				F32;
typedef double				F64;
typedef long long			S64;
typedef unsigned long long	U64;


typedef union
{
	S8  s8[8];
	S16 s16;
	S32 s32;
	S64 s64;
	F32 f32;
	F64 f64;
} uval;


struct DataSourceIdentifier
{
	U8 u8SAC;
	U8 u8SIC;
};


struct F64XY
{
	F64						f64X;
	F64						f64Y;
};

// 004 Messages
/*struct sAliveMessage
{
	U32						u32TimeOfMessage;		// Elapsed time since last midnight (millisecond)
	BOOL					bSTCAFuncStatus;
	BOOL					bAPWFuncStatus;
	BOOL					bMSAWFuncStatus;
	BOOL					bAPMFuncStatus;
};

struct sConflictCharacteristics
{
	BOOL					bDefinesFilterSetting;
	BOOL					bLinearPredictionFilterSet;
	BOOL					bCurrentProximityFilterSet;
	BOOL					bManoeuvreHazardFilterSet;
	BOOL					bConflictSeverityHigh;
};

struct sSafetyNetsMessage
{
	eSafetyNetsMessageType	eMessageType;
	U32						u32TimeOfMessage;		// Elapsed time since last midnight (millisecond)
	S32						s32TrackNumber1;
	S32						s32TrackNumber2;
	S32						u16AlertIdentifier;
	F64						f64ConflictTimingSec;
	sConflictCharacteristics sConflictCharacteristics;
	string					strAreaDefinitionName;
	F64						f64TransversalDistanceDeviation;
};*/

struct ConflictTimingAndSeparation
{
	U8						u8TC;
	U8						u8TCA;
	U8						u8CHS;
	U8						u8MHS;
	U8						u8CVS;
	U8						u8MVS;

	/* Subfield #1

		Time to Conflict	*/
	F64						f64TimeToConflict;

	/* Subfield #2

		Time to Closest Approach	*/
	F64						f64TimeToClosestApproach;

	/* Subfield #3

		Current Horizontal Separation	*/
	F64						f64CurrentHorizontalSeparation;

	/* Subfield #4

		Estimated Minimum horizontal separation	*/
	F64						f64EstimatedMinHorizontalSeparation;

	/* Subfield #5

		Current Vertical Separation	*/
	F64						f64CurrentVerticalSeparation;

	/* Subfield #6

		Estimated Minimum Vertical Separation	*/
	F64						f64EstimatedMinVerticalSeparation;

};

struct SafetyNetFunctionSystemStatus
{
	/*	0 : Default
		1 : MRVA Function	*/
	U8						u8MRVA;

	/*	0 : Default
		1 : RAMLD Function	*/
	U8						u8RAMLD;

	/*	0 : Default
		1 : RAMHD Function	*/
	U8						u8RAMHD;

	/*	0 : Default
		1 : MSAW Function	*/
	U8						u8MSAW;

	/*	0 : Default
		1 : APW Function 	*/
	U8						u8APW;

	/*	0 : Default
		1 : CLAM Function	*/
	U8						u8CLAM;

	/*	0 : Default
		1 : STCA Function	*/
	U8						u8STCA;

	/*	0 : Default
		1 : AFDA Function	*/
	U8						u8AFDA;

	/*	0 : Default
		1 : RIMCA Function	*/
	U8						u8RIMCA;

	/*	0 : Default
		1 : ACAS RA Function	*/
	U8						u8ACASRA;

	/*	0 : Default
		1 : NTCA Function 	*/
	U8						u8NTCA;

	/*	0 : Default
		1 : System degraded	*/
	U8						u8DG;

	/*	0 : Default
		1 : Overflow error	*/
	U8						u8OF;

	/*	0 : Default
		1 : Overload error 	*/
	U8						u8OL;

};


struct ConflictCharacteristics
{
	U8						u8CN;
	U8						u8CC;
	U8						u8CP;
	U8						u8CD;

	/* Subfield #1 Conflict Nature
		
		Conflict location in military airspace
		0 : Conflict not predicted to occur in military airspace
		1 : Conflict predicted to occur in military airspace	*/
	U8						u8MAS;

	/*	Conflict location in civil airspace
		0 : Conflict not predicted to occur in civil airspace
		1 : Conflict predicted to occur in civil airspace	*/
	U8						u8CAS;

	/*	Fast lateral divergence
		0 : Aircraft are not fast diverging laterally at current time
		1 : Aircraft are fast diverging laterally at current time	*/
	U8						u8FLD;

	/*	Fast vertical divergence
		0 : Aircraft are not fast diverging vertically at current time
		1 : Aircraft are fast diverging vertically at current time	*/
	U8						u8FVD;

	/*	Type of separation infringement
		0 : Minor separation infringement
		1 : Major separation infringement	*/
	U8						u8Type;

	/*	Crossing test
		0 : Aircraft have not crossed at starting time of conflict
		1 : Aircraft have crossed at starting time of conflict	*/
	U8						u8Cross;

	/*	Divergence test
		0 : Aircraft are not diverging at starting time of conflict
		1 : Aircraft are diverging at starting time of conflict	*/
	U8						u8Div;

	/*	Runway/Runway crossing in RIMCAS
		0 : Default
		1 : Runway/Runway Crossing	*/
	U8						u8RRC;

	/*	Runway/Taxiway Crossing in RIMCAS
		0 : Default
		1 : Runway/Taxiway Crossing 	*/
	U8						u8RTC;

	/*	0 : Default
		1 : Msg Type 4(MSAW) indicates MRVA	*/
	U8						u8MRVA;


	/* Subfield #2 Conflict Classification

		Identification of conflict categories definition table	*/
	U8						u8TableID;

	/*	Conflict Severity Class	*/
	U8						u8SeverityClass;

	/*	Conflict Severity
		0 : LOW
		1 : HIGH	*/
	U8						u8CS;
	

	/* Subfield #3 Conflict Probability

		Probability of the conflict	*/
	U8						u8Probability;

	/* Subfield #1 Conflict Duration

		Conflict Druation	*/
	U32						u32Duraion;
};

struct AreaDefinition
{
	U8						u8AN;
	U8						u8CAN;
	U8						u8RT1;
	U8						u8RT2;
	U8						u8SB;
	U8						u8G;

	S8						s8AreaName[8];
};

struct SafetyNetsMessage
{
	/*	001 : Alive Message
		002 : Route Adherence Monitor Longitudinal Deviation
		003 : Route Adherence Monitor Heading Deviation
		004 : Minimum Safe Altitude Warning
		005 : Area Proximity Warning
		006 : Clearance Level Adherence Monitor
		007 : Short Term Conflict Alert
		008 : Approach Funnel Deviation Alert
		009 : RIMCAS Arrival / Landing Monitor (ALM)
		010 : RIMCAS Arrival / Departure Wrong Runway Alert (WRA)
		011 : RIMCAS Arrival / Departure Opposite Traffic Alert (OTA)
		012 : RIMCAS Departure Monitor (RDM)
		013 : RIMCAS Runway / Taxiway Crossing Monitor (RCM)
		014 : RIMCAS Taxiway Separation Monitor (TSM)
		015 : RIMCAS Unauthorized Taxiway Movement Monitor(UTMM)
		016 : RIMCAS Stop Bar Overrun Alert (SBOA)
		017 : End Of Conflict (EOC)
		018 : ACAS Resolution Advisory (ACASRA)
		019 : Near Term Conflict Alert (NTCA) 	*/
	U8						u8MessageType;

	/* Elapsed time since last midnight (millisecond) */
	U32						u32TimeOfMessage;		

	U16						u16TrackNumber1;
	U16						u16TrackNumber2;

	U16						u16AlertIdentifier;

	SafetyNetFunctionSystemStatus	sSafetyNetFuncSysStatus;

	ConflictTimingAndSeparation		sConflictTimingAndSeparation;

	F64						f64TransversalDistanceDeviation;

	//S8						s8AreaDefinitionName[8];
	
	AreaDefinition			sAreaDefinition;

	ConflictCharacteristics	sConflictCharacter;
};

// 010 Messages

struct SMDTargetReportDescriptor
{
	/*	Type of Transmitted(TYP) 
		000 SSR multilateration
		001 Mode S multilateration
		010 ADS-B
		011 PSR
		100 Magnetic Loop System
		101 HF multilateration
		110 Not defined
		111 Other types 	*/
	U8						u8TYP;

	/*	0 : No differential correction (ADS-B)
		1 : Differential conrrection (ADS-B) */
	U8						u8DCR;

	/*  0 : Chain 1
		1 : Chain 2		*/
	U8						u8CHN;

	/*  0 : Transponder Ground bit not set
	    1 : Transponder Ground bit set*/
	U8						u8GBS;

	/*  0 : No Corrupted reply in multilateration
		1 : Corrupted reply in multilateration	*/
	U8						u8CRT;

	/*	0 : Actual target report
		1 : Simulated target report	*/
	U8						u8SIM;

	/*	0 : Default
		1 : Test target		*/
	U8						u8TST;

	/*	0 : Report from target transponder
		1 : Report from field monitor (fixed transponder) */
	U8						u8RAB;

	/*	00 : Undetermined
		01 : Loop start
		10 : Loop finish */
	U8						u8LOP;

	/*	00 : Undetermined
		01 : Aircraft
		10 : Ground vehicle
		11 : Helicopter */
	U8						u8TOT;

	/*  0 : Absence of SPI
	    1 : Special Position Identification */
	U8						u8SPI;
};

struct Mode3ACode
{
	/*	0 : Code validated
		1 : Code not validated	*/
	U8						u8V;

	/*	0 : Default
		1 : Grabled code	*/
	U8						u8G;

	/*	0 : Mode-3/A code derived from the reply of the transponder
		1 : Mode-3/A code not extracted during the last scan	*/
	U8						u8L;

	/*	Mode-3/A reply in octal representation */
	U16						u16Mode3A;

};

struct TargetStatus
{
	/*	0 : Confirmed track
		1 : Track in initialisation phase	*/
	U8						u8CNF;

	/*	0 : Default
		1 : Last report for a track		*/
	U8						u8TRE;

	/*	00 : No extrapolation
		01 : Predictable extrapolation due to sensor refresh period
		10 : Predictable extrapolation in masked area
		11 : Extrapolation due to uppredictable absence of detection
	*/
	U8						u8CST;

	/*	0 : Default
		1 : Horizontal manoeuvre	*/
	U8						u8MAH;

	/*	0 : Tracking performed in 'Sensore Plane', i.e. neither slant range correction nor projection was applied
		1 : Slant range correction and a suitable projection technique are used to track in a 2D. referance plane, tangential to the earth model at the sensor Site co-ordinates. 	*/
	U8						u8TCC;

	/*	0 : Measured position
		1 : Smoothed position	*/
	U8						u8STH;

	/*	00 : Unknown type of movement
		01 : Taking-off
		10 : Landing
		11 : Other type of movements	*/
	U8						u8TOM;

	/*	000 : No doubt
		001 : Doubtful correlation (Undetermined reason)
		010 : Doubtful correlation in clutter
		011 : Loss of accuracy
		100 : Loss of accuracy in clutter
		101 : Unstable track
		111 : Previously coasted
	*/
	U8						u8DOU;

	/*	00 : Nerge or split indication undetermined
		01 : Track merged by association to plot
		10 : Track merged by no-association to plot
		11 : Split track	*/
	U8						u8MRS;

	/*	0 : Default
		1 : Ghost track	*/
	U8						u8GHO;
};


struct TargetReportMessage
{
	/*	1 : Target Report
		2 : Start of Update Cycle
		3 : Periodic Status Message
	    4 : Event - triggered Status Message	*/
	U8						u8MessageType;

	SMDTargetReportDescriptor	sTargetReportDes;

	/*	X : RHO
		Y : Theta	*/
	F64XY					sMeasuredPositionPolarCoord;

	/*	X : Latitude in WGS-84
		Y : Longitude in WGS-84	*/
	F64XY					sPositionInWGS84Coord;

	/*	X : X-Component
		Y : Y-Component	*/
	F64XY					sPositionInCartesianCoord;

	Mode3ACode				sMode3ACode;

	S32						s32FlightLevelInBinary;

	S32						s32MeasuredHeight;

	U8						u8AmplitudeOfPrimaryPlot;

	/* Elapsed time since last midnight (millisecond)	*/
	U32						u32TimeOfMessage;				

	U16						u16TrackNumber;

	TargetStatus			sTargetStatus;

	/*	X : Ground Speed
		Y : Track Angle	*/
	F64XY					sCalculatedTrackVelocityPolarCoord;	

	/*	X : Vx
		Y : Vy	*/
	F64XY					sCalculatedTrackVelocityCartesianCoord;

	/*	X : Ax
		Y : Ay	*/
	F64XY					sCalculatedAcceleration;

	U32						u32TargetAddress;

	S8						s8TargetIdentification[8];

	/*	0 : Unknown
		1 : ATC equipment maintenance
		2 : Airport maintenance
		3 : Fire
		4 : Bird scarer
		5 : Snow plough
		6 : Runway sweeper
		7 : Emergency
		8 : Police
		9 : Bus
		10 : Tug(push/tow)
		11 : Grass cutter
		12 : Fuel
		13 : Baggage
		14 : Catering
		15 : Aircraft maintenace
		16 : Flyco (follow me)	*/
	U8						u8VehicleFleetIdentification;
};

// 021 Messages
struct AircraftOperationalStatus
{
	/*	TCAS Resolution Advisory active
		0 : TCAS II or ACAS RA not active
		1 : TCAS RA active	*/
	U8						u8RA;

	/*	Target Trajectory Chane Report Capability
		0 : no capability for Trajectory Change Reports
		1 : support for TC+0 reports only
		2 : support for multiple TC reports
		3 : reserved	*/
	U8						u8TC;

	/*	Target State Report Capability
		0 : no capability to support Target State Reports
		1 : capable of supporing target State Reports	*/
	U8						u8TS;

	/*	Air-Referenced Velocity Report Capability
		0 : no capability to generate ARV-reports
		1 : capable of generate ARV-reports	*/
	U8						u8ARV;

	/*	Cockpit Display of Traffic Information airborne
		0 : CDTI not operational
		1 : CDTI operational	*/
	U8						u8CDTI_A;

	/* TCAS System Status
		0 : TCAS operational
		1 : TCAS not operational	*/
	U8						u8notTCAS;

	/*	Single Antenna
		0 : Antenna Diversity
		1 : Single Antenna only	*/
	U8						u8SA;
};



struct ADSBTargetReportDescriptor
{
	/*	Address Type
		0 : 24-Bit ICAO address
		1 : Duplicate address
		2 : Surface vehicle address
		3 : Anonymous address
		4 : Reserved for future use
		5 : Reserved for future use
		6 : Reserved for future use
		7 : Reserved for future use	*/
	U8						u8ATP;

	/*	Altitude Reporting Capability
		0 : 25 ft
		1 : 100 ft
		2 : Unknown
		3 : Invalid 	*/
	U8						u8ARC;

	/*	Range Check
		0 : Default
		1 : Range Check passed, CPR Validation pending	*/
	U8						u8RC;

	/*	Report Type
		0 : Report from target transponder
		1 : Report from field monitor 	*/
	U8						u8RAB;

	/*	Differential Correction
		0 : No differential correction (ADS-B)
		1 : Differential correction (ADS-B)	*/
	U8						u8DCR;

	/*	Ground Bit Setting
		0 : Ground Bit not set
		1 : Ground Bit set 	*/
	U8						u8GBS;

	/*	Simulated Target
		0 : Actual target report
		1 : Simulated target report	*/
	U8						u8SIM;

	/*	Test Target
		0 : Default
		1 : Test Target	*/
	U8						u8TST;

	/*	Selected Altitude Available
		0 : Equipment capable to provide Selected Altitude
		1 : Equipment not capable to provide Selected Altitude	*/
	U8						u8SAA;

	/*	Confidence Level
		0 : Report valid
		1 : Report suspect
		2 : No information
		3 : Reserved for future use	*/
	U8						u8CL;

	/*	List Lookup Check
		0 : Default
		1 : List Lookup failed	*/
	U8						u8LLC;

	/*	Independent Position Check
		0 : Default
		1 : Independent Position Check failed	*/
	U8						u8IPC;

	/*  No-go Bit Status
		0 : NOGO-bit not set
		1 : NOGO-bit set	*/
	U8						u8NOGO;

	/*	Compact Position Reporting
		0 : CPR Validation correct
		1 : CPR Validation failed	*/
	U8						u8CPR;

	/*	Local Decoding Position Jump
		0 : LDPJ not detected
		1 : LDPJ detected	*/
	U8						u8LDPJ;

	/*	Range Check
		0 : Defualt
		1 : Range Check failed	*/
	U8						u8RCF;

};

struct ADSBQualityIndicators
{
	/*	Navigation Uncertainty Category for velocity or Navigation Accuracy Category for Velocity*/
	U8						u8UNCr_or_NACv;

	/* Navigation Uncertainty Category for Position NUCp or Navigation Integrity Category NIC.*/
	U8						u8NUCp_orNIC;

	/* Navigation Integrity Category for Barometric Altitude*/
	U8						u8NIC_baro;

	/* Surveillance Integrity Level*/
	U8						u8SIL;

	/* Navigation Accuracy Category for Position*/
	U8						u8NAC_p;

	/* SIL-Supplement
		0 : measured per flight-hour
		1 : measured per sample	*/
	U8						u8SILS;

	/* Horizontal Position System Design Assurance Level */
	U8						u8SDA;

	/* Geometric Altitude Accuracy */
	U8						u8GVA;
	
	/* Position Integrity Category	*/
	U8						u8PIC;

};

struct ADSBTrajectoryIntent
{
	/*	Trajectory Intent Status

		0 : Trajectory Intent Data is available for this aircraft
		1 : Trajectory Intent Data is not available for this aircraft	*/
	U8						u8NAV;

	/*	0 : Trajectory Intent Data is valid
		1 : Trajectory Intent Data is not valid	*/
	U8						u8NVB;

	/*	0 : TCP number available
		1 : TCP number not available	*/
	U8						u8TCA;

	/*	0 : TCP compliance
		1 : TCP non-compliance	*/
	U8						u8NC;

	/* Trajecotry Change Point number */
	U8						u8TCPNumber;

	F32						f32Altitude;
	F32						f32Latitude;
	F32						f32Longitude;

	/*	0 : Unknown
		1 : Fly by waypoint(LT)
		2 : Fly over waypoint(LT)
		3 : Hold pattern(LT)
		4 : Procedure hold(LT)
		5 : Procedure turn(LT)
		6 : RF leg(LT)
		7 : Top of climb(VT)
		8 : Top of descent(VT)
		9 : Start of level(VT)
		10 : Cross-over altitude(VT)
		11 : Transition altitude(VT)	*/
	U8						u8PointType;

	/*	00 : N/A
		01 : Turn right
		10 : Turn left
		11 : No turn	*/
	U8						u8TD;

	/*  Turn Radius Availabilty
		0 : TTR not available
		1 : TRR avilable	*/
	U8						u8TRA;

	/*  0 : TOV not available
		1 : TOV avilable	*/
	U8						u8TOA;

	/* Time over point	*/
	F32						f32TOV;

	/* TCP Turn radius*/
	U16						u16TTR;
};

struct ADSBTargetReportsMessage
{
	AircraftOperationalStatus	m_008sAircraftOperStatus;

	U8						m_015u8ServiceIdentification;

	/*	0 : No ADS-B Emitter Category Information
		1 : light aircraft <= 15500 lbs
		2 : 15500 lbs < small aircraft < 75000 lbs
		3 : 75000 lbs < medium a/c < 300000 lbs
		4 : High Vortex Large
		5 : 300000 lbs <= heavy aircraft
		6 : highly manoeuvrable (5g acceleration capability) and high speed (>400 knots cruise)
		7 : reserved
		8 : reserved
		9 : reserved
		10 : rotocraft
		11 : glider / sailplane
		12 : lighter-than-air
		13 : unmanned aerial vehicle
		14 : space / transatmospheric vehicle
		15 : ultralight / handglider / paraglider
		16 : parachutist / skydiver
		17 : reserved
		18 : reserved
		19 : reserved
		20 : surface emergency vehicle
		21 : surface service vehicle
		22 : fixed ground or tethered obstruction
		23 : cluster obstacle
		24 : line obstacle	*/
	U8						m_020u8EmitterCategory;

	ADSBTargetReportDescriptor	m_040sTargetReportDes;

	U16						m_070u16Mode3ACode;
	U32						m_071u32TimeOfApplicabilityForPosition;
	U32						m_072u32TimeOfApplicabilityForVelocity;
	U32						m_073u32TimeOfMessageReceptionForPosition;
	U32						m_075u32TimeOfMessageReceptionForVelocity;
	U32						m_077u32TimeOfASTERIXReportTransmission;

	U32						m_080u32TargetAddress;

	ADSBQualityIndicators	m_090sQualityIndicators;

	ADSBTrajectoryIntent	m_110sTrajectoryIntent;

	/*	X : Latitude in WGS-84
		Y : Longitude in WGS-84	*/
	F64XY					m_130sPositionInWGS84Coord;

	F64XY					m_131sHighResolutionPositionInWGS84Coord;//131메세지로 인한 추가 익 - 190708

	S32						m_140s32GeometricHeight;

	//S32						s32FlightLevel;
	F32						m_145f32FlightLevel;

	S32						m_146s32SelectedAltitude;

	U16						m_161u16TrackNumber;

	S32						m_150s32AirSpeed;

	F32						m_155f32BarometricVerticalRate;

	F32						m_157f32GeometricVerticalRate;

	/*	X : Ground Speed
		Y : Track Angle	*/
	F64XY					m_160sAirborneGroundVector;

	S8						m_170s8TargetIdentification[8];
};


// 034 Messages

struct SystemConfigurationAndStatus
{
	U8						u8COM;
	U8						u8PSR;
	U8						u8SSR;
	U8						u8MDS;

	/*	COM subfield

		0 : System is released for operational use
		1 : Operational use of System is inhibited	*/
	U8						u8COM_NOGO;

	/*	0 : RDPC-1 selected
		1 : RDPC-2 selected	*/
	U8						u8COM_RDPC;

	/*	0 : Default situation
		1 : Reset of RDPC	*/
	U8						u8COM_RDPR;

	/*	0 : Default, no overload
		1 : Overload in RDP	*/
	U8						u8COM_OVLRDP;

	/*	0 : Default, no overload
		1 : Overload in transmission sybsystem	*/
	U8						u8COM_OVLXMT;

	/*	0 : Monitoring system connected
		1 : Monitoring system disconnected	*/
	U8						u8COM_MSC;

	/*	0 : valid
		1 : invalid 	*/
	U8						u8COM_TSV;

	/*	PSR subfield
		
		0 : antenna 1
		1 : antenna 2	*/
	U8						u8PSR_ANT;

	/*	00 : No channel selected
		01 : Channel A only selectecd
		10 : Channel B only selectecd
		11 : Diversity mode (Channel A and B selected)	*/
	U8						u8PSR_CHAB;

	/*	0 : No overload
		1 : Overload	*/
	U8						u8PSR_OVL;

	/*	0 : Monitoring system connected
	1 : Monitoring system disconnected	*/
	U8						u8PSR_MSC;

	/*	SSR subfield

		0 : antenna 1
		1 : antenna 2	*/
	U8						u8SSR_ANT;

	/*	00 : No channel selected
		01 : Channel A only selectecd
		10 : Channel B only selectecd
		11 : Invalid combination	*/
	U8						u8SSR_CHAB;

	/*	0 : No overload
		1 : Overload	*/
	U8						u8SSR_OVL;

	/*	0 : Monitoring system connected
		1 : Monitoring system disconnected	*/
	U8						u8SSR_MSC;

	/*	MDS subfield

		0 : antenna 1
		1 : antenna 2	*/
	U8						u8MDS_ANT;

	/*	00 : No channel selected
		01 : Channel A only selectecd
		10 : Channel B only selectecd
		11 : Illegal combination	*/
	U8						u8MDS_CHAB;

	/*	0 : No overload
		1 : Overload	*/
	U8						u8MDS_OVLSUR;

	/*	0 : Monitoring system connected
		1 : Monitoring system disconnected	*/
	U8						u8MDS_MSC;

	/*	0 : Channel A in use
		1 : Channel B in use	*/
	U8						u8MDS_SCF;

	/*	0 : Channel A in use
		1 : Channel B in use	*/
	U8						u8MDS_DLF;
	
	/*	0 : No overload
		1 : Overload	*/
	U8						u8MDS_OVLSCF;

	/*	0 : No overload
		1 : Overload	*/
	U8						u8MDS_OVLDLF;
};

struct SystemProcessingMode
{
	U8						u8COM;
	U8						u8PSR;
	U8						u8SSR;
	U8						u8MDS;

	/*	COM subfield

		000 : No reduction active
		001 : Reduction step 1 active
		010 : Reduction step 2 active
		011 : Reduction step 3 active
		100 : Reduction step 4 active
		101 : Reduction step 5 active
		110 : Reduction step 6 active
		111 : Reduction step 7 active	*/
	U8						u8COM_REDRDP;

	/*	000 : No reduction active
		001 : Reduction step 1 active
		010 : Reduction step 2 active
		011 : Reduction step 3 active
		100 : Reduction step 4 active
		101 : Reduction step 5 active
		110 : Reduction step 6 active
		111 : Reduction step 7 active	*/
	U8						u8COM_REDXMT;

	/*	PSR subfield
		
		0 : Linear polarization
		1 : Circular polarization	*/
	U8						u8PSR_POL;

	/*	000 : No reduction active
		001 : Reduction step 1 active
		010 : Reduction step 2 active
		011 : Reduction step 3 active
		100 : Reduction step 4 active
		101 : Reduction step 5 active
		110 : Reduction step 6 active
		111 : Reduction step 7 active	*/
	U8						u8PSR_REDRAD;

	/*	00 : STC Map-1
		01 : STC Map-2
		10 : STC Map-3
		11 : STC Map-4	*/
	U8						u8PSR_STC;

	/*	SSR subfield

		000 : No reduction active
		001 : Reduction step 1 active
		010 : Reduction step 2 active
		011 : Reduction step 3 active
		100 : Reduction step 4 active
		101 : Reduction step 5 active
		110 : Reduction step 6 active
		111 : Reduction step 7 active	*/
	U8						u8SSR_REDRAD;

	/*	MDS subfield

		000 : No reduction active
		001 : Reduction step 1 active
		010 : Reduction step 2 active
		011 : Reduction step 3 active
		100 : Reduction step 4 active
		101 : Reduction step 5 active
		110 : Reduction step 6 active
		111 : Reduction step 7 active	*/
	U8						u8MDS_REDRAD;

	/*	0 : Autonomous
		1 : Not autonomous	*/
	U8						u8MDS_CLU;

};

struct MessageCountValues
{
	/*	0 : No detection (number of misses)
		1 : Single PSR target reports (Non-Mode S)
		2 : Single SSR target reports (Non-Mode S)
		3 : SSR + PSR target reports (Non-Mode S)
		4 : Single All-Call target reports (Mode S)
		5 : Single Roll-Call target reports (Mode S)
		6 : All-Call + PSR (Mode S) target reports
		7 : Roll-Call + PSR (Mode S) target reports
		8 : Filter for Weather data
		9 : Filter for Jamming strobe
		10 : Filter for PSR data
		11 : Filter for SSR/Mode S data
		12 : Filter for SSR/Mode S+PSR data
		13 : Filter for Enhanced Surveillance data
		14 : Filter for PSR+Enhanced Surveillance
		15 : Filter for PSR+Enhanced Surveillance + SSR/Mode S data not in Area of Prime Interest
		16 : Filter for PSR+Enhanced Surveillance + all SSR/Mode S data		*/
	U8						u8TYP;

	U16						u16COUNTER;
};

struct MonoRadarServiceMessage
{
	/*	1 : North marker message
		2 : Sector crossing message
		3 : Geographical filtering message
		4 : Jamming Strobe message	*/
	U8						u8MessageType;

	/* Elapsed time since last midnight (millisecond)	*/
	U32						u32TimeOfMessage;

	F32						f32AntennaRotationPeriod;
	F32						f32SectorNumber;
	SystemConfigurationAndStatus	sSystemConfigStatus;
	SystemProcessingMode			sSystemProcessMode;
	MessageCountValues				sMessageCountValue;
};
/*
struct SectorCrossingMessage
{
	
	U32						u32TimeOfMessage;

	F32						f32SectorNumber;

	SystemConfigurationAndStatus	sSystemConfigStatus;
	SystemProcessingMode			sSystemProcessMode;
	MessageCountValues				sMessageCountValue;
};*/

// 048 Messages
struct MonoRadarTargetReportDescriptor
{
	/*	000 : No detection
		001 : Single PSR detection
		010 : Single SSR detection
		011 : SSR + PSR detection
		100 : Single ModeS All-call
		101 : Single All-Call + PSR
		110 : ModeS All-Call + PSR
		111 : ModeS Roll-Call + PSR	*/
	U8						u8TYP;

	/*	0 : Actual target report
		1 : Simulated target report	*/
	U8						u8SIM;

	/*	0 : Report from RDP Chain 1
		1 : Report from RDP Chain 2	*/
	U8						u8RDP;

	/*	0 : Absence of SPI
		1 : Special Position Identification	*/
	U8						u8SPI;

	/*	0 : Report from aircraft transponder
		1 : Report from field monitor(fixed transponder)	*/
	U8						u8RAB;

	/*	0 : Real target report
		1 : Test target report	*/
	U8						u8TST;

	/*	0 : No X-Pulse present
		1 : X-Pulse present	*/
	U8						u8XPP;

	/*	0 : No military emergency
		1 : Military emergency	*/
	U8						u8ME;

	/*	0 : No military identification
		1 : Military identification	*/
	U8						u8MI;

	/*	00 : No Mode 4 interrogation
		01 : Friendly target
		10 : Unknown target
		11 : No reply	*/
	U8						u8FOE_FRI;


};

struct Mode2Code
{
	/*	0 : Code validated
		1 : Code not validated	*/
	U8						u8V;

	/*	0 : Default
		1 : Grabled code	*/
	U8						u8G;

	/*	0 : Mode-2 code derived from the reply of the transponder
		1 : Smoothed Mode-2 code as provided by local tracker	*/
	U8						u8L;

	/*	Mode-2 reply in octal representation */
	U16						u16Mode2Code;

};

struct Mode1Code
{
	/*	0 : Code validated
		1 : Code not validated	*/
	U8						u8V;

	/*	0 : Default
		1 : Grabled code	*/
	U8						u8G;

	/*	0 : Mode-1 code derived from the reply of the transponder
		1 : Smoothed Mode-1 code as provided by local tracker	*/
	U8						u8L;

	/*	Mode-1 code */
	U16						u16Mode1Code;

};

struct TrackStatus
{
	/*	Confirmed vs. Tentative Track
		0 : Confirmed Track
		1 : Tentative Track	*/
	U8						u8CNF;

	/*	Type of Sensor(s) maintaining Track
		00 : Combined Track
		01 : PSR Track
		10 : SSR/Mode S Track
		11 : Invalid	*/
	U8						u8RAD;

	/*	Signals level of confidence in plot to track association process
		0 : Normal confidence
		1 : Low confidence in plot to track association	*/
	U8						u8DOU;

	/*	Manoeuvre detection in Horizontal Sense
		0 : No horizontal man.sensed
		1 : Horizontal man.sensed	*/
	U8						u8MAH;

	/*	Climbing / Descending Mode
		00 : Maintaining
		01 : Climbing
		10 : Descending
		11 : Unknown	*/
	U8						u8CDM;

	/*	Signal for End_of_Track
		0 : Track still alive
		1 : End of track lifetime(last report for this track)	*/
	U8						u8TRE;

	/*	Ghost vs. true target
		0 : True target track
		1 : Ghost target track	*/
	U8						u8GHO;

	/*	Track maintained with track information from neighbouring Node B on the cluster, or network
		0 : no
		1 : yes	*/
	U8						u8SUP;

	/*	Type of plot coordinate transformation mechanism
		0 : Radar plane
		1 : Slant range correction 	*/
	U8						u8TCC;
};

struct CommunicationsCapability
{
	/*	Communications capability of the transponder
		0 : No communications capability(surveillance only)
		1 : Comm. A and Comm. B capability
		2 : Comm. A and Comm. B, Uplink ELM
		3 : Comm. A and Comm. B, Uplink ELM and Downlink ELM
		4 : Level 5 Transponder capability
		5 to 7 : Not assigned	*/
	U8						u8COM;

	/*	Flight Status
		0 : No alert, no SPI, aircraft airborne
		1 : No alert, no SPI, aircraft on ground
		2 : Alert, no SPI, aircraft airborne
		3 : Alert, no SPI, aircraft on ground
		4 : Alert, SPI, aircraft airborne or on ground
		5 : No alert, SPI, aircraft airborne or on ground
		6 & 7 Not assigend	*/
	U8						u8STAT;

	/*	SI/II Transponder Capability
		0 : SI-code capble
		1 : II-code capble	*/
	U8						u8SI;

	/*	Node-S Specific Service Capability 
		0 : No
		1 : Yes	*/
	U8						u8MSSC;

	/*	Altitude reporting capability
		0 : 100ft resolution
		1 : 25ft resolution	*/
	U8						u8ARC;

	/*	Aircraft identification capability
		0 : No
		1 : Yes	*/
	U8						u8AIC;

	/*	BDS 1,0 bit 16	*/
	U8						u8B1A;

	/*  BDS 1,0 bits 37/40, */
	U8						u8B1B;

};

struct MonoRadarTargetReportsMessage
{
	MonoRadarTargetReportDescriptor	sTargetReportDes;

	/*	X : RHO
		Y : Theta	*/
	F64XY					sMeasuredPositionPolarCoord;

	/*	X : X-Component
		Y : Y-Component	*/
	F64XY					sCalculatedPositionCartesianCoord;

	Mode2Code				sMode2Code;
	Mode1Code				sMode1Code;
	Mode3ACode				sMode3ACode;

	S32						s32Height;
	S32						s32Height_3DRadar;

	U32						u32TimeOfDay;

	U16						u16TrackNumber;

	TrackStatus			sTrackStatus;

	/*	X : Ground Speed
		Y : Heading	*/
	F64XY					sCalculatedTrackVelocityPolarCoord;

	U32						u32AircraftAddress;

	CommunicationsCapability	sCommunicationsACASCapability;

	S8						s8AircraftIdentification[8];
};


// 062 Messages

struct TrackMode3ACode
{
	/*	0 : Code validated
		1 : Code not validated	*/
	U8						u8V;

	/*	0 : Default
		1 : Grabled code	*/
	U8						u8G;

	/*	0 : No Change
		1 : Mode 3/A has change
	*/
	U8						u8CH;

	/*	Mode-3/A reply in octal representation */
	U16						u16Mode3A;
};

struct SystemTrackStatus
{
	/*	0 : Multisensor track
		1 : Monosensor track	*/
	U8						u8MON;

	/*	0 : Default value
		1 : SPI present in the last report received from a sensor capable of decoding this data	*/
	U8						u8SPI;

	/*	Most Reliable Height
		0 : Barometric altitude(Mode C) more reliable
		1 : Geometric altitude more reliable	*/
	U8						u8MRH;

	/*	Source of calculated track altitude for I062/130
		000 : no source
		001 : GNSS
		010 : 3D radar
	    011 : triangulation
		100 : height from coverage
		101 : speed look-up table
		110 : default height
		111 : multilateration 	*/
	U8						u8SRC;

	/*	0 : Confirmed track
		1 : Tentative track	*/
	U8						u8CNF;

	/*	0 : Actual track
		1 : Simulated track		*/
	U8						u8SIM;

	/*	0 : default value
		1 : last message transmitted to the user for the track	*/
	U8						u8TSE;

	/*	0 : default value
		1 : first message transmitted to the user for the track	*/
	U8						u8TSB;

	/*	0 : Not flight-plan correlated
		1 : Flight plan correlated	*/
	U8						u8FPC;

	/*	0 : default value
		1 : ADS-B data inconsistent with other surveillance information	*/
	U8						u8AFF;

	/*	0 : default value
		1 : Slave Track Promtion	*/
	U8						u8STP;

	/*	0 : Complementary service used 
		1 : Background service used	*/
	U8						u8KOS;

	/*	0 : track not resulting from amalgamation process
		1 : track resulting from amalgamation process	*/
	U8						u8AMA;

	/*	00 : No Mode 4 interrogation
		01 : Friendly target
		10 : Unknown target
		11 : No reply	*/
	U8						u8MD4;

	/*	0 : default value
		1 : Military Emergency present in the last report received from a sensor capable of decoding this data	*/
	U8						u8ME;

	/*	0 : default value
		1 : Military Identification present in the last report received from a sensor capable of decoding this data 	*/
	U8						u8MI;

	/*	00 : No Mode 5 interrogation
		01 : Friendly target
		10 : Unknown target
		11 : No reply	*/
	U8						u8MD5;

	/*	0 : default value
		1 : Age of the last received track update is higher than system dependent threshold(coasting)	*/
	U8						u8CST;

	/*	0 : default value
		1 : Age of the last received PSR track update is higher than system dependent threshold	*/
	U8						u8PSR;

	/*	0 : default value
		1 : Age of the last received SSR track update is higher than system dependent threshold	*/
	U8						u8SSR;

	/*	0 : default value
		1 : Age of the last received Mode S track update is higher than system dependent threshold	*/
	U8						u8MDS;

	/*	0 : default value
		1 : Age of the last received ADS-B track update is higher than system dependent threshold	*/
	U8						u8ADS;

	/*	0 : default value
		1 : Special Used Code (Mode A codes to be defined in the system to mark a track with special interest)	*/
	U8						u8SUC;

	/*	0 : default value
		1 : Assigned Mode A Code Conflict(same discrete Mode A Code assigned to another track)	*/
	U8						u8AAC;

	/*	Surveillance Data Status
		00 : Combined
		01 : Co-operative only
		10 : Non-Cooperative only
		11 : Not defined	*/
	U8						u8SDS;

	/*	Emergency Status Indication
		0 : No emergency
		1 : General emergency
		2 : Lifeguard/Medical
		3 : Minimum fuel
		4 : No communications
		5 : Unlawful interference
		6 : "Downed" Aircraft
		7 : Undefined	*/
	U8						u8EMS;

	/*	0 : No indication
		1 : Potential False Track Indication	*/
	U8						u8PFT;

	/*	0 : default value
		1 : Track created / updated with FPL Data	*/
	U8						u8FPLT;

	/*	0 : default value
		1 : Duplicate Mode 3/A Code	*/
	U8						u8DUPT;

	/*	0 : default value
		1 : Duplicate Flight Plan	*/
	U8						u8DUPF;

	/*	0 : default value
		1 : Duplicate Flight Plan due to manual correlation	*/
	U8						u8DUPM;
};

struct TargetIdentification
{
	/*	00 : Callsign or registration downlinked from target
		01 : Callsign not downlinked from target
		10 : Registration not downlinked from target
		11 : Invalid	*/
	U8						u8STI;

	S8						s8TargetCharacter[8];
};

struct TimeOfDepartureArrival
{
	/*	0 = Scheduled off-block time
		1 = Estimated off-block time
		2 = Estimated take-off time
		3 = Actual off-block time
		4 = Predicted time at runway hold
		5 = Actual time at runway hold
		6 = Actual line-up time
		7 = Actual take-off time
		8 = Estimated time of arrival
		9 = Predicted landing time
		10 = Actual landing time
		11 = Actual time off runway
		12 = Predicted time to gate
		13 = Actual on-block time	*/
	U8						u8YTP;

	/*	00 : Today
		01 : Yesterday
		10 : Tomorrow
		11 : Invalid	*/
	U8						u8DAY;

	/*	Hours, from 0 to 23	*/
	U8						u8HOR;

	/*	Minutes, from 0 to 59	*/
	U8						u8MIN;

	/*	0 : Seconds available
		1 : Seconds not available	*/
	U8						u8AVS;

	/*	Seconds, from 0 to 59	*/
	U8						u8SEC;
};

struct FlightPlanRelatedData
{
	U8						u8TAG;
	U8						u8CSN;
	U8						u8IFI;
	U8						u8FCT;
	U8						u8TAC;
	U8						u8WTC;
	U8						u8DEP;
	U8						u8DST;
	U8						u8RDS;
	U8						u8CFL;
	U8						u8CTL;
	U8						u8TOD;
	U8						u8AST;
	U8						u8STS;
	U8						u8STD;
	U8						u8STA;
	U8						u8PEM;
	U8						u8PEC;

	/*	FPPS Identification Tag	*/
	DataSourceIdentifier	sIdentificationTag;

	/*	Call Sign */
	S8						s8CallSign[7];

	/*	Flight Plan Number*/
	S32						s32PlanNumber;

	/* Flight Category 
	
		00 : Unknown
		01 : General Air Traffic 
		10 : Operational Air Traffic
		11 : Not applicable	*/
	U8						u8GATOAT;

	/*	00 : Instrument Flight Rules
		01 : Visual Flight Rules
		10 : Not applicable
		11 : Controlled Visual Flight Rules	*/
	U8						u8FR1FR2;

	/*	00 : Unknown
		01 : Approved
		10 : Exempt
		11 : Not Approved 	*/
	U8						u8RVSM;

	/*	0 : Normal Priority Flight
		1 : High Priority Flight	*/
	U8						u8HPR;

	/*	Type of Aircraft		*/
	U8						u8AircraftType[4];

	/*  Wake Turbulence Category
		L : Light
		M : Medium
		H : Heavy
		J : Super	*/
	U8						u8WakeTurbulenceCategory;

	U8						u8DepartureAirport[4];
	U8						u8DestinationAirport[4];
	
	/*	Runway Designation 	First Number*/
	U8						u8RDFirstNumber;

	/*	Runway Designation 	Second Number*/
	U8						u8RDSecondNumber;

	/*	Runway Designation 	Letter*/
	U8						u8RDLetter;

	/*	Current Cleared Flight Level	*/
	S32						s32CurrentClearedFlightLevel;

	/*	Current Control Position(Centre)	*/
	U8						u8ControlCentre;

	/*	Current Control Position(Position)	*/
	U8						u8ControlPosition;

	TimeOfDepartureArrival	sTimeOfDepartureArrival[10];

	/*	Aircraft Stand	*/
	U8						u8AircraftStand[6];

	/*	Stand Status			
		00 : Empty
		01 : Occupied
		10 : Unknown
		11 : Invalid	*/
	U8						u8SSEMP;

	/*	Stand Status	
		00 : Available
		01 : Not available
		10 : Unknown
		11 : Invalid	*/
	U8						u8SSAVL;

	/*	Standard Instrument Departure	*/
	U8						u8SID[7];

	/*	Standard Instrument Arrival	*/
	U8						u8STAR[7];

	/*	Pre-Emergency Mode-3A Validity
		0 : No valid Mode 3/A available
		1 : Valid Mode 3/A available	*/
	U8						u8Mode3AVA;

	/*	Pre-Emergency Mode-3/A reply in octal representation */
	U16						u16Mode3A;

	/*	Pre-Emergency Call Sign */
	U8						u8PreEmergencyCallSign[7];
};

struct SystemTrackMessage
{
	U8						u8ServiceIndentification;

	S32						s32TrackNumber;

	TrackMode3ACode			sTrackMode3ACode;

	U32						u32TimeOfTrackInformation;

	SystemTrackStatus		sTrackStatus;

	/*	X : X Component
		Y : Y Component	*/
	F64XY					sCalculatedTrackPositionCartesian;

	/*	X : Latitude
		Y : Longitude	*/
	F64XY					sCalculatedPositionInWGS84Coord;

	U16						u16ExtendedMode1Code;
	U16						u16TrackMode2Code;

	S32						s32CalculatedTrackGeometricAltitude;

	S32						s32CalculatedTrackBarometricAltitude;

	S32						s32MeasuredFlightLevel;

	/*	X : Vx
		Y : Vy	*/
	F64XY					sCalculatedTrackVelogityCartesian;

	/*	X : Ax
		Y : Ay	*/
	F64XY					sCalculatedAccelerationCartesian;

	F32						f32CalculatedRateOfClimbDescent;

	TargetIdentification	sTargetIndentification;

	/*	0 : Unknown 
		1 : ATC equipment maintenance 
		2 : Airport maintenance 
		3 : Fire 
		4 : Bird scarer 
		5 : Snow plough 
		6 : Runway sweeper 
		7 : Emergency 
		8 : Police 
		9 : Bus 
		10 : Tug (push/tow) 
		11 : Grass cutter 
		12 : Fuel 
		13 : Baggage 
		14 : Catering 
		15 : Aircraft maintenance 
		16 : Flyco (follow me) 	*/
	U8						u8VehicleFleetIndentification;

	U32						u32TargetAddress;

	/*
		1 : light aircraft <= 7000 kg
		2 : reserved
		3 : 7000 kg < medium aircraft < 136000 kg
		4 : reserved
		5 : 136000 kg <= heavy aircraft
		6 : highly manoeuvrable (5g acceleration capability) and high speed (> 400 knots cruise)
		7 : reserved
		8 : reserved
		9 : reserved
		10 : rotocraft
		11 : glider / sailplane
		12 : lighter-than-air
		13 : unmanned aerial vehicle
		14 : space / transatmospheric vehicle
		15 : ultralight / handglider / paraglider
		16 : parachutist / skydiver
		17 : reserved
		18 : reserved
		19 : reserved
		20 : surface emergency vehicle
		21 : surface service vehicle
		22 : fixed ground or tethered obstruction
		23 : reserved
		24 : reserved	*/
	U8						u8EmitterCategory;

	FlightPlanRelatedData	sFlightPlanRelatedData;

};
#endif

