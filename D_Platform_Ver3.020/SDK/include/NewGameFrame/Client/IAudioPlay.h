#include <Windows.h>

#ifdef AUDIO_EXPORTS
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif AUDIO_EXPORTS

interface IAudioPlay
{
public:
	virtual ~IAudioPlay(void) {};
public:
	///��������
	virtual void Play() = 0;
	///��������
	virtual void Play(const char* stFileName) = 0;
	///ֹͣ����
	virtual void Stop() = 0;
	///��ͣ
	virtual void Pause() = 0;
	///��������
	virtual void Continue() = 0;
	//��ȡ����
	virtual int GetVolume() = 0;
	///��������
	virtual void SetVolume(int iVolume) = 0;
	///�Ƿ��ڲ�����
	virtual bool IsPlaying() = 0;
	///���²���
	virtual void ResetPlay() = 0;
	///��֡
	virtual void SetPosition(int iPosition) = 0;
	///��ȡ��ǰ֡
	virtual int GetPosition() = 0;
	///��ȡ�����ļ�����
	virtual int GetLength() = 0;
	///�Ƿ�ѭ������
	virtual void SetLoopPlay(bool isLoop) = 0;
	///�Ƿ�ѭ������
	virtual int GetLoopPlay() = 0;
public:
	virtual void Next() = 0;
	virtual void Prep() = 0;
	virtual void Add(const char* stFileName) = 0;
	virtual void Delete(const char* stFileName) = 0;
};
AUDIO_API IAudioPlay * CreateAudioPlay();
AUDIO_API void DeleteAudioPlay(IAudioPlay** pAudioPlay);

extern "C" AUDIO_API IAudioPlay * CreateAudioPlayC();
extern "C" AUDIO_API void DeleteAudioPlayC(IAudioPlay** pAudioPlay);

typedef IAudioPlay *(*CREATEAUDIOPLAYC)();
typedef void (*DELETEAUDIOPLAYC)(IAudioPlay** pAudioPlay);
