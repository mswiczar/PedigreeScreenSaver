#include "windows.h"



typedef struct TDATOS
	{
		LPVOID Buffer;
		DWORD  BytesUsed;
		DWORD  BytesComress;
	} DATOS ;

typedef struct	TITEMLIST
	{
		LPVOID  NextItem;
		DATOS  UnDato;  
	} ITEMLIST;

typedef struct TLISTA
	{
		ITEMLIST* Header;
		ITEMLIST* Tail;
		int cantidadElementos;
	} LISTA;


class CListA {
public:
		CListA(WCHAR * aFileName,DWORD asample,DWORD asampleBase);
		~CListA();
		void AddItem(ITEMLIST* PItemList);
		LISTA* mylistaADD;
		LISTA* mylistaFlush;
		LISTA* mylista1;
		LISTA* mylista2;
		CRITICAL_SECTION MyMutex;
		HANDLE MytrheadFlush;
		HANDLE Myevent;
		BOOL Running;
		DWORD WINAPI Flush(LPVOID lpParameter );
		ITEMLIST* Removeitem();
		WCHAR * FileName;
		DWORD mysample;
		DWORD mysampleBase;
};



