/**
 *		���ߺ�������
 */
#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#include "define.h"

#include <vector>
#include <string>
using namespace std;

/**
 *		�ַ����ָ�
 *		@param		str			[in]		���ָ���ַ���
 *					delim		[in]		�ָ��
 *					rst			[out]		�������
 */
void split( const string& str, const string& delim, vector<string>& rst );

/**
 *		ȥ���ַ�����β���ַ���
 *		@param		str			[in/out]		��������ַ���
 *					cs			[in]			�����ַ���
 *					rst			[out]			��������( 0:���ǰ�ˣ�1:�����ˣ�2:���ǰ��� )
 */
void strip( string& str, string cs, int type );

/**
 *		ȥ���ַ����е��ַ�
 *		@param		str			[in]		��������ַ���
 *					c			[in]		ȥ���ַ�
 *		@return		�������ַ���
 */
string removefromstring( string str, char c );

/**
 *		�ж��ַ��������Ƿ���ͬ���������ַ�˳��
 *		@param		s1			[in]		���Ƚϵ��ַ���
 *					s2			[in]		���Ƚϵ��ַ���
 *		@return		true����ͬ��false����ͬ
 */
bool IsSame_NoOrder( string s1, string s2 );

/**
 *		ɾ���ļ������������ļ������������ļ��У�
 *		@param		strDir			[in]		�ļ���·��
 *		@return		true���ɹ���false��ʧ��
 */
bool CleanDir( string strDir );

/**
 *		�����ļ���
 *		@param		strDir			[in]		�ļ���·��
 *		@return		true���ɹ���false��ʧ��
 */
bool CreateDir( string strDir );

/**
 *		ɾ���ļ�
 *		@param		strDir			[in]		�ļ�·��
 *		@return		true���ɹ���false��ʧ��
 */
bool DeleteFile( string& strFile );

/**
 *		�����ļ�
 *		@param		strDir			[in]		�ļ���·��
 *		@return		true���ɹ���false��ʧ��
 */
bool CreateFile( string strFile );

/**
 *		��ȡ���ļ����б�
 *		@param		arrDirList			[out]		���ļ����б�
 *					strDir				[in]		�ļ���·��
 *		@return		���ļ��и���
 */
int GetSubDirList( vector<string>& arrDirList, string strDir );

/**
 *		��ȡ���ļ��б�
 *		@param		arrFileList			[out]		���ļ��б�
 *					strDir				[in]		�ļ���·��
 *		@return		���ļ�����
 */
int GetSubFileList( vector<string>& arrFileList, string strDir );

/**
 *		��ȡ��ǰʱ�䣨��Linuxƽ̨��Ч��
 *		@param		sec			[out]		��
 *					usec		[out]		΢��
 *		@return		�Ƿ�ɹ�
 */
bool GetTime( long& sec, long& usec );

#endif	//	_TOOLKIT_H_
