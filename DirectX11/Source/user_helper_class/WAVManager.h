#ifndef __WAV_MANAGER_H_
#define __WAV_MANAGER_H_


#ifndef _MAP_
#include	<map>
#endif
namespace load_wav_struct{
	struct tagChunk{
		char id[4];
		size_t size;
	};

	struct tagRiffCheck
	{
		tagChunk head;
		char format[4];
	};

	struct tagWaveFileFormat
	{
		unsigned short audioFormat;
		unsigned short channels;
		unsigned int samplePerSecond;
		unsigned int bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
	};
};

//	WAV���
struct tagWAVFile{
	load_wav_struct::tagRiffCheck		riff;
	load_wav_struct::tagWaveFileFormat	fileFormat;
	size_t			bufferSize;
	char*			pBuffer;
	const char*		pFilepath;
	int				format;
	unsigned int	bufferID[2];
	unsigned int	sourceID;

};
class WAVManager
{
private:
	static std::map<const int, tagWAVFile> m_wavFiles;

public:
	//------------------------------------------------------------------------------
	//
	//  @brief		WAV�t�@�C����ǂݍ���
	//	@param[in]	fileNum		�ݒ肷��t�@�C���ԍ�
	//	@param[in]	pFilepath	�t�@�C���p�X
	//	@return		true:����	false:���s
	//
	//------------------------------------------------------------------------------
	static bool LoadWAV(const int fileNum, const char* pFilepath);

private:
	static void GetWAVInfo(tagWAVFile* pWAVFile ,FILE* pFile);
	static void CreateBuffer(tagWAVFile* pWAVFile, FILE* pFile);

	static unsigned int BufferRead(
		FILE* pFile,
		char* pBuffer,
		const size_t bufferSize
		);





};
#endif // !__WAV_MANAGER_H_
