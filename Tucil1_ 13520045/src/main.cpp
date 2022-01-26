#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>

using namespace std;
using namespace std::chrono;

// deklarasi dan inisiasi variabel global
int xdir[]={1,1,0,-1,-1,-1,0,1};
int ydir[]={0,-1,-1,-1,0,1,1,1};
int boardrows, boardcols;

void readFile(string file_name, vector<vector<char>> *board, vector<string> *wordlist ){
    ifstream file_in(file_name.c_str());
    if (!file_in){
        cerr <<"Cannot open the File: " <<file_name<<endl;
        exit(1);
    }    
    string line;
    while (getline(file_in, line)) {
        vector<char> vecline;
        for (char &c: line) {
            if (c!=' '){
                vecline.push_back(c);
            }
        }
        if (vecline.size()>0){
            (*board).push_back(vecline);
            }
        else break;
    }

    string wordlistline;
    while (getline(file_in, wordlistline)) {
        (*wordlist).push_back(wordlistline);
        }
    file_in.close();
}

void printOutput(string word, vector<vector<char>> board, int row, int col, int dir){
    int i,j,k;
    for (i=0;i<boardrows;i++){
        for (j=0;j<boardcols;j++){
            board[i][j]='-';
        }
    }
    int len=word.size();
    for (k=0;k<word.size();k++){
        board[row][col]=word[k];
        row+=ydir[dir];
        col+=xdir[dir];
    }
    for (i=0;i<boardrows;i++){
        for (j=0;j<boardcols;j++){
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

bool search(string word, vector<vector<char>> board, int row, int col, int *dir, int *count){
    int len = word.size();
    int i,y,x,d;
    *count +=1;
    if (board[row][col]==word[0]){
        for (d=0;d<8;d++){
            y=row+ydir[d];
            x=col+xdir[d];
            for (i=1;i<len;i++){
                if (x<0 || x>=boardcols || y<0 || y>=boardrows) break;
                *count += 1;
                if (board[y][x]!=word[i]) break;
                y+=ydir[d];
                x+=xdir[d];
            }
            if (i==len) {
                *dir=d;
                return true;
                }
            }
            *dir=0;
            return false;
        }
    *dir=0;
    return false;
}

int main(){
    string file_name;
    vector<vector<char>> board;
    vector<string> wordlist;
    int word, row,col;
    int dir,count, totalcount=0;
    double totaldiff=0;
    bool isAvail;

    cout << "Welcome to 2D word search program!\nInsert file name: ";
    cin >> file_name;
    cout << "\nResults:\n";
    readFile(file_name, &board, &wordlist);
    boardrows=board.size();
    boardcols=board[0].size();

    for (word=0;word<wordlist.size();word++){
        isAvail=false;
        cout << wordlist[word] << "\n";
        for (row=0;row<boardrows;row++){
            for (col=0;col<boardcols;col++){
                count=0;
                high_resolution_clock::time_point start = high_resolution_clock::now();
                isAvail=search(wordlist[word], board,row,col, &dir, &count);
                high_resolution_clock::time_point end = high_resolution_clock::now();
                duration<double> diff=duration_cast<duration<double>>(end-start);
                totalcount+=count;
                totaldiff+=diff.count();
                if (isAvail){
                    printOutput(wordlist[word], board, row, col, dir);
                    break;
                }
            }
            if (isAvail) break;

        }
        if (!isAvail) {
            cout << "Word not found!\n\n";
        }
    }
    cout << "Number of letter comparison: " << totalcount << " times\n";
    cout << "Word search program runtime: " << totaldiff*1000 << " ms \n\n";
}