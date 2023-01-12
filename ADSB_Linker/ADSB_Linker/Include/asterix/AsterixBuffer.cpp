#include "stdafx.h"
#include "AsterixBuffer.h"


CAsterixBuffer::CAsterixBuffer()
{
	m_s32Length = 0;
	m_s32Index = 0;
}


CAsterixBuffer::~CAsterixBuffer()
{
}

void CAsterixBuffer::ClearBuffer()
{
	m_s32Length = 0;
	m_s32Index = 0;
}

S32 CAsterixBuffer::GetLength()
{
	return m_s32Length;
}

S32 CAsterixBuffer::GetIndex()
{
	return m_s32Index;
}

BOOL CAsterixBuffer::ReadBufferU8(U8 * pu8)
{
	if (m_s32Index < 0 || m_s32Index >= m_s32Length) {
		return FALSE;
	}

	*pu8 = m_u8Buffer[m_s32Index++];

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferS8(S8 * ps8)
{
	U8 u8Temp;

	if (!ReadBufferU8(&u8Temp)) {
		return FALSE;
	}

	*ps8 = static_cast<S8>(u8Temp);

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferU16(U16 * pu16)
{
	*pu16 = 0;

	for (S16 i = 0; i < 2; i++) {
		
		U8 u8Temp;

		if (!ReadBufferU8(&u8Temp)) {
			return FALSE;
		}

		*pu16 = (*pu16 << 8) + u8Temp;
	}

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferS16(S16 * ps16)
{
	U16 u16Temp;

	if (!ReadBufferU16(&u16Temp)) {
		return FALSE;
	}

	*ps16 = static_cast<S16>(u16Temp);

	return TRUE;

}

BOOL CAsterixBuffer::ReadBufferU24(U32 * pu32)
{
	*pu32 = 0;

	for (S16 i = 0; i < 3; i++)
	{
		U8 u8Temp;

		if (!ReadBufferU8(&u8Temp)) {
			return FALSE;
		}

		*pu32 = (*pu32 << 8) + u8Temp;
	}

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferS24(S32 * ps32)
{
	U32 u32Temp;

	if (!ReadBufferU24(&u32Temp)) {
		return FALSE;
	}

	*ps32 = static_cast<S32>(u32Temp);

	if (u32Temp & 0x00800000) {
		*ps32 |= 0xFF000000;
	}

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferU32(U32 * pu32)
{
	*pu32 = 0;

	for (S16 i = 0; i < 4; i++)
	{
		U8 u8Temp;

		if (!ReadBufferU8(&u8Temp)) {
			return FALSE;
		}

		*pu32 = (*pu32 << 8) + u8Temp;
	}

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferS32(S32 * ps32)
{
	U32 u32Temp;

	if (!ReadBufferU32(&u32Temp)) {
		return FALSE;
	}

	*ps32 = static_cast<S32>(u32Temp);

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferU64(U64  * pu64)
{
	pu64 = 0;

	for (S16 i = 0; i < 8; i++)
	{
		U8 u8Temp;

		if (!ReadBufferU8(&u8Temp)) {
			return FALSE;
		}

		*pu64 = (*pu64 << 8) + u8Temp;
	}

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferS64(S64  * ps64)
{
	U64 u64Temp;

	if (!ReadBufferU64(&u64Temp)) {
		return FALSE;
	}

	*ps64 = static_cast<S64>(u64Temp);

	return TRUE;
}

BOOL CAsterixBuffer::ReadBufferF64(F64 * pf64)
{
	*pf64 = 0; 
	
	uval uv;
	uv.s64 = 0;

	for (S8 i = 7; i >= 0; i--)
	{
		U8 u8Temp;

		if (!ReadBufferU8(&u8Temp))	{
			return FALSE;
		}

		uv.s8[static_cast<U8>(i)] = u8Temp;
	}

	*pf64 = uv.f64;

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferU8(U8 u8)
{
	if (m_s32Length < 0 || m_s32Length >= ASTERIX_BUFFER) {
		return FALSE;
	}

	m_u8Buffer[m_s32Length++] = u8;

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferS8(S8 s8)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>(s8);
	
	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferU8(S32 s32Position, U8 u8)
{
	if (s32Position < 0 || s32Position >= ASTERIX_BUFFER) {
		return FALSE;
	}

	m_u8Buffer[s32Position] = u8;

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferU16(U16 u16)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>(u16 >> 8);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>(u16 & 0x00FF);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferS16(S16 s16)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>(s16 >> 8);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>(s16 & 0x00FF);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferU24(U32 u32)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>((u32 & 0x00FF0000) >> 16);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u32 & 0x0000FF00) >> 8);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>(u32 & 0x000000FF);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferS24(S32 s32)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>((s32 & 0x00FF0000) >> 16);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((s32 & 0x0000FF00) >> 8);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>(s32 & 0x000000FF);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferU32(U32 u32)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>(u32 >> 24);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u32 & 0x00FF0000) >> 16);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u32 & 0x0000FF00) >> 8);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>(u32 & 0x000000FF);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferS32(S32 s32)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>(s32 >> 24);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((s32 & 0x00FF0000) >> 16);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((s32 & 0x0000FF00) >> 8);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>(s32 & 0x000000FF);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferU64(U64 u64)
{
	U8 u8Temp;

	u8Temp = static_cast<U8>(u64 >> 56);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}


	u8Temp = static_cast<U8>((u64 & 0x00FF000000000000LL) >> 48);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u64 & 0x0000FF0000000000LL) >> 40);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u64 & 0x000000FF00000000LL) >> 32);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u64 & 0x00000000FF000000LL) >> 24);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u64 & 0x0000000000FF0000LL) >> 16);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>((u64 & 0x000000000000FF00LL) >> 8);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	u8Temp = static_cast<U8>(u64 & 0x00000000000000FFLL);

	if (!WriteBufferU8(u8Temp)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CAsterixBuffer::WriteBufferF64(F64 f64)
{
	uval uv;
	uv.f64 = f64;

	for (S8 i = 7; i >= 0; i--)
	{
		WriteBufferS8(uv.s8[static_cast<U8>(i)]);
	}

	return TRUE;
}