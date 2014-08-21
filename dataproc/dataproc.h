#ifndef _DATA_PROC_H_
#define _DATA_PROC_H_

#include <stdio.h>
#include <vector>
#include <string.h>

using namespace std;

#define  ERRORNUM		0xFFFFFFFF
#define	 STRICTCOEFF	0.7

//	肖维勒系数表
const double ChauvenetC[] = { 0,
	0.5, 0.71, 1.38, 1.54, 1.65, 1.73, 1.8, 1.86, 1.92, 1.96,		//	1~10
	2, 2.03, 2.07, 2.1, 2.13, 2.15, 2.18, 2.2, 2.22, 2.24,			//	11~20
	2.33,															//	25
	2.39,															//	30
	2.49,															//	40
	2.58,															//	50
	2.8,															//	100
	3																//	185
};

//	数据结构
class CData
{
public:
	void SetData( double dValue, int iTrMd = 0 );
	void Transfer();
	void PrintToScreen();

private:
	void Transfer_Sqrt();
	void Transfer_ArcSin();
	void Transfer_log();

public:
	double	m_dValue;					//	真实值
	double	m_dTransVal;				//	转换后值
	bool	m_bTransed;					//	是否已转换
	int		m_iStatus;					//	状态 (第?轮被剔除, 0:保留)
	int		m_iTransMethod;				//	转换方式 ( 0:不转换; 1:加1开方; 2:开方取反正弦; 3:加1取对数 )
};


class CDataProc
{
public:
	CDataProc();
	~CDataProc();

public:
	void SetStrategy( int iStgType = 0, int wParam = 1, double lParam = 0 );
	void SetTransMethod( int iTrMd = 0 );
	void SetStrictCoeff( double dSc = STRICTCOEFF );

	bool Proc();

	bool ReadDataFromFile( string strFile );
	bool WriteToFile( string strFile );
	void PrintToScreen();
	void PrintRemainData();

	void GenerateData( vector<double> arrData );
	double GetAverage();

private:
	bool SingleProc();
	bool CheckFinish();

	bool CheckFinish_LoopCount( int iCount );
	bool CheckFinish_DeleteCount( int iCount );
	bool CheckFinish_DeletePercent( double dPercent );
	bool CheckFinish_Fluctuation( double dPercent );
	bool CheckFinish_AvrChange( double dPercent );
	bool CheckFinish_LastLoopDelCount( int iCount );

	double GetStandardError();
	double GetMaxValue();
	double GetMinValue();
	double GetDeltaValue();
	double GetFluctuation();

	bool GetValueFromDataPool();

public:
	double GetChauvC( int iCount );
	double GetAverage( vector<double> arrValue );
	double GetStandardError( vector<double> arrValue );
	double GetMaxValue( vector<double> arrValue );
	double GetMinValue( vector<double> arrValue );
	double GetDeltaValue( vector<double> arrValue );
	double GetFluctuation( vector<double> arrValue );
	double GetMidValue( double dSv, double dEv, int iSp, int iEp, int iPos );

private:
	vector<CData>	m_arrDataPool;			//	数据池
	vector<double>	m_arrValue;				//	数值
	int				m_iNormalCount;			//	当前正常数据个数
	int				m_iProcLoop;			//	当前处理轮次
	bool			m_bFinished;			//	是否结束 ( 本轮次无异常数据 或 策略完成)
	double			m_dLastLoopAvr;			//	上一轮次平均值
	int				m_iLastDelCount;		//	上一轮次删除个数

	double			m_dStrictCoeff;			//	严格系数
	int				m_iTransMethod;			//	转换策略
	int				m_iStrategy;			//	处理策略 ( 0:轮次上限; 1:删除个数上限; 2:删除占比上限; 3:波动率上限; 4:均值变化率上限; 5: 上轮次删除个数上限; )
	int				m_wParam;				//	策略参数
	double			m_lParam;				//	策略参数

	static const int		MAXLOOPCOUNT = 20;
};

#endif	//	_DATA_PROC_H_