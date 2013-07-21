/************************************************************************************************************

	AUTHOR : coderhead_42

	FILE NAME : simulate.cpp

	CREATION DATE : Sun 07 Apr 2013 04:39:30 PM IST

	LAST MODIFIED : Sun 07 Apr 2013 04:49:23 PM IST
			
*************************************************************************************************************/


#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include<typeinfo>

using namespace std;

typedef long long LL;
typedef vector<int> VI;
typedef vector<int>::iterator VII;
typedef vector<vector <int> > VVI;
typedef vector<vector <int> >::iterator VVII;
typedef vector<string> VSTR;
typedef vector<string>::iterator VSTRI;
typedef string STR;
typedef string::iterator STRI;
typedef pair<int,int> PII;
typedef list<int> LI;
typedef list<int>::iterator LII;


#define INF (int)1e9
#define LINF (long long)1e18
#define EPS 1e-9
#define MP make_pair
#define PB push_back
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int) (x).size())
#define FOR(i,a,b) for(int i=a;i<b;i++)
#define FORi(i,a,b) for (i=a;i<b;i++)
#define ROF(i,a,b) for(int i=a-1;i>=b;i--)
#define ROFi(i,a,b) for (i=a-1;i>=0;i--)
#define DIS(a)	sort(all(a)); a.erase(unique(all(a)),a.end())
#define SI ({int x;scanf("%d",&x);x;})
#define SC ({char c;scanf("%c",&c);c;})
#define SIC ({scanf("%*c");})
#define SMP(a,b,c) printf("%d%s",a,b==c-1?"":"\n");
#define IATOV(a) ({vector<int> v(a,a+sizeof(a)/sizeof(int));v;})
#define CATOV(a) ({vector<char> v(a,a+sizeof(a)/sizeof(char));v;})
#define BS(a,x) ({int z=(lower_bound(ALL(a),x)-(a).begin());(z==0&&a[0]!=x)?-1:z;})

#define DEBUG 1

/****************************************************************************************************************************/

int main(){

#ifdef DEBUG
  double tmp_start = clock();
  fprintf(stderr, "Start\n");
#endif

int n=20;
int r=20;

FOR(i,10,20)
	cout<<r*cos(i*((double)2*3.14159265359/n))+20<<" "<<r*sin(i*((double)2*3.14159265359/n))<<"\n";
FOR(i,0,10)
	cout<<r*cos(i*((double)2*3.14159265359/n))+20<<" "<<r*sin(i*((double)2*3.14159265359/n))<<"\n";

	

	

#ifdef DEBUG
  fprintf(stderr, "Total time = %.2lf sec\n", (double)(clock() - tmp_start) / CLOCKS_PER_SEC);
#endif

return 0;
}
