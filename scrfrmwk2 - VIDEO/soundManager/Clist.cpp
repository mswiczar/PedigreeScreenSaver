#include "stdafx.h"	
#include "Clist.h"
#include "math.h"
//#include "stdio.h"
//#include "stdlib.h"


void Shrink(LPVOID lpParameter,DWORD sample,DWORD samplebase)
{
	div_t div_result;
	ITEMLIST* PItemList = (ITEMLIST*) lpParameter;
	DWORD indice;
	unsigned char * charcharBuff1 = (unsigned char *) PItemList->UnDato.Buffer;
	if (samplebase != 48)
	{
		indice = 1;
	}
	else
	{	
		indice = (samplebase*1000/sample);
	}

	for (int i= 0 ; i <=  PItemList->UnDato.BytesUsed -1 ; i++)
	{
		  
		div_result = div( i, (indice*2) );
		if ( div_result.rem == 0) 
		{
			charcharBuff1[i/indice]   =  charcharBuff1[i];
			charcharBuff1[i/indice+1] =  charcharBuff1[i+1];
		};
	}
	PItemList->UnDato.BytesComress =  PItemList->UnDato.BytesUsed / indice;
}


DWORD WINAPI FlushListA(LPVOID lpParameter )
{
	int i;
	CListA* AListA;
	AListA = (CListA*)lpParameter; 
	HANDLE myFile;
	DWORD BytesWriten;
	ITEMLIST*	PItemList;
	while (AListA->Running)
	{
		WaitForSingleObject(AListA->Myevent,1000);
    		EnterCriticalSection(&AListA->MyMutex);		
				ResetEvent(AListA->Myevent);
				if  (AListA->mylistaADD == AListA->mylista1)
				{
					AListA->mylistaADD   = AListA->mylista2;
					AListA->mylistaFlush = AListA->mylista1;
				}
				else
				{
					AListA->mylistaADD   = AListA->mylista1;
					AListA->mylistaFlush = AListA->mylista2;
				}
				AListA->mylistaADD->cantidadElementos = 0;
			LeaveCriticalSection(&AListA->MyMutex);
			PItemList = AListA->Removeitem();
			if (PItemList != NULL)
			{ 
				myFile = CreateFileW(AListA->FileName,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				SetFilePointer(myFile,0,NULL,FILE_END);
				while (PItemList != NULL)	
				{
					//Shrink(PItemList,AListA->mysample,AListA->mysampleBase);
					//WriteFile(myFile, PItemList->UnDato.Buffer,PItemList->UnDato.BytesComress,&BytesWriten,NULL);
					WriteFile(myFile, PItemList->UnDato.Buffer,PItemList->UnDato.BytesUsed,&BytesWriten,NULL);
					VirtualFree(PItemList->UnDato.Buffer,PItemList->UnDato.BytesUsed,MEM_DECOMMIT);
					VirtualFree(PItemList->UnDato.Buffer,0,MEM_RELEASE);
					VirtualFree(PItemList,sizeof(ITEMLIST),MEM_DECOMMIT);
					VirtualFree(PItemList,0,MEM_RELEASE);
					PItemList = AListA->Removeitem();
				}

				CloseHandle(myFile);
			}
		}

		return(0);
}



CListA::CListA(WCHAR * aFileName, DWORD asample,DWORD asampleBase)
{
	this->FileName = aFileName;
	this->mysample = asample;
	this->mysampleBase = asampleBase; 
	
	// borro el archivo por si existe
	HANDLE myfileWrite = CreateFileW(aFileName , GENERIC_WRITE , 0 , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL);
	CloseHandle(myfileWrite);
	Running = true;
	Myevent = CreateEvent(NULL,false,false,NULL);
	ResetEvent(Myevent);
	MytrheadFlush = CreateThread(NULL,0,FlushListA,(LPVOID)this,CREATE_SUSPENDED,NULL);
	InitializeCriticalSection(&MyMutex);
	mylista1 = (LISTA*)VirtualAlloc(NULL,sizeof(LISTA), MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	mylista2 = (LISTA*)VirtualAlloc(NULL,sizeof(LISTA), MEM_COMMIT,PAGE_EXECUTE_READWRITE);

	mylista1->cantidadElementos =0;
	mylista1->Header=NULL;
	mylista1->Tail =NULL;
		
	mylista2->cantidadElementos =0;
	mylista2->Header=NULL;
	mylista2->Tail =NULL;

	mylistaADD = mylista1;

	ResumeThread(MytrheadFlush);
}

CListA::~CListA()
{
	Running= false;
	SetEvent(Myevent);
	WaitForSingleObject(MytrheadFlush,INFINITE);
	CloseHandle(MytrheadFlush);
	CloseHandle(Myevent);
	DeleteCriticalSection(&MyMutex);

	VirtualFree(mylista1,sizeof(LISTA),MEM_DECOMMIT);
	VirtualFree(mylista1,0,MEM_RELEASE);

	VirtualFree(mylista2,sizeof(LISTA),MEM_DECOMMIT);
	VirtualFree(mylista2,0,MEM_RELEASE);

};

void CListA::AddItem(ITEMLIST* PItemList) 
{
    EnterCriticalSection(&MyMutex);		
		PItemList->UnDato.BytesComress =0;
		if (mylistaADD->cantidadElementos++ == 1000)
		{
			SetEvent(Myevent);
		}
		PItemList->NextItem = NULL;
		if (mylistaADD->Header == NULL )
		{
			mylistaADD->Header = PItemList;
			mylistaADD->Tail   = PItemList;
		}
		else
		{
	 		mylistaADD->Tail->NextItem = PItemList;
			mylistaADD->Tail = PItemList;
		}
	LeaveCriticalSection(&MyMutex);
}

ITEMLIST* CListA::Removeitem()
{
	ITEMLIST* Salida = NULL;
	if (mylistaFlush->Header != NULL)
	{
		Salida =  mylistaFlush->Header;
		mylistaFlush->Header = (ITEMLIST*) mylistaFlush->Header->NextItem;
	}
	return 	Salida;
}
