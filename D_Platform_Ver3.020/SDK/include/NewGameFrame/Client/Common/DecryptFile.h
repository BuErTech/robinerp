#ifndef DECRYPTFILE_HHH
#define DECRYPTFILE_HHH

/// ѹ���ļ��õ�FilePos
/// ���ܺ󣬻����һ�� PackPosInfo����stl��map��¼�˸����ļ�������Ӧ���ڴ��ַ�����ڴ泤��
/// �����ܺ�ֻ��Ҫ PackPosInfo[�ļ���] �����ɵõ����ļ������ļ�����Ӧ���ڴ�


/// ������ѹ������ѹ���ڴ�
/// fileNameѹ����·��  passWord��ѹ���루���ģ� pLen ѹ������С
/// ����ѹ�����ڴ棬��Ҫ����free�ͷ�
void * DecryptZipFile(const char *fileName,const char*passWord,unsigned long *pLen);

/// ���ڴ��л�ȡͼƬ
/// fileName ����ͼƬ�����·�� BufZipѹ�������ڴ� lenBufZipѹ�������ڴ泤��  pBuf����ͼƬ�ڴ� password��ѹ����
/// ���ظ���ͼƬ��С ��Ϊ0 �򵼳�ʧ��
unsigned long LoadFileFromZip(const char *fileName,void *BufZip,unsigned int lenBufZip,char **pBuf, const char *pHeadInfo);

#endif