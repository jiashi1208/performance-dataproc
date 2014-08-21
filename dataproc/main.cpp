#include <stdio.h>

#include "dataproc.h"
#include "toolkit.h"


#define RESULTCSV	"result.csv"
#define DATACSV		"data.csv"

int printUsage()
{
	printf("Usage: sysperfproc:\n");

#if 0
	printf("\t dataproc in.csv out.csv 3 0.7\n");
	printf("\t dataproc in.csv out.csv 0 3 0 0.7\n");
#endif

	printf("\t sysperfproc csvdir\n");

	return -1;
}

int test()
{
	string strInFile = DATACSV;
	string strOutFile = RESULTCSV;
	int iCount = 3;
	double dStCoeff = 0.7;

	CDataProc dataProc;
	if( !dataProc.ReadDataFromFile(strInFile) )
		return -1;

	dataProc.SetStrategy( 0, iCount, 0 );
	dataProc.SetStrictCoeff(dStCoeff);
	dataProc.Proc();

	dataProc.PrintRemainData();
	dataProc.WriteToFile(strOutFile);

	return 0;
}

int simpleproc(int argc, char** argv)
{
	string strInFile = argv[1];
	string strOutFile = argv[2];
	int iCount = atoi(argv[3]);
	double dStCoeff = atof(argv[4]);

	CDataProc dataProc;
	if( !dataProc.ReadDataFromFile(strInFile) )
		return -1;

	dataProc.SetStrategy( 0, iCount, 0 );
	dataProc.SetStrictCoeff(dStCoeff);
	dataProc.Proc();

	dataProc.PrintRemainData();
	dataProc.WriteToFile(strOutFile);

	return 0;
}

int fullproc(int argc, char** argv)
{
	string strInFile = argv[1];
	string strOutFile = argv[2];
	int iStrategy = atoi(argv[3]);
	int wParam = atoi(argv[4]);
	double lParam = atof(argv[5]);
	double dStCoeff = atof(argv[6]);

	CDataProc dataProc;
	if( !dataProc.ReadDataFromFile(strInFile) )
		return -1;

	dataProc.SetStrategy( iStrategy, wParam, lParam );
	dataProc.SetStrictCoeff(dStCoeff);
	dataProc.Proc();

	dataProc.PrintRemainData();
	dataProc.WriteToFile(strOutFile);

	return 0;
}


#define TYPE_COUNT	3

#define BG	0
#define FG	1
#define OP	2

const string TESTTYPE[TYPE_COUNT] = { "bg", "fg", "op" };
string strPath;

vector<double> arr_TestTime[TYPE_COUNT];

vector<double> arr_CPUmax[TYPE_COUNT];
vector<double> arr_CPUavr[TYPE_COUNT];

vector<double> arr_MEMmax[TYPE_COUNT];
vector<double> arr_MEMavr[TYPE_COUNT];

vector<double> arr_NetIn[TYPE_COUNT];
vector<double> arr_NetOut[TYPE_COUNT];

vector<double> arr_PowerCPU[TYPE_COUNT];
vector<double> arr_PowerGPS[TYPE_COUNT];
vector<double> arr_PowerTotal[TYPE_COUNT];

bool GetSingleValueFromSingleFile( string fileName, int iLine, vector<double>& arrValue )
{
	fileName = strPath + "/" + fileName;

	FILE* hfile = fopen( fileName.c_str(), "r" );
	if( hfile == NULL )
	{
		printf("file not found: %s\n", fileName.c_str());
		return false;
	}

	char tmp[1024] = {0};
	for( int i = 0; i < iLine; i++ )
		fgets( tmp, 1024, hfile );

	fgets( tmp, 1024, hfile );
	string strLine = tmp;
	strip( strLine, "\r\n", 1 );

	vector<string> rst;
	split( strLine, ",", rst );
	if( rst.size() != 2 )
	{
		fclose(hfile);
		return false;
	}

	double dTmp = atof( rst[1].c_str() );
	arrValue.push_back(dTmp);

	//printf("\t %d, %.2f\n", iLine, dTmp);

	fclose(hfile);
	return true;
}

void GetSingleValue( vector<string>& arrFile, int iLine, vector<double>& arrValue )
{
	int iSize = arrFile.size();
	if( iSize <= 0 )
		return;

	arrValue.clear();
	for( int i = 0; i < iSize; i++ )
	{
		GetSingleValueFromSingleFile( arrFile[i], iLine, arrValue );
	}
}

int GetFileByType( vector<string>& arrFile, vector<string>& arrTypeFile, string strType, string suffix )
{
	int iSize = arrFile.size();
	if( iSize <= 0 )
		return 0;

	arrTypeFile.clear();
	vector<string> rst;
	for( int i = 0; i < iSize; i++ )
	{
		split( arrFile[i], ".", rst );
		if( rst.size() != 2 )
			continue;

		if( strcmp( rst[1].c_str(), suffix.c_str() ) != 0 )
			continue;

		split( arrFile[i], "_", rst );
		if( rst.size() != 5 )
			continue;

		if( strcmp( rst[0].c_str(), "SysPerfRst" ) != 0 )
			continue;

		if( strcmp( rst[3].c_str(), strType.c_str() ) != 0 )
			continue;

		arrTypeFile.push_back(arrFile[i]);
	}

	return arrTypeFile.size();
}

void GetValue( vector<string>& arrFile, int iType )
{
	vector<string> arrTypeFile;
	GetFileByType( arrFile, arrTypeFile, TESTTYPE[iType], "csv" );

	GetSingleValue( arrTypeFile, 0, arr_TestTime[iType] );
	GetSingleValue( arrTypeFile, 1, arr_CPUmax[iType] );
	GetSingleValue( arrTypeFile, 2, arr_CPUavr[iType] );
	GetSingleValue( arrTypeFile, 3, arr_MEMmax[iType] );
	GetSingleValue( arrTypeFile, 4, arr_MEMavr[iType] );
	GetSingleValue( arrTypeFile, 5, arr_NetIn[iType] );
	GetSingleValue( arrTypeFile, 6, arr_NetOut[iType] );
	GetSingleValue( arrTypeFile, 7, arr_PowerCPU[iType] );
	GetSingleValue( arrTypeFile, 8, arr_PowerGPS[iType] );
	GetSingleValue( arrTypeFile, 9, arr_PowerTotal[iType] );
}

double GetAverageWithNoiseDelete( vector<double>& arrValue, int iLoop = 3, double dSc = 0.7 )
{
	CDataProc dataproc;
	dataproc.GenerateData( arrValue );
	dataproc.SetStrategy( 0, iLoop, 0 );
	dataproc.SetStrictCoeff( dSc );
	dataproc.Proc();
	return dataproc.GetAverage();
}

void SingleProc( vector<string>& arrFile, int iType )
{
	GetValue( arrFile, iType );

	CDataProc dataproc;
	double dCPUmax = dataproc.GetMaxValue( arr_CPUmax[iType] );
	double dCPUavr = GetAverageWithNoiseDelete( arr_CPUavr[iType] );
	double dMEMmax = dataproc.GetMaxValue( arr_MEMmax[iType] );
	double dMEMavr = GetAverageWithNoiseDelete( arr_MEMavr[iType] );
	double dNetIn = GetAverageWithNoiseDelete( arr_NetIn[iType] );
	double dNetOut = GetAverageWithNoiseDelete( arr_NetOut[iType] );
	double dPowerCPU = GetAverageWithNoiseDelete( arr_PowerCPU[iType] );
	double dPowerGPS = GetAverageWithNoiseDelete( arr_PowerGPS[iType] );
	double dPowerTotal = GetAverageWithNoiseDelete( arr_PowerTotal[iType] );

	FILE* hRstFile = fopen( RESULTCSV, "a" );
	if( NULL == hRstFile )
	{
		printf("result.csv file cannot open\n");
		return;
	}

	fprintf( hRstFile, "%s\n", TESTTYPE[iType].c_str() );
	fprintf( hRstFile, "CPU max,%.2f\n", dCPUmax);
	fprintf( hRstFile, "CPU avr,%.2f\n", dCPUavr);
	fprintf( hRstFile, "MEM max,%d\n", (int)dMEMmax);
	fprintf( hRstFile, "MEM avr,%d\n", (int)dMEMavr);
	fprintf( hRstFile, "Net In,%.2f\n", dNetIn);
	fprintf( hRstFile, "Net Out,%.2f\n", dNetOut);
	fprintf( hRstFile, "CPU Power,%.2f\n", dPowerCPU);
	fprintf( hRstFile, "GPS Power,%.2f\n", dPowerGPS);
	fprintf( hRstFile, "Total Power,%.2f\n\n", dPowerTotal);

	fclose(hRstFile);

	FILE* hDataFile = fopen( DATACSV, "a" );
	if( NULL == hDataFile )
	{
		printf("data.csv file cannot open\n");
		return;
	}

	fprintf( hDataFile, "%s\n", TESTTYPE[iType].c_str() );
	fprintf( hDataFile, "Time,CPU max,CPU avr,MEM max,MEM avr,Net In,Net Out, CPU Power,GPS Power,Total Power\n" );

	int iCount = arr_TestTime[iType].size();
	for( int i = 0; i < iCount; i++ )
	{
		fprintf( hDataFile, "%d,%.2f,%.2f,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n", (int)arr_TestTime[iType][i],
			arr_CPUmax[iType][i], arr_CPUavr[iType][i], (int)arr_MEMmax[iType][i], (int)arr_MEMavr[iType][i],
			arr_NetIn[iType][i], arr_NetOut[iType][i], arr_PowerCPU[iType][i], arr_PowerGPS[iType][i], arr_PowerTotal[iType][i] );
	}
	fprintf( hDataFile, "\n" );
	
	fclose(hDataFile);
}

int sysperfproc( string dirname )
{
	DeleteFile( string(RESULTCSV) );
	DeleteFile( string(DATACSV) );

	vector<string> arr_fileName;

	int iCount = GetSubFileList( arr_fileName, dirname );
	if( iCount <= 0 )
	{
		printf("no file found in dir: %s\n", dirname.c_str());
		return -1;
	}

	SingleProc( arr_fileName, BG );
	SingleProc( arr_fileName, FG );
	SingleProc( arr_fileName, OP );

	printf("success!\n");
	return 0;
}

int main(int argc, char** argv)
{
#if 0
	return test();
#endif

#if 1
	if( argc != 5 && argc != 7 )
		return printUsage();

	if( 5 == argc )
		return simpleproc( argc, argv );

	if( 7 == argc )
		return fullproc( argc, argv );
#endif

#if 0
#if 1
	if( argc != 2 )
		return printUsage();
	strPath = argv[1];
#else
	strPath = "./659mi11/";
#endif
	return sysperfproc( strPath );
#endif
}