#ifndef _DEFINE_H_
#define _DEFINE_H_

#define MAX_CHAR_LENGTH		1024
#define MAX_WCHAR_LENGTH	2050

//#define V_LINUX			//	����ƽ̨��Ϣ����������ΪWindows��

#ifdef V_LINUX
#define stricmp strcasecmp
#endif

#ifdef V_LINUX
#define		CHINESE_CHAR_WIDTH	2
#else
#define		CHINESE_CHAR_WIDTH	2
#endif

//	��ע����ö��
enum ELABELTYPE
{
	ePoiLabel = 0,
	eLineBrand,
	eLineText,
	eLineArrow,
	eLineAll,		//	LineBrand + LineText + LineArrow (��·ͼ�����Ϊ������)
};

#define		CHINA		"china"
#define		ALLCITY		"allcity"

#define		LABEL_COUNT		4			//	��ע���͸���: PoiLabel, LineText, LineBrand, LineArrow

#define		MIN_LEVEL	3				//	��С�����߼���
#define		MAX_LEVEL	19				//	�������߼���

#define		CHINA_MIN_LEVEL		MIN_LEVEL		//	ȫ������С����
#define		CHINA_MAX_LEVEL		9				//	ȫ������󼶱�
#define		CITY_MIN_LEVEL		10				//	���а���С����
#define		CITY_MAX_LEVEL		MAX_LEVEL		//	���а���󼶱�

enum ELABELKEY			//	��עmid�ļ�Key�ֶ�ö��
{
	eGuid = 0,
	eName,
	eCatalog,
	eRank,
	eDirection,
	eStrgroup,
	eShowtype,
	eDatatype,
	eAngle,
	eRotate,
};

//	�����붨��
#define CK_UNDEFINED				-1			//	δ����
#define CK_OK						0			//	�ɹ�
#define CK_UNEXCEPTED_ERROR			1			//	δ֪����

#define CK_MDF_FILE_ERROR			10			//	MidMif�ļ�����
#define CK_RULE_FILE_ERROR			11			//	�����ļ�����
#define CK_REPORT_FILE_ERROR		12			//	report�ļ�����
#define CK_MDF_ERROR				13			//	MidMif�ļ����ݴ���
#define CK_RULE_ERROR				14			//	�����ļ����ݴ���

#define CK_PASSED					20			//	����ͨ��
#define CK_UNSURE					21			//	�����޷�ȷ���Ƿ�ͨ��
#define CK_EXIST_ERROR				22			//	������ʧ��
#define CK_ASSERT_ERROR				23			//	��֤��ʧ��

//	�����ļ�����ö��
enum ERULETYPE
{
	eruleVIPPoi = 0,				//	��ҪPOI
	eruleSubwayLine,				//	������
	eruleSubwayStation,				//	����վ
	erulePOILayer,					//	POIͼ��
	eruleRoadLayer,					//	��·ͼ��
	eruleLineBrand,					//	��·����
	eruleLineArrow,					//	��·��ͷ
};


#define SubWayStation_Normal_Building_Catalog	"010A0304"			//	�ڽ���ͨ����վcatalog
#define SubWayStation_Normal_Builded_Catalog	"010A0301"			//	������ͨ����վcatalog
#define SubWayStation_Trans_Building_Catalog	"010A0306"			//	�ڽ����˵���վcatalog
#define SubWayStation_Trans_Builded_Catalog		"010A0303"			//	���ɻ��˵���վcatalog

#define SubWayLine_Building_Catalog		"050204"					//	�ڽ�������catalog
#define SubWayLine_Builded_Catalog		"050202"					//	���ɵ�����catalog

#endif	//	_DEFINE_H_
