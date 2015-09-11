/* 
 * File:   reg_input.h
 * Author: cheungsinshuen
 *
 * Created on June 6, 2015, 1:32 PM
 */

#ifndef REG_INPUT_H
#define	REG_INPUT_H
#include "PSteiner.h"

vector<double> OnOffOpts(string f_name);
PSInst read_in( string arg );
void print_inst(PSInst psinst);

void process_all_regular_data(){
    
    vector<vector<double> > OnOffSol;
    
    ifstream infile("RegularFilesNames.txt");
    
    while(infile){
        string s;
        if(!getline(infile,s)) break;
        string st="pcstp/"; st.append(s);
        cout<<"-------------------------------------------------------computes for file "<< st;
        OnOffSol.push_back(OnOffOpts(st));
    }
    
    cout<<endl;
    cout.precision(15);
    cout<<"=================Output Begin======================"<<endl;
    
    for (auto it= OnOffSol.begin(); it!=OnOffSol.end();it++){
        for (auto it2=it->begin(); it2!=it->end();it2++){
            cout<<*it2<< "   ";
        }
        cout<<endl;
    }
    
}

vector<double> OnOffOpts(string f_name){
    PSInst psinst=read_in (f_name);
    print_inst(psinst);
    PSteiner PS(psinst);
    
    PS.Algo();
    vector<double> ans = PS.Out_sol();
    return ans;
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


#endif	/* REG_INPUT_H */

