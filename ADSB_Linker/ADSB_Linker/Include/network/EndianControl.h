//###########################################################################
//
// FILE:    EndianControl.h
//
// DESCRIPTION: 
//		사용하는 스로세서의 Endian 체크 후 필요에 따라 Endian 변환 수행에 사용하는 클래스
//		
//		*클래스 사용 예시
//		ex)
//			CEndianControl cEncidnaControl;
//			bool bProcessorEndian = 0;
//
//			bProcessorEndian = cEncidnaControl.IsLittleEndian();
//			
//			int nData = 1000; // endian 변환할 데이터
//
//			//프로세서가 Little Endian 인 경우 데이터를 Big Endian 으로 변환
//			if(bProcessorEndian == DEFINE_LITTLE_ENDIAN)
//			{
//				cEncidnaControl.SwitchEndian(&nData, sizeof(nData));
//			}
//			
//==========================================================================

#pragma once
#define DEFINE_LITTLE_ENDIAN 1
#define DEFINE_BIG_ENDIAN 0
#define DEFINE_ASCENDING_BIT_ORDER 1
#define DEFINE_DESCENDING_BIT_ORDER 0

#pragma pack( push, 1 )
	/**
	* 비트오더를 확인하기 위한 구조체
	*/
	struct TBitOrderCheckStruct
	{
		union
		{
			unsigned char unchData;
			struct
			{
				unsigned char unchBit1 : 1;
				unsigned char unchBit7 : 7;
			} BIT;
		};
	};
#pragma pack( pop )

/**
*  (1) 기능
* - Endian 변환을 수행하는 클래스 
*/
extern "C" class CEndianControl
{
public:
	/** 
		CEndianControl 생성자
		
	    @param		void
	    
	    @return		
	    
	    @remarks
	*/
	CEndianControl(void);

	/** 
		CEndianControl 소멸자
		
	    @param		void
	    
	    @return		
	    
	    @remarks
	*/
	~CEndianControl(void);

	/** 
		엔디안 변환 수행 함수
		
	    @param		void * pvData 엔디안 변환을 수행할 데이터 포인터
	    @param		int nByteSize 엔디안 변환을 수행할 데이터의 byte Size
	    
	    @return		bool true:성공 / false:실패
	    
	    @remarks
	*/
	static bool SwitchEndian( void *pvData, int nByteSize );

	// 사용하는 프로세서가 LittleEndian일 경우 1(DEFINE_LITTLE_ENDIAN), BigEndian일 경우 0(DEFINE_BIG_ENDIAN)
	
	/** 
		사용하는 프로세서의 Endian 체크함수
			    
	    @return		bool 1: DEFINE_LITTLE_ENDIAN. 사용하는 프로세서가 LittleEndian일 경우
	                     0: DEFINE_BIG_ENDIAN. 사용하는 프로세서가 BigEndian일 경우
	    @remarks
	*/
	static bool IsLittleEndian();

	/** 
		Bit field의 메모리 배열 순서를 확인하는 함수
			    
		@return		bool 사용하는 컴파일러가 연동설계검증장비에서 정의한 ASCENDING_BIT_ORDER일 경우 1,
					     사용하는 컴파일러가 연동설계검증장비에서 정의한 DESCENDING_BIT_ORDER일 경우 0을 return한다.
	    
	    @remarks
	*/
	bool IsAscendingBitOrder();
};

