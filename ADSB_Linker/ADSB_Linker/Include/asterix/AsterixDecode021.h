#pragma once

#include "AsterixDecode.h"

class CAsterixDecode021 : public CAsterixDecode
{
public:
	CAsterixDecode021();
	virtual ~CAsterixDecode021();

	BOOL DecodeMessage(CAsterixBuffer& rBuffer);

	ADSBTargetReportsMessage GetDecodeResult();

protected:
	void Clear();

	void I021_008(CAsterixBuffer& rBuffer);
	void I021_010(CAsterixBuffer& rBuffer);
	void I021_015(CAsterixBuffer& rBuffer);
	void I021_016(CAsterixBuffer& rBuffer);
	void I021_020(CAsterixBuffer& rBuffer);
	void I021_040(CAsterixBuffer& rBuffer);
	void I021_070(CAsterixBuffer& rBuffer);
	void I021_071(CAsterixBuffer& rBuffer);
	void I021_072(CAsterixBuffer& rBuffer);
	void I021_073(CAsterixBuffer& rBuffer);
	void I021_074(CAsterixBuffer& rBuffer);
	void I021_075(CAsterixBuffer& rBuffer);
	void I021_076(CAsterixBuffer& rBuffer);
	void I021_077(CAsterixBuffer& rBuffer);
	void I021_080(CAsterixBuffer& rBuffer);
	void I021_090(CAsterixBuffer& rBuffer);
	void I021_110(CAsterixBuffer& rBuffer);
	void I021_130(CAsterixBuffer& rBuffer);
	void I021_131(CAsterixBuffer& rBuffer);
	void I021_132(CAsterixBuffer& rBuffer);
	void I021_140(CAsterixBuffer& rBuffer);
	void I021_145(CAsterixBuffer& rBuffer);
	void I021_146(CAsterixBuffer& rBuffer);
	void I021_148(CAsterixBuffer& rBuffer);
	void I021_150(CAsterixBuffer& rBuffer);
	void I021_151(CAsterixBuffer& rBuffer);
	void I021_152(CAsterixBuffer& rBuffer);
	void I021_155(CAsterixBuffer& rBuffer);
	void I021_157(CAsterixBuffer& rBuffer);
	void I021_160(CAsterixBuffer& rBuffer);
	void I021_161(CAsterixBuffer& rBuffer);
	void I021_165(CAsterixBuffer& rBuffer);
	void I021_170(CAsterixBuffer& rBuffer);
	void I021_200(CAsterixBuffer& rBuffer);
	void I021_210(CAsterixBuffer& rBuffer);
	void I021_220(CAsterixBuffer& rBuffer);
	void I021_230(CAsterixBuffer& rBuffer);
	void I021_250(CAsterixBuffer& rBuffer);
	void I021_260(CAsterixBuffer& rBuffer);
	void I021_271(CAsterixBuffer& rBuffer);
	void I021_295(CAsterixBuffer& rBuffer);
	void I021_400(CAsterixBuffer& rBuffer);
	void I021_RE(CAsterixBuffer& rBuffer);
	void I021_SP(CAsterixBuffer& rBuffer);


private:
	ADSBTargetReportsMessage	m_sTargetReportMsg;

};

