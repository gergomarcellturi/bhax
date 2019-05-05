#include <iostream>
#include <cstdlib>	
#include <cmath>	
#include <ctime>	

class PolarGen 
{

	public:

		PolarGen(); 
        double next(); 
        ~PolarGen(){} 

	private:
		bool notStored;
		double storedValue;
};


PolarGen::PolarGen()
{
	notStored = true;
	std::srand (std::time(NULL));
};


double PolarGen::next() 
{ 
	if (notStored)
{
		double u1, u2, v1, v2, w;

		do
        {
			u1 = std::rand () / (RAND_MAX + 1.0); 
			u2 = std::rand () / (RAND_MAX + 1.0);
			v1 = 2 * u1 - 1;
			v2 = 2 * u2 - 1;
			w = v1 * v1 + v2 * v2;
        }
		while (w > 1);

		double r = std::sqrt ((-2 * std::log (w)) / w);

		storedValue = r * v2; 
		notStored = !notStored;

		return r * v1; 
	}
	
	else
	{
		notStored = !notStored;
		return storedValue;
	}
};    

int main() 
{

	PolarGen rnd;

	for (int i = 0; i < 10; ++i) 
        std::cout << rnd.next() << std::endl;

}
