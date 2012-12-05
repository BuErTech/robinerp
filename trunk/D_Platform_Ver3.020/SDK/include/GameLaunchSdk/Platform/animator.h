#ifndef _ANIMATOR_H_INCLUDE
#define _ANIMATOR_H_INCLUDE

#include <string>
#include <vector>

#define CHUNK 16384

typedef unsigned long  uLong; 
typedef unsigned char  Byte;  
typedef Byte  FAR Bytef;

/**
 * �Զ��嶯����
 */
class Animator
{
public:
	/**
	 * �����õ�������ṹ
	 */
	struct Point
	{
		Point():x(0),y(0) {}
		Point(int _x,int _y):x(_x),y(_y) {}

		int x,y;                                 // ����X,Y
	};

    /**
     * �����ļ�ͷ�ṹ
     */
    struct FileHeader
    {
		FileHeader():files_count(0),width(0),height(0),speed(0) {}

        char marker[3];                         // �ļ���ʶ "S","P","R"
        int files_count;                        // �ļ�����

	    int width,height;                       // �����ĸ߿�
		int speed;                              // �����Ĳ����ٶ�
    };

    /**
     * �ļ��ṹ
     */
    struct FileStruct
    {
		FileStruct():startx(0),starty(0),isDisplay(true),startposition(0),uncompressed_size(0),compressed_size(0),
			speed(0)
		{
		
		}

        char  filename[256];                    // �ļ���������
	    int startx,starty;                      // �������ŵĿ�ʼλ��
		bool isDisplay;                         // �Ƿ���ʾ����
		int speed;                              // ÿ֡�����Ĳ����ٶ�

        uLong startposition;                    // �ļ�������ѹ�����еĿ�ʼλ��

        uLong uncompressed_size;                // �������ļ�����
        uLong compressed_size;                  // ѹ������ļ�����
    };

	/// ���캯��
	Animator();
	/// ��������
	~Animator();

	/// ��ָ���Ķ����ļ�(·������Ϊ����ڵ�ǰĿ¼��·��)
	bool OpenAnimator(std::string filepath); 
	/// ���ڴ��м��ض����ļ�
	bool OpenAnimatorFromMemory(Byte *data,uLong length);
	/// �رն����ļ�
	void CloseAnimator(void);
	/// ���������浽ָ����·��(·������Ϊ����·�������Ϊ�յĻ�����ʾʹ�����Ǵ򿪵Ķ����ļ�����������ھ��׳�����)
	bool SaveAnimator(std::string filepath="");
	/// ��⵱ǰ�Ƿ��ڹ�����
	bool IsWorking(void);

	/// �õ���ǰ��Ĺ���·��
	inline std::string GetCurrentPath(void) { return m_CurrentPath + "\\"; }
	/// �õ���ǰ����������
	inline std::string GetName(void) { return m_FileName; }
	/// �õ���ǰ������֡��
	inline int GetFrameCount(void) { return (int)m_FileList.size(); }
	/// ���ö����ĸ߿�
	void SetWidthAndHeight(int w,int h);
	/// �õ������ĸ߿�
	void GetWidthAndHeight(int *w,int *h);
	/// ���ö������ŵ��ٶ�
	inline void SetSpeed(int s) { m_Header.speed = s; }
	/// �õ��������ŵ��ٶ�
	inline int GetSpeed(void) { return m_Header.speed; }
	/// ����ָ������֡���ٶ�
	void SetFrameSpeed(int index,int s);
	/// �õ�ָ������֡���ٶ�
	int GetFrameSpeed(int index);
	/// ����ָ��֡����ʾλ��
	void SetStartPosForFrame(int index,Animator::Point pos);
	/// �õ�ָ��֡����ʾλ��
	Animator::Point GetStartPosForFrame(int index);	
	/// �õ�ָ��֡����Ϣ
	Animator::FileStruct* GetFrameInfoById(int index);
	/// �õ�ָ��֡ͼƬ���Ƶ�֡��Ϣ
	Animator::FileStruct* GetFrameInfoByName(std::string filename);

	/// ���ָ����֡ͼƬ�������ļ���
	bool AddFrame(std::string filepath,int sx,int sy,bool isShow,int sp);
	/// ��յ�ǰ�Ķ���֡�б�
	inline void ClearFrame(void) { m_FileList.clear(); }
	/// ����ָ��ID�õ�֡ͼƬ����
	Byte* GetFrameById(int index,uLong* length);
	/// ����ָ�����Ƶõ�֡ͼƬ����
	Byte* GetFrameByName(std::string name,uLong* length);
	/// ��ָ����·���õ����е�֡ͼƬ(·������Ϊ����·����������óɿյĻ���ʾʹ�õ�ǰ·��)
	bool GetAllFrames(std::string path="");

private:
    /// ����ѹ���ƶ����ļ����ƶ��İ��ļ���
    int def(FILE *source, FILE *dest,uLong *size,int level,bool iswrite=true);
    /// ���ڽ�ѹѹ�����е����е��ļ�
    int inf(FILE *source, FILE *dest,uLong length,uLong *clength);
	/// ���ڽ�ѹѹ�����е��ƶ��ļ����ڴ���
	int inf(FILE *source, Byte *dest,uLong length,uLong *clength);
	/// ���ڽ���ָ���ڴ��е����ݵ�ָ���ڴ���
	int inf(Byte *src, Byte *dest,uLong startpos,uLong length,uLong *clength);
    /// �����ƶ���С���ڴ����ݵ�ָ���ڴ���
    void CopyDataToMemory(Bytef *dest,Bytef *src,uLong pos,uLong size);
	/// �����ļ�·����������Ӧ��Ŀ¼
	void CreateDirectionByPath(std::string path,std::string filename);

	/// ����ļ�ͷ���ļ��ṹ�鵽�ļ���
	bool AddFileHeaderAndFileStructs(void);
	/// ѹ�����е�ǰϵͳ��ӵ�е�����֡�������ļ���
	bool CompressAllFileInSystem(void);

private:
    std::string                             m_FileName;     // Ҫ�����Ķ����ļ�·����
	std::string                             m_CurrentPath;  // ��ǰ�����·��

    FileHeader                              m_Header;       // �����ļ���ͷ
    std::vector<FileStruct>                 m_FileList;     // �����ļ��ж�����֡�б�

	FILE*                                   m_File;         // �ļ���д
	Byte*                                   m_AnimData;     // �������� 
};

#endif
