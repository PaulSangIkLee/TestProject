//###########################################################################
//
// FILE:    EndianControl.h
//
// DESCRIPTION: 
//		����ϴ� ���μ����� Endian üũ �� �ʿ信 ���� Endian ��ȯ ���࿡ ����ϴ� Ŭ����
//		
//		*Ŭ���� ��� ����
//		ex)
//			CEndianControl cEncidnaControl;
//			bool bProcessorEndian = 0;
//
//			bProcessorEndian = cEncidnaControl.IsLittleEndian();
//			
//			int nData = 1000; // endian ��ȯ�� ������
//
//			//���μ����� Little Endian �� ��� �����͸� Big Endian ���� ��ȯ
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
	* ��Ʈ������ Ȯ���ϱ� ���� ����ü
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
*  (1) ���
* - Endian ��ȯ�� �����ϴ� Ŭ���� 
*/
extern "C" class CEndianControl
{
public:
	/** 
		CEndianControl ������
		
	    @param		void
	    
	    @return		
	    
	    @remarks
	*/
	CEndianControl(void);

	/** 
		CEndianControl �Ҹ���
		
	    @param		void
	    
	    @return		
	    
	    @remarks
	*/
	~CEndianControl(void);

	/** 
		����� ��ȯ ���� �Լ�
		
	    @param		void * pvData ����� ��ȯ�� ������ ������ ������
	    @param		int nByteSize ����� ��ȯ�� ������ �������� byte Size
	    
	    @return		bool true:���� / false:����
	    
	    @remarks
	*/
	static bool SwitchEndian( void *pvData, int nByteSize );

	// ����ϴ� ���μ����� LittleEndian�� ��� 1(DEFINE_LITTLE_ENDIAN), BigEndian�� ��� 0(DEFINE_BIG_ENDIAN)
	
	/** 
		����ϴ� ���μ����� Endian üũ�Լ�
			    
	    @return		bool 1: DEFINE_LITTLE_ENDIAN. ����ϴ� ���μ����� LittleEndian�� ���
	                     0: DEFINE_BIG_ENDIAN. ����ϴ� ���μ����� BigEndian�� ���
	    @remarks
	*/
	static bool IsLittleEndian();

	/** 
		Bit field�� �޸� �迭 ������ Ȯ���ϴ� �Լ�
			    
		@return		bool ����ϴ� �����Ϸ��� �������������񿡼� ������ ASCENDING_BIT_ORDER�� ��� 1,
					     ����ϴ� �����Ϸ��� �������������񿡼� ������ DESCENDING_BIT_ORDER�� ��� 0�� return�Ѵ�.
	    
	    @remarks
	*/
	bool IsAscendingBitOrder();
};

