#include "stdafx.h"
#include "FFTBufferManager.h"
#include <winbase.h>

//#define min(x,y) (x < y) ? x : y

FFTBufferManager::FFTBufferManager(int inNumberFrames) :
	mNeedsAudioData(0),
	mHasAudioData(0),
	mNumberFrames(inNumberFrames),
	mAudioBufferSize(inNumberFrames * sizeof(int32_t)),
    mAudioBufferCurrentIndex(0)
    
{
	mAudioBuffer = (int32_t*)malloc(mAudioBufferSize);	
	mSpectrumAnalysis = SpectrumAnalysisCreate(mNumberFrames);
	InterlockedIncrement(&mNeedsAudioData);

}

FFTBufferManager::~FFTBufferManager()
{
	free(mAudioBuffer);
	SpectrumAnalysisDestroy(mSpectrumAnalysis);
}



void FFTBufferManager::GrabAudioData( void* mData , UInt32 mDataByteSize )
{
	if (mAudioBufferSize < mDataByteSize)	return;
	UInt32 bytesToCopy = min(mDataByteSize, mAudioBufferSize - mAudioBufferCurrentIndex);
	memcpy(mAudioBuffer+mAudioBufferCurrentIndex, mData, bytesToCopy);
	mAudioBufferCurrentIndex += bytesToCopy / sizeof(int32_t);
	if (mAudioBufferCurrentIndex >= mAudioBufferSize / sizeof(int32_t))
	{

			InterlockedIncrement(&mHasAudioData);
			InterlockedIncrement(&mNeedsAudioData);
	}
}

Boolean	FFTBufferManager::ComputeFFT(int32_t *outFFTData)
{
	int zzz;
	WCHAR salida[512];
	if (HasNewAudioData())
	{
			SpectrumAnalysisProcess(mSpectrumAnalysis, mAudioBuffer, outFFTData, true);
				/*
			for (zzz=0; zzz<100; zzz++)
			{
				ZeroMemory(salida,sizeof(salida));
				wsprintfW(salida,L"z= %d, %d\n",zzz,outFFTData[zzz]);
				OutputDebugStringW(salida);
			}
			*/
			InterlockedIncrement(&mNeedsAudioData);
			InterlockedDecrement(&mHasAudioData);
			mAudioBufferCurrentIndex = 0;
			return true;
	}
	else if (mNeedsAudioData == 0)
	{
			InterlockedIncrement(&mNeedsAudioData);
	}

	return false;
}