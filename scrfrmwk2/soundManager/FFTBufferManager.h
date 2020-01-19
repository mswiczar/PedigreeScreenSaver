
#define int32_t int
#define UInt32 unsigned int
#define Boolean bool


//#include <AudioToolbox/AudioToolbox.h>
//#include <libkern/OSAtomic.h>

#include "SpectrumAnalysis.h"

class FFTBufferManager
{
public:
	FFTBufferManager(int inNumberFrames);
	~FFTBufferManager();
	
	volatile long	HasNewAudioData()	{ return mHasAudioData; }
	volatile long	NeedsNewAudioData() { return mNeedsAudioData; }

	UInt32				GetNumberFrames() { return mNumberFrames; }
	void				GrabAudioData( void* mData , UInt32 mDataByteSize );
	Boolean				ComputeFFT(int32_t *outFFTData);
	
private:
	volatile long	mNeedsAudioData;
	volatile long	mHasAudioData;
	
	H_SPECTRUM_ANALYSIS mSpectrumAnalysis;
	
	int32_t*			mAudioBuffer;
	UInt32				mNumberFrames;
	UInt32				mAudioBufferSize;
	int32_t				mAudioBufferCurrentIndex;
};