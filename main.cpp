/* 
 * File:   main.cpp
 * Author: cheungsinshuen
 *
 * Created on May 5, 2015, 5:09 PM
 */

#include"PSteiner.h"
#include"reg_input.h"
//#include"irreg_input.h"
//#include"boost/graph/adjacency_matrix.hpp"
using namespace std;
//using std::numeric_limits;
/*
 * 
 */


void test_irreg_read_in(){

    Irreg_Input IR= irreg_read_in("k100.8/K100.8_R1_online_14");
    cout<<"num_nodes="<<IR.num_nodes<<endl;
    cout<<"num_edges="<<IR.num_edges<<endl;
    cout<<"PntyTermSize="<<IR.ArrTerminals.size()<<endl;
    for (int i=1; i<= IR.num_nodes;i++){
        cout<<"neighboring nodes of "<<i<<" : ";
        for (auto it=IR.node_to_edges[i].begin(); it!=IR.node_to_edges[i].end(); it++){
            cout<< *it<<"  ";
        }
        cout<<endl;
    }
    cout<<"PntyTerms are: ";
    for (auto it=IR.ArrTerminals.begin(); it!=IR.ArrTerminals.end();it++){
        cout<<*it<<" ";
    }        
    cout<<endl; 
}


void test_irreg_read_in_Alter(){

    Irreg_Input IR= irreg_read_in_Alter("reduced_inputs/K100.con.red_online_1");
    cout<<"num_nodes="<<IR.num_nodes<<endl;
    cout<<"num_edges="<<IR.num_edges<<endl;
    cout<<"PntyTermSize="<<IR.ArrTerminals.size()<<endl;
    for (int i=1; i<= IR.num_nodes;i++){
        cout<<"neighboring nodes of "<<i<<" : ";
        for (auto it=IR.node_to_edges[i].begin(); it!=IR.node_to_edges[i].end(); it++){
            cout<< *it<<"  ";
        }
        cout<<endl;
    }
    cout<<"PntyTerms are: ";
    for (auto it=IR.ArrTerminals.begin(); it!=IR.ArrTerminals.end();it++){
        cout<<*it<<" ";
    }        
    cout<<endl; 
}






vector<double> irreg_algo_impl(string irreg_fname, string perm_fname){
    Irreg_Input IR= irreg_read_in(irreg_fname);
    PSteiner PS(IR);
    
    vector<int> perm=perm_vec_read_in( perm_fname);
    for(auto it=perm.begin(); it!=perm.end();it++){
        cout<<*it<<" ";
    }
    PS.Modify_Arr_Perm(perm);    
    PS.Algo();
    PS.print_PS();
    PS.print_Algo_sol();
    PS.print_cost_lists();
    return PS.Out_sol();
} 
/*
void process_irreg_from_flist(){
    vector< vector<double> > Sol;
    ifstream infile("IrregFilesNames.txt");
    
    while(infile){
        cout<<"infile now"<<endl;
        string s;
        if(!getline(infile,s)) break;
        cout<<"-------------------------------------------------------computes for file "<< s<<endl;
        Sol.push_back(irreg_algo_impl(s, ));
    }
    
    cout<<endl;
    cout<<"=================Output Begin======================"<<endl;
    cout.precision(15);
    for (auto it= Sol.begin(); it!=Sol.end();it++){
        for (auto it2=it->begin(); it2!=it->end();it2++){
            cout<<*it2<< "   ";
        }
        cout<<endl;
    }

}

*/

vector<int> get_perm_from_file(string arg, int ifile_number){
    vector<int> perm;
    string FNamePref=arg;
    string FName;

    for(int i=1; i<=ifile_number; i++){
        FName=FNamePref;
        stringstream ss;
        ss << i;
        string str = ss.str();
        FName.append(str);
        
        //cout<<FName<<endl;
        
        Irreg_Input IR= irreg_read_in(FName);
        
        cout<<IR.ArrTerminals.size()<<endl;
        int new_arr=0;
        
        
        if(perm.empty()){
            if(IR.ArrTerminals_Pnty[0]>IR.ArrTerminals_Pnty[1]){
                perm.push_back(IR.ArrTerminals[0]);
                perm.push_back(IR.ArrTerminals[1]);
            }
            else{
                perm.push_back(IR.ArrTerminals[1]);
                perm.push_back(IR.ArrTerminals[0]);
            }

        }            
        else {
            for(auto it = IR.ArrTerminals.begin(); it!=IR.ArrTerminals.end(); it++){
                if(find(perm.begin(), perm.end(), *it)==perm.end()){
                    new_arr=*it;
                }
            }
            if(new_arr!=0){
                perm.push_back(new_arr);
            }
        }
    }
    return perm;
    

}



int main(int argc, char** argv) {
    
    
    
    //process_all_regular_data();
    
    //avector<double> ans=irreg_algo_impl("k100.8/K100.8_R1_online_14","K100.8_R1_online_permutation");
 
    //cout<< ans[0]<< "  " << ans[1]<<endl;
    
    //test_irreg_read_in_Alter();
    
    
    Irreg_Input IR= irreg_read_in(argv[1]);
    PSteiner PS(IR);
    PS.No_Perm();
    PS.Algo();
    PS.print_PS();
    PS.print_Algo_sol();
    //PS.print_cost_lists();
    cout.precision(15);
    vector<double> ans=PS.Out_sol();
    cout<< ans[1]<<endl;
    
    
    
    /* this part is used to generate figure-6 online cases
    vector <int> perm=get_perm_from_file("k100.8/k100.8_R1_online_", 14);
    for(auto it = perm.begin(); it!=perm.end(); it++){
        cout<< *it<<" ";
    }
    cout<<endl;
    cout<<"Size = "<<perm.size()<<endl;
    
    
    Irreg_Input IR= irreg_read_in_Alter("k100.8/k100.8_R1_online_13");
    PSteiner PS(IR);
    
    vector<int> newperm;
    for(int i=0; i<14;i++)
        newperm.push_back(perm[i]);
    
    
    //vector<int> perm=perm_vec_read_in( perm_fname);
    
    PS.Modify_Arr_Perm(newperm);    
    PS.Algo();
    PS.print_PS();
    PS.print_Algo_sol();
    PS.print_cost_lists();
    */
    /*
    vector <int> perm=get_perm_from_file("reduced_inputs/P400.con.red_online_",79);
    for(auto it = perm.begin(); it!=perm.end(); it++){
        cout<< *it<<" ";
    }
    cout<<endl;
    cout<<"Size = "<<perm.size()<<endl;
    
    
    Irreg_Input IR= irreg_read_in_Alter("reduced_inputs/P400.con.red_online_79");
    PSteiner PS(IR);
    
    
    
    
    //vector<int> perm=perm_vec_read_in( perm_fname);
    
    PS.Modify_Arr_Perm(perm);    
    PS.Algo();
    PS.print_PS();
    PS.print_Algo_sol();
    PS.print_cost_lists();
    */
    
    
    
    //test_irreg_read_in_Alter();
    
    //process_irreg_from_flist();
    //process_all_regular_data();
    
    //test_irreg_read_in();
    //vector <double > ret=irreg_algo_impl("k100.8/K100.8_R2_online_14", "k100.8/K100.8_R2_online_permutation");
    //vector <double > ret=irreg_algo_impl("k100.8/K100.8_R2_online_14", "k100.8/K100.8_R2_online_permutation");
    //vector <double > ret=irreg_algo_impl("k200/K200_R2_online_32", "k200/K200_R2_online_permutation");
    //vector <double > ret=irreg_algo_impl("k200/K200_R1_online_32", "k200/K200_R1_online_permutation");
    
    
    //cout<< ret[0]<< " " << ret[1]<<endl;
    return 0;
}
