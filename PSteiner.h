/* 
 * File:   PSteiner.h
 * Author: cheungsinshuen
 *
 * Created on May 8, 2015, 3:11 PM
 */

#ifndef PSTEINER_H
#define	PSTEINER_H
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<vector>
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<sstream>
#include<cmath>
using namespace std;
// structures defined here
struct node_pair{ // to describe an edge
    int node1, node2;
    
    bool operator ==(const node_pair & other) const{
        return ((node1==other.node1 && node2==other.node2) || (node1==other.node2 && node2==other.node1)) ;
    }
    
    node_pair& operator =(const node_pair & other) {
        if (this!=&other)
        {   this->node1=other.node1;  this->node2=other.node2 ; } 
        return *this;
    }
    node_pair(){
        node1=0;
        node2=0;
    }
    
};
struct node_j_pair{
    int node, j;
    bool operator ==(const node_j_pair & other) const{
        return (node==other.node && j==other.j);
    }
    node_j_pair(){
        node=0; j=0;
    }
    node_j_pair& operator = (const node_j_pair & other) {
        if(this!=&other){
            this->node=other.node;
            this->j=other.j;
        }
        return *this;
    }    
};

struct edge_j_pair{
    int node1,node2, j;
    bool operator ==(const edge_j_pair & other) const{
        return ((node1==other.node1 && node2==other.node2) || (node2==other.node1 && node1==other.node2)) && (j==other.j);
    }
    
    edge_j_pair& operator = (const edge_j_pair & other) {
        if(this!=&other){
            this->node1=other.node1;
            this->node2=other.node2; 
            this->j=other.j;
        }
        return *this;
    }
    
    edge_j_pair(){
        node1=0;node2=0;j=0;
    }
    
    
};

struct moat_j_pair{
    unordered_set <int> moat;
    int j;

    bool operator ==(const moat_j_pair & other) const{
        return (moat==other.moat && j==other.j);
    }
    
    moat_j_pair& operator = (const moat_j_pair & other) {
        if(this!=&other){
            this->moat=other.moat;
            this->j=other.j;
        }
        return *this;
    }
    
  
};





struct node_val_pair{
    int node;
    double val; 
    bool operator ==(const node_val_pair & other) const{
        return (node==other.node && (fabs(val-other.val)<.0001));
    }
    node_val_pair& operator =(const node_val_pair & other) {
        if (this!=&other)
        {   this->node=other.node;  this->val=other.val ; } 
        return *this;
    }
    node_val_pair(){
        node=0;
        val=0;
    }
    
};

/* [discarded] 
struct int_maps_to_moat{
    unordered_map<int, unordered_set<int>> mapping;
    int_maps_to_moat(){
        
    }
};
*/

struct edge_val_pair{
    int node1, node2;
    double val;
    edge_val_pair(){
        node1=0; node2=0;
        val=0;
    }

};

struct double_val_pair{
    double current_val;
    double target_val;
    double_val_pair(){
        current_val=0;
        target_val=0;
    }
    double_val_pair& operator =(const double_val_pair & other) {
        if (this!=&other)
        {   this->current_val=other.current_val;  this->target_val=other.target_val ; } 
        return *this;
    }
    bool operator ==(const double_val_pair & other) const{
        return (fabs(current_val-other.current_val)<.0001 && (fabs(target_val-other.target_val)<.0001));
    }
};
struct  moat_val_pair{
    unordered_set<int > moat;
    double val;
};

// in namespace std defines hash functions for later use in unordered_map
namespace std {

  template <>
  struct hash<unordered_set<int> >
  {
    std::size_t operator()(const unordered_set<int>& set) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      size_t ret=0;
      for (auto it=set.begin(); it!=set.end(); it++){
          ret=ret^hash<int>() (*it);
      }
      return ret;
    }
  };
  
  template <>
  struct hash< node_pair >
  {
    std::size_t operator()(const node_pair& n_pr) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      
      return hash<int>()(n_pr.node1)^hash<int>()(n_pr.node2);
    }
  };
  
  template <>
  struct hash< node_j_pair >
  {
    std::size_t operator()(const node_j_pair& n_pr) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      
      return hash<int>()(n_pr.node)^hash<int>()(n_pr.j);
    }
  };
  
  template <>
  struct hash< edge_j_pair >
  {
    std::size_t operator()(const edge_j_pair& n_pr) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      
      return hash<int>()(n_pr.node1)^hash<int>()(n_pr.node2)^hash<int>()(n_pr.j);
    }
  };
  
    template <>
  struct hash< moat_j_pair >
  {
    std::size_t operator()(const moat_j_pair& n_pr) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      
      return hash< unordered_set<int > >()(n_pr.moat)^hash<int>()(n_pr.j);
    }
  };

} 


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

struct node_bool_pair{
    int node;
    bool bv;
    node_bool_pair(){
        node=0;
        bv=false;
    }
};

struct path{
    vector< node_pair > path_edges ; // set of edges in the path
    double cost;
    //more?
};


class PSteiner {
public:
    PSteiner();
    PSteiner(const PSteiner& orig);
    PSteiner(const PSInst& );
    virtual ~PSteiner();
     /* methods */
    const node_val_pair Find_dual_j_limit(int j, unordered_set<int> ActMoat1) ; // return the index of the node that would reach level-j limit first; covers case 1)
    const edge_val_pair Find_tight_j_edge_1(int j, unordered_set<int> ActMoat1);  // return the edge and value for case 2)
    const edge_val_pair Find_tight_j_edge_2(int j, unordered_set<int> ActMoat1, unordered_set<int> ActMoat2);  // return the edge and value for case 2)
    moat_val_pair Find_pnty_j_moat(int j, unordered_set<int> ActMoat1);   // not const because of initializing moat when used.
    void Consolidate(int j);
    void Dijkstra(const int s1, const int s2);  //this function modifies member F
    
    void DG_Dijkstra(int j ,const int s1, const int s2);  //this function modifies member F, used in Dual_Growth 
    
    void Dual_Growth(int j);
    
    void Update_dual_edges1(double incr, int j, unordered_set<int> ActMoat1);

    void Update_dual_edges2(double incr, int j, unordered_set<int> ActMoat1, unordered_set<int> ActMoat2);
    
    void Update_dual_nodes1(double incr, int j, unordered_set<int> ActMoat1);

    void Update_dual_pnty1(double incr, int j, unordered_set<int> ActMoat1);

    node_bool_pair Update_Consolidate_CarryOn(int j,int node);
    
    void Join_CC(int j);
    
    void Algo();
    
    bool NodeTight(node_val_pair, edge_val_pair, moat_val_pair );
    
    bool EdgeTight(node_val_pair, edge_val_pair, moat_val_pair );

    bool MoatTight(node_val_pair, edge_val_pair, moat_val_pair );
    
    void Bill2Pnty(unordered_set<int> this_moat);
    
    void print_PS(){
        bool edge_match = (Graph.size()==NumEdges);
        bool arriving_terms_match=(ArrTerminals.size()==ArrTerminals_Pnty.size())&& (ArrTerminals_Pnty.size()==NumArrTerminals)&& (NumArrTerminals==terms_pnty.size());
        cout<<"number of nodes = "<<NumVertices<<" match? " << (node_to_edges.size() == NumVertices)  <<endl;
        cout<<"number of edges = "<<NumEdges<<" match? "<< edge_match<<endl;
        cout<<"root index = "<< RootInd<<endl;
        cout<<"number of arriving terminals = "<< NumArrTerminals<<" match? "<< arriving_terms_match<<endl;
        cout<<"number of  = "<<NumVertices<<endl;
        
        cout<<"node_to_edges:"<<endl;
        for(int i=1; i<= NumVertices; i++){
            cout<< i<<":    ";
            for(auto it=node_to_edges[i].begin(); it!=node_to_edges[i].end(); it++){
                cout<<*it<<" ";
            }
            cout<<endl;
        }
        
        
    }
    
    void print_Algo_sol(){
        cout<<"bought edges:"<<endl;
        double total_conn=0;
        for(auto it=bought_edges.begin(); it!=bought_edges.end();it++){
            node_pair tp; tp.node1=min(it->node1, it->node2); tp.node2=max(it->node1, it->node2);
            total_conn+=(double)Graph[tp];
            cout<< it->node1<<" "<< it->node2 <<endl;
        }
        
        cout<<"total connection costs = "<<ConnCost<<endl<<endl; 
        cout<<"penalty terminals:"<<endl;
        double total_pnty=0;
        for(auto it=terms_paid_pnty.begin(); it!=terms_paid_pnty.end();it++){
            cout<< *it<< " ";
            //total_pnty+=terms_pnty[*it];
        }
        //cout<<"total penalty cost = "<< total_pnty<<endl<<endl;
        
        cout<<endl<<"Offline Optimal = "<< OptimalVal<< endl;
        //cout<<"Online Algo Sol = "<< total_conn+total_pnty<<endl;
    }
    

    
  
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
    
    
    /* Algorithm-related Variables */
    unordered_map <node_pair,  bool  > F; // set of purchased edges 
    unordered_map <edge_j_pair, double > dual_edge; 
    // tracks accumulated dual variable value for each edge and each level_j   -- case 1)
    //for each edge_j_pair, need have "node1 < node2" 
    
    unordered_map <node_j_pair, double > dual_j_node;
    // tracks accumulated dual variable value for each node at level j  --case 2), 3)
    unordered_map< moat_j_pair, double  > dual_j_pnty;
    // tracks accumulated penalty value for each level-j and moat in level-j
    
    
    //[discarded] vector <unordered_map < unordered_set <int>, double_val_pair > > moats_pnty; // tracks accumulated penalty of each level j moat

    
    unordered_map <int, double> terms_pnty; // maps node index to its penalty value, initialized in 
    vector <unordered_map < int, unordered_set<int> > > moats; // vector of moats, each corresponds to a level. 
                                                 //each is an unordered_map that maps a node to the moat it belongs to 
                                                 //use it to track \bar{F}^j  
    unordered_set <int>  previous_active_terms; // tracks all previously active terminals
    //vector< node_val_pair > bought_edges; //  tracks all edges bought, a.k.a. set F in paper
    
    int current_active_term;
    unordered_map < int, unordered_set<int>> ConnComp;// the set of terminals that are connected to the root
    
    unordered_set<int> terms_paid_pnty; // tracks all terms that paid pnty in Algo.
    vector<node_pair> bought_edges;// useful at all ????
    unordered_set<int> _A;
    unordered_set<int> _Aj;
    double ConnCost;
}; 




    


    
    


#endif	/* PSTEINER_H */

