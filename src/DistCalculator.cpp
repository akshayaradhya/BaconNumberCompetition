#include "../include/DistCalculator.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include<list>
#include<queue>
#include<fstream>
#include<sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include<cstring>

using namespace std;
std::string lPath;
uint64_t highest_actor = 2000000; 
uint64_t highest_movie = 1200000;
int64_t fin_count;

vector<uint64_t> *movie_map = new vector<uint64_t>[1200000];
vector<uint64_t> *actor_movie_map = new vector<uint64_t>[2000000];


void BFS_OneLevel(std::queue<uint64_t> &Q,std::vector<bool> &visited_act,std::vector<bool> &visited_mov,std::vector<uint64_t> &dist)
{
    auto Q_size = Q.size();
    for (int i =0; i< Q_size; ++i)
    {
        uint64_t top = Q.front();
        Q.pop();
        std::vector<uint64_t>& moviesOfTop = actor_movie_map[top]; //vector of movies the actor 'top' has been in
        auto currentDist = dist[top];
        //cout<<"Top is: "<<top<<endl;
        for (auto& movie : moviesOfTop) // replace all moviesOfTop[i] below with movie
        {
            //cout << moviesOfTop[i] << "Movies of top"  << endl;
            if(!visited_mov[movie])
            {
                //cout<<"Movies: "<<movie<<endl;
                visited_mov[movie] = true;
                std::vector<uint64_t>& actorsInMovie = movie_map[movie];
                for (auto& actor : actorsInMovie) //iterating over actors in movie[i]
                {
                    if(!visited_act[actor])
                    {
                        //cout<<"Actors: "<<actor<<endl;
                        Q.push(actor);
                        dist[actor] = currentDist + 1;
//                        prev_node[actor]=top;
                        visited_act[actor] = true;
                    }
                }
            }
        }
    }

}

int64_t BFS(uint64_t start, uint64_t goalnode)
{
    if (start == goalnode)
        return 0;

    //cout << "Alive" << endl;
    std::vector<bool> f_visited_mov(highest_movie, false);
    std::vector<bool> f_visited_act(highest_actor, false);
    std::vector<bool> b_visited_mov(highest_movie, false);
    std::vector<bool> b_visited_act(highest_actor, false);
//    std::vector<int64_t> b_prev_node(highest_actor, -1);
//    std::vector<int64_t> f_prev_node(highest_actor, -1);
    std::queue<uint64_t> f_Q;
    std::queue<uint64_t> b_Q;
    f_Q.push(start); //push starting actor in queue
    b_Q.push(goalnode); //push starting actor in queue

    //cout << "Alive" << endl;
    f_visited_act[start] = true;
    b_visited_act[goalnode] = true;
//    f_prev_node[start]=start;
//    b_prev_node[goalnode]=goalnode;
    ////cout << "Alive" << endl;
    //uint64_t* prev_node = new uint64_t[highest_actor];
    std::vector<uint64_t> f_dist(highest_actor, 0);
    std::vector<uint64_t> b_dist(highest_actor, 0);
    //vector<uint64_t> prev_node[num_act];
    //cout << "Alive" << endl; 
    //memset(prev_node,-1,highest_actor); // setting all values to -1
    //cout << "Alive after memset" << endl;
    // uint64_t flag = 0;
    // dist[start] = 0;
    while(!f_Q.empty() && !b_Q.empty())
    {
        BFS_OneLevel(f_Q,f_visited_act,f_visited_mov,f_dist);
        BFS_OneLevel(b_Q,b_visited_act,b_visited_mov,b_dist);
//        BFS_OneLevel(f_Q,f_visited_act,f_visited_mov,f_dist,f_prev_node);
//        BFS_OneLevel(b_Q,b_visited_act,b_visited_mov,b_dist,b_prev_node);
       // cout<<"BFS One done"<<endl;

        std::vector<int64_t> common_node;
        //int count=0;
        for (int64_t i =0; i<highest_actor;++i)
        {
            if (b_visited_act[i] && f_visited_act[i])
            {
                common_node.push_back(f_dist[i]+b_dist[i]);
                //cout<<"Common_Node found: "<<i<<" --- "<<f_dist[i]+b_dist[i]<<endl;
            }
        }
        if(!common_node.empty())
        {
            int64_t min=common_node[0];
            for (long long i : common_node) {
                if(i < min)
                    min= i;
            }
            /*int64_t nodecheck1=min; int64_t nodecheck2=min;
            int flag=1;
            while(flag!=0)
            {
                cout<<"Path starting from mid: "<<nodecheck1<<"  "<<nodecheck2<<endl;
                nodecheck1=f_prev_node[nodecheck1];
                nodecheck2=b_prev_node[nodecheck2];
                if(nodecheck1==start && nodecheck2 ==goalnode)
                    break;
            }*/
            return min;
        }
    }

    return -1;
}

DistCalculator::DistCalculator(std::string edgeListFile) {
    lPath = edgeListFile;

    //std::string lPath = "playedin.csv";	

	std::ifstream file(lPath);
    
    string dummyline;
    std::getline(file,dummyline);
        int64_t fir,sec;
    while(file >> fir 
    && file.ignore(std::numeric_limits<std::streamsize>::max(), ',')
    && file >> sec){
       movie_map[sec].push_back(fir);
	    actor_movie_map[fir].push_back(sec);
}
	





	
    //cout<<"File reading complete";
}


int64_t DistCalculator::dist(Node a, Node b)
{
   // TODO: implement distance calculation 
    if (a == b)
        return 0;
    else
        fin_count = BFS(a,b);
        
    return fin_count;
}
