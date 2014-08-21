/**
 *		工具函数集合
 */
#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#include "define.h"

#include <vector>
#include <string>
using namespace std;

/**
 *		字符串分割
 *		@param		str			[in]		待分割的字符串
 *					delim		[in]		分割符
 *					rst			[out]		结果数组
 */
void split( const string& str, const string& delim, vector<string>& rst );

/**
 *		去除字符串首尾的字符串
 *		@param		str			[in/out]		待清理的字符串
 *					cs			[in]			清理字符串
 *					rst			[out]			清理类型( 0:清除前端；1:清除后端；2:清除前后端 )
 */
void strip( string& str, string cs, int type );

/**
 *		去除字符串中的字符
 *		@param		str			[in]		待处理的字符串
 *					c			[in]		去除字符
 *		@return		处理后的字符串
 */
string removefromstring( string str, char c );

/**
 *		判断字符串内容是否相同（不考虑字符顺序）
 *		@param		s1			[in]		待比较的字符串
 *					s2			[in]		待比较的字符串
 *		@return		true：相同；false：不同
 */
bool IsSame_NoOrder( string s1, string s2 );

/**
 *		删除文件夹下所有子文件（不包括子文件夹）
 *		@param		strDir			[in]		文件夹路径
 *		@return		true：成功；false：失败
 */
bool CleanDir( string strDir );

/**
 *		创建文件夹
 *		@param		strDir			[in]		文件夹路径
 *		@return		true：成功；false：失败
 */
bool CreateDir( string strDir );

/**
 *		删除文件
 *		@param		strDir			[in]		文件路径
 *		@return		true：成功；false：失败
 */
bool DeleteFile( string& strFile );

/**
 *		创建文件
 *		@param		strDir			[in]		文件夹路径
 *		@return		true：成功；false：失败
 */
bool CreateFile( string strFile );

/**
 *		获取子文件夹列表
 *		@param		arrDirList			[out]		子文件夹列表
 *					strDir				[in]		文件夹路径
 *		@return		子文件夹个数
 */
int GetSubDirList( vector<string>& arrDirList, string strDir );

/**
 *		获取子文件列表
 *		@param		arrFileList			[out]		子文件列表
 *					strDir				[in]		文件夹路径
 *		@return		子文件个数
 */
int GetSubFileList( vector<string>& arrFileList, string strDir );

/**
 *		获取当前时间（仅Linux平台有效）
 *		@param		sec			[out]		秒
 *					usec		[out]		微秒
 *		@return		是否成功
 */
bool GetTime( long& sec, long& usec );

#endif	//	_TOOLKIT_H_
