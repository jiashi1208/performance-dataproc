#include "toolkit.h"

#ifndef V_LINUX
//	Windows 平台
#include <io.h>
#include <direct.h>

/**
 *		创建文件夹
 *		@param		strDir			[in]		文件夹路径
 *		@return		true：成功；false：失败
 */
bool CreateDir( string strDir )
{
	if( strDir.empty() )
		return false;

	int iRtn = _access( strDir.c_str(), 0 );		//	判断文件夹是否存在

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
 *		获取子文件夹列表
 *		@param		arrDirList			[out]		子文件夹列表
 *					strDir				[in]		文件夹路径
 *		@return		子文件夹个数
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
 *		获取子文件列表
 *		@param		arrFileList			[out]		子文件列表
 *					strDir				[in]		文件夹路径
 *		@return		子文件个数
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
//	linux 平台
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>

/**
 *		创建文件夹
 *		@param		strDir			[in]		文件夹路径
 *		@return		true：成功；false：失败
 */
bool CreateDir( string strDir )
{
	if( strDir.empty() )
		return false;

	int iRtn = access( strDir.c_str(), 0 );		//	判断文件夹是否存在

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
 *		获取子文件夹列表
 *		@param		arrDirList			[out]		子文件夹列表
 *					strDir				[in]		文件夹路径
 *		@return		子文件夹个数
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
 *		获取子文件列表
 *		@param		arrFileList			[out]		子文件夹列表
 *					strDir				[in]		文件夹路径
 *		@return		子文件个数
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
 *		获取当前时间（仅Linux平台有效）
 *		@param		sec			[out]		秒
 *					usec		[out]		微秒
 *		@return		是否成功
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
 *		删除路径下所有子文件（不包括子文件夹）
 *		@param		strDir			[in]		路径
 *		@return		比较结果（true：成功；false：失败）
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
 *		删除文件
 *		@param		strDir			[in]		文件路径
 *		@return		true：成功；false：失败
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
 *		创建文件
 *		@param		strDir			[in]		文件夹路径
 *		@return		true：成功；false：失败
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
 *		字符串分割
 *		@param		str			[in]		待分割的字符串
 *					delim		[in]		分割符
 *					rst			[out]		结果数组
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
 *		去除字符串首尾的字符串
 *		@param		str			[in/out]		待清理的字符串
 *					cs			[in]			清理字符串
 *					rst			[out]			清理类型( 0:清除前端；1:清除后端；2:清除前后端 )
 */
void strip( string& str, string cs, int type )
{
	if( str.empty() || cs.empty() )
		return;

	int len = str.length();

	if( type == 0 )			//	清除前端
	{
		int pos = 0;
		for( ; pos < len; pos++ )
		{
			if( cs.find( str[pos] ) == string::npos )
				break;
		}

		str = str.substr( pos, len-pos );
	}

	if( type == 1 )			//	清除后端
	{
		int pos = len-1;
		for( ; pos >= 0; pos-- )
		{
			if( cs.find( str[pos] ) == string::npos )
				break;
		}

		str = str.substr( 0, pos+1 );
	}

	if( type == 2 )		//	清除前后端
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
 *		去除字符串中的字符
 *		@param		str			[in]		待处理的字符串
 *					c			[in]		去除字符
 *		@return		处理后的字符串
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
		if( (int)str[i] < 0  )		//	处理汉字
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
 *		判断字符串内容是否相同（不考虑字符顺序）
 *		@param		s1			[in]		待比较的字符串
 *					s2			[in]		待比较的字符串
 *		@return		比较结果（true：相同；false：不同）
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
		char ch = s1[i];				//	顺序处理 s1 中的字符

		int iPos = s2.find(ch);			//	在 s2 中寻找 s1 中的字符
		if( iPos == string::npos )
			return false;

		s2 = s2.erase(iPos,1);			//	找到后删除该字符
	}

	return true;
}
