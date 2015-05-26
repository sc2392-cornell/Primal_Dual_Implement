/* 
 * File:   main.cpp
 * Author: cheungsinshuen
 *
 * Created on May 5, 2015, 5:09 PM
 */

#include<cstdlib>
#include"PSteiner.h"
//#include"boost/graph/adjacency_matrix.hpp"
using namespace std;
//using std::numeric_limits;
/*
 * 
 */

PSInst read_in( string arg );
void print_inst(PSInst);


int main(int argc, char** argv) {
    string f_name=argv[1];//"pcstp/P400.4.dat";
    //for(int i=0; i<=9; i++){
        //if(i==9){f_name="pcstp/K100.1.dat";}
        //else{f_name[11]+=1;}
        PSInst psinst=read_in (f_name);
    print_inst(psinst);
        PSteiner PS(psinst);
    //PS.print_PS();
    
        PS.Algo();
        cout<<"??"<<endl;
        vector<double> ans = PS.Out_sol();
        cout.precision(15);
        for(auto it=ans.begin();it!=ans.end();it++){
            cout<<*it<< " ";
        }
        cout<<endl;
    //}
    
    
    return 0;
}

PSInst read_in( string arg ){
    PSInst psinst;
    ifstream infile(arg);
    
    while (infile)
    {
        string s;
        if (!getline( infile, s )) break;
        
        psinst.root_ind=stoi(s);
        // read in root index in first line
        
        
        
        if (!getline( infile, s )) break;
        istringstream ss( s );
        while (ss)
        {
            string s;
            if(!getline( ss, s, ' ' )) { break;}
            psinst.num_nodes=stoi(s);
            if(!getline( ss, s, ' ' )) { break;}
            psinst.num_edges=stoi(s);
            if(!getline( ss, s, ' ' )) { break;}
            psinst.num_arr_terms=stoi(s);
            if(!getline( ss, s, ' ' )) { break;}
            if(s[0]==' '){
                if(!getline( ss, s, ' ' )) { break;}
            }
                psinst.opt_val_offline=stoi(s);
            
        }
        for(int i=0; i< psinst.num_arr_terms;i++){
            if (!getline( infile, s )) break;
            psinst.ArrTerminals.push_back(stoi(s));
        }
        for(int i=0; i< psinst.num_arr_terms;i++){
            if (!getline( infile, s )) break;
            psinst.ArrTerminals_Pnty.push_back(stoi(s));
        }
        for(int i=0; i< psinst.num_edges;i++){
            if (!getline( infile, s )) break;
            istringstream ss(s);
            node_pair temp;
            if (!getline( ss, s, ' ' )) break;
            temp.node1=(stoi(s));// first element
            if (!getline( ss, s, ' ' )) break;
            temp.node2=(stoi(s));//second element
            if (!getline( ss, s, ' ' )) break;
            int val =stoi(s);// edge cost
            psinst.Graph[temp]=val;
            
            psinst.node_to_edges[temp.node1].push_back(temp.node2);
            psinst.node_to_edges[temp.node2].push_back(temp.node1);

        }
        

    }

    if (!infile.eof())
    {
        cerr << "Fooey!\n";
    }

    return psinst;

}

void print_inst(PSInst psinst){
    bool edge_match = (psinst.Graph.size()==psinst.num_edges);
    bool arriving_terms_match=(psinst.ArrTerminals.size()==psinst.ArrTerminals_Pnty.size())&& (psinst.ArrTerminals_Pnty.size()==psinst.num_arr_terms);
    cout<<"number of nodes = "<<psinst.num_nodes<<endl;
    cout<<"number of edges = "<<psinst.num_edges<<" match? "<< edge_match<<endl;
    cout<<"root index = "<< psinst.root_ind<<endl;
    cout<<"number of arriving terminals = "<< psinst.num_arr_terms<<" match? "<< arriving_terms_match<<endl;
    cout<<"number of  = "<<psinst.num_nodes<<endl;
}
