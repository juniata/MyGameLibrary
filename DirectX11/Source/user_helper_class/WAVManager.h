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

//	WAV情報
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
	//  @brief		WAVファイルを読み込む
	//	@param[in]	fileNum		設定するファイル番号
	//	@param[in]	pFilepath	ファイルパス
	//	@return		true:成功	false:失敗
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
