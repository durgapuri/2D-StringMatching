//vectors are 1-indexed and strings inside the vectors are 0 indexed

#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fcntl.h>
#include <queue>
#include <unordered_map>
#include <math.h>
#include <unistd.h>
using namespace std;
#define LIM 1
#define ll long long
#define d 256
#define MOD 101


void writeToFile(string str){
    int fd = open("timing_zhu.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

vector<int> hashTextFunc(vector<string>& textmat, ll textcol, ll patrow)
{	
	// cout<<"Inside hashRowFunc()"<<endl;
	vector<int> hashedMat(textcol+1);
	for(ll j=0; j<textcol; j++)
	{	hashedMat[j+1]=0;
		for(ll i=1; i<=patrow; i++)
		{	int val=textmat[i][j];
			hashedMat[j+1]=(hashedMat[j+1]*d + val) % MOD;
		}
	}

	// cout<<"printing hashed text"<<endl;
	// for(ll j=1; j<=textcol; j++)
	// {
	// 	cout<<hashedMat[j]<<" ";
	// }
	// cout<<endl;

	// cout<<"completed hashRowFunc()"<<endl;
	return hashedMat;
}

/*returns*/
vector<int> hashPatFunc(vector<string> patmat, ll patcol, ll patrow)
{
	// cout<<"in hashPatFunc:"<<endl;
	vector<int> hashPat(patcol+1);
	for(ll j=0; j<patcol; j++)
	{	hashPat[j+1]=0;
		for(ll i=1; i<=patrow; i++)
		{	int val=patmat[i][j];
			// cout<<val<<" "<<patmat[i][j]<<endl;
			hashPat[j+1]=(hashPat[j+1]*d + val) % MOD;
		}
	}

	// cout<<"printing hashed pat:"<<endl;
	// for(ll j=1; j<=patcol; j++)
	// {
	// 	cout<<hashPat[j]<<" ";
	// }
	// cout<<endl;
	return hashPat;
}

void updateHashRow(vector<string>& textmat,vector<int>& hashRow, ll row, ll dm, ll textcol, ll patrow)
{	
	//vectors are 1-indexed and strings inside the vectors are 0 indexed
	for(ll j=0; j<textcol; j++)
	{	int val=textmat[row-patrow+1][j];//char to remove from rolling hash
		int val1=textmat[row+1][j];//char to be inserted into the rolling hash
		hashRow[j+1]=( (hashRow[j+1]+ d*MOD - val*dm ) * d + val1) % MOD;
	}

	// cout<<"Updated hash row:"<<endl;
	// for(ll i=1;i<=hashRow.size();i++){
	// 	cout<<hashRow[i]<<" ";
	// }
	// cout<<endl;
}


void getLPS(vector<int> patt, vector<int> text, int lps[], int m){
	int i=1, len=0;
	lps[0]=0;

	// cout<<"patt in getLPS(): "<<endl;
	// for(int i=0;i<m;i++){
	// 	cout<<patt[i]<<" ";
	// }
	// cout<<endl;


	while(i<m){
		if(patt[len]==patt[i]){
			len++;
			lps[i++]=len;
		}
		else{
			if(len==0){
				lps[i]=0;
				i++;
			}
			else{
				len=lps[len-1];
			}
		}
	}

	// cout<<"lps: "<<endl;
	// for(int i=0;i<m;i++){
	// 	cout<<lps[i]<<" ";
	// }
	// cout<<endl;

}

void KMPFunc(vector<int> patt, vector<int> text, ll *found, ll *pos){
	// cout<<"Inside KMPFunc"<<endl;
	int n, m, i, j;
	n=text.size()-1;
	m=patt.size()-1;

	patt.erase(patt.begin());
	text.erase(text.begin());
	/*The received vectors patt and text are 1-indexed. Erase has been used to use it as a 0-indexed vector in kmp logic*/

	// cout<<"Erased from begin"<<endl;


	// cout<<"m : kmp "<<m<<endl;
	// cout<<"n : kmp "<<n<<endl;

	int lps[m];

	getLPS(patt, text, lps, m);

	i=0; j=0;
	while(i<n){
		if(text[i]==patt[j]){
			i++;
			j++;
		}
		if(j==m){//have matched till the end of pattern
			// cout<<"found pattern at "<<i-j<<endl;
			(*pos)=i-j+1;
			// j=lps[j-1];
			(*found)=1;
			break; //breaking at first match of pattern
		}

		else if(i<n && text[i]!=patt[j]){
			if(j==0){
				i++;
			}
			else{
				j=lps[j-1];
			}
		}
	}
}

void zhu_taka(ll textrow, ll textcol,ll patrow, ll patcol, vector<string>& textmat, vector<string>& patmat){
	/* compute hash for first p1xn2 submatrix of text matrix, similar to rolling hash function concept in RK*/
	vector<int> hashRow(textcol+1, 0);
	hashRow=hashTextFunc(textmat,textcol,patrow);

	// cout<<"Returned from hashRowFunc()"<<endl;
	
	// for computing place value 256^(m-1)
	ll dm=1;
	for(ll j=1; j<patrow; j++){
		// cout<<"dm before"<<d*dm<<endl;
		dm=(d*dm)%MOD;
		// cout<<"dm "<<dm<<endl;
	}
	// cout<<"out"<<dm<<endl;

	/*compute hash for pattern(p1xp2) using rolling hash function similar to RK*/
	vector<int> hashPat(patcol+1);
	hashPat=hashPatFunc(patmat,patcol,patrow);

	ll startrow=patrow;
	ll found=0;
	ll colFound;
	int rowFound;
	vector<pair<ll,ll> >patPos;
	while(found==0 && startrow <= textrow)
	{
		// cout<<"About to call kmp"<<endl;
		KMPFunc(hashPat,hashRow,&found,&colFound);		
		rowFound=startrow;
		if(found==1) patPos.push_back(make_pair(rowFound-patrow, colFound-1));
		found=0;

		if(found==0 && startrow <= textrow-patrow+1){
			updateHashRow(textmat,hashRow,startrow,dm,textcol,patrow);			
		}
		startrow++;
	}
	if(patPos.size()>0){//Pattern found atleast once
		cout<<"Pattern found at:"<<endl;
		for(auto &p:patPos){
			cout<<p.first<<","<<p.second<<endl;
		}
	}
	else{
		cout<<"Not found"<<endl;
	}
	// if(found==1)
	// 	cout<<"Pattern found at "<<rowFound-patrow<<" "<<colFound-1<<endl;
	// else
	// 	cout<<"Not found"<<endl;
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

        cout<<"textrow:"<<textrow<<" patrow:"<<patrow<<endl;

        initInput(textrow, textcol, patrow, patcol);

        vector<string> textmat(textrow+1);
        vector<string> patmat(patrow+1);
        /*Text size-> n1xn2
          Pattern size-> p1xp2*/
        

        // input text matrix
        for(ll i=1; i<=textrow; i++){
            getline(infile, line);
            textmat[i]=line;
            // cin>>textmat[i];
        }
        // cout<<"text done"<<endl;

        //input pattern matrix
        for(ll i=1; i<=patrow; i++){
            getline(infile, line);
            patmat[i]=line;
            // cin>>patmat[i];
        }
		// cout<<"patt done"<<endl;

		cout << "Testcase No.:" << k + 1 << endl;
	    clock_t start = clock();

	    zhu_taka(textrow,textcol,patrow,patcol, textmat, patmat);

	    double duration = 1000 * (clock() - start) / (double)CLOCKS_PER_SEC;

	    str += to_string(duration) + "\n";
	}		

    writeToFile(str);

}