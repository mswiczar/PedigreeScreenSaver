#include "windows.h"
#include "mmsystem.h"
#include "Mmreg.h"
#include "FFTBufferManager.h"
#include "MSSoundManager.h"


#define TAMANO_BUFFER 10

typedef void (WINAPI *functiontocall)( void*);


class CRecMic
{

	
public:
	int varsilvo;
	int *l_fftData;
	signed long* fftData;
	unsigned int fftLength;
	FFTBufferManager* thebuff;
	bool hasNewFFTData;


	HINSTANCE ahInst;
	WAVEFORMATEX MyWaveFormatex;
	HWAVEIN  mywaveIn;
	unsigned int GMyDeviceIn;
	WAVEHDR myWaveHdr1[TAMANO_BUFFER];
	DWORD sample;
	DWORD sampleBase;
	CRecMic();
	~CRecMic();
	bool IsPaused;
	void *thecall;
	void *theparam;
	void StartRec(void * thecallback , void * theparameter);
	void StopRec(void);
	void SetWaveFormat(void);
	void Pause(void);
	void Continue(void);
	ULONG tiempo;
	ULONG Kbytes;

	HANDLE RawHandle;
	DWORD segs;
	HWND ahwnd;
	WCHAR afile[512];

	int posicion;
	HWAVEOUT mywaveout;
	bool Stoped;
	long running;
	HANDLE athread;
	bool reset;
	HWND acallbackwindowput;


};

