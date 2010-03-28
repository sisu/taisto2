#include<iostream>
#include<string>
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
int main(int argc,char** argv)
{
    if(argc<2)return 0 ;
    char* fname = argv[1];
    std::ifstream fin(fname);
    std::string str;
    std::string modelname = fname;
    modelname.resize(modelname.size()-4);
    while(fin>>str)
    {
        if(str.find("POSITION")!=std::string::npos)
        {
            break;
        }
    }
    std::getline(fin,str);
    int n = 0;
    while(true)
    {
        float f;
        fin>>f;
        char comma;
        fin>>comma;
        if(f>10){
            n = 3*f;
            break;
        }
        position.push_back(f);
    }
    std::getline(fin,str);
    std::getline(fin,str);
    for(int i=0;i<n;i++)
    {
        float f;
        fin>>f;
        char comma;
        fin>>comma;
        if(f>10)
            break;
        normal.push_back(f);
    }
    std::cout<<"//"<<position.size()/3<<"\n";
    std::getline(fin,str);
    std::getline(fin,str);
    std::getline(fin,str);
    fin>>n;
    std::getline(fin,str);
    std::getline(fin,str);
    std::getline(fin,str);
    n*=3;
    for(int i=0;i<n;i++)
    {
        int x;
        fin>>x;
        char comma;
        fin>>comma;
        pi.push_back(x);
    }
    for(int i=0;i<n;i++)
    {
        int x;
        fin>>x;
        char comma;
        fin>>comma;
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

    std::cout<<"float "<<modelname<<"_data[] = {";
    for(int i=0;i<components.size();i++)
    {
        std::copy(components[i].begin(),components[i].end(),
                std::ostream_iterator<float>(std::cout,","));
        std::cout<<"\n";
    }
    std::cout<<"0};\n";
    std::cout<<"short "<<modelname<<"_index[] = {";
    for(int i=0;i<index.size();i++)
    {
        if(i!=0)std::cout<<",";
        std::cout<<index[i];
        if(i%16==0)
            std::cout<<"\n";
    }
    std::cout<<"};\n";
    std::cout<<" Model "<<modelname<<"_model = {"<<modelname<<"_data,"<<modelname<<"_index,sizeof("<<modelname<<"_data)/(6*sizeof(float)),sizeof("<<modelname<<"_index)/sizeof(short)};"<<"\n";



}

