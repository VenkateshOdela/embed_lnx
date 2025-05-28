#include <stdio.h>

int main()
{
	//char acBuffer[5]; which gives random signed and unsigned values if not initialized.

	// unsigned char acBuffer[5]; //which gives random unsigned values if not initialized

	//char acBuffer[5] = 0; //invalid initializer

	char acBuffer[5] ={ 0 } ; //initializing  the all buffer content to zero;

	for(int i=0; i<5; i++)
	{
		printf("acBuffer[%d] = %d \n",i,acBuffer[i]);

	}
}
