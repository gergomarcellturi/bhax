int *getPointers(int* array);
typedef int(*KillMe) (int, int);
int PleaseLoL(int a, int b);
KillMe SmokeWeed(int);




int main()
{

	int a = 1; /* egész */

	int *b = &a; /* egészre mutató mutató */

	int &c = *b; /* egész referenciája */

	int d[5] = {0,1,2,3,4}; /* egészek tömbje */

	int (&e)[5] = d; /* egészek tömbjének referenciája */

	int *f[5]; /* egészre mutató mutatók tömbje */

	int *g = getPointers(d); /* egészre mutató mutatót visszaadó függvény */

	int* (*asd)(int*) = getPointers; /* egészre mutató mutatót visszaadó függvényre mutató mutató*/

	KillMe EndMyMisery = SmokeWeed(420); /* egészet visszaadó és két egészet kapó függvényre mutató mutatót
visszaadó, egészet kapó függvény */

	return 0;
}

int* getPointers(int* array)
{
	return array;
}


int PleaseLoL(int a, int b)
{
	return 420;
}

KillMe SmokeWeed(int everyday)
{
	return PleaseLoL;
}

