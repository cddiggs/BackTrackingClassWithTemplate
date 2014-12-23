//============================================================================
// Name        : sudoku.cc
// Author      : Dr. Enoch Lee - Auburn University Montgomery 
//  		 and Chris Diggs
// Compile     : make all
// Description : Soduku solver one row at a time
//
// Sample Input:
//
// 1 0 0 0 0 0 0 0 0 
// 0 0 5 4 0 6 1 0 0 
// 0 8 0 0 0 0 0 9 0 
// 0 0 4 0 1 0 5 0 0 
// 0 7 0 0 9 0 0 2 0 
// 0 0 6 0 8 0 3 0 0 
// 0 2 0 1 0 0 0 7 0 
// 0 0 0 5 0 3 6 0 0 
// 0 0 0 0 0 0 0 0 0 
//
//Sample Output:
//
// Solution # 1 : 
//
// -------------------------------
// | 1  4  2 | 8  3  9 | 7  5  6 |
// | 9  3  5 | 4  7  6 | 1  8  2 |
// | 6  8  7 | 2  5  1 | 4  9  3 |
// -------------------------------
// | 8  9  4 | 3  1  2 | 5  6  7 |
// | 3  7  1 | 6  9  5 | 8  2  4 |
// | 2  5  6 | 7  8  4 | 3  1  9 |
// -------------------------------
// | 4  2  3 | 1  6  8 | 9  7  5 |
// | 7  1  9 | 5  2  3 | 6  4  8 |
// | 5  6  8 | 9  4  7 | 2  3  1 |
// -------------------------------
//
// How-To-RUn  : ./sudoku 
// Enter the puzzle file name: evil2.txt
//
// Check out BackTracking Class with Template for a Sudoku solution by row 
// (rather than by each individual chair)
//============================================================================



#include <iostream>
#include "backtrack.h"
#include "backtrack.cc"
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <queue>
#include<list>
#include<string.h>
#include<algorithm>
#include<stdio.h>
using namespace std;


typedef struct{
  int x,y;
} point;

typedef struct{
  int Board[9][9];
  //int freecount;  //how many open squares remain?
  //list<point *> emptyspaces;//the empty spaces remain
  int currRow;
  vector<vector<int> > UsedSpots;
  //vector<point *> move;  //record all the moves made use for backtracking!
} board;


void InitPuzzle(board *);
  // Print puzzle in current state
void printPuzzle(board *);

bool verifyValue(int x_cord, int y_cord);  //aux function to test if the value on added to (x_cord,y_cord) yields no conflict at the current state

bool testNextSpot(board * theboard, int value, int openSpot);


int main(){

  //Initpuzzle();
  board *thisboard=new board;
  InitPuzzle(thisboard);
  string fname;
  ifstream infile;
  int temp;
  cout<<"\nEnter the puzzle file name :";
  cin>>fname;
  infile.open(fname.c_str());

  if (!infile){
    cout<<"\nCannot open the file!";
    exit(1);
  }
  (thisboard->UsedSpots).resize(10);
  //Read data.  assume the data set is right, i.e. no checking
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      infile>>temp;
      if (temp!=0){
	(thisboard->Board)[j][i]=temp;
	(thisboard->UsedSpots)[i].push_back(j);
      }
    }
  }
  infile.close();
  // Print the board before solving.
  printPuzzle(thisboard);

  cout <<endl;
  BT<vector<int> > puzzle(9,false, (void*)thisboard);
  puzzle.backtrack();
  if (puzzle.get_num_sols()==0)
    {
      cout<<"\nNo solutions found!\n";
    }
  cout << endl;
  return 0;
} // main


void InitPuzzle(board * theboard) {
  for (int y=0; y<9; y++) {
    for (int x=0; x<9; x++) {
      theboard->Board[x][y] = 0;
    }
  }
 // theboard->freecount=81;
} //InitPuzzle

void printPuzzle(board * theboard) {
  for (int y=0; y<9; y++) {
    if (y % 3== 0) {
      cout << "-------------------------------" << endl;
    }
    for (int x=0; x<9; x++) {
      if (x % 3 == 0) {
	cout << "|";
      }
      if (theboard->Board[x][y] >0&&theboard->Board[x][y] <10) {
	cout << " " << theboard->Board[x][y] << " ";
      } else {
	cout << " . ";
      }
    }
    cout << "|" << endl;
  }
  cout << "-------------------------------" << endl;
} // printPuzzle


//problem specific functions
template<class T>
bool BT<T>::is_a_solution()
{
  board * temp=(board *)(input);
  bool find=(temp->currRow==9);
  //the following should be included for any problem
  if (find)
    {
      finished=true;
      (num_sol)++;
      return true;
    }
  else{
    return false;
  }
}

template<class T>
void BT<T>::process_solution()
{
  board * temp=(board *)(input);
  cout<<"\nSolution # "<<num_sol<<" : \n";
  printPuzzle((board *)input);
}

template<class T>
void BT<T>::make_move()  //update board
{
  int k=a.size()-1;
  board *temp=(board *)(input);
	
  vector<int> tempC = a[k];
  int j=0;
  for(int i=0; i<9; i++){
	if(temp->Board[i][temp->currRow]==0){
		temp->Board[i][temp->currRow]=tempC[j];
		j++;
	}
  }
  temp->currRow++;
  return;
}

template<class T>
void BT<T>::unmake_move()
{
  int k=a.size()-1;
  board * temp=(board *)(input);
  temp->currRow--;
  for(int i=0; i<9; i++){
	bool flag=false;
	for (int j=0; j<temp->UsedSpots[temp->currRow].size(); j++){
		if (i==temp->UsedSpots[temp->currRow][j]){
			flag=true;
			break;
		}
	}
	if(!flag)
		temp->Board[i][temp->currRow]=0;
  }

}

template<class T>
void BT<T>::construct_candidates(std::vector<T>& c)
{
 int t,next, i,j;
  board *temp=(board *)(input);
  int row=temp->currRow;

  vector<int> tempC, spot;

  for(int j=0;j<9; j++){
	if (temp->Board[j][row]==0){
		spot.push_back(j);
	}
  }

  for(int i=1; i<10; i++){
	bool contains=false;
	for(int j=0;j<9; j++){
		if (i==temp->Board[j][row]){
			contains=true;
			break;
		}
	}
	if(!contains)
		tempC.push_back(i);
  }
  
  do{
	bool pass=true;
	for(int i=0;i<tempC.size();i++){
		pass=testNextSpot(temp, tempC[i], spot[i]);
		if (!pass)
			break;
	}
	if(pass){
		c.push_back(tempC);
	}

  }while (next_permutation(tempC.begin(), tempC.end()));
}

bool testNextSpot(board * theboard, int value, int openSpot){

  for(int i=0;i<9;i++){  //col
    if (theboard->Board[openSpot][i]==value){
      return false;
    }
  }
  for(int i=0;i<3;i++){  //subgrid
    for(int j=0;j<3;j++){
      if (theboard->Board[3*(openSpot/3)+i][3*(theboard->currRow/3)+j]==value){
	return false;
      }
    }
  }
  return true;

}
