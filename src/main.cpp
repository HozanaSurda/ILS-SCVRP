#include <cstdio>
#include <random>
#include <chrono>
#include <algorithm>
#include "Data.hpp" //armazena os dados das instancias e tambem dos movimentos
#include "DataReader.hpp" //funcao ParseDataset que le a instancia
#include "iniSolGen.hpp"
#include "intraExchange.hpp"


void VND(Data &d) {
	
	vector<int> moves;
	moves.push_back(1);
	//moves.push_back(2);
	//moves.push_back(3);
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(moves.begin(), moves.end(), std::default_random_engine(seed));

	for(int i = 0; i < moves.size(); ) {
		int delta = 0; //diferenca que vai fazer na FO se aplicado o movimento
		if     (moves[i] == 1) delta = Exchange::doMove(d);
		//else if(moves[i] == 2) delta = Opt2::doMove(d);
		//else if(moves[i] == 3) delta = Op3::doMove(d);
		
		if(delta < 0) {
			//d.printSol();
			//printf("%d\n", delta);
			i = 0;
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    		std::shuffle(moves.begin(), moves.end(), std::default_random_engine(seed));
		}
		else {
			++i;
		}
	}
}

void ILS(Data &d, int restarts, int iterations) {
	int best_global_value = numeric_limits<int>::max();
	vector< list<int> > best_global_solution;

	for (int iter_restart = 0; iter_restart < restarts; ++iter_restart) {
		
		Generator::generateInitialSolution(d);
		
		int iter = 0;
		int swapSize = 3;
			
		while(iter++ < iterations) {
			int val_pre_VND = d.getSolValue();	
			VND(d);
			int val_pos_VND = d.getSolValue();	
			
			//printf("%d %d\n", val_pre_VND, val_pos_VND);
			
			if(val_pos_VND < val_pre_VND) { //houve melhoria na solucao gerada
				iter = 0;
				swapSize = 3;
				puts("HOUVE ALGUMA MELHORIA");
			}
			else {
				++swapSize;
			}
			//perturbation(d, swapSize);
		}

		int val_pos_ILS = d.getSolValue();
		if(val_pos_ILS < best_global_value) {
			best_global_value    = val_pos_ILS;
			best_global_solution = d.sol;
		}
	}

	d.sol = best_global_solution;
	printf("BEST OBJ %d\n", d.getSolValue());
	d.printSol();
	return ;
}


int main(int argc, char **argv) {
    Data data;
    ParseDataset(argv[1], data);
    ILS(data, 1, 2);
}
