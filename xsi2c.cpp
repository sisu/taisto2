#include<iostream>
#include<iterator>
#include<set>
#include<vector>
#include<fstream>
#include<map>


std::vector<float> position;
std::vector<float> normal;
std::vector<int> pi;
std::vector<int> ni;

typedef std::vector<float> component;
std::vector<component> components;
std::map<component,int> c2i;
std::vector<int> index;
int main()
{

    std::string str;
    while(std::cin>>str)
    {
        if(str.find("POSITION")!=std::string::npos)
        {
            break;
        }
    }
    std::getline(std::cin,str);
    int n = 0;
    while(true)
    {
        float f;
        std::cin>>f;
        char comma;
        std::cin>>comma;
        if(f>10){
            n = 3*f;
            break;
        }
        position.push_back(f);
    }
    std::getline(std::cin,str);
    std::getline(std::cin,str);
    for(int i=0;i<n;i++)
    {
        float f;
        std::cin>>f;
        char comma;
        std::cin>>comma;
        if(f>10)
            break;
        normal.push_back(f);
    }
    std::cout<<"//"<<position.size()/3<<"\n";
    std::getline(std::cin,str);
    std::getline(std::cin,str);
    std::getline(std::cin,str);
    std::cin>>n;
    std::getline(std::cin,str);
    std::getline(std::cin,str);
    std::getline(std::cin,str);
    n*=3;
    for(int i=0;i<n;i++)
    {
        int x;
        std::cin>>x;
        char comma;
        std::cin>>comma;
        pi.push_back(x);
    }
    for(int i=0;i<n;i++)
    {
        int x;
        std::cin>>x;
        char comma;
        std::cin>>comma;
        ni.push_back(x);
    }
    std::cout<<"#include\"Model.hpp\"\n";
    for(int i=0;i<n;i++)
    {
        int po = pi[i];
        int no = ni[i];
        component c(6); 
        for(int j=0;j<3;j++)
            c[j]=position.at(po*3+j);
        for(int j=0;j<3;j++)
            c[j+3]=normal[no*3+j];
        if(c2i.count(c)==0)
        {
            components.push_back(c);
            c2i[c]=components.size()-1;
        }
        int ind = c2i[c];
        index.push_back(ind);
    }

    std::cout<<"float ukko_data[] = {";
    for(int i=0;i<components.size();i++)
    {
        std::copy(components[i].begin(),components[i].end(),
                std::ostream_iterator<float>(std::cout,","));
        std::cout<<"\n";
    }
    std::cout<<"0};\n";
    std::cout<<"short ukko_index[] = {";
    for(int i=0;i<index.size();i++)
    {
        if(i!=0)std::cout<<",";
        std::cout<<index[i];
        if(i%16==0)
            std::cout<<"\n";
    }
    std::cout<<"};\n";
    std::cout<<" Model ukko_model = {ukko_data,ukko_index,sizeof(ukko_data)/(6*sizeof(float)),sizeof(ukko_index)/sizeof(short)};"<<"\n";



}

