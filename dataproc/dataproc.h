#ifndef _DATA_PROC_H_
#define _DATA_PROC_H_

#include <stdio.h>
#include <vector>
#include <string.h>

using namespace std;

#define  ERRORNUM		0xFFFFFFFF
#define	 STRICTCOEFF	0.7

//	Фά��ϵ����
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

//	���ݽṹ
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
	double	m_dValue;					//	��ʵֵ
	double	m_dTransVal;				//	ת����ֵ
	bool	m_bTransed;					//	�Ƿ���ת��
	int		m_iStatus;					//	״̬ (��?�ֱ��޳�, 0:����)
	int		m_iTransMethod;				//	ת����ʽ ( 0:��ת��; 1:��1����; 2:����ȡ������; 3:��1ȡ���� )
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
	vector<CData>	m_arrDataPool;			//	���ݳ�
	vector<double>	m_arrValue;				//	��ֵ
	int				m_iNormalCount;			//	��ǰ�������ݸ���
	int				m_iProcLoop;			//	��ǰ�����ִ�
	bool			m_bFinished;			//	�Ƿ���� ( ���ִ����쳣���� �� �������)
	double			m_dLastLoopAvr;			//	��һ�ִ�ƽ��ֵ
	int				m_iLastDelCount;		//	��һ�ִ�ɾ������

	double			m_dStrictCoeff;			//	�ϸ�ϵ��
	int				m_iTransMethod;			//	ת������
	int				m_iStrategy;			//	������� ( 0:�ִ�����; 1:ɾ����������; 2:ɾ��ռ������; 3:����������; 4:��ֵ�仯������; 5: ���ִ�ɾ����������; )
	int				m_wParam;				//	���Բ���
	double			m_lParam;				//	���Բ���

	static const int		MAXLOOPCOUNT = 20;
};

#endif	//	_DATA_PROC_H_