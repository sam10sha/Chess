#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int a[8][8]={{0,50,10,5,0,5,5,0}, {0,50,10,5,0,-5,10,0}, 
			{0,50,20,10,0,-10,10,0}, {0,50,30,27,25,0,-25,0}, 
			{0,50,30,27,25,0,-25,0}, {0,50,20,10,0,-10,10,0},
			{0,50,10,5,0,-5,10,0}, {0,50,10,5,0,5,5,0}};
	printf("3,4 = %d\n 4,5 = %d\n 6,6 = %d\n 0,0 = %d\n", a[4][4],a[3][5],a[1][6], a[7][0]);
}
		
