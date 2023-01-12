#include "stdafx.h"
#include "AsterixDecode.h"


CAsterixDecode::CAsterixDecode()
{
}


CAsterixDecode::~CAsterixDecode()
{
}

BOOL CAsterixDecode::DecodeFSpec(CAsterixBuffer& rBuffer)
{
	U8 u8FSPEC;
	S16 s16Index = 0;

	do 
	{
		rBuffer.ReadBufferU8(&u8FSPEC);

		for (S16 i = 0; i < 7; i++)
		{
			++s16Index;

			if (s16Index >= MAX_FSPEC) {
				return FALSE;
			}

			if (u8FSPEC & (0x80 >> i)) {
				m_bFSpec[s16Index] = TRUE;
			}
		}
	} while (u8FSPEC & 0x01);

	return TRUE;
}
