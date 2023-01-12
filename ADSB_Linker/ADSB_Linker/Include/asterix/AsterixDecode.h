#pragma once


#include "CommonDefine.h"
#include "AsterixBuffer.h"
#include "AsterixUtility.h"

using namespace std;

#define MAX_FSPEC		128

class CAsterixDecode
{
public:
	CAsterixDecode();
	virtual ~CAsterixDecode();


protected:
	BOOL DecodeFSpec(CAsterixBuffer& rBuffer);
	virtual void Clear() = 0;

	BOOL m_bFSpec[MAX_FSPEC];
	DataSourceIdentifier	m_sDataSourceIdentifier;
};

