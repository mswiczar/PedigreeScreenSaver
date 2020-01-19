#include "stdlib.h"
#include "stdafx.h"
#include "CRecMic.h"
#include <math.h>

#define MAX_NB_BYTES 1024*10 
#define TAMANO_BUFFER 10
#pragma comment(lib, "winmm.lib")

float  colorLevels[] = {
0., 1., 0., 0., 0., 
.333, 1., .7, 0., 0., 
.667, 1., 0., 0., 1., 
1., 1., 0., 1., 1., 
};


#define SInt8 signed char

#define UInt8 unsigned char

#define CLAMP(min,x,max) (x < min ? min : (x > max ? max : x))

inline double linearInterp(double valA, double valB, double fract)
{
	return valA + ((valB - valA) * fract);
}

void renderFFTToTex(void * dwUser)
{


	CRecMic* MyRecMic =(CRecMic*) dwUser;
	WCHAR salida[512];

	static int numLevels = sizeof(colorLevels) / sizeof(float) / 5;
	ARRAY_DATA arraydata[90];
	int y, maxY;
	maxY= 300;
	memset(arraydata, 0, sizeof(arraydata));

	for (y=0; y<maxY; y++)
	{
		float yFract = (float)y / (float)(maxY - 1);
		float fftIdx = yFract * ((float)MyRecMic->fftLength-1);
		
		double fftIdx_i, fftIdx_f;
		fftIdx_f = modf(fftIdx, &fftIdx_i);
		
		SInt8 fft_l, fft_r;
		float fft_l_fl, fft_r_fl;
		float interpVal;
		
		fft_l = (MyRecMic->fftData[(int)fftIdx_i] & 0xFF000000) >> 24;
		fft_r = (MyRecMic->fftData[(int)fftIdx_i + 1] & 0xFF000000) >> 24;
		fft_l_fl = (float)(fft_l + 80) / 64.;
		fft_r_fl = (float)(fft_r + 80) / 64.;
		interpVal = fft_l_fl * (1. - fftIdx_f) + fft_r_fl * fftIdx_f;



		interpVal = sqrt(CLAMP(0., interpVal, 1.));



		
		
		UInt32 newPx = 0xFF000000;





	
		int level_i;
		const float *thisLevel = colorLevels;
		const float *nextLevel = colorLevels + 5;
		for (level_i=0; level_i<(numLevels-1); level_i++)
		{
			if ( (*thisLevel <= interpVal) && (*nextLevel >= interpVal) )
			{
				double fract = (interpVal - *thisLevel) / (*nextLevel - *thisLevel);
				newPx = 
				((UInt8)(255. * linearInterp(thisLevel[1], nextLevel[1], fract)) << 24)
				|
				((UInt8)(255 * linearInterp(thisLevel[2], nextLevel[2], fract)) << 16)
				|
				((UInt8)(255. * linearInterp(thisLevel[3], nextLevel[3], fract)) << 8)
				|
				(UInt8)(255. * linearInterp(thisLevel[4], nextLevel[4], fract))
				;
				
				if(y<90)
				{
					//unsigned char	A = ((unsigned char)(255. * linearInterp(thisLevel[1], nextLevel[1], fract)) );
					unsigned char	B = ((unsigned char)(255 * linearInterp(thisLevel[2], nextLevel[2], fract)) );
					unsigned char	G = ((unsigned char)(255. * linearInterp(thisLevel[3], nextLevel[3], fract))  );
					unsigned char	R = ((unsigned char)(255. * linearInterp(thisLevel[4], nextLevel[4], fract))  );
					
					arraydata[y].R =R;
					arraydata[y].G =G;
					arraydata[y].B =B;

					/*
					if ((R !=0 ) ||(G !=0 )|| (B !=0 ))
					{
						if (R !=0 )
						{
							if((y >20) && (
									   y <30))
							{
								if(R>20) 
								{
									ZeroMemory(salida,sizeof(salida));
									wsprintfW(salida,L"Level: %d , RGB (%d , %d , %d ) \n",y,arraydata[y].R,arraydata[y].G,arraydata[y].B);
									OutputDebugStringW(salida);
								}
							}
						}
					}
					*/
				}
				break;
			}
			thisLevel+=5;
			nextLevel+=5;
		}
	}
	
	if (SetArray(arraydata)==1)
	{
		ZeroMemory(salida,sizeof(salida));
		wsprintfW(salida,L"SILVO\n");
		((functiontocall) MyRecMic->thecall) (MyRecMic->theparam);
		OutputDebugStringW(salida);

		MyRecMic->varsilvo=1;
	}
	/*
	ZeroMemory(salida,sizeof(salida));
	wsprintfW(salida,L"-------------------\n");
	OutputDebugStringW(salida);
	*/

	ZeroMemory(salida,sizeof(salida));
	wsprintfW(salida,L"NO!!\n");
	OutputDebugStringW(salida);
		
	
	MyRecMic->hasNewFFTData  = false;
}



static void setFFTData(int32_t * FFTDATA, int32_t LENGTH,void * dwUser)
{
	CRecMic* MyRecMic =(CRecMic*) dwUser;

	if (LENGTH != (MyRecMic->fftLength))
	{
		MyRecMic->fftLength = LENGTH;
		MyRecMic->fftData = (signed long *)(realloc(MyRecMic->fftData, LENGTH * sizeof(signed int)));
	}
	memmove(MyRecMic->fftData, FFTDATA, MyRecMic->fftLength * sizeof(float));


	MyRecMic->hasNewFFTData = true;
}


void dibujar(void * dwUser)
{
	CRecMic* MyRecMic =(CRecMic*) dwUser;

	if(MyRecMic->thebuff !=NULL)
	{
		if (MyRecMic->thebuff->HasNewAudioData())
		{
			if (MyRecMic->thebuff->ComputeFFT(MyRecMic->l_fftData))
			{
				setFFTData(MyRecMic->l_fftData,MyRecMic->thebuff->GetNumberFrames() / 2, dwUser);
			}
			else
			{
				MyRecMic->hasNewFFTData = false;
			}
	
			if (MyRecMic->hasNewFFTData)
			{
				renderFFTToTex(dwUser);
			}
		}
	
	}
	

	
}




void CALLBACK WaveInProcedure(HWAVEIN hwi, UINT uMsg,    DWORD dwInstance,    DWORD dwParam1,   DWORD dwParam2     )
{
	WCHAR salida[512];
	LPWAVEHDR myLPWAVEHDR;
	if (uMsg = WIM_DATA) 
	{
		myLPWAVEHDR = (LPWAVEHDR) dwParam1;
		if (myLPWAVEHDR != NULL)
		{
				CRecMic* MyRecMic =(CRecMic*) myLPWAVEHDR->dwUser;
				waveInUnprepareHeader  (MyRecMic->mywaveIn,  myLPWAVEHDR, sizeof(WAVEHDR));
				int musetra[1024];
				short int *in =(short int *) myLPWAVEHDR->lpData;
				memset(musetra, 0, sizeof(musetra));
				if( MyRecMic->thebuff != NULL) 
				{
					if (MyRecMic->thebuff->NeedsNewAudioData())
					{
						for (int zzz=0; zzz<myLPWAVEHDR->dwBytesRecorded/2; zzz++)
						{
							musetra[zzz] = in[zzz]<<8;;
						}
						MyRecMic->thebuff->GrabAudioData(musetra, myLPWAVEHDR->dwBytesRecorded*2); 
					}
				}

				dibujar(MyRecMic);

				if (MyRecMic->reset==false)
				{
					myLPWAVEHDR->dwFlags = 0;
					waveInPrepareHeader    (MyRecMic->mywaveIn,  myLPWAVEHDR, sizeof(WAVEHDR));
					waveInAddBuffer    (MyRecMic->mywaveIn,myLPWAVEHDR, sizeof(WAVEHDR));
				};
		}
	}
}







void CRecMic::StartRec(void * thecallback , void * theparameter)
{
	thecall = thecallback;
	theparam =theparameter;

	waveInStart(this->mywaveIn);
	IsPaused = false;
};


void CRecMic::StopRec(void)
{
	reset =true;
	Sleep(500);
	Sleep(500);
	

	if (MMSYSERR_NOERROR != waveInStop(mywaveIn))
	{
		//::MessageBox(0,L"MMSYSERR_NOERROR != waveInStop(mywaveIn)",L"Error",16);
	}
	Sleep(100);

	if (MMSYSERR_NOERROR != waveInReset(mywaveIn))
	{
		//::MessageBox(0,L"MMSYSERR_NOERROR != waveInReset(mywaveIn)",L"Error",16);
	
	}
	Sleep(100);

	if (MMSYSERR_NOERROR != waveInClose(mywaveIn))
	{
		//::MessageBox(0,L"MMSYSERR_NOERROR != waveInClose(mywaveIn)",L"Error",16);
		
	}
	Sleep(100);
	reset=false;

}


void CRecMic::Pause(void)
{
	if (IsPaused != true)
	{
		waveInStop(mywaveIn);
		IsPaused = true;
	};

}


void CRecMic::Continue(void)
{
	if (IsPaused )
	{
		waveInStart(mywaveIn);
		IsPaused = false;
	};

}



void CRecMic::SetWaveFormat(void)
{
	this->MyWaveFormatex.cbSize          = sizeof(WAVEFORMATEX);
	this->MyWaveFormatex.nChannels       = 1;
	this->MyWaveFormatex.nSamplesPerSec  = 44100;
	this->MyWaveFormatex.wBitsPerSample  = 16;
	this->MyWaveFormatex.wFormatTag      = WAVE_FORMAT_PCM;
	this->MyWaveFormatex.nBlockAlign     = this->MyWaveFormatex.nChannels * (this->MyWaveFormatex.wBitsPerSample/8);
	this->MyWaveFormatex.nAvgBytesPerSec = this->MyWaveFormatex.nBlockAlign * this->MyWaveFormatex.nSamplesPerSec;
}




CRecMic::CRecMic()
{
	this->running=false;
	this->tiempo=0;
	this->Kbytes=0;
	reset=false;
	thebuff = new FFTBufferManager(4096);
	l_fftData = new int[2048];
	fftData = new signed long[2048];
	DWORD  myThreadIdIN;
	GMyDeviceIn   = WAVE_MAPPER;   
	int tamano;
	int i = 0;
	MMRESULT LRet;
	this->SetWaveFormat();
	LRet = waveInOpen(&this->mywaveIn,this->GMyDeviceIn,&this->MyWaveFormatex,(DWORD_PTR)WaveInProcedure,NULL,CALLBACK_FUNCTION);
	if (MMSYSERR_NOERROR  == LRet)
	{
		for (i = 0;i <TAMANO_BUFFER; i++)	
		{
			tamano =  2048;
    		myWaveHdr1[i].lpData         = (LPSTR) VirtualAlloc(NULL,tamano, MEM_COMMIT,PAGE_EXECUTE_READWRITE);
			myWaveHdr1[i].dwFlags        = 0;
			myWaveHdr1[i].dwBufferLength = tamano;
			myWaveHdr1[i].dwUser         = (DWORD_PTR) this;
			waveInPrepareHeader(this->mywaveIn,&myWaveHdr1[i], sizeof(WAVEHDR));
			waveInAddBuffer    (this->mywaveIn,&myWaveHdr1[i], sizeof(WAVEHDR));
		}
	}

}

CRecMic::~CRecMic()
{
	waveInClose(mywaveIn);
}
