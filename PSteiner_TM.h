/* 
 * File:   PSteiner_TM.h
 * Author: cheungsinshuen
 *
 * Created on September 1, 2015, 10:52 AM
 */

#ifndef PSTEINER_TM_H
#define	PSTEINER_TM_H
#include"PSteiner.h"

struct z_j_pair{ // to describe an edge
    int z, j;
    vector<int> path;
    
    bool operator ==(const z_j_pair & other) const{
        return z==other.z && j==other.j;
    }
    
    z_j_pair& operator =(const z_j_pair & other) {
        if (this!=&other)   {this->z=other.z;this->j=other.j; this->path=other.path;} 
        return *this;
    }
    z_j_pair(){
        z=0;
        j=0;
        vector<int> temp;
        path=temp;
    }
    
};

class PSteiner_TM {
public:
    
    //constructor, destructor....
    PSteiner_TM();
    PSteiner_TM(const PSteiner_TM& orig);
    PSteiner_TM(const PSInst PS){
        Graph=PS.Graph;
        ArrTerminals=PS.ArrTerminals;
        ArrTerminals_Pnty=PS.ArrTerminals_Pnty;
        NumVertices=PS.num_nodes;
        NumEdges=PS.num_edges;
        NumArrTerminals=PS.num_arr_terms;
        OptimalVal=PS.opt_val_offline;
        RootInd=PS.root_ind;
        node_to_edges=PS.node_to_edges;
        // finished importing from PSteiner instance
        
        
        //initialize vars for algorithm use
        ConnCost=0;
        PntyCost=0;

        
        
        
    };// construct initial instance from  PSteiner read-in's.
    
    
    
    PSteiner_TM(const Irreg_Input& inst) {
        Graph=inst.Graph;
        ArrTerminals=inst.ArrTerminals;
        ArrTerminals_Pnty=inst.ArrTerminals_Pnty;// remark: this one is not really used directly
        NumVertices=inst.num_nodes; // number of vertices in graph
        NumEdges=inst.num_edges;// number of edges in Graph
        OptimalVal=0; // Optimal value to offline problem  is left empty
        RootInd=inst.root_ind;
        NumArrTerminals=inst.num_arr_terms;
        /*
        for (int i=0; i< ArrTerminals.size();i++){// define terms_pnty for fast retrieval 
            terms_pnty[ArrTerminals[i]]=(double)ArrTerminals_Pnty[i]/2;
        }
         */
        //define node_to_edges:
        node_to_edges=inst.node_to_edges;
    
        ConnCost=0;PntyCost=0;
    };
    
    
    virtual ~PSteiner_TM();

    int Modify_Arr_Perm(vector<int> perm){
        if(perm.size()!=ArrTerminals.size()) {cerr<<"Modify_Arr_Perm size doesnt match!"; return 0;}
        vector<int> NewArrTerminals_Pnty;
        for(int i=0; i<perm.size();i++){
            for (int it = 0; it<ArrTerminals.size();it++){
                if(ArrTerminals[it]==perm[i]){NewArrTerminals_Pnty.push_back(ArrTerminals_Pnty[it]); break;}
                // it = the index in ArrTerminals_pnty that corresponds to 
            }
            
        }
        if(NewArrTerminals_Pnty.size()!=ArrTerminals_Pnty.size()){
            cerr<<"-- Permutation Error: NewArrTerminals_Pnty size doesn't match"<<endl;
        }
        else{
            ArrTerminals=perm;
            ArrTerminals_Pnty=NewArrTerminals_Pnty;
            RootInd=ArrTerminals[0];
        }
        return 0;
    };
    
    
    
    //Algo
    z_j_pair TM_Dijkstra(int i); // finds the closest terminal to i in Z, determine j, and modify X_j
    
    unordered_set<int> Find_W(int i, int j);// finds W_i 
    
    void FindDistFromSource(int s);
    void Update_rho(int i, unordered_set<int> W, int j,vector<int> path);// updates rho_i, and buy edge or pay penalty, accordingly
    
    int TM_Algo(){
        
        
        
        

        if(ArrTerminals.size()<2)
        {
            cerr<<"Error: Too few arriving terminals";      
            return 0;
        }
        else{
            if (ArrTerminals_Pnty[0]<ArrTerminals_Pnty[1]){
                int T_ind=ArrTerminals_Pnty[0];
                ArrTerminals_Pnty[0]=ArrTerminals_Pnty[1];
                ArrTerminals_Pnty[1]=T_ind;
                T_ind=ArrTerminals[0];
                ArrTerminals[0]=ArrTerminals[1];
                ArrTerminals[1]=T_ind;
            }
        }
        
        RootInd=ArrTerminals[0];
        
        Z.insert(RootInd);
        
        for (int ind=0; ind<ArrTerminals.size(); ind++){
            cout<<"The "<<ind<<"-th terminal index = "<<ArrTerminals[ind]<<endl;
        //main loop
            /*if(ind==0){
                Z.insert(ArrTerminals[ind]);  //  insert the root to Z --- the set of connected terminals
                continue;
            }*/
            //finds closest terminal in Z to i, set j, add i to X_j
            if(Z.find(ArrTerminals[ind])!=Z.end()){
                L_BoughtEdges.push_back(BoughtEdges); 
                L_ConnCost.push_back(ConnCost);
                L_PntyCost.push_back(PntyCost);
                continue;
            }
            
            z_j_pair zjpair= TM_Dijkstra(ArrTerminals[ind]); 
            
            
            //Find_W  
            unordered_set<int> W= Find_W(ArrTerminals[ind], zjpair.j);   
            
            Update_rho(ind, W, zjpair.j, zjpair.path);
                
            L_BoughtEdges.push_back(BoughtEdges); 
            L_ConnCost.push_back(ConnCost);
            L_PntyCost.push_back(PntyCost);
        }
        
    };
    
    void PrintInst(){
        cout<<"ArrTerminals = ";
        for(auto it = ArrTerminals.begin(); it!=ArrTerminals.end(); it++){
            cout<<*it<< " ";
        }
        cout<<endl;
        cout<<"ArrTerminals_Pnty = ";
        for(auto it = ArrTerminals_Pnty.begin(); it!=ArrTerminals_Pnty.end(); it++){
            cout<<*it<< " ";
        }
        cout<<endl;
        cout<<"RootInd = "<<RootInd<<endl;
        cout<<"Number of Edges = "<<Graph.size()<<endl;
        cout<<"Number of Nodes = "<<node_to_edges.size()<<endl;
    }
    
    void Out(){
        cout.precision(15);
        cout<<"TotalCost="<<ConnCost+PntyCost<<endl;
        cout<<"ConnCost="<<ConnCost<<endl;
        cout<<"PntyCost="<<PntyCost<<endl;
        cout<<"Connected Terms: ";
        for (auto it= Z.begin(); it!=Z.end(); it++){
            cout<< *it<<" ";
        }
        cout<<endl;
        cout<<"Penalized Terms: ";
        for (auto it= PenTerms.begin(); it!=PenTerms.end(); it++){
            cout<< *it<<" ";
        }
        cout<<endl;
        
        cout<<"====Start Output L_BoughtEdges==="<<endl;
        
        for(int ind=0; ind<L_BoughtEdges.size();ind++){
            cout<<"------ ind = "<< ind<<" -----"<<endl;
            vector<int> Left, Right;
            for (auto it=L_BoughtEdges[ind].begin();it!=L_BoughtEdges[ind].end(); it++){
                Left.push_back(it->node1);
                Right.push_back(it->node2);
            }
            cout<<"LeftNodes and RightNodes:"<<endl;
            for(auto it=Left.begin();it!=Left.end(); it++){
                cout<< *it<<",";
            }
            cout<<endl;
            for(auto it=Right.begin();it!=Right.end(); it++){
                cout<< *it<<",";
            }
            cout<<endl;
        
        }
        
        
        cout<<"====Start Output Total Costs Sequentially ==="<<endl;
        for(int ind=0; ind<L_ConnCost.size();ind++){
            cout<<L_ConnCost[ind]+L_PntyCost[ind]<<endl;
        }
        
       
    };
    vector<double> Out_sol(){
        vector<double> ret; ret.push_back(OptimalVal);
        //check feasibility:
        
        ret.push_back(PntyCost+ConnCost);
        
        return ret;//ret[0] is OptimalVal, ret[1] is OnlineVal
    };
    
    
private:
    
    /* Instance Information */
    unordered_map <node_pair,  int  > Graph; // 2-dim vector with format: [index_node1, index_node2]
    vector <int>  ArrTerminals, ArrTerminals_Pnty;// indices of arriving terminals
    int NumVertices; // number of vertices in graph
    int NumEdges;// number of edges in Graph
    int NumArrTerminals;// number of arriving terminals 
    int OptimalVal; // Optimal value to offline problem 
    int RootInd;//root index .... root is always active in Steiner Tree setting
    unordered_map <int, vector<int> >  node_to_edges; //in the form node1 -> {node2's}]
    
    
    
    /* algo related vars*/
    unordered_set<int> Z; //corres. to Z
    vector<unordered_set<int> > X; // corres. to  X_j's
    unordered_map<node_pair, int> distmap;// finds distance between node_pair
    unordered_map<node_pair, bool> F_distmap;// TRUE if the node_pair has been updated in distmap
    unordered_map <int, double> rho ;// rho_i for each i in terminals
    unordered_map <int, double> F_rho ;// TRUE if rho_i is updated, FALSE if rho_i is 0 
    unordered_set<node_pair> BoughtEdges;//records edges bought
    unordered_set<int> PenTerms;// records terms that pays penalty
    unordered_map <node_pair,  bool  > F; // set of purchased edges 
    double ConnCost,PntyCost;
    vector<double> L_ConnCost, L_PntyCost;

    vector<unordered_set<node_pair> > L_BoughtEdges;
    
    
    

    
 
};

#endif	/* PSTEINER_TM_H */

