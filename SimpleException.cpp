//André Gardner adgardner1@dmacc.edu 12/1/15
//This program demonstrates simple exception handling

#include <math.h>
#include <string>
#include <iostream>
using namespace std;

//Function prototype
int findRoot(int);

int main()
{
	double input=0, result=0;

	cout<<"Enter a number with a perfect square root."<<endl;
	cin>>input;

	try
	{
		result=findRoot(input);
		cout<<"The square root of "<<input<<" is "<<result<<endl;
	}
	catch(string exceptionString)
	{
		cout<<exceptionString<<endl;
	}
	
	system("pause");
	return 0;
}

//Find the square root, but only return a value if it's a perfect square.
int findRoot(int num)
{
	int wat=0;
	double wut=0;
	wut=sqrt(num);
	wat=sqrt(num);
	
	if(wat/wut!=1)
	{
		throw string ("Error: That number is not a perfect square.\n");
	}
	else
	{
		return sqrt(num);
	}
}