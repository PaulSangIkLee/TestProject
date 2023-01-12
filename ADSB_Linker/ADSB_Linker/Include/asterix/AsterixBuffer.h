#pragma once

#include "CommonDefine.h"

#define ASTERIX_BUFFER	1024


class CAsterixBuffer
{
public:
	CAsterixBuffer();
	~CAsterixBuffer();

public:
	void ClearBuffer();
	//BOOL CopyBuffer(const unsigned char* pData, long nLength, long nOffset = 0);
	S32 GetLength();
	S32 GetIndex();

	// Read Buffers
	BOOL ReadBufferU8(U8 * pu8);
	BOOL ReadBufferS8(S8 * ps8);
	BOOL ReadBufferU16(U16 * pu16);
	BOOL ReadBufferS16(S16 * ps16);
	BOOL ReadBufferU24(U32 * pu32);
	BOOL ReadBufferS24(S32 * ps32);
	BOOL ReadBufferU32(U32 * pu32);
	BOOL ReadBufferS32(S32 * ps32);
	BOOL ReadBufferU64(U64  * pu64);
	BOOL ReadBufferS64(S64  * ps64);
	BOOL ReadBufferF64(F64 * pf64);

	// Write Buffers
	BOOL WriteBufferU8(U8 u8);
	BOOL WriteBufferS8(S8 s8);
	BOOL WriteBufferU8(S32 s32Position, U8 u8);
	BOOL WriteBufferU16(U16 u16);
	BOOL WriteBufferS16(S16 s16);
	BOOL WriteBufferU24(U32 u32);
	BOOL WriteBufferS24(S32 s32);
	BOOL WriteBufferU32(U32 u32);
	BOOL WriteBufferS32(S32 s32);
	BOOL WriteBufferU64(U64 u64);
	BOOL WriteBufferF64(F64 f64);

private:
	U8			m_u8Buffer[ASTERIX_BUFFER];
	S32			m_s32Length;
	S32			m_s32Index;

};

