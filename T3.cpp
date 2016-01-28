//André Gardner adgardner1@dmacc.edu 11/5/15
//This program lets you play tic tac toe

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
using namespace std;

//function prototypes
bool checkWin(char[3][3], char);
void takeTurn(string, string, char[3][3], bool &);
void display(char[3][3]);

//constants
const char P1='X', P2='O';

int main()
{
	string player1, player2, winningPlayer;
	char board[3][3];

	//Fills the array so it's not just NULL
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			board[i][j]='-';
		}
	}

	cout<<"Player 1 enter your name: ";
	cin>>player1;

	cout<<"\nPlayer 2 enter your name: ";
	cin>>player2;

	//randomly set who is going first
	srand(time(NULL));
	bool isP1Turn=false;
	//TEST IF IT'S ACTUALLY RANDOM
	int test=rand()%2;
	cout<<test<<endl;
	system("pause");
	isP1Turn=(test==0)?true : false;

	bool p1Win=false, p2Win=false, winner=false;
	//Takes turns until a player wins
	do
	{
		display(board);
		takeTurn(player1, player2, board,isP1Turn);
		p1Win=checkWin(board,P1);
		winner=(p1Win)? true: false;
		if(!winner)
		{
			p2Win=checkWin(board,P2);
			winner=(p2Win)? true: false;
		}
		
	}while(!winner);

	//display the winner
	display(board);
	winningPlayer=(p1Win)?player1 : player2;
	cout<<winningPlayer<<" WON!!!"<<endl;
	system("pause");
}
//Checks who's turn it is then prompts them for their move
void takeTurn(string p1, string p2, char board[3][3], bool &turn)
{
	string player;
	char piece;
	short int x=0, y=0;

	if(turn)
	{
		player=p1;
		piece=P1;
		turn=false;
	}
	else
	{
		player=p2;
		piece=P2;
		turn=true;
	}

	bool ok=false;
	do
	{
		cout<<"Where would you like to place your next piece "<<player<<"?"<<endl;
		cout<<"Column: ";
		cin>>x;
		cout<<"\nRow: ";
		cin>>y;
		
		x-=1;
		y-=1;
		
		if(board[y][x]==P1 || board[y][x]==P2)
		{
			cout<<"\nSorry, that place has already been used. Try again"<<endl;
			ok=false;
		}
		else
		{
			board[y][x]=piece;
			ok=true;
		}

	}while(!ok);
}

//Checks to see if anyone has made a winning move. There's probably an easier way to check this...
bool checkWin(char board[3][3], char piece)
{
	//Checks each row
	if(board[0][0]==piece && board[0][1]==piece && board[0][2]==piece)
	{
		return true;
	}
	else if(board[1][0]==piece && board[1][1]==piece && board[1][2]==piece)
	{
		return true;
	}
	else if(board[2][0]==piece && board[2][1]==piece && board[2][2]==piece)
	{
		return true;
	}

	//Checks each column
	else if(board[0][0]==piece && board[1][0]==piece && board[2][0]==piece)
	{
		return true;
	}
	else if(board[0][1]==piece && board[1][1]==piece && board[2][1]==piece)
	{
		return true;
	}
	else if(board[0][2]==piece && board[1][2]==piece && board[2][2]==piece)
	{
		return true;
	}

	//Checks diagonals
	else if(board[0][0]==piece && board[1][1]==piece && board[2][2]==piece)
	{
		return true;
	}
	else if(board[2][0]==piece && board[1][1]==piece && board[0][2]==piece)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Displays the board
void display(char board[3][3])
{
	system("CLS");
	cout<<board[0][0]<<" || "<<board[0][1]<<" || "<<board[0][2]<<endl;
	cout<<"==========="<<endl;
	cout<<board[1][0]<<" || "<<board[1][1]<<" || "<<board[1][2]<<endl;
	cout<<"==========="<<endl;
	cout<<board[2][0]<<" || "<<board[2][1]<<" || "<<board[2][2]<<endl;
}