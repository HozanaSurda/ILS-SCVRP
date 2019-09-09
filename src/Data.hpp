#ifndef DATA_CVRP_H_
#define DATA_CVRP_H_

#include<vector>
#include <list>

using namespace std;

class Data {
public:
    //dados da instancia    
    int qtVehicle;
    int dimension;
    int capacity;
    vector< vector<int> > distances; 
    vector<int> demands;

    //dados dos movimentos
    vector< list<int> > sol;
    vector<int> capacities;
    
    int getSolValue() {
    	int val = 0;
    	for(int i = 0; i < sol.size(); ++i) {
    		int ant = 0;
        	list<int>::iterator it = sol[i].begin();
        	for (++it; it != sol[i].end(); ++it) {
        		val += distances[ant][*it];
        		ant = *it;
        	}
        }
        return val;
    }

    void printSol() {
    	for(int i = 0; i < qtVehicle; ++i) {
            printf("Route #%d(%d):", i + 1, capacities[i]);
            for(auto j : sol[i]) {
                printf(" %d", j + 1);
            }
            printf("\n");
        }
    }
    
};

#endif
