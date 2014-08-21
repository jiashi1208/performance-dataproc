#include <stdio.h>
#include <math.h>

#include "dataproc.h"


void CData::SetData( double dValue, int iTrMd /* = 0 */ )
{
	m_dValue = dValue;
	m_iTransMethod = iTrMd;
	m_iStatus = 0;

	Transfer();
}

void CData::Transfer()
{
	m_dTransVal = m_dValue;
	m_bTransed = true;

	switch(m_iTransMethod)
	{
	case 1:
		Transfer_Sqrt();
		break;

	case 2:
		Transfer_ArcSin();
		break;

	case 3:
		Transfer_log();
		break;

	default:
		break;
	}
}

void CData::PrintToScreen()
{
	printf("val: %.2f, tval: %.2f, status: %d", m_dValue, m_dTransVal, m_iStatus);
}

void CData::Transfer_Sqrt()
{
	m_dTransVal = sqrt(m_dValue+1);
}

void CData::Transfer_ArcSin()
{
	m_dTransVal = asin( sqrt(m_dValue) );
}

void CData::Transfer_log()
{
	m_dTransVal = log(m_dValue+1);
}




CDataProc::CDataProc()
{
	m_arrDataPool.clear();
	m_bFinished = false;
	m_iStrategy = 0;
	m_iTransMethod = 0;
	m_dStrictCoeff = STRICTCOEFF;
}

CDataProc::~CDataProc()
{
	m_arrDataPool.clear();
}

double CDataProc::GetMidValue( double dSv, double dEv, int iSp, int iEp, int iPos )
{
	double delta = (dEv-dSv)/(iEp-iSp);
	return dSv + delta * ( iPos - iSp );
}

double CDataProc::GetChauvC( int iCount )
{
	if( iCount <= 0 )
		return ERRORNUM;

	if( iCount > 0 && iCount <= 20 )
		return ChauvenetC[iCount];

	if( iCount > 20 && iCount <= 25 )
		return GetMidValue( ChauvenetC[20], ChauvenetC[21], 20, 25, iCount );

	if( iCount > 25 && iCount <= 30 )
		return GetMidValue( ChauvenetC[21], ChauvenetC[22], 25, 30, iCount );

	if( iCount > 30 && iCount <= 40 )
		return GetMidValue( ChauvenetC[22], ChauvenetC[23], 30, 40, iCount );

	if( iCount > 40 && iCount <= 50 )
		return GetMidValue( ChauvenetC[23], ChauvenetC[24], 40, 50, iCount );

	if( iCount > 50 && iCount <= 100 )
		return GetMidValue( ChauvenetC[24], ChauvenetC[25], 50, 100, iCount );

	if( iCount > 100 && iCount <= 185 )
		return GetMidValue( ChauvenetC[25], ChauvenetC[26], 100, 185, iCount );

	return ChauvenetC[26];
}

double CDataProc::GetAverage( vector<double> arrValue )
{
	int iSize = arrValue.size();
	if( iSize <= 0 )
		return ERRORNUM;

	double dSum = 0;
	for( int i = 0; i < iSize; i++ )
		dSum += arrValue[i];

	return dSum/iSize;
}

double CDataProc::GetStandardError( vector<double> arrValue )
{
	int iSize = arrValue.size();
	if( iSize <= 1 )
		return 0;

	double dAvr = GetAverage(arrValue);
	double dTmp = 0;
	for( int i = 0; i < iSize; i++ )
		dTmp += (arrValue[i]-dAvr) * (arrValue[i]-dAvr);

	return sqrt( dTmp/(iSize-1) );
}

double CDataProc::GetMaxValue( vector<double> arrValue )
{
	int iSize = arrValue.size();
	if( iSize <= 0 )
		return ERRORNUM;

	double tmp = arrValue[0];
	for( int i = 1; i < iSize; i++ )
	{
		if( arrValue[i] > tmp )
			tmp = arrValue[i];
	}

	return tmp;
}

double CDataProc::GetMinValue( vector<double> arrValue )
{
	int iSize = arrValue.size();
	if( iSize <= 0 )
		return ERRORNUM;

	double tmp = arrValue[0];
	for( int i = 1; i < iSize; i++ )
	{
		if( arrValue[i] < tmp )
			tmp = arrValue[i];
	}

	return tmp;
}

double CDataProc::GetDeltaValue( vector<double> arrValue )
{
	return GetMaxValue(arrValue) - GetMinValue(arrValue);
}

double CDataProc::GetFluctuation( vector<double> arrValue )
{
	return GetStandardError(arrValue)/GetAverage(arrValue);
	//return GetDeltaValue(arrValue)/GetAverage(arrValue);
}

void CDataProc::SetStrategy( int iStgType, int wParam, double lParam )
{
	m_iStrategy = iStgType;
	m_wParam = wParam;
	m_lParam = lParam;
}

void CDataProc::SetTransMethod( int iTrMd )
{
	m_iTransMethod = iTrMd;

	for( int i = 0; i < (int)m_arrDataPool.size(); i++ )
	{
		m_arrDataPool[i].m_iTransMethod = iTrMd;
		m_arrDataPool[i].m_bTransed = false;
	}
}

void CDataProc::SetStrictCoeff( double dSc /* = STRICTCOEFF */ )
{
	m_dStrictCoeff = dSc;
}

bool CDataProc::Proc()
{
	GetValueFromDataPool();

	m_iProcLoop = 0;
	m_bFinished = false;
	while( !m_bFinished && m_iProcLoop < MAXLOOPCOUNT && m_iNormalCount > 2 )
	{
		SingleProc();

		if( CheckFinish() )
			m_bFinished = true;

		//PrintToScreen();
		//printf("\n\n");
	}

	return true;
}

void CDataProc::GenerateData( vector<double> arrData )
{
	m_arrDataPool.clear();
	int iSize = arrData.size();
	for( int i = 0; i < iSize; i++ )
	{
		CData tmpData;
		tmpData.SetData( arrData[i], m_iTransMethod );
		m_arrDataPool.push_back(tmpData);
	}
}

bool CDataProc::ReadDataFromFile( string strFile )
{
	//GenerateData();
	FILE* hfile = fopen( strFile.c_str(), "r" );
	if( NULL == hfile )
	{
		printf("file open failed\n");
		return false;
	}

	char tmp[1024] = {0};
	while( fgets( tmp, 1024, hfile ) )
	{
		double dValue = atof(tmp);
		CData tmpData;
		tmpData.SetData( dValue, m_iTransMethod );
		m_arrDataPool.push_back(tmpData);
	}

	fclose(hfile);
	return true;
}

bool CDataProc::WriteToFile( string strFile )
{
	FILE* hfile = fopen( strFile.c_str(), "w" );
	if( NULL == hfile )
	{
		printf("open output file failed\n");
		return false;
	}

	int iSize = m_arrDataPool.size();
	if( iSize <= 0 )
	{
		printf("data count illegal\n");
		return false;
	}

	for( int i = 0; i < iSize; i++ )
	{
		fprintf( hfile, "%.2f,%d\n", m_arrDataPool[i].m_dValue, m_arrDataPool[i].m_iStatus );
	}

	fprintf( hfile, "%.2f\n", GetAverage() );

	fclose(hfile);
	return true;
}

void CDataProc::PrintToScreen()
{
	int iSize = m_arrDataPool.size();
	printf("Original Count: %d, Current Count: %d, Loop Count: %d\n", iSize, m_iNormalCount, m_iProcLoop);

	for( int i = 0; i < iSize; i++ )
	{
		printf("[idx:%d] ", i);
		m_arrDataPool[i].PrintToScreen();
		printf("\n");
	}

	printf("Data Fluctuation: %.2f", GetFluctuation()*100);
}

void CDataProc::PrintRemainData()
{
	int iSize = m_arrDataPool.size();
	printf("Original Count: %d, Remain Count: %d, Loop Count: %d\n", iSize, m_iNormalCount, m_iProcLoop);

	for( int i = 0; i < iSize; i++ )
	{
		if( m_arrDataPool[i].m_iStatus != 0 )
			continue;

		printf("[idx:%d] ", i);
		m_arrDataPool[i].PrintToScreen();
		printf("\n");
	}

	printf("Data Average: %.2f\n", GetAverage());
	printf("Data Fluctuation: %.2f%%\n", GetFluctuation()*100);
}

bool CDataProc::SingleProc()
{
	int iSize = m_arrDataPool.size();
	if( iSize <= 0 || m_iNormalCount <= 0 )
		return false;

	m_iProcLoop ++;

	int iCurrDelCount = 0;
	double dCVC = GetChauvC(m_iNormalCount) * m_dStrictCoeff;
	double dArv = GetAverage();
	double dStdErr = GetStandardError();

	for( int i = 0; i < iSize; i++ )
	{
		if( m_arrDataPool[i].m_iStatus != 0 )
			continue;

		if( abs(m_arrDataPool[i].m_dTransVal - dArv) > dCVC * dStdErr )
		{
			m_arrDataPool[i].m_iStatus = m_iProcLoop;
			iCurrDelCount ++;
			m_iNormalCount --;
		}
	}

	m_iLastDelCount = iCurrDelCount;
	if( 0 == iCurrDelCount )
		m_bFinished = true;

	return true;
}

bool CDataProc::CheckFinish()
{
	switch( m_iStrategy )
	{
	case 0:
		return CheckFinish_LoopCount(m_wParam);

	case 1:
		return CheckFinish_DeleteCount(m_wParam);

	case 2:
		return CheckFinish_DeletePercent(m_lParam);

	case 3:
		return CheckFinish_Fluctuation(m_lParam);

	case 4:
		return CheckFinish_AvrChange(m_lParam);

	default:
		return CheckFinish_LoopCount(1);
	}
}

bool CDataProc::CheckFinish_LoopCount( int iCount )
{
	return m_iProcLoop >= iCount;
}

bool CDataProc::CheckFinish_DeleteCount( int iCount )
{
	return ((int)m_arrDataPool.size()-m_iNormalCount) <= iCount;
}

bool CDataProc::CheckFinish_LastLoopDelCount( int iCount )
{
	return m_iLastDelCount <= iCount;
}

bool CDataProc::CheckFinish_DeletePercent( double dPercent )
{
	int iSize = m_arrDataPool.size();
	if( iSize <= 0 )
		return true;

	return (iSize-m_iNormalCount)/iSize <= dPercent;
}

bool CDataProc::CheckFinish_Fluctuation( double dPercent )
{
	return GetFluctuation() <= dPercent;
}

bool CDataProc::CheckFinish_AvrChange( double dPercent )
{
	if( 0 == m_dLastLoopAvr )
		return false;

	double dAvr = GetAverage();
	return abs((dAvr-m_dLastLoopAvr)/m_dLastLoopAvr) <= dPercent;
}

double CDataProc::GetAverage()
{
	GetValueFromDataPool();
	return GetAverage( m_arrValue );
}

double CDataProc::GetStandardError()
{
	GetValueFromDataPool();
	return GetStandardError( m_arrValue );
}

double CDataProc::GetMaxValue()
{
	GetValueFromDataPool();
	return GetMaxValue( m_arrValue );
}

double CDataProc::GetMinValue()
{
	GetValueFromDataPool();
	return GetMinValue( m_arrValue );
}

double CDataProc::GetDeltaValue()
{
	GetValueFromDataPool();
	return GetDeltaValue( m_arrValue );
}

double CDataProc::GetFluctuation()
{
	GetValueFromDataPool();
	return GetFluctuation( m_arrValue );
}

bool CDataProc::GetValueFromDataPool()
{
	int iSize = m_arrDataPool.size();
	if( iSize <= 0 )
		return false;

	m_iNormalCount = 0;
	m_arrValue.clear();
	for( int i = 0; i < iSize; i++ )
	{
		if( !m_arrDataPool[i].m_bTransed )
			m_arrDataPool[i].Transfer();

		if( m_arrDataPool[i].m_iStatus != 0 )
			continue;

		m_arrValue.push_back( m_arrDataPool[i].m_dTransVal );
		m_iNormalCount ++;
	}

	return true;
}
