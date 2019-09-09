#ifndef __INTRAROUTE__MOV__EXCHANGE__H__
#define __INTRAROUTE__MOV__EXCHANGE__H__

#include <vector>
#include <list>
#include <algorithm>
#include "Data.hpp"

using namespace std;

class Exchange {

public:

    static int doMove(Data &data) {        
        
        int bestRoute = -1;
        list<int>::iterator bestit1, bestit2;
        int delta = 0;

        vector<list<int>> &sol = data.sol;

        for(int i = 0; i < sol.size(); ++i) {
            if(sol[i].size() <= 3) continue; //tem que haver pelomenos 2 pontos para fazer troca
        	
            int i1 = 2; 
            list<int>::iterator it1 = sol[i].begin(); ++it1;
            
            while(i1 < sol[i].size()) {
                
                int i2 = i1 + 1;
                if(i2 >= sol[i].size())break; //CARAI
                
                list<int>::iterator it2 = it1; ++it2;
                
                list<int>::iterator it1ant, it1pos, it2ant, it2pos;
                it1ant = it1; --it1ant;
                it1pos = it1; ++it1pos;
                it2ant = it2; --it2ant;
                it2pos = it2; ++it2pos;

                int temp_delta = 0;
                temp_delta -=  (data.distances[*it1ant][*it1] + data.distances[*it1][*it2] + data.distances[*it2][*it2pos]);
                temp_delta +=  (data.distances[*it1ant][*it2] + data.distances[*it2][*it1] + data.distances[*it1][*it2pos]);
               
               if(temp_delta < delta) {
                    bestRoute = i; 
                    bestit1   = it1;
                    bestit2   = it2;
                    delta     = temp_delta;
                }
                
                while(++i2 < sol[i].size()) {
                    ++it2;

                    it1ant = it1; --it1ant;
                    it1pos = it1; ++it1pos;
                    it2ant = it2; --it2ant;
                    it2pos = it2; ++it2pos;

                    temp_delta = 0;
                    
                    temp_delta -= (data.distances[*it1ant][*it1] + data.distances[*it1][*it1pos] + data.distances[*it2ant][*it2] + data.distances[*it2][*it2pos]);
                    temp_delta += (data.distances[*it1ant][*it2] + data.distances[*it2][*it1pos] + data.distances[*it2ant][*it1] + data.distances[*it1][*it2pos]);

                    if(temp_delta < delta) {
                        bestRoute = i;
                        bestit1   = it1;
                        bestit2   = it2;
                        delta     = temp_delta;
                    }
                }

                ++i1; ++it1;
            }

        }
        
        //encontrou movimento de melhora
        if(bestRoute != -1) {
            data.sol[bestRoute].insert(bestit1, *bestit2);
            data.sol[bestRoute].insert(bestit2, *bestit1);

            data.sol[bestRoute].erase(bestit1);
            data.sol[bestRoute].erase(bestit2);


        }

        return delta;
    }
        
};



#endif
