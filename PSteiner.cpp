/* 
 * File:   PSteiner.cpp
 * Author: cheungsinshuen
 * 
 * Created on May 8, 2015, 3:11 PM
 */

#include "PSteiner.h"

PSteiner::PSteiner() { 
}

PSteiner::PSteiner(const PSteiner& orig) {
}

PSteiner::PSteiner(const PSInst& inst) {
    Graph=inst.Graph;
    ArrTerminals=inst.ArrTerminals;
    ArrTerminals_Pnty=inst.ArrTerminals_Pnty;// remark: this one is not really used directly
    NumVertices=inst.num_nodes; // number of vertices in graph
    NumEdges=inst.num_edges;// number of edges in Graph
    OptimalVal=inst.opt_val_offline; // Optimal value to offline problem 
    RootInd=inst.root_ind;
    NumArrTerminals=inst.num_arr_terms;
    for (int i=0; i< ArrTerminals.size();i++){// define terms_pnty for fast retrieval 
        terms_pnty[ArrTerminals[i]]=(double)ArrTerminals_Pnty[i]/2;
    }
    //define node_to_edges:
    node_to_edges=inst.node_to_edges;
    
    /*initialize ConnComp*/
    for(int i=1; i<= NumVertices; i++){
        unordered_set<int > temp; temp.insert(i);
        ConnComp[i]=temp;
        bought_tree[i]=temp;
    }
    ConnCost=0;PntyCost=0;
    
    
}



PSteiner::PSteiner(const Irreg_Input& inst) {
    Graph=inst.Graph;
    ArrTerminals=inst.ArrTerminals;
    ArrTerminals_Pnty=inst.ArrTerminals_Pnty;// remark: this one is not really used directly
    NumVertices=inst.num_nodes; // number of vertices in graph
    NumEdges=inst.num_edges;// number of edges in Graph
    OptimalVal=0; // Optimal value to offline problem  is left empty
    RootInd=inst.root_ind;
    NumArrTerminals=inst.num_arr_terms;
    for (int i=0; i< ArrTerminals.size();i++){// define terms_pnty for fast retrieval 
        terms_pnty[ArrTerminals[i]]=(double)ArrTerminals_Pnty[i]/2;
    }
    //define node_to_edges:
    node_to_edges=inst.node_to_edges;
    
    /*initialize ConnComp*/
    for(int i=1; i<= NumVertices; i++){
        unordered_set<int > temp; temp.insert(i);
        ConnComp[i]=temp;
        bought_tree[i]=temp;

    }
    ConnCost=0;PntyCost=0;
    
    
}


PSteiner::~PSteiner() {
}

/*GOOD*/
const node_val_pair PSteiner::Find_dual_j_limit(int j,unordered_set<int> ActMoat1){

    node_j_pair njp; njp.j=j;
    node_val_pair nvp; nvp.val=pow(2.0,j);
    //if(ActMoat1==ActMoat2) return nvp;
    
    for(auto it1=ActMoat1.begin(); it1!=ActMoat1.end(); it1++){
        
        njp.node=*it1;
        if( pow(2.0,j)- dual_j_node[njp]<nvp.val){
            nvp.val=pow(2.0,j)-dual_j_node[njp];
            nvp.node=*it1;
        }
        
    }
    
    return nvp;
}

    
 /*GOOD*/
const edge_val_pair PSteiner::Find_tight_j_edge_1(int j, unordered_set<int> ActMoat1){ // only 1 moat is active
    edge_j_pair tjp; tjp.j=j; 
    node_pair np;
    edge_val_pair min_pair; min_pair.node1=0; min_pair.node2=0; min_pair.val=pow(2.0,j);
    double temp_v=0;
    if(!ActMoat1.empty()){
        for(auto it1=ActMoat1.begin(); it1!=ActMoat1.end(); it1++){
            for(auto it2=node_to_edges[*it1].begin(); it2!=node_to_edges[*it1].end(); it2++){
                if( ActMoat1.find( *it2)!= ActMoat1.end()) continue;
                tjp.node1=min(*it1,*it2); tjp.node2=max(*it1,*it2);
                np.node1=tjp.node1; np.node2=tjp.node2;
 
                temp_v=((double)Graph[np] - dual_edge[tjp]);
                
                if(min_pair.val>temp_v) {// compair current gap with minimum gap so far;
                    min_pair.val=temp_v; min_pair.node1=np.node1; min_pair.node2=np.node2;
                }
            }
            
        }
    }
    return min_pair;
}    
    
    
/* GOOD */
const edge_val_pair PSteiner::Find_tight_j_edge_2(int j, unordered_set<int> ActMoat1, unordered_set<int> ActMoat2){
    edge_j_pair tjp; tjp.j=j; 
    node_pair np;
    edge_val_pair min_pair; min_pair.node1=0; min_pair.node2=0; min_pair.val=500000;
    double temp_v=0;
    if(ActMoat1!=ActMoat2){
        for(auto it1=ActMoat1.begin(); it1!=ActMoat1.end(); it1++){
            for(auto it2=node_to_edges[*it1].begin(); it2!=node_to_edges[*it1].end(); it2++){
                if(ActMoat1.find(*it2)!=ActMoat1.end()) continue; // if it1 and it2 are in same moat, then dont count it.
                tjp.node1=min(*it1,*it2); tjp.node2=max(*it1,*it2);
                np.node1=tjp.node1; np.node2=tjp.node2;
                if( ActMoat2.find(*it2)!=ActMoat2.end()){// if it1 and it2 in different active moats, then edge gap needs be discounted by 1/2
                    temp_v= ((double)Graph[np] - dual_edge[tjp])/2;
                }
                else{// else, just take the gap 
                    temp_v=((double)Graph[np] - dual_edge[tjp]);
                }
                if(min_pair.val>temp_v) {// compair current gap with minimum gap so far;
                    min_pair.val=temp_v; min_pair.node1=np.node1; min_pair.node2=np.node2;
                }
            }
            
        }
    }
    return min_pair;
}// return the edge and value for case 2)

/*GOOD*/
moat_val_pair PSteiner::Find_pnty_j_moat(int j, unordered_set<int> ActMoat1){
    moat_j_pair tp1; tp1.j=j; tp1.moat=ActMoat1; 
    moat_val_pair mvp; mvp.val=500000;
    
    
    //bool P_non_empty = !prev_terms.empty();
    double temp_pnty1=0; 
    for(auto it= ActMoat1.begin(); it!=ActMoat1.end(); it++){
        if(prev_terms.find(*it)!=prev_terms.end() ){ 
            temp_pnty1+= terms_pnty[*it];
        }
    }
    temp_pnty1-= dual_j_pnty[tp1]; 
    
    
    mvp.moat=ActMoat1; mvp.val=temp_pnty1;
    
    
    return mvp;
    
}


/*node_bool_pair PSteiner::Update_Consolidate_CarryOn(int j, int node){ // find if there is a previous_active_terms that belongs to the same level-j moat as node, but not connected in F
    node_bool_pair ret;
    if(previous_active_terms.empty()) return ret;
    if(node==0) return ret;
    for (auto it= (moats[j])[node].begin(); it!=(moats[j])[node].end(); it++ ){
        if(*it == node ) continue;
        if(previous_active_terms.find(*it)!=previous_active_terms.end() && ConnComp[node].find(*it)==ConnComp[node].end() ){
            ret.bv=true; ret.node=*it;
        }
    }
}
*/

//GOOD, except Dijkstra is not checked
void PSteiner::Consolidate(int j){
    bool Modified=true;
    while(Modified){
        Modified=false;
        for(auto it=_Aj.begin();it!=_Aj.end();it++){
            //if(*it==RootInd) continue;// it doesn't make sense to connect root to any previously active nodes 
            for(auto it2=_P[j].begin(); it2!=_P[j].end(); it2++){
                // it and it2 should be in same level j moat, but not in same ConnComp
                if(moats[j][*it].find(*it2)!= moats[j][*it].end() && ConnComp[*it].find(*it2)==ConnComp[*it].end()){
                    DG_Dijkstra(j,*it, *it2);
                    Modified=true;
                }
            }
        }
    }
    //update _Aj
    if(ConnComp[current_active_term].find(RootInd)!=ConnComp[current_active_term].end()){
        _Aj.clear();
        _A.clear();
    }

}
/*
    node_bool_pair carryonR,carryonC;
    
    
    // check RootInd
    carryonR = Update_Consolidate_CarryOn(j, RootInd);
    //check current_active_term
    carryonC = Update_Consolidate_CarryOn(j, current_active_term);
    
    while (carryonR.bv || carryonC.bv) {
        if(carryonR.bv){
            Dijkstra(RootInd, carryonR.node);
  
        }
        else{
            Dijkstra(current_active_term, carryonC.node);  
        }
        carryonR = Update_Consolidate_CarryOn(j, RootInd);
        carryonC = Update_Consolidate_CarryOn(j, current_active_term);  
    }  
*/

/*
void PSteiner::Join_CC(int j){
    if(!_Aj.empty()){
        unordered_set<int> rep_Aj=_Aj;
        for(auto it=rep_Aj.begin();it!=rep_Aj.end();it++){
            for(auto it2=_P[j].begin(); it2!=_P[j].end(); it2++){
                // it and it2 should be in same level j moat, but not in same ConnComp
                if(moats[j][*it].find(*it2)!= moats[j][*it].end() && ConnComp[*it].find(*it2)==ConnComp[*it].end()){
                    DG_Dijkstra(j,*it, *it2);
                }
            }
        }
    }
}
*/

/*
void PSteiner::Dijkstra(const int s1, const int s2){
    cout<<"______Invoked Dijkstra to connect "<<s1<<" to "<<s2<<endl;   
    
    unordered_map<int, bool> Mark; ; // Marked == min_val is obtained
    unordered_map<int, int> Val; Val[s1]=0;// all initialized at 0
    unordered_map<int, bool> FinVal; FinVal[s1]=true;  //  finite==true, infitite==false 
    unordered_map<int, int> TraceBack; // bring to the previous node in shortest path
    
    while(true){
        //finds the next node to explore
        int MinRec=1000000; int cn=0;
        for(auto it=Val.begin(); it!=Val.end();it++){
            if( Mark[it->first]==true) continue;
            if(MinRec > it->second || it->first !=0){
                MinRec=it->second; cn=it->first;
            }
        }
        //MinNd is the next to explore and mark;
        if(cn==0) {cerr<<"Wrong!"<<endl;break; }
        if(cn==s2) break;

        Mark[cn]=true;
        // explore nbrs of cn
        for(auto itr=node_to_edges[cn].begin(); itr!= node_to_edges[cn].end(); itr++){
            if( Mark[*itr]) continue;//skip if *itr is NOT in same moat or is already marked
            //*itr is a neighbor that needs be updated
            node_pair np; np.node1=min(cn, *itr); np.node2=max(cn,*itr);
            
            int npdist=0; // npdist is the true dist of np, incorporating the set of bought edges in F
            if(F[np]==false) npdist=Graph[np];
            
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
        
     
   
    int NOW=s2;
    
    while(true){
        node_pair np; np.node1=min(s2, TraceBack[s2]);np.node2=max(s2, TraceBack[s2]);
        if(!F[np]){
            //update F accordingly
            cout<<"Buy edge  "<<np.node1<<" -- "<<np.node2<<endl; 
            F[np]=true; 
            ConnCost+=(double)Graph[np]; 
            
            //Join ConnComp accordingly;
            unordered_set<int> NCC=ConnComp[np.node1];
            for(auto it=ConnComp[np.node2].begin();it!=ConnComp[np.node2].end();it++){
                NCC.insert(*it);
            }
            for(auto it=NCC.begin();it!=NCC.end();it++){
                ConnComp[*it]=NCC;
            }
            
        }
        //next in line
        NOW=TraceBack[NOW];
        if(NOW==s1) break;
    }
    
    
    
}
*/

void PSteiner::DG_Dijkstra(int j, const int s1, const int s2){
    cout<<"______Invoked DG_Dijkstra to connect "<<s1<<" to "<<s2<<endl;   
    if((moats[j])[s1].find(s2)==(moats[j])[s1].end() ) {cout<<" ERROR~ "<<endl;}
    cout<<"Moat Size= "<< (moats[j])[s1].size()<<" and "<< (moats[j])[s2].size();
    cout<<"Moat Contains" <<endl;
    for(auto it=(moats[j])[s1].begin();it!=(moats[j])[s1].end();it++){cout<<" "<<*it; }
    cout<<endl;
    unordered_map<int, bool> Mark; ; // Marked == min_val is obtained
    unordered_map<int, int> Val; Val[s1]=0;// all initialized at 0
    unordered_map<int, bool> FinVal; FinVal[s1]=true;  //  finite==true, infitite==false 
    unordered_map<int, int> TraceBack; // bring to the previous node in shortest path
    int path_len=0;
    while(true){
        //finds the next node to explore
        int MinRec=1000000;
        int cn=0;// cn means 'current node'
        for(auto it=Val.begin(); it!=Val.end();it++){
            if( Mark[it->first]==true) continue; // if node has already obtained min_value, then move on
            if(MinRec > it->second || it->first !=0){ // otherwise, update MinRec by its value
                MinRec=it->second; cn=it->first;// by the end of this loop, cn is the node with mininum value 
            }
        }
        //MinNd is the next to explore and mark;
        if(cn==0) {cerr<<"Wrong!"<<endl;break; }
        if(cn==s2) break;

        Mark[cn]=true;// mark cn as 'already obtained min_value'
        // now: explore nbrs of cn
        
        unordered_set<int> my_neighbors= bought_tree[cn];
        for(auto itr=node_to_edges[cn].begin(); itr!= node_to_edges[cn].end(); itr++){
            my_neighbors.insert(*itr);
        }
        
        
        
        for(auto itr=my_neighbors.begin(); itr!= my_neighbors.end(); itr++){
            if(moats[j][s1].find(*itr) ==moats[j][s1].end() || Mark[*itr] || *itr==cn) continue;//skip if *itr is NOT in same moat or is already marked
            //*itr is a neighbor that needs be updated
            node_pair np; np.node1=min(cn, *itr); np.node2=max(cn,*itr);
            
            int npdist=0; // npdist is the true dist of np, incorporating the set of bought edges in F
            if( my_neighbors.find(*itr)==my_neighbors.end()  ) npdist=Graph[np];
            
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
        
        /*
        for(auto Nei=Nodes.begin();Nei!=Nodes.end();Nei++){
            
            if(Mark[*Nei] || find(node_to_edges[NOW].begin(), node_to_edges[NOW].end(),*Nei)==node_to_edges[NOW].end()) continue;
            
            //construct node_pair of NOW and Nei
            node_pair np; np.node1=min(NOW, *Nei); np.node2=max(NOW,*Nei);
            
            double npdist=0; // npdist is the true dist of np, incorporating the set of bought edges in F
            if(F[np]==false) npdist=(double)Graph[np];
            
            if(!FinVal[*Nei]){ // if Nei has infinite value, update anyway
                Val[*Nei]=Val[NOW] + npdist;
                FinVal[*Nei]=true;
                TraceBack[*Nei]=NOW;
            }
            else{// if Nei has a finite value, then update if current route is shorter
                if(Val[*Nei]> Val[NOW]+npdist){
                    Val[*Nei]=Val[NOW]+npdist;
                    TraceBack[*Nei]=NOW;
                }
            }
            //Update MinVal by smallest Nei's
            if(MinVal>Val[*Nei]){
                MinVal=Val[*Nei];
                MinNei=*Nei;
            }
            
        }
        cout<<"MinNei = "<< MinNei<< "MinVal = "<< MinVal<<endl;
        //safety feature1
        if(MinNei==0){
            int Min=0;
            for(auto it=Nodes.begin(); it!=Nodes.end();it++){
                if(FinVal[*it]==true && Mark[*it]==false ){
                    if(MinVal>Val[*it])
                }
            }
        
        }
        //MinNei is the next NOW
        Mark[MinNei]=true; NOW=MinNei;
        if(MinNei==s2)    break;
    }*/
   
    int NOW=s2;
    
    while(true){
        node_pair np; np.node1=min(NOW, TraceBack[NOW]);np.node2=max(NOW, TraceBack[NOW]);
        if(bought_tree[NOW].find(TraceBack[NOW])==bought_tree[NOW].end()){
            //update F accordingly
            cout<<"Buy edge  "<<np.node1<<" -- "<<np.node2<<endl; 
            F[np]=true; 
            ConnCost+=(double)Graph[np]; 
            
            //Join ConnComp accordingly;
            unordered_set<int> NCC=ConnComp[np.node1];
            for(auto it=ConnComp[np.node2].begin();it!=ConnComp[np.node2].end();it++){
                NCC.insert(*it);
            }
            for(auto it=NCC.begin();it!=NCC.end();it++){
                ConnComp[*it]=NCC;
            }
            
            
            unordered_set<int> NBT=bought_tree[np.node1];
            for(auto it=bought_tree[np.node2].begin();it!=bought_tree[np.node2].end();it++){
                NBT.insert(*it);
            }
            for(auto it=NBT.begin();it!=NBT.end();it++){
                bought_tree[*it]=NBT;
            }
            
            
            
            //push to bought_edges:
            bought_edges.push_back(np);
            
        }
        else{
            cout<<"Existing edge  "<<np.node1<<" -- "<<np.node2<<endl; 
        }
        //next in line
        path_len++;
        NOW=TraceBack[NOW];
        if(NOW==s1) break;
    }
    cout<<"path lenth"<< path_len;
    
    
}



bool PSteiner::NodeTight(node_val_pair np, edge_val_pair ep, moat_val_pair mp){
    bool ret= (np.val<=ep.val) && (np.val<= mp.val);
    return ret;
}

bool PSteiner::EdgeTight(node_val_pair np, edge_val_pair ep, moat_val_pair mp){
    bool ret= (ep.val<np.val) && (ep.val<= mp.val);
    return ret;
}

bool PSteiner::MoatTight(node_val_pair np, edge_val_pair ep, moat_val_pair mp){
    bool ret= !(EdgeTight(np,ep,mp) || NodeTight(np,ep,mp)) ;
    return ret;
}

void PSteiner::Bill2Pnty(unordered_set<int> tmoat){
    for(auto it=tmoat.begin();it!=tmoat.end();it++){
        //if(prev_terms.find(*it)!=prev_terms.end())  terms_paid_pnty.insert(*it);
        if(*it==current_active_term || *it == RootInd)  terms_paid_pnty.insert(*it);
    }
}

void PSteiner::Dual_Growth(int level_j){
    
    unordered_set <int> ActMoatC=(moats[level_j])[current_active_term];
    unordered_set <int> ActMoatR=(moats[level_j])[RootInd];
    //_Aj1=_Aj;// _Aj1 is the set of active terms to be passed to next level. _Aj can be change in Dual_Growth, while _Aj1 does not change;
    //_A.clear(); // records active terms for next level                                                                        //therefore DO NOT use _Aj in for loops
    while(!_Aj.empty()){
        
        Consolidate(level_j);// connect components if possible
        
        //cout<<"size of _Aj = "<<_Aj.size()<<endl;
        
        if(_Aj.empty()) {break;} // if no active terms left, break the while loop
        
        else if(_Aj.size()==2){ // both are active at current level
//            cout<<"             Dual_Growth with active terms "<<current_active_term<<endl;
            //cout<<"[2 act]"<<endl;
            node_val_pair nvpC = Find_dual_j_limit(level_j, ActMoatC);//cout<<nvpC.val;// 
            node_val_pair nvpR = Find_dual_j_limit(level_j, ActMoatR);//cout<<"  "<<nvpR.val<<endl;// 
            node_val_pair nvp; if(nvpC.val<nvpR.val){nvp=nvpC;} else{nvp=nvpR;}
            
//            cout<<"CP6:"<<nvp.node<< "  "<<nvp.val<<endl;
            edge_val_pair evp=Find_tight_j_edge_2(level_j, ActMoatC, ActMoatR);//cout<<evp.val<<endl;
//            cout<<"CP7:"<<evp.node1<<" "<<evp.node2 <<" "<<evp.val<<endl;
            moat_val_pair mvp,mvpC,mvpR;
            mvpC=Find_pnty_j_moat(level_j, ActMoatC);//cout<<mvpC.val;
            mvpR=Find_pnty_j_moat(level_j, ActMoatR);//cout<<"  "<<mvpR.val<<endl;
            if(mvpR.val<=mvpC.val){mvp=mvpR;}
            else{mvp=mvpC;}
//            cout<<"CP8:"<<mvp.moat.size()<< "  "<<mvp.val<<endl;            
            double incr= min(nvp.val,min(evp.val, mvp.val)); // increment over y_S^j for moat S containing current_active_term
//            cout<<"             Dual_Growth: incr = "<<incr<<endl;
            /* UPDATE dual records */
            Update_dual_edges2(incr, level_j, ActMoatC,ActMoatR);
    
            Update_dual_nodes1(incr, level_j, ActMoatC);
            Update_dual_nodes1(incr,level_j,ActMoatR);

            Update_dual_pnty1(incr, level_j, ActMoatC);
            Update_dual_pnty1(incr, level_j, ActMoatR);
            
            /*UPDATE boolean values */
            
            /*case: node become inactive*/
            if(NodeTight(nvp,evp,mvp)) {//node tight: erase corresponding active term from _Aj
                //cout<<"Node "<< nvp.node<<" Tight"<<endl;
                for(auto it=moats[level_j][nvp.node].begin();it!=moats[level_j][nvp.node].end();it++){
                    _Aj.erase(*it);
                }
                //update _A
                if(ActMoatC.find(nvp.node)!=ActMoatC.end()) _A.insert(current_active_term);
                else {_A.insert(RootInd);}
                /*Update structures: deactivate all nodes in this active moat */
                /* no structure to be updated*/
            }
            /*case: moat (penalty) becomes tight*/
            else if(MoatTight(nvp,evp,mvp)) {
                //cout<<"Moat Tight --";
                /*for(auto it=(mvp.moat).begin();it!=(mvp.moat).end();it++){cout<<*it;}
                for(auto it=(ActMoatR).begin();it!=(ActMoatR).end();it++){cout<<*it;}
                cout<<endl;*/
                if(mvp.moat==ActMoatC){ // moat tight: erase corresponding active term from _Aj
                    //_A.insert(current_active_term); 
                    _Aj.erase(current_active_term); 
                    //cout<<"current_term is set inactive"<<endl;
                    Bill2Pnty(mvp.moat);
                }
                else{
                    //_A.insert(RootInd); 
                    _Aj.erase(RootInd); 
                    //cout<<"root is set inactive"<<endl;
                    Bill2Pnty(mvp.moat);
                }
                /*update structure*/
                /* no structure to be updated*/
            }
            /* case: edge is tight, do not make any boolean variable false */
            else{
                //cout<<"Edge Tight "<< evp.node1<<" -- "<<evp.node2 <<endl;

                /*Update structure*/
                unordered_set<int> nmoat=(moats[level_j])[evp.node1];
                for(auto it=(moats[level_j])[evp.node2].begin();it!=(moats[level_j])[evp.node2].end();it++){
                    nmoat.insert(*it);
                }
                
                for(auto it=nmoat.begin();it!=nmoat.end();it++){
                    (moats[level_j])[*it] = nmoat;
                }
                /*Combined two moats that contain the two endpoints of the edge*/
                moat_j_pair mjp; mjp.j=level_j; mjp.moat=nmoat; 
                moat_j_pair mjp1; mjp1.j=level_j; mjp1.moat=(moats[level_j])[evp.node1]; 
                moat_j_pair mjp2; mjp2.j=level_j;mjp2.moat=(moats[level_j])[evp.node2];
                /*Update dual_j_pnty since there are colliding moats*/
                dual_j_pnty[mjp]=dual_j_pnty[mjp1]+dual_j_pnty[mjp2];
                
                
                
            }

            
        }
        else if(_Aj.size()==1){// the case of only one moat being active 
            
            //int Term=0; 
            unordered_set<int> ActMoatT;
            if(_Aj.find(current_active_term)!=_Aj.end()) ActMoatT=ActMoatC;
            else ActMoatT=ActMoatR;
            //cout<<"[1 act] =" <<endl;
            node_val_pair nvp =Find_dual_j_limit(level_j, ActMoatT);// 
            edge_val_pair evp=Find_tight_j_edge_1(level_j, ActMoatT);//
            moat_val_pair mvp=Find_pnty_j_moat(level_j, ActMoatT);
            
            //cout<< nvp.val<<"  "<< evp.val<<"  "<<mvp.val<<endl;
            
            /*If node is tight*/
            double incr= min(nvp.val,min(evp.val, mvp.val));
            
            Update_dual_edges1(incr, level_j, ActMoatT);
    
            Update_dual_nodes1(incr,level_j,ActMoatT);

            Update_dual_pnty1(incr, level_j, ActMoatT);
            
            if(NodeTight(nvp,evp,mvp)) {// node tight: erase corresponding active node from _Aj
                //cout<<"Node " << nvp.node<<" Tight"<<endl;
                for(auto it=moats[level_j][nvp.node].begin();it!=moats[level_j][nvp.node].end();it++){
                    _Aj.erase(*it);
                }
                //update _A
                if(ActMoatC.find(nvp.node)!=ActMoatC.end()) _A.insert(current_active_term);
                else {_A.insert(RootInd);}
            }
            else if(MoatTight(nvp,evp,mvp)) {// moat tight: erase corresponding active node from _Aj
                //cout<<"Moat Tight"<<endl;
                if(mvp.moat==ActMoatC){
                    //_A.insert(current_active_term);
                    _Aj.erase(current_active_term);                   
                    //cout<<"current_term is set inactive"<<endl;
                    Bill2Pnty(ActMoatC);
                }
                if(mvp.moat==ActMoatR){
                    //_A.insert(RootInd);
                    _Aj.erase(RootInd);
                    //cout<<"Root is set inactive"<<endl;
                    Bill2Pnty(ActMoatR);
                }
                /*update structure*/
                /* no structure to be updated*/
            }
            else{// edge tight: erase NO terminal from _Aj, updated moat structure instead
                /*Update structure*/
                //cout<<"Edge Tight"<<evp.node1<<" -- "<<evp.node2<<endl;
                unordered_set<int> nmoat=(moats[level_j])[evp.node1];
                for(auto it=(moats[level_j])[evp.node2].begin();it!=(moats[level_j])[evp.node2].end();it++){
                    nmoat.insert(*it);
                }
                //cout<<"--  nmoat size="<<nmoat.size()<<endl;
                for(auto it=nmoat.begin();it!=nmoat.end();it++){
                    (moats[level_j])[*it] = nmoat;
                }
                /*Combined two moats that contain the two endpoints of the edge*/
                moat_j_pair mjp; mjp.j=level_j; mjp.moat=nmoat; 
                moat_j_pair mjp1; mjp1.j=level_j; mjp1.moat=(moats[level_j])[evp.node1]; 
                moat_j_pair mjp2; mjp2.j=level_j;mjp2.moat=(moats[level_j])[evp.node2];
                /*Update dual_j_pnty since there are colliding moats*/
                dual_j_pnty[mjp]=dual_j_pnty[mjp1]+dual_j_pnty[mjp2];
            }
            
            
            
        }
        ActMoatC=(moats[level_j])[current_active_term];
        ActMoatR=(moats[level_j])[RootInd];
    }
    
    
}


/* update by adding incr to all relevant edges in "unordered_map <edge_j_pair, double > dual_edge" in level-j */

void PSteiner::Update_dual_edges1(double incr, int j, unordered_set<int> ActMoat1){
    if(!ActMoat1.empty()){
        edge_j_pair ejp; ejp.j=j;
        for(auto it1=ActMoat1.begin(); it1!=ActMoat1.end(); it1++ ){
            for(auto it2=node_to_edges[*it1].begin(); it2!=node_to_edges[*it1].end();it2++){
                if(ActMoat1.find(*it2)!=ActMoat1.end()) continue;// if *it1 and *it2 are in same moat, then not to increase on the edge
                //////
/*check error*/ if(*it1==*it2){cout<<"ERRRRRor!"<<endl; break;}
                ejp.node1=min(*it1,*it2); ejp.node2=max(*it1,*it2);
                dual_edge[ejp]+=incr;
            }
        }
        
    }
}

void PSteiner::Update_dual_edges2(double incr, int j, unordered_set<int> ActMoat1, unordered_set<int> ActMoat2){
    if(ActMoat2!=ActMoat1 && incr>0.0001){
        edge_j_pair ejp; ejp.j=j;
        for(auto it1=ActMoat1.begin(); it1!=ActMoat1.end(); it1++ ){
            for(auto it2=node_to_edges[*it1].begin(); it2!=node_to_edges[*it1].end();it2++){
                if(ActMoat1.find(*it2)!=ActMoat1.end()) continue;// if *it1 and *it2 are in same moat, then not to increase on the edge
                //////
/*check error*/ if(*it1==*it2){cout<<"ERRRRRor!"<<endl; break;}
                ejp.node1=min(*it1,*it2); ejp.node2=max(*it1,*it2);
                dual_edge[ejp]+=incr;
            }
        }
        for(auto it1=ActMoat2.begin(); it1!=ActMoat2.end(); it1++ ){
            for(auto it2=node_to_edges[*it1].begin(); it2!=node_to_edges[*it1].end();it2++){
                if(ActMoat1.find(*it2)!=ActMoat2.end()) continue;// if *it1 and *it2 are in same moat, then not to increase on the edge
                //////
/*check error*/ if(*it1==*it2){cout<<"ERRRRRor!"<<endl; break;}
                ejp.node1=min(*it1,*it2); ejp.node2=max(*it1,*it2);
                dual_edge[ejp]+=incr;
            }
        }   
    }
}

void PSteiner::Update_dual_nodes1(double incr, int j, unordered_set<int> ActMoat1){
    node_j_pair njp; njp.j=j;
    for(auto it= ActMoat1.begin(); it!=ActMoat1.end();it++){
        njp.node=*it;
        dual_j_node[njp]+=incr;
    }
}




void PSteiner::Update_dual_pnty1(double incr, int j, unordered_set<int> ActMoat1){
    moat_j_pair mjp; mjp.moat=ActMoat1; mjp.j=j;
    dual_j_pnty[mjp]+=incr;

}

void PSteiner::Algo(){
    prev_terms.insert(RootInd);
    
    for (int ind=0; ind< ArrTerminals.size(); ind++){
        current_active_term=ArrTerminals[ind];
        prev_terms.insert(current_active_term);
        
        cout<<"_________________________New Terminal Arrives: "<< current_active_term<<endl;
        terms_pnty[RootInd]+=terms_pnty[current_active_term];
        _Aj.clear();
        _Aj.insert(RootInd); _Aj.insert(current_active_term);
         

        for (int level_j=0;level_j<=100; level_j++ ){
            if(_Aj.empty()) break;  
            _A.clear();

            //update _P[j]
            for(auto it=_Aj.begin(); it!=_Aj.end();it++){
                _P[level_j].insert(*it);
            }
            
            

            //cout<<"______________Invoke level_"<<level_j<<"_____________"<<endl;

            // Prevent Segmentation Fault: enlarge size of moats if needed
            if(moats.size()<level_j+1) {
                unordered_map<int, unordered_set <int> > temp;
                moats.push_back(temp);
            }

            // Updating \bar F^j  is done whenever F is updated 
            for(int i=1; i<=NumVertices; i++){
                for(auto it=ConnComp[i].begin(); it!=ConnComp[i].end(); it++){
                    (moats[level_j])[i].insert(*it);
                }
            }

            // End Updating*

            //Consolidate step: modifies F  if consolidate_done==true 

            
          /*  Consolidate(level_j);
            cout<<"Consolidate Done"<<endl;
            if(_Aj.empty()) break;           
          */
            //_Aj1.clear(); // clear the set of active terms for next round
            Dual_Growth(level_j);
            //cout<<"Dual_Growth Done"<<endl;
            //_Aj.clear();
            //update active terms for next round
            //if(moats[level_j][current_active_term].find(RootInd)!= moats[level_j][current_active_term].end()){
            //    _Aj.insert(current_active_term); _Aj.insert(RootInd);
            //}
            _Aj=_A;
            
        }
        if(!terms_paid_pnty.empty() && terms_paid_pnty.find(current_active_term)!=terms_paid_pnty.end()){
            PntyCost+=terms_pnty[current_active_term]*2;
        }
        ConnCostList.push_back(ConnCost);
        PntyCostList.push_back(PntyCost);
        //previous_active_terms.insert(current_active_term);
        
    }
    
}

vector<int> perm_vec_read_in(string arg){
    vector<int> ret;
    ifstream infile(arg);
    if (infile){
        string s;
        if (!getline( infile, s )) {cerr<<"something is wrong"; }
        string tmp_str;
        for(int i=0; i<s.length();i++){
            if(s[i]>='0' && s[i]<='9') {
                tmp_str.push_back(s[i]); 
                if(i+1==s.length() || (s[i+1]<'0' ||s[i+1]>'9')){
                    int tmp_int=stoi(tmp_str);
                    ret.push_back(tmp_int);
                    tmp_str.clear();
                }
            }
        }
    }
    return ret;


}

Irreg_Input irreg_read_in(string arg){
    Irreg_Input IRinst;
    IRinst.root_ind=1;
    IRinst.num_arr_terms=0;
    ifstream infile(arg);
    if (infile)
    {
        //cout<<"in loop"<<endl;
        string s;
        if (!getline( infile, s )) {cerr<<"something is wrong"; } //skip first line
        if (!getline( infile, s )){cerr<<"something is wrong"; } //skip second line
        if (!getline( infile, s )){cerr<<"something is wrong"; } // start from third line:
        //// start reading in nodes ///
        //cout<<"s len = "<<s.length()<<endl;
        while(infile){
            i_node t_i_node; 
            int s_pos = 0;
            string temp_str;// stores the current string of interest
            int str_counter=0;
            while (s_pos+1 <= s.length()) { // skip all spaces and read in node_ind, node_v, node_h values;
                if (s[s_pos] <'0' || s[s_pos]>'9') {
                    //cout<<"space"<<endl;
                } else {// current s[s_pos] is not a space
                    temp_str.push_back(s[s_pos]);
                    if ( s.length()==s_pos+1 ||  (s[s_pos+1] <'0' || s[s_pos+1]>'9')  ) { // end of current string. need to update node info accordingly
                        int t_int=stoi(temp_str);
                        //cout<< t_int<<endl;
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
                //cout<<"s_pos="<<s_pos<<endl;
                s_pos++;
                
            }
            //cout<<"read in node: "<<t_i_node.index<<"  "<<t_i_node.v<<"  "<<t_i_node.h<<"  "<<t_i_node.weight<<endl;
            if(t_i_node.weight!=0) {
                IRinst.ArrTerminals.push_back(t_i_node.index); 
                IRinst.ArrTerminals_Pnty.push_back(t_i_node.weight);
            }
            IRinst.num_nodes++;
            IRinst.Nodes.push_back(t_i_node);// add node info to IRinst.Nodes
            if (!getline( infile, s )) break;//get a new line;
            if(s[0]=='l') break;    
        }
        //cout<<"node reading-in done. Node number is "<< IRinst.num_nodes<<endl;
        
        //construct node_to_edges;
        vector<int> temp;
        for(int i=1; i<= IRinst.num_nodes;i++){
            IRinst.node_to_edges[i]=temp;
        }
        
        
        if(s[0]!='l') {cerr<<"something is wrong____ cant find letter l"; }
        if (!getline( infile, s )){cerr<<"something is wrong___cant get next line"; } //get a new line;
        if (!getline( infile, s )){cerr<<"something is wrong___cant get next line"; } //get a new line; start with this line
        // read in the edges' info
        while(infile){            
            i_edge t_i_edge;
            
            int s_pos = 0;
            string temp_str;// stores the current string of interest
            int str_counter=0;
            while (s_pos+1 <= s.length()) { // skip all spaces and read in node_ind, node_v, node_h values;
                if (s[s_pos] <'0' || s[s_pos]>'9') {
                    //do nothing
                } else {// current s[s_pos] is not a space
                    temp_str.push_back(s[s_pos]);
                    if ( s.length()==s_pos+1 ||  s[s_pos+1]  <'0' || s[s_pos+1]>'9') { // end of current string. need to update node info accordingly
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
                cerr<<"something went wrong!___edge info incorrect"<<endl;
                for (auto it =s.begin(); it!=s.end();it++)
                    cout<<*it;
                cout<<endl;            
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

            if(!getline( infile, s )) break;//get a new line;
            if(s[0]=='r') break;    
        }
        //cout<<"edge reading-in done, edge number is "<<IRinst.num_edges<<endl;
        //cout<<"Arriving term number is "<<IRinst.ArrTerminals.size()<<endl;
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

Irreg_Input irreg_read_in_Alter(string arg){
    Irreg_Input IRinst;
    IRinst.root_ind=1;
    IRinst.num_arr_terms=0;
    ifstream infile(arg);
    if (infile)
    {
        //cout<<"in loop"<<endl;
        string s;
        if (!getline( infile, s )) {cerr<<"something is wrong"; } //skip first line
        if (!getline( infile, s )){cerr<<"something is wrong"; } //skip second line
        if (!getline( infile, s )){cerr<<"something is wrong"; } // start from third line:
        //// start reading in nodes ///
        //cout<<"s len = "<<s.length()<<endl;
        while(infile){
            i_node t_i_node; 
            int s_pos = 0;
            string temp_str;// stores the current string of interest
            int str_counter=0;
            while (s_pos+1 <= s.length()) { // skip all spaces and read in node_ind, node_v, node_h values;
                if (s[s_pos] <'0' || s[s_pos]>'9') {
                    //cout<<"space"<<endl;
                } else {// current s[s_pos] is not a space
                    temp_str.push_back(s[s_pos]);
                    if ( s.length()==s_pos+1 || (s[s_pos+1] <'0' || s[s_pos+1]>'9') ) { // end of current string. need to update node info accordingly
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
                //cout<<"s_pos="<<s_pos<<endl;
                s_pos++;
                
            }
            //cout<<"read in node: "<<t_i_node.index<<"  "<<t_i_node.v<<"  "<<t_i_node.h<<"  "<<t_i_node.weight<<endl;
            if(t_i_node.weight!=0) {
                IRinst.ArrTerminals.push_back(t_i_node.index); 
                IRinst.ArrTerminals_Pnty.push_back(t_i_node.weight);
            }
            IRinst.num_nodes++;
            IRinst.Nodes.push_back(t_i_node);// add node info to IRinst.Nodes
            if (!getline( infile, s )) break;//get a new line;
            if(s[0]=='l') break;    
        }
        //cout<<"node reading-in done. Node number is "<< IRinst.num_nodes<<endl;
        
        //construct node_to_edges;
        vector<int> temp;
        for(int i=1; i<= IRinst.num_nodes;i++){
            IRinst.node_to_edges[i]=temp;
        }
        
        
        if(s[0]!='l') {cerr<<"something is wrong"; }
        //if (!getline( infile, s )){cerr<<"something is wrong"; } //get a new line;
        if (!getline( infile, s )){cerr<<"something is wrong"; } //get a new line; start with this line
        // read in the edges' info
        while(infile){            
            i_edge t_i_edge;
            
            int s_pos = 0;
            string temp_str;// stores the current string of interest
            int str_counter=0;
            while (s_pos+1 <= s.length()) { // skip all spaces and read in node_ind, node_v, node_h values;
                if (s[s_pos] <'0' || s[s_pos]>'9') {
                    //do nothing
                } else {// current s[s_pos] is not a space
                    temp_str.push_back(s[s_pos]);
                    if ( s.length()==s_pos+1 ||  (s[s_pos+1] <'0' || s[s_pos+1]>'9')) { // end of current string. need to update node info accordingly
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
                cout<< t_i_edge.len <<"  "<<t_i_edge.x<<"  "<<t_i_edge.y<<endl;
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

            if(!getline( infile, s )) break;//get a new line;
            if(s[0]=='r') break;    
        }
        //cout<<"edge reading-in done."<<endl;
        
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