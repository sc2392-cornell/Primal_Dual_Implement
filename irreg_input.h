/* 
 * File:   irreg_input.h
 * Author: cheungsinshuen
 *
 * Created on June 6, 2015, 1:30 PM
 */

#ifndef IRREG_INPUT_H
#define	IRREG_INPUT_H

#include "PSteiner.h"
using namespace std;

struct i_node{
    int index;
    int v;
    int h;
    int weight;
};

struct i_edge{
    int x;
    int y;
    int len;
};

/*
struct PSInst{
    int root_ind;
    int opt_val_offline;
    int num_arr_terms;
    int num_edges;
    int num_nodes;
    unordered_map <node_pair,  int> Graph;
    vector <int> ArrTerminals;// indices
    vector <int> ArrTerminals_Pnty;// penalties
    unordered_map <int, vector<int> >  node_to_edges; //in the form node1 -> {node2's}]
    
};
*/

struct Irreg_Input {
    vector<i_node > Nodes;
    int root_ind;
    int num_arr_terms;
    int num_nodes;
    int num_edges;
    unordered_map<node_pair, int> Graph;
    vector<int> ArrTerminals;
    vector<int> ArrTerminals_Pnty;
    unordered_map<int, vector<int> >node_to_edges; 
};

Irreg_Input irreg_read_in(string arg){
    Irreg_Input IRinst;
    IRinst.root_ind=1;
    IRinst.num_arr_terms=0;
    ifstream infile(arg);
    if (infile)
    {
        cout<<"in loop"<<endl;
        string s;
        if (!getline( infile, s )) {cerr<<"something is wrong"; } //skip first line
        if (!getline( infile, s )){cerr<<"something is wrong"; } //skip second line
        if (!getline( infile, s )){cerr<<"something is wrong"; } // start from third line:
        //// start reading in nodes ///
        cout<<"s len = "<<s.length()<<endl;
        while(infile){
            i_node t_i_node; 
            int s_pos = 0;
            string temp_str;// stores the current string of interest
            int str_counter=0;
            while (s_pos+1 <= s.length()) { // skip all spaces and read in node_ind, node_v, node_h values;
                if (s[s_pos] == ' ') {
                    cout<<"space"<<endl;
                } else {// current s[s_pos] is not a space
                    temp_str.push_back(s[s_pos]);
                    if ( s.length()==s_pos+1 ||  s[s_pos+1] == ' ') { // end of current string. need to update node info accordingly
                        int t_int=stoi(temp_str);
                        if(str_counter==0) {
                            t_i_node.index=t_int;
                        }
                        else if(str_counter==1){
                            t_i_node.v=t_int;
                        }
                        else if(str_counter==2){
                            t_i_node.h=t_int;
                        }
                        else if(str_counter==3){
                            t_i_node.weight=t_int;
                            break;
                        }
                        temp_str.clear();
                        str_counter++;
                    }
                }
                cout<<"s_pos="<<s_pos<<endl;
                s_pos++;
                
            }
            cout<<"read in node: "<<t_i_node.index<<"  "<<t_i_node.v<<"  "<<t_i_node.h<<"  "<<t_i_node.weight<<endl;
            if(t_i_node.weight!=0) {
                IRinst.ArrTerminals.push_back(t_i_node.index); 
                IRinst.ArrTerminals_Pnty.push_back(t_i_node.weight);
            }
            IRinst.num_nodes++;
            IRinst.Nodes.push_back(t_i_node);// add node info to IRinst.Nodes
            if (!getline( infile, s )) break;//get a new line;
            if(s[0]=='l') break;    
        }
        //construct node_to_edges;
        vector<int> temp;
        for(int i=1; i<= IRinst.num_nodes;i++){
            IRinst.node_to_edges[i]=temp;
        }
        
        
        
            if(s[0]!='l') {cerr<<"something is wrong"; }
            if (!getline( infile, s )){cerr<<"something is wrong"; } //get a new line;
            if (!getline( infile, s )){cerr<<"something is wrong"; } //get a new line; start with this line
        // read in the edges' info
        while(infile){            
            i_edge t_i_edge;
            
            int s_pos = 0;
            string temp_str;// stores the current string of interest
            int str_counter=0;
            while (s_pos+1 <= s.length()) { // skip all spaces and read in node_ind, node_v, node_h values;
                if (s[s_pos] == ' ') {
                    //do nothing
                } else {// current s[s_pos] is not a space
                    temp_str.push_back(s[s_pos]);
                    if ( s.length()==s_pos+1 ||  s[s_pos+1] == ' ') { // end of current string. need to update node info accordingly
                        int t_int=stoi(temp_str);
                        if(str_counter==0) {
                            //do nothing
                        }
                        else if(str_counter==1){
                            t_i_edge.x=t_int;
                        }
                        else if(str_counter==2){
                            t_i_edge.y=t_int;
                        }
                        else if(str_counter==3) {
                            t_i_edge.len=t_int;
                            break;
                        }
                        temp_str.clear();
                        str_counter++;
                    }
                }
                s_pos++;
            }
            
            // sanity check!
            if (t_i_edge.len==0 || t_i_edge.x==0 || t_i_edge.y==0){
                cerr<<"something went wrong!";
                break;
            }
            //read in to IRinst.Graph
            node_pair tnp;
            tnp.node1=min(t_i_edge.x, t_i_edge.y);
            tnp.node2=max(t_i_edge.x, t_i_edge.y);
            IRinst.node_to_edges[tnp.node1].push_back(tnp.node2);
            IRinst.node_to_edges[tnp.node2].push_back(tnp.node1);            
            IRinst.Graph[tnp]=t_i_edge.len;
            //update num_edges;
            IRinst.num_edges++;

            if (!getline( infile, s )) break;//get a new line;
            if(s[0]=='r') break;    
        }
    }
    /*
    if (!infile.eof())
    {
        cerr << "Fooey!\n";
    }*/

    // update IRinst.node_to_edges
    
    
    
    cout<< IRinst.num_nodes<< IRinst.num_edges<<endl;
    
    return IRinst;
    
}






#endif	/* IRREG_INPUT_H */

