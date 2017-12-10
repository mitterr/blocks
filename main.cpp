#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <map>
#include <unordered_map>
#include <bitset>
#include <complex>
#include <complex>
#include <cmath>
#include <iomanip>
#include <math.h>
#include <cstring>
#include <queue>
#include <set>
#include <fstream>
#include "muParser.h"
#define mp make_pair
#define pb push_back
const int MOD = 1e9 + 7;
using namespace std;
using namespace mu;
pair<string , string> v;
Parser parser;
struct node
{
    string type;
    string opr;
};
map<string, node> block;
map<string , vector<string>> g;
map<int, double> var;

bool correct = 1;
void dfs(string name)
{
    //  cout << var['a'] << " " << var['b'] << " " << var['c'] << "\n";
    //  cout << name << "\n";
    if(!correct) return;
    //cout << name << "\n";
    if(block[name].type == "cloud")
    {
        if(block[name].opr[0] == 's') return;
        dfs(g[name][0]);
    }
    if(block[name].type == "io")
    {
        string y = block[name].opr;
        if(y.find("read") < 10000)
            for(int i = 0;  i < y.size(); i++)
            {
                if(y[i] == '(')
                {
                    char c;
                    while(y[i] != ')')
                    {
                        i++;
                        if(y[i] != ' ' && y[i] != ',' && y[i] != ')')
                        {
                            c = y[i];
                            double p;
                            string fictivnayastroka ="";
                            fictivnayastroka += c;
                            cout << "please input " << y[i] << ":  ";
                            cin >> p;
                            parser.DefineVar( fictivnayastroka , &var[c]);
                            var[(int)c] = p;
                        }
                    }
                }
            }
        else
        {
            if(y.find("write") >= 10000)
            {
                correct = 0;
                return;
            }
            for(int i = 0;  i < y.size(); i++)
            {
                if(y[i] == '(')
                {
                    char c;
                    while(y[i] != ')')
                    {
                        i++;
                        if(y[i] != ' ' && y[i] != ')' && y[i] != ',')
                        {
                            c = y[i];
                            double p;
                            cout << y[i] << " = ";
                            cout << setprecision(6) << fixed << var[(int)y[i]] << "\n";
                        }
                    }
                }
            }
            
        }
        dfs(g[name][0]);
    }
    if(block[name].type == "block")
    {
        int to = 0;
        string y = block[name].opr;
        bool flag = 0;
        char left;
        string right;
        for(int i = 0;  i < y.size(); i++)
        {
            if(y[i] != ' ' && !flag)
            {
                left = y[i];
                flag = 1;
            }
            if(y[i] == '=')
            {
                while(i < y.size())
                {
                    i++;
                    if((y[i] >= 'a' && y[i] <='z') || (y[i] == '+' || y[i] == '-' || y[i] == '^' || y[i] == '*' || y[i] == '/' || y[i] == '.') || (y[i] >= '0' && y[i] <= '9') || y[i] == '(' || y[i] == ')')
                        right.push_back(y[i]);
                }
            }
        }
        string sss ;
        sss.pb(left);
        // cout << right << "\n";
        parser.DefineVar( sss, &var[left]);
        if(correct)
            try
        {
            parser.SetExpr(right);
            var[left] = parser.Eval();
        } catch (Parser:: exception_type &e)
        {
            //  if(correct) cout << "INCORRECT INPUT!\n";
            correct = 0;
            return;
        }
        dfs(g[name][to]);
    }
    if(block[name].type == "decision")
    {
        string y;
        for(int i  = 0; i < block[name].opr.size(); i++)
        {
            if(block[name].opr[i] != '$') y.pb(block[name].opr[i]);
        }
        bool flag = 0;
        // y+= "####";
        string left;
        string right;
        string znak;
        for(int i  = 0; i < y.size() - 4; i++)
        {
            if(y[i] == '\\' && y[i+1] == 'n' && y[i+2] == 'e')
            {
                y[i] = '!';
                y[i+1] = '=';
                y[i+2] = ' ';
            }
            if(y[i] == '\\' && y[i+1] == 'l' && y[i+2] == 'e' && y[i+3] == 'q' )
            {
                y[i] = '<';
                y[i+1] = '=';
                y[i+2] = ' ';
                y[i+3] = ' ';
                //cout << "Afafa" << "\n";
            }
            if(y[i] == '\\' && y[i+1] == 'g' && y[i+2] == 'e' && y[i+3] == 'q' )
            {
                y[i] = '>';
                y[i+1] = '=';
                y[i+2] = ' ';
                y[i+3] = ' ';
            }
        }
        string t;
        for(int i = 0; i < y.size(); i++)
        {
            if(y[i] != '\\') t.pb(y[i]);
        }
        y = t;
        int ok = 0;
        //  cout << y <<"\n";
        try
        {
            parser.SetExpr(y);
            //  cout << y << "\n";
            ok = parser.Eval();
            //  cout << "ok =" << ok << "\n";
        }
        catch (Parser:: exception_type &e)
        {
            //  if(correct) cout << y<<"\n";
            correct = 0;
            return;
        }
        if(correct)
            dfs(g[name][(int)(ok ^ 1)]);
        
    }
    
}
map<string,pair<int, int>> ord;
int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    ifstream fin;
    string fname;
    fin.open("shema.tex");
    //   freopen("/Users/mitterr/Desktop/block-scheme/block-scheme/shema.tex" , "r" , stdin);
    string s;
    bool flag = 0;
    int unt = 0;
    vector<string> names;
    while(getline(fin , s))
    {
        unt++;
        
        if(s.find("\\node") < 10000)
        {
            node x;
            string name;
            for(int i = 0; i < s.size(); i++)
            {
                if(s[i] == '[')
                {
                    string p;
                    while(s[i] != ']' && s[i] != ',')
                    {
                        i++;
                        if(s[i] != ']' && s[i] != ',') p.push_back(s[i]);
                    }
                    // cout << p << "\n";
                    x.type = p;
                }
                
                if(s[i] == '(')
                {
                    string p;
                    while(s[i] != ')')
                    {
                        i++;
                        if(s[i] != ')')
                            p.push_back(s[i]);
                    }
                    name = p;
                    names.pb(name);
                }
                if(s[i] == '{')
                {
                    string p;
                    while(s[i] != '}')
                    {
                        i++;
                        if(s[i] != '}')
                            p.push_back(s[i]);
                    }
                    x.opr = p;
                }
            }
            block[name] = x;
        }
        else
            if(s.find("\\path") < 10000)
            {
                int ind = 2;
                string u , v;
                //cout << s << "\n";
                for(int i = 0; i < s.size(); i++)
                {
                    bool flag = 0;
                    if(s[i] == '(')
                    {
                        string p;
                        while(s[i] != ')')
                        {
                            i++;
                            if(s[i] != ')')
                                p.push_back(s[i]);
                        }
                        u = p;
                        flag = 1;
                    }
                    //  cout << u << "\n";
                    if(flag && block[u].type == "decision")
                    {
                        if(s.find("yes") < 10000)
                        {
                            //cout << u << " " << unt << "\n";
                            ord[u].first = unt;
                        }
                        if(s.find("{no}") < 10000)
                        {
                            //cout << u << " " << unt << "\n";
                            ord[u].second = unt;
                        }
                    }
                    if(flag)
                        while (s[i] != '(' && i < s.size())
                        {
                            i++;
                        }
                    
                    if(s[i] == '(' && flag)
                    {
                        string p;
                        while(s[i] != ')')
                        {
                            i++;
                            if(s[i] != ')')
                                p.push_back(s[i]);
                        }
                        v = p;
                    }
                }
                //    if(
                //   cout << u << " " << v << "\n";
                if(u.size() > 0 && v.size() > 0)
                    g[u].pb(v);
            }
    }
    //    for(auto i : block)
    //    {
    //        cout << i.first << " " << i.second.type << " " << i.second.opr << "\n";
    //    }
    //    cout << "-------------------------------------------\n";
    for(auto i : names)
    {
        if(block[i].type == "decision")
        {
            
            if(ord[i].first > ord[i].second)
            {
                //   cout << i << "\n";
                swap(g[i][0] , g[i][1]);
            }
        }
    }
    int begcnt = 0;
    for (auto i : names)
    {
        if(block[i].type == "cloud" && block[i].opr == "begin" )
            begcnt++;
    }
    if(begcnt > 1)
    {
        cout << "INCORRECT INPUT!\n";
        return 0;
    }
    for(auto i : names)
    {
        if(g[i].size() > 2 || (g[i].size() == 0 && block[i].type != "cloud"))
        {
            cout << "INCORRECT INPUT!\n";
            return 0;
        }
        if(block[i].type == "decision" && g[i].size() != 2)
        {
            cout << "INCORRECT INPUT!\n";
            return 0;
        }
        if((block[i].type != "decision" && g[i].size() >= 2))
        {
            cout << "INCORRECT INPUT!\n";
            return 0;
        }
        
        
    }
    fin.close();
    dfs("init");
    if(!correct)
    {
        cout << "INCORRECT INPUT!\n";
        return 0;
        
    }
    //   dfs("do");
    //   dfs("if");
    //cout << var['a'] << " " << var['b'] << " " << var['c'] << "\n";
    //   dfs("output");
    //cout << var['a'] << " " << var['b'] << " " << var['c'] << "\n";
    return 0;
}
