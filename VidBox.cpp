//André Gardner 4/2/15 adgardner1@dmacc.edu
//This program allows a user to rent or return DVDs to a kiosk. It also allows a maintenance person to have admin controls over said kiosk.

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

//Function prototypes
void getData(int &, double &);
void sales(int, int &, double &, bool &);
void returns(int, int &, double &);
void closeDown(int, double);
bool admin(int &, double &);

//Function prototypes handling display and menu choice stuff
void display (string);//Sets title
void message(string, string);//just displays a message
int menu(string, string, string);//reg user
int menu(string, string, string, string);//admin

//Function prototypes that handle all the logging stuff that needs to go on. Overloading is incase unit loses power, we know what happened right up to that point
void logStuff(int);//Log ID
void logStuff(int, int, double, string);//General log
void logStuff(int, int, double, int);//Log return
void logStuff(int, int, double, int, double);//Log rental
void logStuff(int, int, double, double);//Log withdrawal

//Constants
const double TAX=.07, TOP_COST=4.99, NEW_COST=3.99, STD_COST=2.99;
const int ADMIN=99959;
int LINE_LENGTH=60;
char LINE_SYMBOL='=';

/***************************************************************************************************************
********************					MAIN								************************************
***************************************************************************************************************/
int main()
{
	int boxVids, userID, choice;
	double boxMoney;
	bool dayEnd=false, off35=false, goodID;
	char ID[6];

	getData(boxVids, boxMoney);//This gets how many videos and money are in the unit at day start
	
	while(!dayEnd)
	{
		goodID=false;
		while(!goodID)
		{
			//Get user's ID #
			display("VidBox Main Menu");
			cout<<"Enter your User ID #: ";
			cin>>ID;

			userID=atoi(ID);//converts ID to an int for the rest of the program

			//Makes sure ID number is valid
			if(userID<0 || userID>99999)
			{
				cout<<"Please enter a positive number or make sure it's only 5 digits in length."<<endl;
				goodID=false;
				system("pause");
			}
			else
			{
				goodID=true;
			}
			
			//Checks to see if user eligible for 35% discount if renting
			if(ID[0]=='9')
			{
				off35=true;
			}
			else
			{
				off35=false;
			}
		}
		logStuff(userID);

		//Checks userID against ADMIN number to determine what menu to display
		if(userID==ADMIN)
		{
			dayEnd=admin(boxVids,boxMoney);
		}
		else
		{
			choice=menu("VidBox Main Menu", "Rent DVDs", "Return DVDs", "Exit");
			if(choice==1)
			{
				sales(userID, boxVids, boxMoney, off35);
			}
			else if(choice==2)
			{
				returns(userID, boxVids, boxMoney);
			}
			else
			{
				message("GOODBYE", "Thank you for using Vid Box");
				dayEnd=true;
			}
		}
	}
	return 0;
}

/***********************************************************************************
* Reads file that populates the vids & money variables for the rest of the program *
***********************************************************************************/
void getData(int &boxVids, double &boxMoney)
{
	string input;
	ifstream inputFile;
	
	//Opens and reads the file
	inputFile.open("dayin00.dat");
	inputFile>>boxVids;
	inputFile>>boxMoney;
	message("VIDBOX BOOTUP","Getting day start data......");
	inputFile.close();
}
/******************************************************************************
* Gets # of returns and adjusts the total number of videos in box accordingly *
******************************************************************************/
void returns(int userID, int &boxVids, double &boxMoney)
{
	int numReturns;

	//Change title and get # of returns
	display("VidBox Video Returns");
	cout<<"How many DVDs are you returning? ";
	cin>>numReturns;

	//Thank user
	message("VidBox Video Returns", "Thank you for returning your DVDs!");

	//Change # of vids in box & log
	boxVids+=numReturns;
	logStuff(userID,boxVids, boxMoney, numReturns);
}

/*****************************************************************************************************************************
* This prompts for how many of each release type the user wants, displays the total cost then thanks them for their business *
*****************************************************************************************************************************/
void sales(int userID, int &boxVids, double &boxMoney, bool &off35)
{
	double total=0;
	int topRelease=0, newRelease=0, stdRelease=0, rentals=0, num=0;
	bool ok=false;
	double discount=0;
	
	

//Get how much of each type of release the customer's buying
	do
	{
		display("VidBox DVD Rental");
		cout<<"There are "<<boxVids<<" videos in this unit."<<endl;
		cout<<"\nHow many top releases would you like to rent? ";
		cin>>topRelease;
	
		cout<<"\nHow many new releases would you like to rent? ";
		cin>>newRelease;
	
		cout<<"\nHow many standard releases would you like to rent? ";
		cin>>stdRelease;
		rentals=topRelease+newRelease+stdRelease;

		//Making sure that we have enough vids in the unit & we're not renting out a negative number of vids.
		if(topRelease>=0&&newRelease>=0&&stdRelease>=0&&rentals<boxVids)
		{
			ok=true;
		}
		else
		{
			cout<<"You can't rent a negative number or more than the number of videos in this unit";
		}

	}while(!ok);
	
	//Determine which (if any) discounts apply. 35% for IDs starting with 9 and for every 2 rented, 3rd is free
	if(off35)
	{
		discount=.35;
	}
	else
	{
		discount=0;
	}

	if(topRelease>=3)
	{
		num=topRelease/3;
		topRelease-=num;
	}
	if(newRelease>=3)
	{
		num=newRelease/3;
		newRelease-=num;
	}
	if(stdRelease>=3)
	{
		num=stdRelease/3;
		stdRelease-=num;
	}


	//Display total # of rentals
	cout<<"\nYou're renting "<<rentals<<" DVDs"<<endl;

	//Calculate cost with discount if any
	total=(topRelease*TOP_COST)+(newRelease*NEW_COST)+(stdRelease*STD_COST);
	if(discount>0)
	{
		discount=discount*total;
		total-=discount;
	}
	total=total+(total*TAX);

	//Display cost
	cout<<"That'll be $"<<fixed<<setprecision(2)<<total<<endl;
	system("pause");

	//Thank user
	message("VidBox DVD Rental", "Thank you for your business!");

	//Change values for vids & $ in box & log
	boxVids-=rentals;
	boxMoney+=total;
	logStuff(userID,boxVids,boxMoney,rentals,total);
}

/*********************************************************************************************************************************
* Runs a different menu that displays info for admin. Allows withdrawals or shuts down the system after saving an end of day log *
*********************************************************************************************************************************/
bool admin(int &boxVids, double &boxMoney)
{
	double money;
	int choice = menu("~~VidBox Maintenance Menu~~", "SUMMARY", "WITHDRAWAL","CLOSE DOWN");
	
	if(choice==1)
	{
		display("~~VidBox Summary~~");
		cout<<"Total videos currently in unit: "<<boxVids<<endl;
		cout<<"Total money currently in unit: $"<<boxMoney<<endl;
		
		system("pause");

		logStuff(ADMIN,boxVids,boxMoney,"requested a summary of the amount of the unit's videos and money");
		return false;
	}
	else if(choice==2)
	{
		do
		{
			display("~~VidBox Maintenance Menu~~");
			cout<<"There is $"<<fixed<<setprecision(2)<<boxMoney<<" in VidBox. How much do you want to withdraw? ";
			cin>>money;

			//Check to make sure money is valid
			if(money>boxMoney)
			{
				cout<<"Not enough money in box"<<endl;
			}
			else if(money<0)
			{
				cout<<"Can't withdraw a negative amount"<<endl;
			}

		}while(money<boxMoney&&money>0);
		
		
		message("~~VidBox Maintenance Menu~~", "Withdrawal complete");
		
		logStuff(ADMIN,boxVids,boxMoney,money);
		return false;
	}
	else
	{
		closeDown(boxVids,boxMoney);

		logStuff(ADMIN,boxVids,boxMoney,"ran end of day system close down");
		return true;
	}
}

/*****************************************************************
* Fills a file named 'dayout99.dat' with the numbers for the day *
*****************************************************************/
void closeDown(int boxVids, double boxMoney)
{
	ofstream outputFile("dayout99.dat");
	outputFile.clear();
	outputFile<<boxVids<<endl;
	outputFile<<boxMoney<<endl;
	outputFile.close();
}

/*******************************************************************************************************************
* These are all methods to handle the logging that has to be done. All log userID, then they branch off from there.*
*******************************************************************************************************************/
//Log ID
void logStuff(int userID)
{
	ofstream outputFile("current11.dat", ios::app);
	outputFile<<userID<<" accessed the box."<<endl;
	outputFile.close();
}

//General log
void logStuff(int userID, int boxVids, double boxMoney, string whatHappened)
{
	ofstream outputFile("current11.dat", ios::app);
	outputFile<<userID<<" "<<whatHappened<<endl;
	outputFile<<"Videos in box: "<<boxVids<<" money in box: "<<right<<setw(5)<<"$"<<fixed<<setprecision(2)<<boxMoney<<endl;
	outputFile.close();
}
		
//Returns
void logStuff(int userID, int boxVids, double boxMoney, int numReturns)
{
	ofstream outputFile("current11.dat", ios::app);
	outputFile<<userID<<" returned "<<setw(5)<<numReturns<<" DVDs."<<endl;
	outputFile<<"Videos in box: "<<boxVids<<" money in box: "<<right<<setw(5)<<"$"<<fixed<<setprecision(2)<<boxMoney<<endl;
	outputFile.close();
}

//Rentals
void logStuff(int userID, int boxVids, double boxMoney, int numRentals, double money)
{
	ofstream outputFile("current11.dat", ios::app);
	outputFile<<userID<<" rented "<<setw(5)<<numRentals<<" DVDs and spent "<<right<<setw(5)<<"$"<<fixed<<setprecision(2)<<money<<endl;
	outputFile<<"Videos in box: "<<boxVids<<" money in box: "<<right<<setw(5)<<"$"<<fixed<<setprecision(2)<<boxMoney<<endl;
	outputFile.close();
}

//Withdrawals
void logStuff(int userID, int boxVids, double boxMoney, double withdrawal)
{
	ofstream outputFile("current11.dat", ios::app);
	outputFile<<setw(10)<<userID<<" withdrew "<<right<<setw(5)<<"$"<<fixed<<setprecision(2)<<withdrawal<<endl;
	outputFile<<"Videos in box: "<<boxVids<<" money in box: "<<right<<setw(5)<<"$"<<fixed<<setprecision(2)<<boxMoney<<endl;
	outputFile.close();
}

/*******************************
* Sets the title of the screen *
*******************************/
void display(string title)
{
	string line;

	system("CLS");	
	
	//Set the title
	cout<<setw(35)<<right<<title<<endl;
	
	//display the seperator line belowe the screen title
	cout<<line.assign(LINE_LENGTH,LINE_SYMBOL)<<"\n"<<endl;
}

/********************************************
* Simple notice for user. Refreshes screen. *
********************************************/
void message(string title, string msg)
{
	
	display(title);

	cout<<setw(30)<<left<<msg<<endl;
	cout<<"\n"<<endl;

	system("pause");
}

/************************************************************************
* Displays menu for user. Provides 3 choices and checks for valid input *
************************************************************************/
int menu(string title, string choice1, string choice2, string choice3)
{
	
	int choice=0;
	display(title);

	cout<<setw(30)<<left<<"1). "<<choice1<<endl;
	cout<<setw(30)<<left<<"2). "<<choice2<<endl;
	cout<<setw(30)<<left<<"3). "<<choice3<<endl;
	
	do
	{
		cout<<setw(30)<<">";
		cin>>choice;
	
		if(choice>3)
			cout<<"Please make a valid choice"<<endl;
	}while(choice>3);

	return choice;
}