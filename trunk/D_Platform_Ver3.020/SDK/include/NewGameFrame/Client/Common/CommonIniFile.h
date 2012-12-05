#pragma once
#define MAX_STRING_SIZE 128
#include "CommonDefines.h"
#include <map>
#include <cstring>

//#include <tchar.h>

class CINIFile  
{
  Tstring lpFileName;
  DWORD mMaxSize ;
public:
	static bool IsFileExist(TCHAR *fileName)
	{
		WIN32_FIND_DATA  d;
		HANDLE  hd= FindFirstFile(fileName,&d);
		bool bRet = (hd != INVALID_HANDLE_VALUE);
		FindClose(hd);
		return bRet;
	}
	static void CopyFileTo(TCHAR *destFileName, TCHAR *srcFileName)
	{
		bool bb=IsFileExist(srcFileName);
		if(!bb)return;
		CopyFile( srcFileName, destFileName, FALSE);

	}
		
	static Tstring GetWinSysPath()
	{
		TCHAR lpBuffer[MAX_STRING_SIZE];
		GetSystemDirectory(lpBuffer, MAX_STRING_SIZE);
		Tstring ss=lpBuffer;
		ss += TEXT("\\");
		return ss;

	}
		
	static Tstring GetAppPath(bool bFource=true)
	 {
         // PengJiLin, 2010-6-7, �̶�ʹ�õڶ��ַ�ʽ��ȡ����·����
         // ��һ�ַ�ʽ��ĳЩ����»�������⡣
         bFource = true;

		if(!bFource)
		{
			TCHAR lpBuffer[MAX_STRING_SIZE];
			ZeroMemory(lpBuffer,sizeof(lpBuffer));
			GetCurrentDirectory(MAX_STRING_SIZE,lpBuffer);
			Tstring ss=lpBuffer;
			ss += TEXT("\\");
			return ss;
		}
		else
		{
			TCHAR szModuleName[MAX_PATH];
			ZeroMemory(szModuleName,sizeof(szModuleName));
			DWORD dwLength=GetModuleFileName(GetModuleHandle(NULL), szModuleName, sizeof(szModuleName));
			Tstring filepath=szModuleName;
			Tstring path;
			int nSlash = filepath.ReverseFind(_T('/'));
			if (nSlash == -1)
				nSlash = filepath.ReverseFind(_T('\\'));
			if (nSlash != -1 && filepath.GetLength() > 1)
			{
				path = filepath.Left(nSlash+1);
				//SetCurrentDirectory(path);
				return path;
			}
			else
			{
				TCHAR lpBuffer[MAX_STRING_SIZE];
				ZeroMemory(lpBuffer,sizeof(lpBuffer));
				GetCurrentDirectory(MAX_STRING_SIZE,lpBuffer);
				Tstring ss=lpBuffer;
				ss += TEXT("\\");
				return ss;
			}

		}
	}

	int GetKeyVal(Tstring secName,Tstring keyName,int lpDefault)
	{
		return (int) GetPrivateProfileInt(
					secName,/// points to section name
					keyName,/// points to key name
					lpDefault,       /// return value if key name not found
					lpFileName///LPCTSTR lpFileName  /// initialization file name
					);

	}
	

	Tstring GetKeyVal(Tstring secName,Tstring keyName,LPCTSTR lpDefault)
	{ 
		TCHAR* re = new TCHAR[mMaxSize + 1];
		ZeroMemory(re,mMaxSize);
		GetPrivateProfileString(
								secName,/// points to section name
								keyName,/// points to key name
								lpDefault,/// points to default string
								re,/// points to destination buffer
								mMaxSize,/// size of destination buffer
								lpFileName /// points to initialization filename
							);
		Tstring ss=re;
		delete []re;
		return ss;
	}
		
	void SetKeyValString(Tstring secName,Tstring keyName,Tstring Val)
	{ 

		WritePrivateProfileString(
								secName,/// pointer to section name
								keyName,/// pointer to key name
								Val,/// pointer to string to add
								lpFileName/// pointer to initialization filename
								);

	}

		
	CINIFile(Tstring FileName,int maxsize=MAX_STRING_SIZE)
	{
		lpFileName=FileName;
		mMaxSize = maxsize;

	}

	~CINIFile()	{}
	void SetINIFileName(Tstring fileName){lpFileName=fileName;}
	
};


#define MAX_PASSWORD_LENGTH    255
struct FileItem
{
	void * _pBufFile;
	ULONG  _nFileLen;
	unsigned char _digest[MAX_PASSWORD_LENGTH];
	FileItem()
	{
		_pBufFile=NULL;
		_nFileLen=0;
		ZeroMemory(_digest,MAX_PASSWORD_LENGTH);
	}
	~FileItem()
	{
		if(_pBufFile!=NULL)
		{
			delete []_pBufFile;
			_pBufFile=NULL;
		}
	}
};

/////��Դ�ļ�������,��Ҫ�Ƕ�ȡͼƬ�ļ���
///// ��ʹ���ߵ��õ���
//class CUnZipRes
//{
//	static std::map<Tstring,FileItem *> mapFile;
//	bool m_bIsFileExist;
//	static unsigned char m_bPassWord[MAX_PASSWORD_LENGTH];
//public:
//	CUnZipRes(){}
//	~CUnZipRes(){}
//	static void ClearMap()
//	{
//		std::map<Tstring,FileItem *>::iterator iter=mapFile.begin();
//		while (iter != mapFile.end())
//		{
//			FileItem * pFileIterm = iter->second;
//			if (pFileIterm != NULL)
//			{
//				delete pFileIterm;
//			}
//			++iter;
//		}
//		mapFile.clear();
//	}
//	
//	static void SetPassWord(char * password)
//	{
//		memset(m_bPassWord,0,MAX_PASSWORD_LENGTH);
//		if(password!=NULL)
//		{
//			memcpy(m_bPassWord,password,min(strlen(password) ,MAX_PASSWORD_LENGTH));
//		}
//	}
//
//	static bool ReadFile(TCHAR * pPath, unsigned char* password)
//	{
//		Tstring strPath(pPath);
//		strPath.Replace("/","\\");
//		strPath.Replace("//","\\");
//		strPath.Replace("\\\\","\\");
//		strPath.Replace("Image","image");
//		//DebugPrintf("�����ļ���%s",pPath);
//
//		Tstring strPathTmp=GetAppPath(false);
//		strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);  ///<ȡ�õ�ǰ����·��
//		int pos=strPath.Find(strPathTmp);
//		Tstring strMap;
//		TCHAR * pchar=NULL;
//		Tstring strP;
//		if(pos==-1)
//		{
//			///�ж����ϼ�Ŀ¼���ļ����ǵ�ǰĿ¼���ļ�
//			strMap=strPathTmp;
//			int pos=strPath.Find("..\\");
//			Tstring strFileName=strPath;
//			if(pos!=-1)  ///<������ϼ�Ŀ¼���ļ������ڵ�ǰ����Ŀ¼��·���³�ȥ�����һ���ļ���
//			{
//				int  posMap=strPathTmp.ReverseFind('\\');
//				strMap=strPathTmp.Left(posMap);
//				strFileName=strPath.Right(strPath.GetLength()-3);
//			}
//
//
//			strP=strMap+"\\"+strFileName;     ///<������Ĺ���·���ټ����ļ���Ϊ�����ļ���ȫ·��
//			pchar=strP.GetBuffer(strP.GetLength() + 1);
//		}else
//		{
//			pchar=strPath.GetBuffer(strPath.GetLength()+1);     ///<����Ǵ���ȫ·������ֱ�Ӵ���
//			int  posMap=strPath.ReverseFind('\\');
//			strMap=strPath.Left(posMap);
//		}	
//		
//		///<������Ѿ���ȡ�����ļ������ٶ�ȡ��
//		if(mapFile.find(strMap)!=mapFile.end() )
//		{
//			return true;
//		}
//
//		unsigned char digest[MAX_PASSWORD_LENGTH];
//		ZeroMemory(digest,MAX_PASSWORD_LENGTH);
//		encryptPW(password,strlen((char *)password),digest);
//		FileItem * pFileItem=new FileItem();
//		UINT len;
//		pFileItem->_pBufFile = DecryptZipFile(pchar,(char*)digest,&len);
//		pFileItem->_nFileLen=len;
//
//		if(len==0||pFileItem->_pBufFile==NULL)
//		{
//			return false;
//		}
//		memcpy(pFileItem->_digest,digest,MAX_PASSWORD_LENGTH);
//		mapFile[strMap]=pFileItem;
//
//		return true;
//	}
//
//	static bool IsFileExist(Tstring fileName)
//	{
//		//Tstring str=GetAppPath(false);
//		//Tstring  strName=str+fileName;
//		WIN32_FIND_DATA  d;
//		HANDLE  hd= FindFirstFile(fileName,&d);
//		bool bRet = (hd != INVALID_HANDLE_VALUE);
//		FindClose(hd);
//		return bRet;
//	}
//
//	static IStream * LoadFileResFromZip(TCHAR *fileName)
//	{
//		if(fileName==NULL)
//		{
//			return NULL;
//		}
//		Tstring strFile(fileName);
//		//DebugPrintf("����ȫ·��%s",fileName);
//		strFile.Replace("/","\\");  ///<��б��ͳһ��"\"
//		strFile.Replace("//","\\");
//		strFile.Replace("\\\\","\\");
//		strFile.Replace("Image","image");
//		//DebugPrintf("ͼƬ����%s",fileName);
//
//		std::map<Tstring,FileItem *>::iterator iter;
//		///���ж��Ƿ���ȫ·��������ǣ���ֱ��ȡ·������MAP����û������У���ֱ�Ӷ����ļ������û�У�����������ж�
//		int posAll=strFile.Find("image");
//		if(posAll==-1)
//		{
//			return NULL;
//		}
//		Tstring strMapAll=strFile.Left(posAll-1);
//		//DebugPrintf("�����е�Image��߲���%s",strMapAll);
//		iter=mapFile.find(strMapAll);
//		TCHAR  * pchar=NULL;
//		Tstring strP;
//		if(iter==mapFile.end())///<����������и�·����˵�����ľ͵�ȫ·��
//		{
//
//			Tstring strPathTmp=GetAppPath(false);   ///<��ȡ��ǰ����·��
//			int posfile=strFile.Find(strPathTmp);   
//			if(posfile!=-1)///<�жϴ���·����������·��û�����У����ȥ
//			{
//				strFile=strFile.Right(strFile.GetLength()-strPathTmp.GetLength());
//			}
//			strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);   ///<������Ϊȡ�õĵ�ǰ����·��������һ����\�������Գ��ȼ�һ
//			
//			
//
//			///<�������ҳ�ͼƬ�����ļ����ڴ��ļ���
//			int pos1=strFile.Find("image");
//			strP=strFile.Right(strFile.GetLength()-pos1);
//			pchar=strP.GetBuffer(strP.GetLength() + 1);   ///<�ȶ����ļ���������
//
//			int pos=strFile.Find("..\\"); 
//			Tstring strMap;
//			if(pos!=-1)  ///<������ϼ�Ŀ¼����������һ���ļ��г�ȥ
//			{
//				int  posMap=strPathTmp.ReverseFind('\\');
//				strMap=strPathTmp.Left(posMap);
//				iter=mapFile.find(strMap);
//
//			}else
//			{
//				Tstring strAll=GetAllPath(strFile); ///<��ȡȫ·��
//				int posTmp=strAll.Find("image");
//				strMap=strAll.Left(posTmp-1);
//				iter=mapFile.find(strMap);
//				if(iter==mapFile.end())
//				{
//					Tstring strMapAll=strMap+"\\"+"image.r";
//					TCHAR * pTmp=strMapAll.GetBuffer(strMapAll.GetLength()+1);
//					ReadFile(pTmp,m_bPassWord);
//					iter=mapFile.find(strMap);
//				}
//				//iter=mapFile.find(strPathTmp);
//			} 
//
//			
//		}else
//		{
//			int pos1=strFile.Find("image");
//			strP=strFile.Right(strFile.GetLength()-pos1);
//			pchar=strP.GetBuffer(strP.GetLength() + 1);
//		}
//
//
//		if(iter==mapFile.end())
//		{
//			return NULL;
//		}
//
//		///<����ҵ��ˣ���ͼƬ���·���ҳ���
//		//int pos1=strFile.Find("image");
//		//Tstring strP=strFile.Right(strFile.GetLength()-pos1);
//		//TCHAR * pchar=strP.GetBuffer(strP.GetLength() + 1);
//		//DebugPrintf("ͼƬ���·��%s",pchar);
//		//Tstring strPath(pchar);
//
//
//
//		void * pBufFile=iter->second->_pBufFile;
//		UINT len=iter->second->_nFileLen;
//		ULONG size=0;
//		unsigned char digest[MAX_PASSWORD_LENGTH]={0};
//		memcpy(digest,iter->second->_digest,MAX_PASSWORD_LENGTH);
//		char *pBuf = NULL;
//		/*int pos=strFile.Find("image");*/
//		//strFile=strFile.Right(strFile.GetLength()-pos);
//		//TCHAR * pcharPath=strFile.GetBuffer(strPath.GetLength() + 1);  ///<��ȥ��ǰEXE��·����ȡ�����·������ͼƬ�ڴ�
//		if(len==0||pBufFile==NULL)
//		{
//			return NULL;
//		}
//		size = LoadFileFromZip(pchar,pBufFile,len,&pBuf,(char*)digest);
//
//		//DebugPrintf("��ȡͼƬ��С%d",size);
//		if(size==0 || pBuf==NULL)
//		{
//			return NULL;
//		}
//		IStream *pStream = NULL;
//		ToIStream(pBuf,size,pStream);
//		if(pBuf!=NULL)
//		{
//			delete []pBuf;
//			pBuf=NULL;
//		}
//		return pStream;
//	}
//	
//	static char * LoadFileBufferFromZip(TCHAR *fileName,int & BuffLen)
//	{
//		if(fileName==NULL)
//		{
//			return NULL;
//		}
//		Tstring strFile(fileName);
//		//DebugPrintf("����ȫ·��%s",fileName);
//		strFile.Replace("/","\\");  ///<��б��ͳһ��"\"
//		strFile.Replace("//","\\");
//		strFile.Replace("\\\\","\\");
//		strFile.Replace("Image","image");
//		//DebugPrintf("ͼƬ����%s",fileName);
//
//		std::map<Tstring,FileItem *>::iterator iter;
//		///���ж��Ƿ���ȫ·��������ǣ���ֱ��ȡ·������MAP����û������У���ֱ�Ӷ����ļ������û�У�����������ж�
//		int posAll=strFile.Find("image");
//		if(posAll==-1)
//		{
//			return NULL;
//		}
//		Tstring strMapAll=strFile.Left(posAll-1);
//		//DebugPrintf("�����е�Image��߲���%s",strMapAll);
//		iter=mapFile.find(strMapAll);
//		TCHAR  * pchar=NULL;
//		Tstring strP;
//		if(iter==mapFile.end())///<����������и�·����˵�����ľ͵�ȫ·��
//		{
//
//			Tstring strPathTmp=GetAppPath(false);   ///<��ȡ��ǰ����·��
//			int posfile=strFile.Find(strPathTmp);   
//			if(posfile!=-1)///<�жϴ���·����������·��û�����У����ȥ
//			{
//				strFile=strFile.Right(strFile.GetLength()-strPathTmp.GetLength());
//			}
//			strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);   ///<������Ϊȡ�õĵ�ǰ����·��������һ����\�������Գ��ȼ�һ
//
//
//
//			///<�������ҳ�ͼƬ�����ļ����ڴ��ļ���
//			int pos1=strFile.Find("image");
//			strP=strFile.Right(strFile.GetLength()-pos1);
//			pchar=strP.GetBuffer(strP.GetLength() + 1);   ///<�ȶ����ļ���������
//
//			int pos=strFile.Find("..\\"); 
//			Tstring strMap;
//			if(pos!=-1)  ///<������ϼ�Ŀ¼����������һ���ļ��г�ȥ
//			{
//				int  posMap=strPathTmp.ReverseFind('\\');
//				strMap=strPathTmp.Left(posMap);
//				iter=mapFile.find(strMap);
//
//			}else
//			{
//				Tstring strAll=GetAllPath(strFile); ///<��ȡȫ·��
//				int posTmp=strAll.Find("image");
//				strMap=strAll.Left(posTmp-1);
//				iter=mapFile.find(strMap);
//				if(iter==mapFile.end())
//				{
//					Tstring strMapAll=strMap+"\\"+"image.r";
//					TCHAR * pTmp=strMapAll.GetBuffer(strMapAll.GetLength()+1);
//					ReadFile(pTmp,m_bPassWord);
//					iter=mapFile.find(strMap);
//				}
//				//iter=mapFile.find(strPathTmp);
//			} 
//
//
//		}else
//		{
//			int pos1=strFile.Find("image");
//			strP=strFile.Right(strFile.GetLength()-pos1);
//			pchar=strP.GetBuffer(strP.GetLength() + 1);
//		}
//
//
//		if(iter==mapFile.end())
//		{
//			return NULL;
//		}
//
//		///<����ҵ��ˣ���ͼƬ���·���ҳ���
//		//int pos1=strFile.Find("image");
//		//Tstring strP=strFile.Right(strFile.GetLength()-pos1);
//		//TCHAR * pchar=strP.GetBuffer(strP.GetLength() + 1);
//		//DebugPrintf("ͼƬ���·��%s",pchar);
//		//Tstring strPath(pchar);
//
//
//
//		void * pBufFile=iter->second->_pBufFile;
//		UINT len=iter->second->_nFileLen;
//		ULONG size=0;
//		unsigned char digest[MAX_PASSWORD_LENGTH]={0};
//		memcpy(digest,iter->second->_digest,MAX_PASSWORD_LENGTH);
//		char *pBuf = NULL;
//		/*int pos=strFile.Find("image");*/
//		//strFile=strFile.Right(strFile.GetLength()-pos);
//		//TCHAR * pcharPath=strFile.GetBuffer(strPath.GetLength() + 1);  ///<��ȥ��ǰEXE��·����ȡ�����·������ͼƬ�ڴ�
//		if(pBufFile==NULL || len==0)
//		{
//			return NULL;
//		}
//		size = LoadFileFromZip(pchar,pBufFile,len,&pBuf,(char*)digest);
//		
//		//DebugPrintf("��ȡͼƬ��С%d",size);
//		if(size==0 || pBuf==NULL)
//		{
//			return NULL;
//		}
//		BuffLen=size;
//		return pBuf;
//	}
//
//
//
//	static Tstring GetAppPath(bool bFource=true)
//	{
//		bFource = true;
//		if(!bFource)
//		{
//			TCHAR lpBuffer[1024];
//			ZeroMemory(lpBuffer,sizeof(lpBuffer));
//			GetCurrentDirectory(1024,lpBuffer);
//			Tstring ss=lpBuffer;
//			ss += "\\";
//			return ss;
//		}
//		else
//		{
//			TCHAR szModuleName[MAX_PATH];
//			ZeroMemory(szModuleName,sizeof(szModuleName));
//			DWORD dwLength=GetModuleFileName(AfxGetInstanceHandle(),szModuleName,sizeof(szModuleName));
//			Tstring filepath=szModuleName;
//			Tstring path;
//			int nSlash = filepath.ReverseFind(_T('/'));
//			if (nSlash == -1)
//				nSlash = filepath.ReverseFind(_T('\\'));
//			if (nSlash != -1 && filepath.GetLength() > 1)
//			{
//				path = filepath.Left(nSlash+1);
//				SetCurrentDirectory(path);
//				return path;
//			}
//			else
//			{
//				TCHAR lpBuffer[1024];
//				ZeroMemory(lpBuffer,sizeof(lpBuffer));
//				GetCurrentDirectory(1024,lpBuffer);
//				Tstring ss=lpBuffer;
//				ss += "\\";
//				return ss;
//			}
//
//		}
//	}
//	
//	static Tstring GetAllPath(Tstring strCurPath)
//	{
//		Tstring strPathTmp=GetAppPath(false);   ///<��ȡ��ǰ����·��
//		strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);   ///<������Ϊȡ�õĵ�ǰ����·��������һ����\�������Գ��ȼ�һ
//		
//		int posCur=strCurPath.Find(strPathTmp);
//		
//		if(posCur!=-1)
//		{
//			strCurPath.Replace("\\\\","\\");
//			return strCurPath;
//		}else
//		{
//			int pos=strCurPath.Find("..\\");  
//			if(pos!=-1)  ///<������ϼ�Ŀ¼����������һ���ļ��г�ȥ
//			{
//				int  posMap=strPathTmp.ReverseFind('\\');
//				strPathTmp=strPathTmp.Left(posMap);
//			}
//			int posfile=strCurPath.Find(".\\");
//			if(posfile!=-1)
//			{
//				strCurPath=strCurPath.Right(strCurPath.GetLength()-posfile-1);
//			}
//			Tstring strAll=strPathTmp+"\\"+strCurPath;
//
//			strAll.Replace("\\\\","\\");
//			return strAll;
//		}
//	}
//};
