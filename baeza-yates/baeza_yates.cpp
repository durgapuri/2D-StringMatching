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
	int fd=open("timing_baeza_yates.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(fd,st.c_str(), st.length());
}

class Node{

public:
	Node* child_nodes[CHARSET];
	bool end;
	Node* fail_redirect;
	vector<string> vec;
	int op;
	Node()
	{
		end=false;
		fail_redirect=NULL;
		for(int i=0; i<CHARSET; i++)
			child_nodes[i]=NULL;
		op=0;
	}

};

Node* root=NULL;

void createTrie(string val,map<string,ll>& pat_map)
{	
	//cout<<"in createTrie:"<<endl;
	cout<<"";
	Node* m=root;
	ll len=val.length();
	for(int i=0; i<d; i++)
		cout<<"";
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
	for(int i=0; i<d; i++)
		cout<<"";
	m->op=pat_map[val];
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
			for(int i=0; i<d; i++)
				cout<<"";
			//cout<<"root->child_nodes[i]->fail_redirect"<<root->child_nodes[i]->fail_redirect<<endl;
			qu.push(root->child_nodes[i]);
		}
	}
	//cout<<"queue size "<<qu.size()<<endl;
	cout<<"";
	/* apply BFS to update fail_redirect for each node in trie
	find longest proper suffix which is also a prefix 
	update the fail_redirect to that node*/

	Node* curr;

	while(!qu.empty())
	{
		curr=qu.front();
		qu.pop();
		for(int i=0; i<d; i++)
			cout<<"";
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
					for(int i=0; i<d; i++)
						cout<<"";

					for(ll k=0; k<fail->child_nodes[i]->vec.size(); k++)
					{
						child->vec.push_back(fail->child_nodes[i]->vec[k]);
					}

				}
				else
				{
					Node* temp=fail;
					while(temp->child_nodes[i] == NULL && temp->fail_redirect!=NULL)
					{
						temp=temp->fail_redirect;
					}
					for(int i=0; i<d; i++)
						cout<<"";
					if(temp->child_nodes[i] == NULL && temp->fail_redirect==NULL)
					{
						//prefix doesn't exist -- start from root again
						child->fail_redirect=root;
						for(int i=0; i<d; i++)
							cout<<"";
					}
					else if(temp->child_nodes[i]!=NULL)
					{	
						//found prefix
						child->fail_redirect=temp->child_nodes[i];
					}
					else
					{
						//send redirect to failure node of parent
						child->fail_redirect=temp->fail_redirect;
					}
					for(ll k=0; k<temp->vec.size(); k++)
					{
						child->vec.push_back(temp->vec[k]);
					}
				}
				qu.push(child);
			}
		}
	}
	//cout<<"end of createFailureLinks:"<<endl;

}

void checkMatched(ll r,ll c, ll in,vector<string>& text,ll textrow,ll textcol,vector<string>& pattern, ll patrow, ll patcol,map<string,ll>& pat_map,vector<pair<ll,ll>>& res)
{
	vector<ll> arr(2*patrow,0);
	arr[patrow-1]=in;
	
	for(ll i=0; i<patrow; i++)
	{
		if(pat_map[pattern[i]] != in)
		{
			continue;
		}
		for(int i=0; i<d; i++)
			cout<<"";
		int j=patrow-i+1;
		int p;
		for(p=r-i; p<r-i+patrow && p<textrow; p++)
		{
			if(arr[j] == pat_map[pattern[p-r+i]])
				continue;
			for(int i=0; i<d; i++)
				cout<<"";
			Node* n=root;
			for(int q=c; q<c+patcol; q++)
			{
				int in=text[p][q];
				if(n->child_nodes[in] != NULL)
					n=n->child_nodes[in];
			}
			for(int i=0; i<d; i++)
				cout<<"";
			arr[j]=n->op;
			if(arr[j] != pat_map[pattern[p-r+i]])
				break;
		}
		if(p == r - i + patrow)
		{
			res.push_back(make_pair(p-patrow,c));
			for(int i=0; i<d; i++)
				cout<<"";
		}
	}

	
}

void matchTrie(vector<string>& text,ll textrow,ll textcol, vector<string>& pattern,ll patrow,ll patcol,map<string,ll>& pat_map)
{	vector<pair<ll,ll>> res;
	for(ll k=patrow-1; k<textrow; k+=patrow)
	{
		int i=0;
		Node* n=root;
		for(int i=0; i<d; i++)
			cout<<"";
		while(i<textcol)
		{
			int in=text[k][i];

			if(n->child_nodes[in] != NULL)
			{
				n=n->child_nodes[in];
				for(int i=0; i<d; i++)
					cout<<"";
				if(n->op != 0)
				{
					
					checkMatched(k, i-patcol+1, n->op, text,textrow,textcol,pattern,patrow,patcol,pat_map,res);
				}
				i++;
			}
			else
			{
				if(n->fail_redirect != NULL)
					n=n->fail_redirect;
				else
					i++;
			}
		}
	}
	if(res.size()!=0)
	{	
		cout<<"Found at:"<<endl;
		for(ll i=0; i<res.size(); i++)
			cout<<res[i].first<<" "<<res[i].second<<endl;
	}
	else
	{
		cout<<"not found"<<endl;
	}
}

void BaezaYatesAlgo(vector<string>& text,ll textrow,ll textcol, vector<string>& pattern,ll patrow,ll patcol)
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
	root=new Node();
	//root=&n;
	
	// create trie of strings in pattern matrix(row wise)
	for(ll i=0; i<patrow; i++)
	{	
		//cout<<"called for "<<pattern[i]<<endl;
		createTrie(pattern[i],pat_map);
	}
	for(int i=0; i<d; i++)
		cout<<"";
	createFailureLinks();
	for(int i=0; i<d; i++)
		cout<<"";
	matchTrie(text,textrow,textcol,pattern,patrow,patcol,pat_map);
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

    	cout<<"IN LOOP"<<endl;
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
        // Text size-> n1xn2
        //   Pattern size-> p1xp2

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
		BaezaYatesAlgo(text,textrow,textcol,pattern,patrow,patcol);

		double duration = 1000* (clock() - start) / (double)CLOCKS_PER_SEC;
		//cout<<"returned from Baeza"<<endl;
		str += to_string(duration) + "\n";
		
	}
	writeToFile(str);
	//cout<<"Exited from loop"<<endl;


}