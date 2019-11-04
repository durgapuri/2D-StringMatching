#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <fcntl.h>
#include <queue>
#include <unordered_map>
#include <math.h>
#include <unistd.h>

#define ll long long
#define LIM 1
using namespace std;

void writeToFile(string str){
    int fd = open("timing_naive.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, str.c_str(), str.length());
}


void initInput(ll textrow, ll textcol, ll patrow, ll patcol){
    vector<string> textmat(textrow+1);
    vector<string> patmat(patrow+1);

    // input text matrix
    for(ll i=0; i<LIM; i++){
        cout<<"";
    }
    // cout<<"text done"<<endl;

    //input pattern matrix
    for(ll i=0; i<LIM; i++){
        cout<<"";
    }
}


void naive(vector<string> text, vector<string> pat, ll textrow, ll textcol, ll patrow, ll patcol, ll arg, ll arg1)
{
    for (int i = 0; i < textrow-patrow+1; i++)
    {
        for (int j = 0; j < textcol-patcol+1; j++)
        {
            bool found = true;
            for (int k = 0; k < patrow; k++)
            {
                for (int l = 0; l < patcol; l++)
                {
                    if (text[i + k][j + l] != pat[k][l])
                    {
                        found = false;
                        
                    }
                }
               
            }
            if(found){
                cout << i <<","<<j<<endl;
            }
        }
    }
}

int main()
{
    cin.sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    ll t;
    string str = "";

    ifstream infile("input.txt");

    string line;
    getline(infile, line);
    t=stoi(line);//no. of test cases
    for(ll k=0;k<t;k++){

        ll textrow,textcol;
        ll patrow,patcol;

        getline(infile, line);
        istringstream iss(line);
        if (!(iss >> textrow >> patrow)) { break; } // error

        // textrow=stoi(getline(infile, line));
        textcol=textrow;
        // patrow=stoi(getline(infile, line));
        patcol=patrow;

        // cout<<"textrow:"<<textrow<<" patrow:"<<patrow<<endl;

        vector<string> textmat(textrow+1);
        vector<string> patmat(patrow+1);
        /*Text size-> n1xn2
          Pattern size-> p1xp2*/
        

        initInput(textrow, textcol, patrow, patcol);

        // input text matrix
        for(ll i=0; i<textrow; i++){
            getline(infile, line);
            textmat[i]=line;
            // cin>>textmat[i];
        }
        // cout<<"text done"<<endl;

        //input pattern matrix
        for(ll i=0; i<patrow; i++){
            getline(infile, line);
            patmat[i]=line;
            // cin>>patmat[i];
        }

    // ll t;
    // cin >> t;
    // string str = "";
    // for (ll k = 0; k < t; k++){

    //     cout<<"\nNew test case:"<<endl;

    //     ll textrow,textcol;
    //     ll patrow,patcol;

    //     cin>>textrow>>textcol;
    //     cin>>patrow>>patcol;
    //     // cout<<"Enter text/pat"<<endl;
        
    //     vector<string> textmat(textrow+1);
    //     vector<string> patmat(patrow+1);
    //     /*Text size-> n1xn2
    //       Pattern size-> p1xp2*/
        

    //     initInput(textrow, textcol, patrow, patcol);

    //     // input text matrix
    //     for(ll i=0; i<textrow; i++){
    //         cin>>textmat[i];
    //     }
    //     // cout<<"text done"<<endl;

    //     //input pattern matrix
    //     for(ll i=0; i<patrow; i++){
    //         cin>>patmat[i];
    //     }
        // cout<<"patt done"<<endl;
        cout << "Testcase No.:" << k + 1 << endl;
        clock_t start = clock();

        ll arg=0, arg1=1;

        naive(textmat, patmat, textrow, textcol, patrow, patcol, arg, arg1);

        double duration = 1000 * (clock() - start) / (double)CLOCKS_PER_SEC;

        str += to_string(duration) + "\n";
    }

    writeToFile(str);
}
