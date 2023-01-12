#pragma once

#include "CommonDefine.h"

class CAsterixUtility
{
public:
	CAsterixUtility();
	virtual ~CAsterixUtility();

	static U8 ICAOCharToChar(U8 u8ICAOchar);
	static U8 CharToICAOChar(U8 u8Char);

	static S32 MeterToFlightLevel4(S32 s32Meter);
	static S32 FlightLevel4ToMeter(S32 s32FlightLevel4);

};

