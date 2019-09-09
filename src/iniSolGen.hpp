#ifndef __INITIAL__SOLUTION__GENERATOR__H__
#define __INITIAL__SOLUTION__GENERATOR__H__

#include <vector>
#include <list>
#include <algorithm>
#include "Data.hpp"

using namespace std;

class Generator {

public:

    static void generateInitialSolution(Data &data) {        
        int k = data.qtVehicle;
        vector< list<int> > sol(k, list<int>() );
        vector<int> capacities(data.dimension, data.capacity);

        for(int i = 0; i < k; ++i) {
            //deposito comeca e termina todas as rotas            
            sol[i].push_back(0);
            sol[i].push_back(0);
        }

        //vou usar para construir as rotas ordenando pela demanda e atribuindo (heuristica bin packing)
        vector< pair<int, int> > points;
        for(int i = 0; i < data.dimension; ++i) {
            points.push_back({data.demands[i], i});   
        }
        sort(points.begin(), points.end());
        

        vector<bool> assigned(data.dimension, false);
        for(int i = 0; i < k; ++i) {
            for(auto &p : points) {
                if(assigned[p.second] == true) continue;
                if(capacities[i] >= p.first) {
                    capacities[i] -= p.first;    
                    assigned[p.second] = true;
                    list<int>::iterator it = sol[i].begin(); ++it;
                    sol[i].insert(it, p.second);
                }
            }
        }
        
        data.sol = sol;
        data.capacities = capacities;
        printf("\nGenerator():[INFO] Solucao gerada com FO %d\n", data.getSolValue());        
        data.printSol();
        
        for(int i = 0; i < data.dimension; ++i) {
            if(assigned[i] == false) {
                printf("Heuristica de Construcao Pereba nao alocou o ponto %d\n", i + 1);
            }
        }
    }
        
};



#endif
