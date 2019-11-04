#include<bits/stdc++.h>
#include<ctime>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
#define ll long long
#define CHARSET 256
#define d 1
void BakerBirdAlgo(vector<string>&,vector<string>&);
void createTrie(string);
void createFailureLinks();
void matchTrie(vector<string>&,vector<vector<ll>>&,ll,ll,ll,ll,map<string,ll>&);

void writeToFile(string st)
{
	int fd=open("timing_baker_bird.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(fd,st.c_str(), st.length());
}

class Node{

public:
	Node* child_nodes[CHARSET];
	bool end;
	Node* fail_redirect;
	vector<string> vec;
	Node()
	{
		end=false;
		fail_redirect=NULL;
		for(int i=0; i<CHARSET; i++)
			child_nodes[i]=NULL;
	}

};

Node* root=NULL;

void createTrie(string val)
{	
	//cout<<"in createTrie:"<<endl;
	Node* m=root;
	ll len=val.length();
	int index;
	//cout<<"m is "<<m<<endl;
	for(ll i=0; i<len; i++)
	{	
		//cout<<"inserting in trie"<<val[i]<<endl;
		index=val[i];

		if(m->child_nodes[index]==NULL)
			m->child_nodes[index]=new Node();

		m=m->child_nodes[index];
	}
	m->vec.push_back(val);
	//cout<<"created for "<<val<<endl;
	for(int i=0; i<d; i++)
		cout<<"";
	m->end=true;
}

void matchTrie(vector<string>& text, vector<vector<ll>>& procText, ll textrow, ll textcol, ll patrow,ll patcol,map<string,ll>& pat_map)
{	
	//cout<<"in matchTrie:"<<endl;
	//for each stirng in text -- row wise
	for(ll i=0; i<textrow; i++)
	{	
		//cout<<"iterating for "<<text[i]<<endl;
		int j=0;
		Node* n=root;
		//iterate string over trie
		while(j<textcol)
		{
			int in=text[i][j];
			if(n->child_nodes[in]!=NULL)
			{
				n=n->child_nodes[in];

				/* found complete string , reached end node of trie
				insert at location procText[i][j] indice for the string from map*/
				for(ll k=0; k<n->vec.size(); k++)
				{
					if(n->vec[k].length() == patcol)
					{	
						//cout<<"at "<<i<<" "<<j<<" "<<pat_map[n->vec[k]]<<endl;
						procText[i][j]=pat_map[n->vec[k]];
					}
				}
				j++;
			}
			else
			{
				if(n->fail_redirect!=NULL)
				{
					n=n->fail_redirect;
				}
				else
					j++;
			}
		}

	}
	// cout<<"processed text"<<endl;
	// for(int i=0; i<textrow; i++)
	// {	for(int j=0; j<textcol; j++)
	// 		cout<<procText[i][j]<<" ";
	// 	cout<<endl;
	// }
}

void createFailureLinks()
{	
	//cout<<"in createFailureLinks:"<<endl;
	queue<Node*> qu;
	Node* fail=root;

	//initialize fail_redirect for all child_ndoes of root as root
	for(int i=0; i<CHARSET; i++)
	{
		if(root->child_nodes[i]!=NULL)
		{	
			//cout<<"for i "<<i<<endl;
			root->child_nodes[i]->fail_redirect=root;
			//cout<<"root->child_nodes[i]->fail_redirect"<<root->child_nodes[i]->fail_redirect<<endl;
			qu.push(root->child_nodes[i]);
		}
	}
	//cout<<"queue size "<<qu.size()<<endl;

	/* apply BFS to update fail_redirect for each node in trie
	find longest proper suffix which is also a prefix 
	update the fail_redirect to that node*/

	Node* curr;

	while(!qu.empty())
	{
		curr=qu.front();
		qu.pop();
		fail=curr->fail_redirect;

		// update fail_redirect for each child of current node
		for(int i=0; i<CHARSET; i++)
		{
			Node* child=curr->child_nodes[i];
			if(child!=NULL)
			{	
				//cout<<"updating for: "<<i<<endl;
				if(fail->child_nodes[i]!=NULL)
				{
					//child of fail node points to prefix of the suffix
					child->fail_redirect=fail->child_nodes[i];

					for(ll k=0; k<fail->child_nodes[i]->vec.size(); k++)
					{
						//cout<<"update vector"<<endl;
						child->vec.push_back(fail->child_nodes[i]->vec[k]);
					}

				}
				else
				{
					Node* temp=fail;
					while(temp->child_nodes[i] == NULL && temp->fail_redirect!=NULL)
					{
						//cout<<"find prefix"<<endl;
						temp=temp->fail_redirect;
					}

					if(temp->child_nodes[i] == NULL && temp->fail_redirect==NULL)
					{
						//prefix doesn't exist -- start from root again
						//cout<<"update fail redirect"<<endl;
						child->fail_redirect=root;
					}
					else if(temp->child_nodes[i]!=NULL)
					{	
						//found prefix
						//cout<<"update fail redirect"<<endl;
						child->fail_redirect=temp->child_nodes[i];
					}
					else
					{
						//send redirect to failure node of parent
						//cout<<"update fail redirect"<<endl;
						child->fail_redirect=temp->fail_redirect;
					}
					for(ll k=0; k<temp->vec.size(); k++)
					{
						//cout<<"update fail redirect"<<endl;
						child->vec.push_back(temp->vec[k]);
					}
				}
				qu.push(child);
			}
		}
	}
	//cout<<"end of createFailureLinks:"<<endl;
	cout<<"";

}

void getLPS(vector<ll>& patt, ll lps[], ll m){
	ll i=1, len=0;
	lps[0]=0;

	// cout<<"patt in getLPS(): "<<endl;
	// for(ll i=0;i<m;i++){
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


void searchByKMP(vector<vector<ll>>& procText,vector<ll> patt,ll textrow,ll textcol,ll patrow,ll patcol)
{
	ll lps[patcol];
	vector<pair<ll,ll>> xy;
	getLPS(patt, lps, patcol);
	//vector<ll> text;
	//cout<<"returned from LPS"<<endl;
	for(ll k=0; k<textcol; k++)
	{
		//text=procText[i];
		ll i=0, j=0;
		while(i<textrow){
			//cout<<"i is "<<i<<" j is "<<j<<endl;
			if(procText[i][k]==patt[j]){
				i++;
				j++;
			
				
			}
			if(j==patcol){//have matched till the end of pattern
				 	//cout<<"found pattern at "<<i-patrow<<" "<<k-patcol+1<<endl;
					xy.push_back(make_pair(i-patrow,k-patcol+1));
					j=lps[j-1];
				}
			else if(i<textrow && procText[i][k]!=patt[j]){
				if(j==0){
					i++;
				}
				else{
					j=lps[j-1];
				}
			}
		}

	}
	cout<<"pattern found at:"<<endl;
	if(xy.size() != 0)
	{
		for(ll i=0; i<xy.size(); i++)
		{	
			cout<<xy[i].first<<" "<<xy[i].second<<endl;
		}
		//cout<<"ended"<<endl;
		cout<<"";
	}
	else
		cout<<"Not found"<<endl;

}

void BakerBirdAlgo(vector<string>& text,ll textrow,ll textcol, vector<string>& pattern,ll patrow,ll patcol)
{	
	ll patindex=1;
	vector<ll> pat_preprocessed(patrow);

	// map strings of pattern matrix to indices and create newly processed pattern
	map<string,ll> pat_map;
	for(ll i=0; i<patrow; i++)
	{
		pat_map[pattern[i]]=patindex++;
		//pat_preprocessed[i]=pat_map[pattern[i]];
		//cout<<"pre pat"<<pat_preprocessed[i]<<endl;
	}

	for(ll i=0; i<patrow; i++)
		pat_preprocessed[i]=pat_map[pattern[i]];

	// for(auto it=pat_map.begin(); it!=pat_map.end(); it++)
	// {
	// 	cout<<it->first<<" "<<it->second<<endl;
	// }
	for(int i=0; i<d; i++)
		cout<<"";
	root=new Node();
	//root=&n;
	
	// create trie of strings in pattern matrix(row wise)
	for(ll i=0; i<patrow; i++)
	{	
		//cout<<"called for "<<pattern[i]<<endl;
		createTrie(pattern[i]);
	}

	createFailureLinks();
	vector<vector<ll>> procText(textrow, vector<ll>(textcol,0));
	for(int i=0; i<d; i++)
		cout<<"";
	matchTrie(text,procText,textrow,textcol,patrow,patcol,pat_map);
	for(int i=0; i<d; i++)
		cout<<"";
	searchByKMP(procText,pat_preprocessed,textrow,textcol,patrow,patcol);
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

        vector<string> text(textrow);
        vector<string> pattern(patrow);
        /*Text size-> n1xn2
          Pattern size-> p1xp2*/

        // input text matrix
        for(ll i=0; i<textrow; i++){
            getline(infile, line);
            text[i]=line;
            // cin>>textmat[i];
        }
        // cout<<"text done"<<endl;

        //input pattern matrix
        for(ll i=0; i<patrow; i++){
            getline(infile, line);
            pattern[i]=line;
            // cin>>patmat[i];
		}
		clock_t start=clock();
		BakerBirdAlgo(text,textrow,textcol,pattern,patrow,patcol);
		double duration = 1000* (clock() - start) / (double)CLOCKS_PER_SEC;
		
		str += to_string(duration) + "\n";
		
	}
	writeToFile(str);

}