#ifndef _DEFINE_H_
#define _DEFINE_H_

#define MAX_CHAR_LENGTH		1024
#define MAX_WCHAR_LENGTH	2050

//#define V_LINUX			//	定义平台信息（不定义则为Windows）

#ifdef V_LINUX
#define stricmp strcasecmp
#endif

#ifdef V_LINUX
#define		CHINESE_CHAR_WIDTH	2
#else
#define		CHINESE_CHAR_WIDTH	2
#endif

//	标注类型枚举
enum ELABELTYPE
{
	ePoiLabel = 0,
	eLineBrand,
	eLineText,
	eLineArrow,
	eLineAll,		//	LineBrand + LineText + LineArrow (道路图层规则为此类型)
};

#define		CHINA		"china"
#define		ALLCITY		"allcity"

#define		LABEL_COUNT		4			//	标注类型个数: PoiLabel, LineText, LineBrand, LineArrow

#define		MIN_LEVEL	3				//	最小比例尺级别
#define		MAX_LEVEL	19				//	最大比例尺级别

#define		CHINA_MIN_LEVEL		MIN_LEVEL		//	全国包最小级别
#define		CHINA_MAX_LEVEL		9				//	全国包最大级别
#define		CITY_MIN_LEVEL		10				//	城市包最小级别
#define		CITY_MAX_LEVEL		MAX_LEVEL		//	城市包最大级别

enum ELABELKEY			//	标注mid文件Key字段枚举
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

//	错误码定义
#define CK_UNDEFINED				-1			//	未定义
#define CK_OK						0			//	成功
#define CK_UNEXCEPTED_ERROR			1			//	未知错误

#define CK_MDF_FILE_ERROR			10			//	MidMif文件错误
#define CK_RULE_FILE_ERROR			11			//	规则文件错误
#define CK_REPORT_FILE_ERROR		12			//	report文件错误
#define CK_MDF_ERROR				13			//	MidMif文件内容错误
#define CK_RULE_ERROR				14			//	规则文件内容错误

#define CK_PASSED					20			//	规则通过
#define CK_UNSURE					21			//	规则无法确定是否通过
#define CK_EXIST_ERROR				22			//	查找项失败
#define CK_ASSERT_ERROR				23			//	验证项失败

//	规则文件类型枚举
enum ERULETYPE
{
	eruleVIPPoi = 0,				//	重要POI
	eruleSubwayLine,				//	地铁线
	eruleSubwayStation,				//	地铁站
	erulePOILayer,					//	POI图层
	eruleRoadLayer,					//	道路图层
	eruleLineBrand,					//	道路标牌
	eruleLineArrow,					//	道路箭头
};


#define SubWayStation_Normal_Building_Catalog	"010A0304"			//	在建普通地铁站catalog
#define SubWayStation_Normal_Builded_Catalog	"010A0301"			//	建成普通地铁站catalog
#define SubWayStation_Trans_Building_Catalog	"010A0306"			//	在建换乘地铁站catalog
#define SubWayStation_Trans_Builded_Catalog		"010A0303"			//	建成换乘地铁站catalog

#define SubWayLine_Building_Catalog		"050204"					//	在建地铁线catalog
#define SubWayLine_Builded_Catalog		"050202"					//	建成地铁线catalog

#endif	//	_DEFINE_H_
