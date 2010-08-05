#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<int> sts[1<<16];
vector<int> pts[1<<16];
vector<int> drs[1<<16];

int main()
{
	ifstream in("taisto2.mid.txt");
	string str;
	getline(in, str);
	int st, dr, pt, vl, i;
	while(in >> st>>dr>>pt>>vl>>i) {
		sts[i].push_back(st);
		pts[i].push_back(pt);
		drs[i].push_back(dr);
	}
	cout<<"struct Note { int start; char pitch; short duration; };\n\n";
	cout<<"struct NoteB { int start; char pitch; };\n\n";
	vector<int> nnums;
	vector<int> ncnt;
	for(int i=0; i<1<<16; ++i) {
		if (sts[i].empty()) continue;
		if (i==2337) cout<<"const NoteB notes"<<i<<"[] = {\n";
		else cout<<"const Note notes"<<i<<"[] = {\n";
		int n = sts[i].size();
		for(int j=0; j<n; ++j) {
			cout<<"\t{"<<sts[i][j]<<','<<pts[i][j];
			if (i!=2337) cout<<','<<drs[i][j];
			cout<<'}';
			if (j<n-1) cout<<',';
			cout<<'\n';
		}
		cout<<"};\n";
		nnums.push_back(i);
		ncnt.push_back(n);
	}
	cout<<"const Note* notes[] = {";
	for(size_t i=0; i<nnums.size(); ++i) {
		if (i+1<nnums.size()) cout<<"notes"<<nnums[i];
		else cout<<0;
		if (i+1<nnums.size()) cout<<',';
	}
	cout<<"};\n";
	cout<<"const int ncounts[] = {";
	for(size_t i=0; i<ncnt.size(); ++i) {
		cout<<ncnt[i];
		if (1+i<ncnt.size()) cout<<',';
	}
	cout<<"};\n";
}
