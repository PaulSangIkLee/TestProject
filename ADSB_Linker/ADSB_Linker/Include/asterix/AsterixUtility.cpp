#include "stdafx.h"
#include "AsterixUtility.h"


CAsterixUtility::CAsterixUtility()
{
}


CAsterixUtility::~CAsterixUtility()
{
}

U8 CAsterixUtility::ICAOCharToChar(U8 u8ICAOchar)
{
	U8 u8Char = u8ICAOchar;

	/* Space */
	if (u8ICAOchar == 0)
	{
		u8Char = 0x20;
	}

	/* Letter */
	if (0 < u8ICAOchar && u8ICAOchar <= 26)
	{
		u8Char = 0x40 + u8ICAOchar;
	}

	return u8Char;
}

U8 CAsterixUtility::CharToICAOChar(U8 u8Char)
{
	U8 u8ICAOchar = u8Char;

	/* Space */
	if (u8Char == 0x20)
	{
		u8ICAOchar = 0;
	}

	/* Letter */
	if (0x40 < u8Char && u8Char <= 0x5A)
	{
		u8ICAOchar = u8Char - 0x40;
	}

	return u8ICAOchar;
}

S32 CAsterixUtility::MeterToFlightLevel4(S32 s32Meter)
{
	F64 f64Meter = s32Meter;
	F64 f64FL4 = floor(f64Meter / (30.48 * 0.25));
	S32 s32FL4 = static_cast<S32>(f64FL4);

	return s32FL4;
}

S32 CAsterixUtility::FlightLevel4ToMeter(S32 s32FlightLevel4)
{
	if (s32FlightLevel4 == 0) {
		return 0;
	}

	F64 f64FL4 = s32FlightLevel4;
	F64 f64Meters = f64FL4 * (30.48 * 0.25);
	S32 s32Meters = static_cast<S32>(floor(f64Meters + 1.0));

	return s32Meters;
}
