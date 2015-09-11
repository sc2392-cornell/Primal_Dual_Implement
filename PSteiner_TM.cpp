/* 
 * File:   PSteiner_TM.cpp
 * Author: cheungsinshuen
 * 
 * Created on September 1, 2015, 10:52 AM
 */

#include "PSteiner_TM.h"
#include "PSteiner.h"

PSteiner_TM::PSteiner_TM() {
}

PSteiner_TM::PSteiner_TM(const PSteiner_TM& orig) {
}

PSteiner_TM::~PSteiner_TM() {
}



z_j_pair PSteiner_TM::TM_Dijkstra(int s1){// computes shortest paths from each node in Z to s1
    if (Z.size()==0 || Z.find(s1)!=Z.end()){
        cerr<<"Set Z is empty or Z contains i!"<<endl;
        z_j_pair tmp;
        return tmp;
    }
    int p_counter=0; //  count number of nodes in Z that has been processed.
    unordered_map<int, bool> Mark; ; // Marked == min_val is obtained
    unordered_map<int, int> Val; Val[s1]=0;// Val[i] is the dist between i and s1.  all initialized at 0 
    unordered_map<int, bool> FinVal; FinVal[s1]=true;  //  finite==true, infitite==false 
    unordered_map<int, int> TraceBack; // bring to the previous node in shortest path
    bool inf_dist=0;
    while(true){
        bool break_it=1;

        //finds the next node to explore
        int MinRec=1000000;
        int cn=0;// cn means 'current node'
        for(auto it=Val.begin(); it!=Val.end();it++){
            if(Mark[it->first]) continue; // if node has already obtained min_value, then move on
            if(MinRec > it->second){ // otherwise, update MinRec by its value
                MinRec=it->second; cn=it->first;// by the end of this loop, cn is the node with mininum value 
            }
        }
        //MinNd is the next to explore and mark;
        if(cn==0) {cout<<"-- TM_Dijkstra Inf. Dist: cn cannot be 0"<<endl;inf_dist=1;break; }
        //cout<<"-- TM_Dijkstra: cn="<<cn<<endl; 
        //if(cn==s2) break;

        Mark[cn]=true;// mark cn as 'already obtained min_value'   and update distmap accordingly
        
        if(cn!=s1){
            node_pair np_r; np_r.node1=min(cn, s1); np_r.node2=max(cn, s1);
            distmap[np_r]=Val[cn];
            F_distmap[np_r]=1;
        }
        // now: explore nbrs of cn
        
        if(Z.find(cn)!=Z.end()){ // 
            p_counter++;
            if (p_counter==Z.size()) break;
        }
        
        //if(p_counter==Z.size())   {cout<<"-- TM_Dijkstra: searched all Z members and cannot find a closest terminal"<<endl;inf_dist=1; break;}
        
        
        // update neighbors of cn
        unordered_set<int> my_neighbors;
        for(auto itr=node_to_edges[cn].begin(); itr!= node_to_edges[cn].end(); itr++){
            my_neighbors.insert(*itr);
        }
        
        
        for(auto itr=my_neighbors.begin(); itr!= my_neighbors.end(); itr++){
            if(Mark[*itr] || *itr==cn) continue;
            else{
                break_it=0;
                //skip if *itr is already marked
                //*itr is a neighbor that needs be updated
                node_pair np; np.node1=min(cn, *itr); np.node2=max(cn,*itr);
            
                int npdist=Graph[np];
            
                if(!FinVal[*itr]){ // if *itr has infinite value, update anyway (*itr doesnt correspond to a key in Val2Node yet)
                    Val[*itr]=Val[cn] + npdist;
                    FinVal[*itr]=true;
                    TraceBack[*itr]=cn;
                }
                else{// if *itr has a finite value, then update if current route is shorter
                    if(Val[*itr]> Val[cn]+npdist){
                        Val[*itr]=Val[cn]+npdist;
                        TraceBack[*itr]=cn;
                    }
                }
            }
        }
        if(break_it) break;
    }
    
    // find min dist guy
    if(p_counter==0) {
        z_j_pair pair;
        pair.z=RootInd;
        pair.j=100;
        pair.path.push_back(RootInd);
        cout<<"-- inf. dist. "<<endl;
        return pair;
    }
    
    
    
    int min_dist=100000;
    int min_dist_guy=0;
    for (auto it = Z.begin(); it!= Z.end(); it++){
        if(Mark[*it] && min_dist>Val[*it]){
            min_dist=Val[*it];
            min_dist_guy=*it;
        }
    }

    if(min_dist_guy==0){
        z_j_pair pair;
        pair.z=RootInd;
        pair.j=100;
        pair.path.push_back(RootInd);
        cout<<"-- inf. dist. "<<endl;
        return pair;
    }
        
    cout<<"-- closest conn. terminal="<<min_dist_guy<<endl;

    
    // add to X_j
    int log_dist_j=(int) floor(log(min_dist) );
    
    cout<<"-- j="<<log_dist_j<<endl;
    
    while(X.size()<log_dist_j){
        unordered_set<int> temp;
        X.push_back(temp);
    }
    
    X[log_dist_j-1].insert(min_dist_guy);
    
    z_j_pair pair;
    pair.z=min_dist_guy;
    pair.j=log_dist_j;
    
    vector<int> path;// z - - - s1, excluding s1.
    path.push_back(min_dist_guy);
    while(TraceBack[min_dist_guy]!=s1){
        path.push_back(TraceBack[min_dist_guy]);
        min_dist_guy=TraceBack[min_dist_guy];
    }
    
    pair.path=path;
    
    
    return pair;
}





unordered_set<int> PSteiner_TM::Find_W(int i, int j){
    //i is the index of arriving terminal
    unordered_set<int> W;
    if(X.size()<j){
        cout<<"-- Find_W: X size smaller than j="<<j<<endl;
        return W;
    }
    
    node_pair npr;
    for (auto it =X[j-1].begin(); it!=X[j-1].end(); it++){
        npr.node1=min(*it, i); npr.node2=max(*it, i);
        if(!F_distmap[npr]){
            cerr<<"-- Find_W : Can't find dist between "<<i<<" and "<< *it<<endl;
        }
        else{
            if (distmap[npr]<pow(2,j-1))  W.insert(*it);
        }
    }
    
    return W;



}// finds W_i 
    
void PSteiner_TM::Update_rho(int i, unordered_set<int> W, int j, vector<int> path){
    // i is the loc in ArrTerminals
    rho[ArrTerminals[i]]=0;
    double sum_W=0;
    for (auto it = W.begin(); it != W.end(); it++){
        sum_W +=rho[*it];
    }
    
    double beta=pow(2,j+1)-sum_W;
    if (beta<ArrTerminals_Pnty[i]){
    // if penalty is bigger, then make connection    
        rho[ArrTerminals[i]]=beta;
        //Z.insert(ArrTerminals[i]);
        
        // Z gets updated
        if(path.size()==0) {cerr<<"Update_rho Error: path has 0 size"<<endl;}
        for(int r=0; r< path.size(); r++){
            node_pair  np;
            int nd2=0;
            if(r==path.size()-1){nd2=ArrTerminals[i];}
            else{nd2=path[r+1];}
            Z.insert(nd2);
            np.node1=min(path[r],nd2);
            np.node2=max(path[r],nd2);
            
            // update purchasing info
            if(F[np]){
            //if np is already bought, then there must be something wrong    
                cerr<<"Update_rho Error: Edge "<<np.node1<<"-"<<np.node2<<" is already bought, can't be bought again!"<<endl;
                break;
            }
            else{
                cout<<"-- "<<np.node1<<"--"<<np.node2<<" gets connected"<<endl;
                ConnCost+=Graph[np];
                BoughtEdges.insert(np);
                F[np]=1;
            }
        }   
    }
    else{
    // if penalty is smaller, then pay penalty instead
        PntyCost+=ArrTerminals_Pnty[i];
        rho[i]=ArrTerminals_Pnty[i];
        PenTerms.insert(ArrTerminals[i]); 
        cout<<"-- "<< i<<" th terminal pays penalty "<<ArrTerminals_Pnty[i]<<endl;
    }



}// updates rho_i, and buy edge or pay penalty, accordingly
    