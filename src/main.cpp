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




/*








#include <iostream>
#include <fstream>
#include <limits>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath> //exp

#include "Data.hpp"
#include "initialSolutionGenerator.hpp"
#include "Opt2.hpp"
#include "PairOpt2.hpp"

using namespace std;

double myAbs(double x) {return x < 0 ? -x : x;}

Data readData(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "\nMissing Parameters\n";
		cout << " ./exeQAP [Instance]" << endl;				
		exit(1);
	}																											

	if (argc > 2) {
		cout << "\nToo many parameters\n";
		cout << " ./exeQAP [Instance]" << endl;
		exit(1);
	}

	ifstream in(argv[1], ios::in);

	int N;
	in >> N;

	Data d(N);

	for (int i = 0; i < N; ++i) 
		for (int j = 0; j < N; ++j) 
			in >> d.flows[i][j];

	for (int i = 0; i < N; ++i) 
		for (int j = 0; j < N; ++j) 
			in >> d.dists[i][j];

	return d;
}

double computeSolutionValue(Data &d, vector<int> &sol) {
	double cost = 0;
	for (int i = 0; i < d.N; ++i)
		for (int j = 0; j < d.N; ++j)
			cost += (d.flows[sol[i]][sol[j]] * d.dists[i][j]);
	return cost;
}

double VND(Data &d, vector<int> &sol, double solution_value) {
	d.makeLSdata(sol); //calcula o custo de cada swap possivel na solucao
	
	vector<int> moves;
	moves.push_back(1);
	moves.push_back(2);
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(moves.begin(), moves.end(), std::default_random_engine(seed));

	for(int i = 0; i < moves.size(); ) {
		double delta = 0;
		if     (moves[i] == 1) delta = pairOpt2::doMove(d, sol);
		else if(moves[i] == 2) delta = Opt2::doMove(d, sol);
		
		
		DEBUGAR(if(myAbs(solution_value + delta - computeSolutionValue(d, sol)) > 0.001){ cout << "O  movimento " << moves[i] << " esta calculando a FO errada!\n" << (int)(solution_value + delta) << "|" << (int)computeSolutionValue(d, sol) << "\n"; exit(1);}else cout << "MOVIMENTO " << moves[i] << " OK\n"; );
		if((solution_value + delta) - solution_value < -0.001) {
			DEBUGAR(cout << "Houve melhora na busca local.\n" );
			solution_value = solution_value + delta;
			i = 0;
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    		std::shuffle(moves.begin(), moves.end(), std::default_random_engine(seed));
		}
		else {
			++i;
		}
	}

	return solution_value;
}

void perturbation(vector<int> &solMod, vector<int> &sol, int k) {
	solMod = sol;
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0,sol.size() - 1);
	for (int i = 0; i < k; ++i) {
		int u = distribution(generator);
		int v = distribution(generator);
		std::swap(solMod[u], solMod[v]);
		DEBUGAR(cout << "p1( " << u << " , " << v << ") ";)
	}
	DEBUGAR(cout << "\n";)
}

vector<int> ILSqap(Data &d, int restarts, int iterations) {
	double best_global_value = numeric_limits<double>::max();
	vector<int> best_global_solution;

	for (int iter_restart = 0; iter_restart < restarts; ++iter_restart) {
		vector<int> solution  = Generator::generateInitialSolution(d, d.N);
		double solution_value = computeSolutionValue(d, solution);
		
		DEBUGAR(
			cout << "Gerou solucao inicial com FO " << solution_value << ": \n";
			cout << "Solucao inicial :\n";
			for (int i = 0; i < d.N; ++i)
				cout << solution[i] << ",";
			cout << "\n";
		)
		
		double best_solution_value = solution_value;
		vector<int> best_solution = solution;

		int iter = 0;
		int k = 2;
		
		solution_value = VND(d, solution, solution_value);
		while(++iter < iterations) {
			double delta = solution_value - best_solution_value;
			if(delta < 0) { //houve melhoria
				best_solution_value = solution_value;
				best_solution       = solution;
				iter = 0;
				k = 3;
				DEBUGAR(cout << "Nova solucao otima da busca local: " << solution_value << "\n";)
			}
			else {
				++k;
				if(k > d.N) k = 3;
			}
			perturbation(solution, best_solution, k);
			solution_value = VND(d, solution, computeSolutionValue(d, solution));
		}

		if(best_solution_value < best_global_value) {
			best_global_value    = best_solution_value;
			best_global_solution = best_solution;
		}

		//DEBUGAR(
			cout << "Solucao final do restart " << iter_restart << " com valor " << (int)best_solution_value << ":\n";
			for (int i = 0; i < d.N; ++i)
				cout << best_solution[i] << ",";
			cout << "\n";
		//)
	}

	return best_global_solution;
}

int main (int argc, char *argv[]) {

	Data d = readData(argc, argv);
	//d.print(5);

	clock_t startTime = clock();
	vector<int> sol = ILSqap(d, 15, d.N*2);
	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

	cout << "Solucao final com custo " << (int) computeSolutionValue(d, sol) << ":\n";
	for (int i = 0; i < d.N; ++i)
		cout << sol[i] << ",";
	cout << "\n";
	return 0;
}*/
