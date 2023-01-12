#include "stdafx.h"
#include "AsterixDecode021.h"


CAsterixDecode021::CAsterixDecode021()
{
}


CAsterixDecode021::~CAsterixDecode021()
{
}

void CAsterixDecode021::Clear()
{
	memset(m_bFSpec, 0x00, MAX_FSPEC * sizeof(BOOL));
	memset(&m_sTargetReportMsg, 0x00, sizeof(ADSBTargetReportsMessage));

}

BOOL CAsterixDecode021::DecodeMessage(CAsterixBuffer& rBuffer)
{
	U8 u8Category;
	rBuffer.ReadBufferU8(&u8Category);

	if (u8Category != 21)
		return FALSE;

	U16 u16Length = 0;

	rBuffer.ReadBufferU16(&u16Length);

	S32 s32MaxIndex = rBuffer.GetIndex() + u16Length - 3;

	while (rBuffer.GetIndex() < s32MaxIndex) {

		Clear();

		if (!DecodeFSpec(rBuffer))
			return FALSE;

		// Data Source Identifier
		if (m_bFSpec[1]) {
			I021_010(rBuffer);
		}

		// Target Report Descriptor
		if (m_bFSpec[2]) {
			I021_040(rBuffer);
		}

		// Track Number
		if (m_bFSpec[3]) {
			I021_161(rBuffer);
		}

		// Service Identification
		if (m_bFSpec[4]) {
			I021_015(rBuffer);
		}

		// Time of Applicability for Position 
		if (m_bFSpec[5]) {
			I021_071(rBuffer);
		}

		// Position in WGS-84 coordinates
		if (m_bFSpec[6]) {
			I021_130(rBuffer);
		}

		// Position in WGS-84 coordinates. high res.
		if (m_bFSpec[7]) {
			I021_131(rBuffer);
		}

		// Time of Applicability for Velocity
		if (m_bFSpec[8]) {
			I021_072(rBuffer);
		}

		// Air Speed
		if (m_bFSpec[9]) {
			I021_150(rBuffer);
		}

		// True Air Speed
		if (m_bFSpec[10]) {
			I021_151(rBuffer);
		}

		// Target Address
		if (m_bFSpec[11]) {
			I021_080(rBuffer);
		}

		// Time of Message Reception of Position
		if (m_bFSpec[12]) {
			I021_073(rBuffer);
		}

		// Time of Message Reception of Position-High Precision
		if (m_bFSpec[13]) {
			I021_074(rBuffer);
		}

		// Time of Message Reception of Velocity
		if (m_bFSpec[14]) {
			I021_075(rBuffer);
		}

		// Time of Message Reception of Velocity-High Precision
		if (m_bFSpec[15]) {
			I021_076(rBuffer);
		}

		// Geometric Height
		if (m_bFSpec[16]) {
			I021_140(rBuffer);
		}

		// Quality Indicators
		if (m_bFSpec[17]) {
			I021_090(rBuffer);
		}

		// MOPS Version
		if (m_bFSpec[18]) {
			I021_210(rBuffer);
		}

		// Mode 3/A Code
		if (m_bFSpec[19]) {
			I021_070(rBuffer);
		}

		// Roll Angle
		if (m_bFSpec[20]) {
			I021_230(rBuffer);
		}

		// Flight Level
		if (m_bFSpec[21]) {
			I021_145(rBuffer);
		}

		// Magnetic Heading
		if (m_bFSpec[22]) {
			I021_152(rBuffer);
		}

		// Target Status
		if (m_bFSpec[23]) {
			I021_200(rBuffer);
		}

		// Barometric Vertical Rate
		if (m_bFSpec[24]) {
			I021_155(rBuffer);
		}

		// Geometric Vertical Rate
		if (m_bFSpec[25]) {
			I021_157(rBuffer);
		}

		// Airborne Ground Vector
		if (m_bFSpec[26]) {
			I021_160(rBuffer);
		}

		// Target Angle Rate
		if (m_bFSpec[27]) {
			I021_165(rBuffer);
		}

		// Time of Report Transmission
		if (m_bFSpec[28]) {
			I021_077(rBuffer);
		}

		// Target Identification
		if (m_bFSpec[29]) {
			I021_170(rBuffer);
		}

		// Emitter Category
		if (m_bFSpec[30]) {
			I021_020(rBuffer);
		}

		// Met Information
		if (m_bFSpec[31]) {
			I021_220(rBuffer);
		}

		// Selected Altitude
		if (m_bFSpec[32]) {
			I021_146(rBuffer);
		}

		// Final State Selected Altitude
		if (m_bFSpec[33]) {
			I021_148(rBuffer);
		}

		// Trajectory Intent
		if (m_bFSpec[34]) {
			I021_110(rBuffer);
		}

		// Service Management
		if (m_bFSpec[35]) {
			I021_016(rBuffer);
		}

		// Aircraft Operational Status
		if (m_bFSpec[36]) {
			I021_008(rBuffer);
		}

		// Surface Capabilities and Characteristics 
		if (m_bFSpec[37]) {
			I021_271(rBuffer);
		}

		// Message Amplitude
		if (m_bFSpec[38]) {
			I021_132(rBuffer);
		}

		// Mode S MB Data
		if (m_bFSpec[39]) {
			I021_250(rBuffer);
		}

		// ACAS Resolution Advisory Report
		if (m_bFSpec[40]) {
			I021_260(rBuffer);
		}

		// Receiver ID
		if (m_bFSpec[41]) {
			I021_400(rBuffer);
		}

		// Data Ages
		if (m_bFSpec[42]) {
			I021_295(rBuffer);
		}

		// Not Used
		if (m_bFSpec[43]) {
			
		}

		// Not Used
		if (m_bFSpec[44]) {

		}

		// Not Used
		if (m_bFSpec[45]) {

		}

		// Not Used
		if (m_bFSpec[46]) {

		}

		// Not Used
		if (m_bFSpec[47]) {

		}

		// RE indicator
		if (m_bFSpec[48]) {
			I021_RE(rBuffer);
		}

		// SP indicator
		if (m_bFSpec[49]) {
			I021_SP(rBuffer);
		}
		
	}

	return TRUE;
}

ADSBTargetReportsMessage CAsterixDecode021::GetDecodeResult()
{
	return m_sTargetReportMsg;
}

void CAsterixDecode021::I021_008(CAsterixBuffer& rBuffer)
{
	U8 u8Temp;

	rBuffer.ReadBufferU8(&u8Temp);

	m_sTargetReportMsg.m_008sAircraftOperStatus.u8RA = (u8Temp & 0x80) >> 7;
	m_sTargetReportMsg.m_008sAircraftOperStatus.u8TC = (u8Temp & 0x60) >> 5;
	m_sTargetReportMsg.m_008sAircraftOperStatus.u8TS = (u8Temp & 0x10) >> 4;
	m_sTargetReportMsg.m_008sAircraftOperStatus.u8ARV = (u8Temp & 0x08) >> 3;
	m_sTargetReportMsg.m_008sAircraftOperStatus.u8CDTI_A = (u8Temp & 0x04) >> 2;
	m_sTargetReportMsg.m_008sAircraftOperStatus.u8notTCAS = (u8Temp & 0x02) >> 1;
	m_sTargetReportMsg.m_008sAircraftOperStatus.u8SA = (u8Temp & 0x01);
}

void CAsterixDecode021::I021_010(CAsterixBuffer& rBuffer)
{
	U8 u8SAC, u8SIC;

	rBuffer.ReadBufferU8(&u8SAC);
	rBuffer.ReadBufferU8(&u8SIC);

	m_sDataSourceIdentifier.u8SAC = u8SAC;
	m_sDataSourceIdentifier.u8SIC = u8SIC;
}

void CAsterixDecode021::I021_015(CAsterixBuffer& rBuffer)
{
	U8 u8ServiceIdent;

	rBuffer.ReadBufferU8(&u8ServiceIdent);

	m_sTargetReportMsg.m_015u8ServiceIdentification = u8ServiceIdent;
}

void CAsterixDecode021::I021_016(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8ServiceMgr;

	rBuffer.ReadBufferU8(&u8ServiceMgr);
}

void CAsterixDecode021::I021_020(CAsterixBuffer& rBuffer)
{
	U8 u8EmitterCategory;

	rBuffer.ReadBufferU8(&u8EmitterCategory);

	m_sTargetReportMsg.m_020u8EmitterCategory = u8EmitterCategory;
}

void CAsterixDecode021::I021_040(CAsterixBuffer& rBuffer)
{
	U8 u8Temp = 0;

	rBuffer.ReadBufferU8(&u8Temp);

	m_sTargetReportMsg.m_040sTargetReportDes.u8ATP = (u8Temp & 0xE0) >> 5;
	m_sTargetReportMsg.m_040sTargetReportDes.u8ARC = (u8Temp & 0x18) >> 3;
	m_sTargetReportMsg.m_040sTargetReportDes.u8RC = (u8Temp & 0x04) >> 2;
	m_sTargetReportMsg.m_040sTargetReportDes.u8RAB = (u8Temp & 0x02) >> 1;

	if (u8Temp & 0x01) {

		rBuffer.ReadBufferU8(&u8Temp);

		m_sTargetReportMsg.m_040sTargetReportDes.u8DCR = (u8Temp & 0x80) >> 7;
		m_sTargetReportMsg.m_040sTargetReportDes.u8GBS = (u8Temp & 0x40) >> 6;
		m_sTargetReportMsg.m_040sTargetReportDes.u8SIM = (u8Temp & 0x20) >> 5;
		m_sTargetReportMsg.m_040sTargetReportDes.u8TST = (u8Temp & 0x10) >> 4;
		m_sTargetReportMsg.m_040sTargetReportDes.u8SAA = (u8Temp & 0x08) >> 3;
		m_sTargetReportMsg.m_040sTargetReportDes.u8CL = (u8Temp & 0x06) >> 1;

	}
	
	if (u8Temp & 0x01) {
		
		rBuffer.ReadBufferU8(&u8Temp);

		m_sTargetReportMsg.m_040sTargetReportDes.u8LLC = (u8Temp & 0x40) >> 6;
		m_sTargetReportMsg.m_040sTargetReportDes.u8IPC = (u8Temp & 0x20) >> 5;
		m_sTargetReportMsg.m_040sTargetReportDes.u8NOGO = (u8Temp & 0x10) >> 4;
		m_sTargetReportMsg.m_040sTargetReportDes.u8CPR = (u8Temp & 0x08) >> 3;
		m_sTargetReportMsg.m_040sTargetReportDes.u8LDPJ = (u8Temp & 0x04) >> 2;
		m_sTargetReportMsg.m_040sTargetReportDes.u8RCF = (u8Temp & 0x02) >> 1;

	}

	while (u8Temp & 0x01) {
		rBuffer.ReadBufferU8(&u8Temp);
	}
}

void CAsterixDecode021::I021_070(CAsterixBuffer& rBuffer)
{
	U16 u16Temp, u16Code;
	U16 u16A, u16B, u16C, u16D;

	rBuffer.ReadBufferU16(&u16Temp);

	u16A = (u16Temp & 0x0E00) >> 9;
	u16B = (u16Temp & 0x01C0) >> 6;
	u16C = (u16Temp & 0x0038) >> 3;
	u16D = (u16Temp & 0x0007);

	u16Code = (u16A << 12) + (u16B << 8) + (u16C << 4) + (u16D);

	m_sTargetReportMsg.m_070u16Mode3ACode = u16Code;
}

void CAsterixDecode021::I021_071(CAsterixBuffer& rBuffer)
{
	U32 u32TimeofDay;
	F64 f64TimeofDay;
	U32 u32TimeofDayMS;

	rBuffer.ReadBufferU24(&u32TimeofDay);

	f64TimeofDay = (1.0 / 128.0) * u32TimeofDay;
	f64TimeofDay = f64TimeofDay * 1000.0;

	u32TimeofDayMS = static_cast<U32> (f64TimeofDay);

	m_sTargetReportMsg.m_071u32TimeOfApplicabilityForPosition = u32TimeofDayMS;
}

void CAsterixDecode021::I021_072(CAsterixBuffer& rBuffer)
{
	U32 u32TimeofDay;
	F64 f64TimeofDay;
	U32 u32TimeofDayMS;

	rBuffer.ReadBufferU24(&u32TimeofDay);

	f64TimeofDay = (1.0 / 128.0) * u32TimeofDay;
	f64TimeofDay = f64TimeofDay * 1000.0;

	u32TimeofDayMS = static_cast<U32> (f64TimeofDay);

	m_sTargetReportMsg.m_072u32TimeOfApplicabilityForVelocity = u32TimeofDayMS;
}

void CAsterixDecode021::I021_073(CAsterixBuffer& rBuffer)
{
	U32 u32TimeofDay;
	F64 f64TimeofDay;
	U32 u32TimeofDayMS;

	rBuffer.ReadBufferU24(&u32TimeofDay);

	f64TimeofDay = (1.0 / 128.0) * u32TimeofDay;
	f64TimeofDay = f64TimeofDay * 1000.0;

	u32TimeofDayMS = static_cast<U32> (f64TimeofDay);

	m_sTargetReportMsg.m_073u32TimeOfMessageReceptionForPosition = u32TimeofDayMS;
}

void CAsterixDecode021::I021_074(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U32 u32Temp;

	rBuffer.ReadBufferU32(&u32Temp);
}

void CAsterixDecode021::I021_075(CAsterixBuffer& rBuffer)
{
	U32 u32TimeofDay;
	F64 f64TimeofDay;
	U32 u32TimeofDayMS;

	rBuffer.ReadBufferU24(&u32TimeofDay);

	f64TimeofDay = (1.0 / 128.0) * u32TimeofDay;
	f64TimeofDay = f64TimeofDay * 1000.0;

	u32TimeofDayMS = static_cast<U32> (f64TimeofDay);

	m_sTargetReportMsg.m_075u32TimeOfMessageReceptionForVelocity = u32TimeofDayMS;
}

void CAsterixDecode021::I021_076(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U32 u32Temp;

	rBuffer.ReadBufferU32(&u32Temp);
}

void CAsterixDecode021::I021_077(CAsterixBuffer& rBuffer)
{
	U32 u32TimeofDay;
	F64 f64TimeofDay;
	U32 u32TimeofDayMS;

	rBuffer.ReadBufferU24(&u32TimeofDay);

	f64TimeofDay = (1.0 / 128.0) * u32TimeofDay;
	f64TimeofDay = f64TimeofDay * 1000.0;

	u32TimeofDayMS = static_cast<U32> (f64TimeofDay);

	m_sTargetReportMsg.m_077u32TimeOfASTERIXReportTransmission = u32TimeofDayMS;
}

void CAsterixDecode021::I021_080(CAsterixBuffer& rBuffer)
{
	U32 u32TargetAddress;

	rBuffer.ReadBufferU24(&u32TargetAddress);

	m_sTargetReportMsg.m_080u32TargetAddress = u32TargetAddress;
}

void CAsterixDecode021::I021_090(CAsterixBuffer& rBuffer)
{
	U8 u8Firstfield = 0;
	U8 u8Firstextension = 0;
	U8 u8Secondextension = 0;
	U8 u8Thirdextension = 0;

	rBuffer.ReadBufferU8(&u8Firstfield);

	m_sTargetReportMsg.m_090sQualityIndicators.u8UNCr_or_NACv  = (u8Firstfield & 0xE0) >> 5;
	m_sTargetReportMsg.m_090sQualityIndicators.u8NUCp_orNIC = (u8Firstfield & 0x1E) >> 1;

	if (u8Firstfield & 0x01)
	{
		rBuffer.ReadBufferU8(&u8Firstextension);
		
		m_sTargetReportMsg.m_090sQualityIndicators.u8NIC_baro = (u8Firstextension & 0x80) >> 7;
		m_sTargetReportMsg.m_090sQualityIndicators.u8SIL = (u8Firstextension & 0x60) >> 5;
		m_sTargetReportMsg.m_090sQualityIndicators.u8NAC_p = (u8Firstextension & 0x1E) >> 1;
	}

	if (u8Firstextension & 0x01)
	{
		rBuffer.ReadBufferU8(&u8Secondextension);

		m_sTargetReportMsg.m_090sQualityIndicators.u8SILS = (u8Secondextension & 0x20) >> 5;
		m_sTargetReportMsg.m_090sQualityIndicators.u8SDA = (u8Secondextension & 0x18) >> 3;
		m_sTargetReportMsg.m_090sQualityIndicators.u8GVA = (u8Secondextension & 0x06) >> 1;
	}

	if (u8Secondextension & 0x01)
	{
		rBuffer.ReadBufferU8(&u8Thirdextension);

		m_sTargetReportMsg.m_090sQualityIndicators.u8PIC = (u8Thirdextension & 0xF0) >> 4;
	}

}

void CAsterixDecode021::I021_110(CAsterixBuffer& rBuffer)
{
	U8 u8Temp;
	U8 u8TIS = 0;
	U8 u8TID = 0;

	rBuffer.ReadBufferU8(&u8Temp);

	u8TIS = u8Temp & 0x80;
	u8TID = u8Temp & 0x40;

	if (u8Temp & 0x01) {
		rBuffer.ReadBufferU8(&u8Temp);
	}

	if (u8TIS) {
		rBuffer.ReadBufferU8(&u8Temp);

		m_sTargetReportMsg.m_110sTrajectoryIntent.u8NAV = (u8Temp & 0x80) >> 7;
		m_sTargetReportMsg.m_110sTrajectoryIntent.u8NVB = (u8Temp & 0x40) >> 6;

		if (u8Temp & 0x01) {
			rBuffer.ReadBufferU8(&u8Temp);
		}

	}

	if (u8TID) {
		U8 u8REP;
		rBuffer.ReadBufferU8(&u8REP);

		for (U8 u8I = 0; u8I < u8REP; u8I++) {

			U16 u16Temp;
			U32 u32Temp;

			rBuffer.ReadBufferU8(&u8Temp);

			m_sTargetReportMsg.m_110sTrajectoryIntent.u8TCA = (u8Temp & 0x80) >> 7;
			m_sTargetReportMsg.m_110sTrajectoryIntent.u8NC = (u8Temp & 0x40) >> 6;
			m_sTargetReportMsg.m_110sTrajectoryIntent.u8TCPNumber = (u8Temp & 0x3F);

			rBuffer.ReadBufferU16(&u16Temp);
			m_sTargetReportMsg.m_110sTrajectoryIntent.f32Altitude = static_cast<F32>(u16Temp);

			rBuffer.ReadBufferU24(&u32Temp);
			m_sTargetReportMsg.m_110sTrajectoryIntent.f32Latitude = static_cast<F32>(u32Temp);

			rBuffer.ReadBufferU24(&u32Temp);
			m_sTargetReportMsg.m_110sTrajectoryIntent.f32Longitude = static_cast<F32>(u32Temp);

			rBuffer.ReadBufferU8(&u8Temp);
			m_sTargetReportMsg.m_110sTrajectoryIntent.u8PointType = (u8Temp & 0xF0) >> 4;
			m_sTargetReportMsg.m_110sTrajectoryIntent.u8TD = (u8Temp & 0x0C) >> 2;
			m_sTargetReportMsg.m_110sTrajectoryIntent.u8TRA = (u8Temp & 0x02) >> 1;
			m_sTargetReportMsg.m_110sTrajectoryIntent.u8TOA = (u8Temp & 0x01);

			rBuffer.ReadBufferU24(&u32Temp);
			m_sTargetReportMsg.m_110sTrajectoryIntent.f32TOV = static_cast<F32>(u32Temp);

			rBuffer.ReadBufferU16(&u16Temp);
			m_sTargetReportMsg.m_110sTrajectoryIntent.u16TTR = u16Temp;

		}

	}
}

void CAsterixDecode021::I021_130(CAsterixBuffer& rBuffer)
{
	S32 s32Temp;
	F64 f64Lati, f64Long;

	//rBuffer.ReadBufferS32(&s32Temp);
	rBuffer.ReadBufferS24(&s32Temp);

	f64Lati = static_cast<F64>(s32Temp);		/* Lalitude */
	//f64Lati = f64Lati * (180.0 / (1024.0 * 1024.0 * 32.0));
	f64Lati = f64Lati * (180.0 / (1024.0 * 1024.0 * 8.0));

	//rBuffer.ReadBufferS32(&s32Temp);
	rBuffer.ReadBufferS24(&s32Temp);
	f64Long = static_cast<F64>(s32Temp);		/* Longitude */
	//f64Long = f64Long * (180.0 / (1024.0 * 1024.0 * 32.0));
	f64Long = f64Long * (180.0 / (1024.0 * 1024.0 * 8.0));

	m_sTargetReportMsg.m_130sPositionInWGS84Coord.f64X = f64Lati;
	m_sTargetReportMsg.m_130sPositionInWGS84Coord.f64Y = f64Long;

}

void CAsterixDecode021::I021_131(CAsterixBuffer& rBuffer)  
{
	// Not Used, Only Decoding
	/*S32 s32Lat;
	S32 s32Long;

	rBuffer.ReadBufferS32(&s32Lat);
	rBuffer.ReadBufferS32(&s32Long);*/
	S32 s32Temp;
	F64 f64Lati, f64Long;

	rBuffer.ReadBufferS32(&s32Temp);
	f64Lati = static_cast<F64>(s32Temp);		/* Lalitude */
	f64Lati = f64Lati * (180.0 / (1024.0 * 1024.0 * 1024.0));

	rBuffer.ReadBufferS32(&s32Temp);
	f64Long = static_cast<F64>(s32Temp);		/* Longitude */
	f64Long = f64Long * (180.0 / (1024.0 * 1024.0 * 1024.0));

	m_sTargetReportMsg.m_131sHighResolutionPositionInWGS84Coord.f64X = f64Lati;
	m_sTargetReportMsg.m_131sHighResolutionPositionInWGS84Coord.f64Y = f64Long;
}

void CAsterixDecode021::I021_132(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8Amp;

	rBuffer.ReadBufferU8(&u8Amp);
}

void CAsterixDecode021::I021_140(CAsterixBuffer& rBuffer)
{
	S16 s16GeoAlt;

	rBuffer.ReadBufferS16(&s16GeoAlt);
//	m_sTargetReportMsg.m_140s32GeometricHeight = static_cast<S32>(s16GeoAlt * (6.25 * 0.3048));//3048을 제외하면 ft임
	m_sTargetReportMsg.m_140s32GeometricHeight = static_cast<S32>(s16GeoAlt * 6.25);
}

void CAsterixDecode021::I021_145(CAsterixBuffer& rBuffer)
{
	S16 s16FlightLevel;

	rBuffer.ReadBufferS16(&s16FlightLevel);
	//m_sTargetReportMsg.f32FlightLevel = static_cast<S32>(s16FlightLevel / 0.25);
	m_sTargetReportMsg.m_145f32FlightLevel = static_cast<F32>((float)s16FlightLevel / 4.0);
}

void CAsterixDecode021::I021_146(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U16 u16Buf;
	U8 u8SAS, u8Source;
	S32 s32ISSA;

	rBuffer.ReadBufferU16(&u16Buf);

	u8SAS = (u16Buf & 0x8000) >> 15;		/* 0 - No source information provided
											* 1 - Source information provided  */

	u8Source = (u16Buf & 0x6000) >> 13;		/* 0 - Unknown
											* 1 - Aircraft Altitude
											* 2 - FCU/MSP Selected Altitude
											* 3 - FMS Selected Altitude       */
	s32ISSA = static_cast<long>(u16Buf);
	s32ISSA = static_cast<S32>(s32ISSA * (30.48 * 0.25));		/* Intermediate State Selected Altitude */

	m_sTargetReportMsg.m_146s32SelectedAltitude = s32ISSA;//190709 익 - 멤버변수 추가
}

void CAsterixDecode021::I021_148(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U16 u16Buf;
	U8 u8MV, u8AH, u8AM;
	S32 s32FAlt;

	rBuffer.ReadBufferU16(&u16Buf);

	u8MV = (u16Buf & 0x8000) >> 15;			/* Manage Vertical Mode
											* 0 - Not active
											* 1 - Active         */

	u8AH = (u16Buf & 0x4000) >> 14;			/* Altitude Hold Mode
											* 0 - Not active
											* 1 - Active        */

	u8AM = (u16Buf & 0x2000) >> 13;			/* Approach Mode
											* 0 - Not active
											* 1 - Active        */

	s32FAlt = static_cast<S32>(u16Buf);
	s32FAlt = static_cast<S32>(s32FAlt * (30.48 * 0.25));		/* Intermediate State Selected Altitude */

}

void CAsterixDecode021::I021_150(CAsterixBuffer& rBuffer)
{
	U16 u16Buf;
	U8 u8IM;
	S32 s32AirSpeed;

	rBuffer.ReadBufferU16(&u16Buf);
	u8IM = (u16Buf & 0x8000) >> 15;			/* Air Speed
											* 0 - IAS
											* 1 - Mach         */
	u16Buf = (u16Buf & 0xEFFF);
	s32AirSpeed = static_cast<S32>(u16Buf);

	if (u8IM == 0)	{
		s32AirSpeed = static_cast<S32>(s32AirSpeed * 1852 / 16384);		/* LSB = 2^(-14)NM/s */
	}
	if (u8IM == 1)	{
		s32AirSpeed = static_cast<S32>(s32AirSpeed * 0.001);		/* LSB = 0.001 */
	}

	m_sTargetReportMsg.m_150s32AirSpeed = s32AirSpeed;
}

void CAsterixDecode021::I021_151(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U16 u16Buf;
	S32 s32TAirSpeed;

	rBuffer.ReadBufferU16(&u16Buf);
	s32TAirSpeed = static_cast<S32>(u16Buf);		/* True Air Speed LSB = 1 knot */
}

void CAsterixDecode021::I021_152(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U16 u16Buf;
	S32 s32MagneticHeading;

	rBuffer.ReadBufferU16(&u16Buf);
	s32MagneticHeading = static_cast<long>(u16Buf);		/* Magnetic Heading  LSB = 360/2^16*/ 
}

void CAsterixDecode021::I021_155(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U16 u16Buf;
	U8 u8RE;
	S32 s32BarVertRate;

	rBuffer.ReadBufferU16(&u16Buf);

	u8RE = (u16Buf & 0x8000) >> 15;
	s32BarVertRate = (u16Buf & 0x7FFF);

	//s32BarVertRate = static_cast<S32>(u16Buf);		/* Barometric Vertical Rate  LSB = 6.25 feet/minute */
	//m_sTargetReportMsg.m_155f32BarometricVerticalRate = static_cast<F32>(s32BarVertRate * (6.25 * 0.3048) / 60);//6.25만 곱셈함
	m_sTargetReportMsg.m_155f32BarometricVerticalRate = static_cast<F32>(s32BarVertRate * 6.25);
}

void CAsterixDecode021::I021_157(CAsterixBuffer& rBuffer)
{
	U16 u16Buf;
	F32 f32VertVel;

	rBuffer.ReadBufferU16(&u16Buf);

//	f32VertVel = static_cast<F32>(u16Buf * (6.25 * 0.3048) / 60); /* Geometric Vertical Rate  LSB = 6.25 feet/minute *///6.25만 곱셈
	f32VertVel = static_cast<F32>(u16Buf * 6.25);
	m_sTargetReportMsg.m_157f32GeometricVerticalRate = f32VertVel;
}

void CAsterixDecode021::I021_160(CAsterixBuffer& rBuffer)
{
	U16 u16Buf;
	S32 s32GroundVector;
	S32 s32TrackAngle;

	rBuffer.ReadBufferU16(&u16Buf);
	s32GroundVector = static_cast<S32>(u16Buf);		/* Ground Speed  LSB = 2^(-14)NM/s
														* -2NM/s < Ground Speed < 2NM/s  */
	rBuffer.ReadBufferU16(&u16Buf);
	s32TrackAngle = static_cast<S32>(u16Buf);		/* Track Angle  LSB = 360/2^16 */
	
//	m_sTargetReportMsg.m_160sAirborneGroundVector.f64X = static_cast<F64>(s32GroundVector * (1852.0 / 16384.0));//1852를 제외하면 NM단위로 됨
	m_sTargetReportMsg.m_160sAirborneGroundVector.f64X = static_cast<F64>(s32GroundVector * (1 / 16384.0));//1852를 제외하면 NM단위로 됨
	m_sTargetReportMsg.m_160sAirborneGroundVector.f64Y = static_cast<F64>(s32TrackAngle * (360.0 / 65536.0));
}

void CAsterixDecode021::I021_161(CAsterixBuffer& rBuffer)
{
	U16 u16TrackNumber;

	rBuffer.ReadBufferU16(&u16TrackNumber);
	m_sTargetReportMsg.m_161u16TrackNumber = u16TrackNumber;
}

void CAsterixDecode021::I021_165(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U16 u16Buf;
	S32 s32TrackAngleRate;

	rBuffer.ReadBufferU16(&u16Buf);
	s32TrackAngleRate = static_cast<S32>(u16Buf);		
}

void CAsterixDecode021::I021_170(CAsterixBuffer& rBuffer)
{
	U8 u8;
	U8 u8STI;
	S8 s8C1, s8C2, s8C3, s8C4, s8C5, s8C6, s8C7, s8C8;
	U8 u8B;

// 	rBuffer.ReadBufferU8(&u8);
// 	u8STI = (u8 & 0xC0);

	rBuffer.ReadBufferU8(&u8);
	s8C1 = ((u8 & 0xFC) >> 2);
	u8B = (u8 & 0x03);

	rBuffer.ReadBufferU8(&u8);
	s8C2 = (u8B << 4) + ((u8 & 0xF0) >> 4);
	u8B = (u8 & 0x0F);

	rBuffer.ReadBufferU8(&u8);
	s8C3 = (u8B << 2) + ((u8 & 0xC0) >> 6);
	s8C4 = (u8 & 0x3F);

	rBuffer.ReadBufferU8(&u8);
	s8C5 = ((u8 & 0xFC) >> 2);
	u8B = (u8 & 0x03);

	rBuffer.ReadBufferU8(&u8);
	s8C6 = (u8B << 4) + ((u8 & 0xF0) >> 4);
	u8B = (u8 & 0x0F);

	rBuffer.ReadBufferU8(&u8);
	s8C7 = (u8B << 2) + ((u8 & 0xC0) >> 6);
	s8C8 = (u8 & 0x3F);

	m_sTargetReportMsg.m_170s8TargetIdentification[0] = CAsterixUtility::ICAOCharToChar(s8C1);
	m_sTargetReportMsg.m_170s8TargetIdentification[1] = CAsterixUtility::ICAOCharToChar(s8C2);
	m_sTargetReportMsg.m_170s8TargetIdentification[2] = CAsterixUtility::ICAOCharToChar(s8C3);
	m_sTargetReportMsg.m_170s8TargetIdentification[3] = CAsterixUtility::ICAOCharToChar(s8C4);
	m_sTargetReportMsg.m_170s8TargetIdentification[4] = CAsterixUtility::ICAOCharToChar(s8C5);
	m_sTargetReportMsg.m_170s8TargetIdentification[5] = CAsterixUtility::ICAOCharToChar(s8C6);
	m_sTargetReportMsg.m_170s8TargetIdentification[6] = CAsterixUtility::ICAOCharToChar(s8C7);
	m_sTargetReportMsg.m_170s8TargetIdentification[7] = CAsterixUtility::ICAOCharToChar(s8C8);
}

void CAsterixDecode021::I021_200(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8Target;
	U8 u8ICF, u8LNAV, u8ME, u8PS, u8SS;
	
	rBuffer.ReadBufferU8(&u8Target);
	u8ICF = (u8Target & 0x80) >> 7;
	u8LNAV = (u8Target & 0x40) >> 6;
	u8ME = (u8Target & 0x20) >> 5;
	u8PS = (u8Target & 0x1C) >> 2;
	u8SS = (u8Target & 0x03);
}

void CAsterixDecode021::I021_210(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8MOPSVersion;

	rBuffer.ReadBufferU8(&u8MOPSVersion);
}

void CAsterixDecode021::I021_220(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8MetInformation;
	U8 u8WS, u8WD, u8TMP, u8TRB;


	rBuffer.ReadBufferU8(&u8MetInformation);

	u8WS = (u8MetInformation & 0x80) >> 7;			/* Wind Speed
													* 0 - Absence of Subfield #1
													* 1 - Presence of Subfield #1      */

	u8WD = (u8MetInformation & 0x40) >> 6;			/* Wind Direction
													* 0 - Absence of Subfield #2
													* 1 - Presence of Subfield #2      */

	u8TMP = (u8MetInformation & 0x20) >> 5;			/* Temperature
													* 0 - Absence of Subfield #3
													* 1 - Presence of Subfield #3      */

	u8TRB = (u8MetInformation & 0x10) >> 4;			/* Turbulence
													* 0 - Absence of Subfield #4
													* 1 - Presence of Subfield #4	   */
	if (u8MetInformation & 0x01) {
		rBuffer.ReadBufferU8(&u8MetInformation);
	}

	if (u8WS) {
		U16 u16Buf;
		S32 s32WindSpeed;
		rBuffer.ReadBufferU16(&u16Buf);
		s32WindSpeed = static_cast<long>(u16Buf);		/* Wind Speed LSB = 1 knot
														* 0 <= Wind Speed <= 300   */
	}

	if (u8WD) {
		U16 u16Buf;
		S32 s32WindDirection;
		rBuffer.ReadBufferU16(&u16Buf);
		s32WindDirection = static_cast<long>(u16Buf);	/* Wind Direction LSB = 1 degree
														* 1 <= Wind direection <= 360   */
	}

	if (u8TMP) {
		U16 u16Buf;
		F64 f64Temperature;
		rBuffer.ReadBufferU16(&u16Buf);
		f64Temperature = static_cast<F64>(u16Buf);		/* Temperature LSB = 0.25 degree C
														* -100 <= Temperature <= 100   */
		f64Temperature = f64Temperature * 0.25;
	}

	if (u8TRB) {
		U8 u8Buf;
		S32 s32Turb;
		rBuffer.ReadBufferU8(&u8Buf);
		s32Turb = static_cast<long>(u8Buf);			/* Turbulence
													* from 0 to 15   */
	}
}

void CAsterixDecode021::I021_230(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U16 u16RollAngle;
	S32 s32RollAngle;

	rBuffer.ReadBufferU16(&u16RollAngle);
	s32RollAngle = static_cast<long>(u16RollAngle);	/* Roll Angle LSB = 0.01 degree
													* -180 <= Roll Angle <= 180  */
}

void CAsterixDecode021::I021_250(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8REP;
	rBuffer.ReadBufferU8(&u8REP);

	for (U8 u8I = 0; u8I < u8REP; u8I++) {
		U64 u64Temp;
		rBuffer.ReadBufferU64(&u64Temp);
	}
}

void CAsterixDecode021::I021_260(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8Temp;
	
	rBuffer.ReadBufferU8(&u8Temp);
	rBuffer.ReadBufferU8(&u8Temp);
	rBuffer.ReadBufferU8(&u8Temp);
	rBuffer.ReadBufferU8(&u8Temp);
	rBuffer.ReadBufferU8(&u8Temp);
	rBuffer.ReadBufferU8(&u8Temp);
	rBuffer.ReadBufferU8(&u8Temp);
}

void CAsterixDecode021::I021_271(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8Temp;

	rBuffer.ReadBufferU8(&u8Temp);

	while (u8Temp & 0x01) {
		rBuffer.ReadBufferU8(&u8Temp);
	}
}

void CAsterixDecode021::I021_295(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8Temp;
	U8 u8AOS, u8TRD, u8M3A, u8QI, u8TI, u8MAM, u8GH;
	U8 u8FL, u8ISA, u8FSA, u8AS, u8TAS, u8MH, u8BVR;
	U8 u8GVR, u8GV, u8TAR, u8TI2, u8TS, u8MET, u8ROA;
	U8 u8ARA, u8SCC;

	u8AOS = u8TRD = u8M3A = u8QI = u8TI = u8MAM = u8GH = 0;
	u8FL = u8ISA = u8FSA = u8AS = u8TAS = u8MH = u8BVR = 0;
	u8GVR = u8GV = u8TAR = u8TI = u8TS = u8MET = u8ROA = 0;
	u8ARA = u8SCC = 0;

	rBuffer.ReadBufferU8(&u8Temp);

	u8AOS	= (u8Temp & 0x80) >> 7;
	u8TRD	= (u8Temp & 0x40) >> 6;
	u8M3A	= (u8Temp & 0x20) >> 5;
	u8QI	= (u8Temp & 0x10) >> 4;
	u8TI	= (u8Temp & 0x08) >> 3;
	u8MAM	= (u8Temp & 0x04) >> 2;
	u8GH	= (u8Temp & 0x02) >> 1;

	if (u8Temp & 0x01) {
		rBuffer.ReadBufferU8(&u8Temp);

		u8FL	= (u8Temp & 0x80) >> 7;
		u8ISA	= (u8Temp & 0x40) >> 6;
		u8FSA	= (u8Temp & 0x20) >> 5;
		u8AS	= (u8Temp & 0x10) >> 4;
		u8TAS	= (u8Temp & 0x08) >> 3;
		u8MH	= (u8Temp & 0x04) >> 2;
		u8BVR	= (u8Temp & 0x02) >> 1;
	}

	if (u8Temp & 0x01) {
		rBuffer.ReadBufferU8(&u8Temp);

		u8GVR	= (u8Temp & 0x80) >> 7;
		u8GV	= (u8Temp & 0x40) >> 6;
		u8TAR	= (u8Temp & 0x20) >> 5;
		u8TI2	= (u8Temp & 0x10) >> 4;
		u8TS	= (u8Temp & 0x08) >> 3;
		u8MET	= (u8Temp & 0x04) >> 2;
		u8ROA	= (u8Temp & 0x02) >> 1;
	}

	if (u8Temp & 0x01) {
		rBuffer.ReadBufferU8(&u8Temp);

		u8ARA	= (u8Temp & 0x80) >> 7;
		u8SCC	= (u8Temp & 0x40) >> 6;
	}

	if (u8AOS) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8TRD) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8M3A) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8QI) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8TI) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8MAM)
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8GH)
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8FL)
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8ISA)
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8FSA)
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8AS)
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8TAS)
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8MH) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8BVR) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8GVR) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8GV) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8TAR) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8TI2) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8TS) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8MET) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8ROA) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8ARA) 
		rBuffer.ReadBufferU8(&u8Temp);

	if (u8SCC) 
		rBuffer.ReadBufferU8(&u8Temp);

}

void CAsterixDecode021::I021_400(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8ReceiverID;

	rBuffer.ReadBufferU8(&u8ReceiverID);
}

void CAsterixDecode021::I021_RE(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8Len;
	rBuffer.ReadBufferU8(&u8Len);

	for (U8 u8 = 1; u8 < u8Len; u8++) {
		U8 u8Byte;
		rBuffer.ReadBufferU8(&u8Byte);
	}
}

void CAsterixDecode021::I021_SP(CAsterixBuffer& rBuffer)
{
	// Not Used, Only Decoding
	U8 u8Len;
	rBuffer.ReadBufferU8(&u8Len);

	for (U8 u8 = 1; u8 < u8Len; u8++) {
		U8 u8Byte;
		rBuffer.ReadBufferU8(&u8Byte);
	}
}

