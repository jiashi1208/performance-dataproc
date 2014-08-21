#include "toolkit.h"

#ifndef V_LINUX
//	Windows ƽ̨
#include <io.h>
#include <direct.h>

/**
 *		�����ļ���
 *		@param		strDir			[in]		�ļ���·��
 *		@return		true���ɹ���false��ʧ��
 */
bool CreateDir( string strDir )
{
	if( strDir.empty() )
		return false;

	int iRtn = _access( strDir.c_str(), 0 );		//	�ж��ļ����Ƿ����

	if( 0 == iRtn )
		return true;
	else if( -1 == iRtn )
	{
		if( _mkdir( strDir.c_str() ) == 0 )
			return true;
		else
			return false;
	}

	return false;
}

/**
 *		��ȡ���ļ����б�
 *		@param		arrDirList			[out]		���ļ����б�
 *					strDir				[in]		�ļ���·��
 *		@return		���ļ��и���
 */
int GetSubDirList( vector<string>& arrDirList, string strDir )
{
	if( strDir.empty() )
		return -1;

	long hfile = 0;
	struct _finddata_t fileinfo;

	int iCount = 0;

	strip( strDir, "/", 1 );
	strDir += "/*";
	if( ( hfile = _findfirst(strDir.c_str(), &fileinfo) ) != -1 )
	{
		do 
		{
			if( fileinfo.attrib & _A_SUBDIR )
			{
				if( strcmp( fileinfo.name, "." ) != 0 && strcmp( fileinfo.name, ".." ) )
				{
					iCount++;
					arrDirList.push_back( string(fileinfo.name) );
				}
			}
		} while ( _findnext( hfile, &fileinfo ) == 0 );

		_findclose(hfile);
	}

	return iCount;
}

/**
 *		��ȡ���ļ��б�
 *		@param		arrFileList			[out]		���ļ��б�
 *					strDir				[in]		�ļ���·��
 *		@return		���ļ�����
 */
int GetSubFileList( vector<string>& arrFileList, string strDir )
{
	if( strDir.empty() )
		return -1;

	long hfile = 0;
	struct _finddata_t fileinfo;

	int iCount = 0;

	strip( strDir, "/", 1 );
	strDir += "/*";
	if( ( hfile = _findfirst(strDir.c_str(), &fileinfo) ) != -1 )
	{
		do 
		{
			if( fileinfo.attrib & _A_ARCH )
			{
				if( strcmp( fileinfo.name, "." ) != 0 && strcmp( fileinfo.name, ".." ) )
				{
					iCount++;
					arrFileList.push_back( string(fileinfo.name) );
				}
			}
		} while ( _findnext( hfile, &fileinfo ) == 0 );

		_findclose(hfile);
	}

	return iCount;
}


#else	//	linux
//	linux ƽ̨
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>

/**
 *		�����ļ���
 *		@param		strDir			[in]		�ļ���·��
 *		@return		true���ɹ���false��ʧ��
 */
bool CreateDir( string strDir )
{
	if( strDir.empty() )
		return false;

	int iRtn = access( strDir.c_str(), 0 );		//	�ж��ļ����Ƿ����

	if( 0 == iRtn )
		return true;
	else if( -1 == iRtn )
	{
		if( mkdir( strDir.c_str(), 0777 ) == 0 )
			return true;
		else
			return false;
	}

	return false;
}

/**
 *		��ȡ���ļ����б�
 *		@param		arrDirList			[out]		���ļ����б�
 *					strDir				[in]		�ļ���·��
 *		@return		���ļ��и���
 */
int GetSubDirList( vector<string>& arrDirList, string strDir )
{
	if( strDir.empty() )
		return -1;

	if( access(strDir.c_str(), 0) == -1 )
		return -1;

	strip( strDir, "/", 1 );
	strDir += "/";
	
	struct dirent* pdirent = NULL;
	struct stat info;
	DIR* pdir;
	pdir = opendir( strDir.c_str() );

	int iCount = 0;

	for( pdirent = readdir(pdir); pdirent != NULL; pdirent = readdir(pdir) )
	{
		if( strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0 )
			continue;

		string tmp = strDir + string( pdirent->d_name );
		if( stat( tmp.c_str(), &info) != 0 )
			return -1;

		if( S_ISDIR( info.st_mode ) )
		{
			arrDirList.push_back( string(pdirent->d_name) );
			iCount++;
		}
	}

	closedir(pdir);

	return iCount;
}

/**
 *		��ȡ���ļ��б�
 *		@param		arrFileList			[out]		���ļ����б�
 *					strDir				[in]		�ļ���·��
 *		@return		���ļ�����
 */
int GetSubFileList( vector<string>& arrFileList, string strDir )
{
	if( strDir.empty() )
		return -1;

	if( access(strDir.c_str(), 0) == -1 )
		return -1;

	strip( strDir, "/", 1 );
	strDir += "/";

	struct dirent* pdirent = NULL;
	struct stat info;
	DIR* pdir;
	pdir = opendir( strDir.c_str() );

	int iCount = 0;

	for( pdirent = readdir(pdir); pdirent != NULL; pdirent = readdir(pdir) )
	{
		if( strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0 )
			continue;

		string tmp = strDir + string( pdirent->d_name );
		if( stat( tmp.c_str(), &info) != 0 )
			return -1;

		if( S_ISREG( info.st_mode ) )
		{
			arrFileList.push_back( string(pdirent->d_name) );
			iCount++;
		}
	}

	closedir(pdir);

	return iCount;
}

/**
 *		��ȡ��ǰʱ�䣨��Linuxƽ̨��Ч��
 *		@param		sec			[out]		��
 *					usec		[out]		΢��
 *		@return		�Ƿ�ɹ�
 */
bool GetTime( long& sec, long& usec )
{
	struct timeval tv;

	int iRtn = gettimeofday( &tv, 0 );

	if( iRtn != 0 )
		return false;

	sec = tv.tv_sec;
	usec = tv.tv_usec;

	return true;
}

#endif

/**
 *		ɾ��·�����������ļ������������ļ��У�
 *		@param		strDir			[in]		·��
 *		@return		�ȽϽ����true���ɹ���false��ʧ�ܣ�
 */
bool CleanDir( string strDir )
{
	if( strDir.empty() )
		return false;

	strip( strDir, "/", 1 );
	strDir += "/";

	vector<string> arrSubFileList;
	int iCount = GetSubFileList( arrSubFileList, strDir );
	for( int i = 0; i < iCount; i++ )
	{
		string strFile = strDir + arrSubFileList[i];
		DeleteFile(strFile);
	}

	return true;
}

/**
 *		ɾ���ļ�
 *		@param		strDir			[in]		�ļ�·��
 *		@return		true���ɹ���false��ʧ��
 */
bool DeleteFile( string& strFile )
{
	if( strFile.empty() )
		return false;

	if( 0 == remove( strFile.c_str() ) )
		return true;
	else
		return false;
}

/**
 *		�����ļ�
 *		@param		strDir			[in]		�ļ���·��
 *		@return		true���ɹ���false��ʧ��
 */
bool CreateFile( string strFile )
{
	if( strFile.empty() )
		return false;

	FILE* hfile = fopen( strFile.c_str(), "w" );
	if( hfile == NULL )
		return false;

	fclose(hfile);
	return true;
}

long GetTimeInterval( long sec_f, long usec_f, long sec_t, long usec_t )
{
	return ( sec_t - sec_f )*1000000 + usec_t - usec_f;
}

/**
 *		�ַ����ָ�
 *		@param		str			[in]		���ָ���ַ���
 *					delim		[in]		�ָ��
 *					rst			[out]		�������
 */
void split( const string& str, const string& delim, vector<string>& rst )
{
	if( str.empty() || delim.empty() )
		return;

	rst.clear();

	int len = str.length();
	int head = 0, pos = 0;

	string tmp;
	while( (pos = str.find( delim.c_str(), head ) ) != string::npos )
	{
		tmp = str.substr( head, pos-head );
		head = pos + delim.length();
		rst.push_back(tmp);
	}

	if( head <= len-1 )
	{
		tmp = str.substr( head, len-head );
		rst.push_back(tmp);
	}
}

/**
 *		ȥ���ַ�����β���ַ���
 *		@param		str			[in/out]		��������ַ���
 *					cs			[in]			�����ַ���
 *					rst			[out]			��������( 0:���ǰ�ˣ�1:�����ˣ�2:���ǰ��� )
 */
void strip( string& str, string cs, int type )
{
	if( str.empty() || cs.empty() )
		return;

	int len = str.length();

	if( type == 0 )			//	���ǰ��
	{
		int pos = 0;
		for( ; pos < len; pos++ )
		{
			if( cs.find( str[pos] ) == string::npos )
				break;
		}

		str = str.substr( pos, len-pos );
	}

	if( type == 1 )			//	������
	{
		int pos = len-1;
		for( ; pos >= 0; pos-- )
		{
			if( cs.find( str[pos] ) == string::npos )
				break;
		}

		str = str.substr( 0, pos+1 );
	}

	if( type == 2 )		//	���ǰ���
	{
		int head = 0, tail = len - 1;
		for( ; head < len; head++ )
		{
			if( cs.find( str[head] ) == string::npos )
				break;
		}

		for( ; tail >= head; tail-- )
		{
			if( cs.find( str[tail] ) == string::npos )
				break;
		}

		str = str.substr( head, tail-head+1 );
	}
}

/**
 *		ȥ���ַ����е��ַ�
 *		@param		str			[in]		��������ַ���
 *					c			[in]		ȥ���ַ�
 *		@return		�������ַ���
 */
string removefromstring( string str, char c )
{
	if( str.empty() )
		return str;

	string rst;
	int pos = 0;

	int iCount = str.length();
	for( int i = 0; i < iCount; i++ )
	{
		if( (int)str[i] < 0  )		//	������
		{
			for( int j = 0; j < CHINESE_CHAR_WIDTH; j++ )
				rst += str[i++];
			i--;
			continue;
		}

		if( str[i] != c )
			rst += str[i];
	}

	return rst;
}

/**
 *		�ж��ַ��������Ƿ���ͬ���������ַ�˳��
 *		@param		s1			[in]		���Ƚϵ��ַ���
 *					s2			[in]		���Ƚϵ��ַ���
 *		@return		�ȽϽ����true����ͬ��false����ͬ��
 */
bool IsSame_NoOrder( string s1, string s2 )
{
	if( ( s1.empty() && !s2.empty() ) || ( !s1.empty() && s2.empty() ) )
		return false;
	else if( s1.empty() && s2.empty() )
		return true;

	if( s1.size() != s2.size() )
		return false;

	int iLen = s1.size();
	for( int i = 0; i < iLen; i++ )
	{
		char ch = s1[i];				//	˳���� s1 �е��ַ�

		int iPos = s2.find(ch);			//	�� s2 ��Ѱ�� s1 �е��ַ�
		if( iPos == string::npos )
			return false;

		s2 = s2.erase(iPos,1);			//	�ҵ���ɾ�����ַ�
	}

	return true;
}
