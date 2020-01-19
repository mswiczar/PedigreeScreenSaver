/*
 *  MSSoundManager.cpp
 *  debuger
 *
 *  Created by Moises Swiczar on 3/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MSSoundManager.h"
#include <string.h>
#include <stdio.h>


static ARRAY_SUM elresult[10];

static int posicion=0;
PARRAY_DATA elarreglo;

int getNext()
{
	if (posicion==9)
	{
		posicion=0;
	}
	else
	{
		posicion++;
	}
	return posicion;
}



int getPrev(int entrada)
{
	

	if (entrada ==0)
	{
		return 10;
	}
	
	else
	{
		return entrada-1;;
	}
	return 0;
}


/*
 char R;
 char G;
 char B;
 
 int  is_ruido;
 int  is_silence;
 
 int  cant_01_30_RED;	
 int  cant_21_30_RED;
 int  cant_31_50_RED;
 int  cant_51_90_RED;
 
 */


int process_Line(int posicion)
{
	
	
	
	elresult[posicion].cant_01_20_RED=0;
	elresult[posicion].cant_21_30_RED=0;
	elresult[posicion].cant_31_50_RED=0;
	elresult[posicion].cant_51_90_RED=0;	
	elresult[posicion].silbido=0;
	elresult[posicion].ruido=0;
	elresult[posicion].silencio=0;
	elresult[posicion].abajo=0;


	/*
	for (int i=0; i<80 ; i++)
	{
		
		if (elarreglo[i].R !=0 )
			{
				if((i >20) && (
						   i <30))
				{
					if(elarreglo[i].R>60) 
					{
						printf("+++++++++++Level: %d , RGB (%d , %d , %d )\n",i,elarreglo[i].R,elarreglo[i].G,elarreglo[i].B);
					}
				}
			}
	
	}
	 */
	for (int i=0; i<80 ; i++)
	{
		
		if(elarreglo[i].R >20)
		{
			if (i < 20) 
			{
				elresult[posicion].cant_01_20_RED++;
			}
			if ((i < 30) && (i > 20))
			{
				elresult[posicion].cant_21_30_RED++;
			}
			if ((i < 50) && (i > 30))
			{
				elresult[posicion].cant_31_50_RED++;
			}	
			if( (i < 90) && (i > 50))
			{
				elresult[posicion].cant_51_90_RED++;
			}
		}
	}
	
	/*
	
	printf("cant_01_20_RED : %d \n",elresult[posicion].cant_01_20_RED );
	
	printf("cant_21_30_RED : %d \n",elresult[posicion].cant_21_30_RED );
	printf("cant_31_50_RED : %d \n",elresult[posicion].cant_31_50_RED );
	printf("cant_51_90_RED : %d \n",elresult[posicion].cant_51_90_RED);
	printf("++++++++++++++++++++++++++\n");

	*/
	
	if(    (elresult[posicion].cant_31_50_RED > 3) )
	{
		elresult[posicion].ruido=1;
		return 0;
	}
	
	if(  (elresult[posicion].cant_51_90_RED > 3) )
	{
		elresult[posicion].ruido=1;
		return 0;
	}

	if(         (elresult[posicion].cant_21_30_RED >= 1) )
	{
		elresult[posicion].silbido = 1;
		return 0;
	}

	if(         (elresult[posicion].cant_01_20_RED >2) )

	{
		elresult[posicion].abajo = 1;
		return 0;
	}
	
	
	
	return 0;
}

			 
int analyze (int posicion_insertar)
{
	
	int prev;

	int sum_ruido=0;
	int sum_silbido=0;
	int sum_vocal=0;
		prev=posicion_insertar;
	for (int i=5; i>0 ; i--)
	{
		if (elresult[prev].ruido ==1)
		{
			sum_ruido = sum_ruido + 10*i;
		}
		if (elresult[prev].abajo ==1)
		{
			sum_vocal = sum_vocal + 10*i;
		}
		if (elresult[prev].silbido ==1)
		{
			sum_silbido = sum_silbido + 10*i;
		}
		
		
		
		prev=getPrev(prev);
	}
	/*
	if(sum_ruido!=0)
	{
		printf("sum_ruido %d \n" , sum_ruido);
	}

	if(sum_vocal!=0)
	{
		printf("sum_vocal %d\n" , sum_vocal);
	}
	
	if(sum_silbido!=0)
	{
		printf("sum_silbido %d\n" , sum_silbido);
	}
	*/
	
	if ((sum_ruido) >70)
	{
		
		return 0;
	}

	if ((sum_vocal) >90)
	{
		return 0;
	}
	
	if (sum_silbido >90)
	{
	//	printf("------------------SILVO\n");
		return 1;
	}
	

	return 0;
}


 
int procesar(int posicion)
{
	process_Line(posicion);
	return analyze(posicion);
}



int InitArray()
{
	memset(elresult, 0, sizeof(elresult));
	return 0;
}


int SetArray(PARRAY_DATA lamuestra)
{
	int posicion_insertar;
	posicion_insertar = getNext();
	elarreglo = lamuestra;
	
	return  procesar(posicion_insertar);
	
	
}




