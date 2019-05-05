#include <stdio.h>

int main()

{
	printf("Első Szám ->  ");
	int a;
	scanf("%d", &a);
	printf("\nMásodik Szám ->  ");
	int b;
	scanf("%d", &b);
	printf("\n-------------\n");

	printf("Első Szám ->  %d\n", a);
	printf("Második Szám ->  %d\n", b);
	printf("\n----- Csere -----\n");
	a = a + b;
	b = a - b;
	a = a - b;


	printf("Első Szám ->  %d\n", a);
	printf("Második Szám ->  %d\n", b);


	return 0;


}