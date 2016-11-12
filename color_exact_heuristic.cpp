/*******************************************************************************
 * MC658 - Projeto e Análise de Algoritmos III - 2s2016
 * Prof: Flavio Keidi Miyazawa
 * PED: Mauro Henrique Mulati
 * Usa ideias e código de Rafael Arakaki e Flávio Keidi Miyazawa 
 ******************************************************************************/

/* IMPLEMENTE AS FUNCOES INDICADAS
 * DIGITE SEU RA: 123153
 * SUBMETA SOMENTE ESTE ARQUIVO */

#include <iostream>
#include <float.h>
#include <lemon/list_graph.h>
#include "mygraphlib.h"
#include <gurobi_c++.h>
#include "color_exact_heuristic.h"

int colorNaive(GraphData& gd, NodeIntMap& color, int& lowerBound, int& upperBound, int timeLimit);

//------------------------------------------------------------------------------
int colorExact(GraphData& gd, NodeIntMap& color, int& lowerBound, int& upperBound, int timeLimit)
/* SUBSTITUA O CONTEÚDO DESTA FUNÇÃO POR SUA IMPLEMENTAÇÃO DO ALGORITMO EXATO.
 * ENTRETANTO, NÃO ALTERE A ASSINATURA DA FUNÇÃO. */
{
	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		vector<GRBVar> y(gd.n);
		ListGraph::NodeMap< vector<GRBVar> > x(gd.g, vector<GRBVar>(gd.n));

		for(int k = 0; k < gd.n; k++) {
			y[k] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY, "y_" + to_string(k));

			for(NodeIt v(gd.g); v != INVALID; ++v) {
				x[v][k] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY, "x_" + gd.vname[v] + "," + to_string(k));
			}
		}
		model.update();

		for (NodeIt v(gd.g); v != INVALID; ++v) {
			GRBLinExpr expr = 0;
			for(int k = 0; k < gd.n; k++){
				expr += x[v][k];
			}
			model.addConstr(expr == 1.0);
		}
		model.update();

		for (EdgeIt e(gd.g); e != INVALID; ++e) {
			Node u = gd.g.u(e);
			Node v = gd.g.v(e);
			
			for(int k = 0; k < gd.n; k++){
				model.addConstr(x[u][k] + x[v][k] <= y[k]);
			}
		}
		model.update();

		GRBLinExpr exprObj = 0;
		for (int k = 0; k < gd.n; k++) {
			exprObj += y[k];
		}
		model.setObjective(exprObj, GRB_MINIMIZE);
		model.update();

		if (timeLimit > 0) model.getEnv().set(GRB_DoubleParam_TimeLimit, timeLimit);

		model.optimize();

		double k_total = 0.0;
		for(int k = 0; k < gd.n; k++) {			
			
			if (BinaryIsOne(y[k].get(GRB_DoubleAttr_X))) 
				k_total += 1.0;
		}

		int status = model.get(GRB_IntAttr_Status);

		if (status == GRB_OPTIMAL) {
			lowerBound = upperBound = model.get(GRB_DoubleAttr_ObjVal);
			return 1;
		} else if (status == GRB_TIME_LIMIT) {
			upperBound = model.get(GRB_DoubleAttr_ObjVal);
			lowerBound = model.get(GRB_DoubleAttr_ObjBound);
		}
		return 0;

	} catch(GRBException e) {
		cerr << "[GRBException] Code: "<< e.getErrorCode() << " getMessage: " << e.getMessage() << endl;
		exit(0);
	}

}
//------------------------------------------------------------------------------
int colorHeuristic(GraphData& gd, NodeIntMap& color, int& lowerBound, int& upperBound, int timeLimit)
/* SUBSTITUA O CONTEÚDO DESTA FUNÇÃO POR SUA IMPLEMENTAÇÃO DA HEURISTICA.
 * ENTRETANTO, NÃO ALTERE A ASSINATURA DA FUNÇÃO. */
{
	return colorNaive(gd, color, lowerBound, upperBound, timeLimit);
}
//------------------------------------------------------------------------------
int colorNaive(GraphData& gd, NodeIntMap& color, int& lowerBound, int& upperBound, int timeLimit)
/* Algoritmo ingênuo para o problema de coloração de vértices */
{
	lowerBound = 1;
	int next = lowerBound;
	
	for(NodeIt i(gd.g); i != INVALID; ++i){
		color[i] = next;
		next++;
	}
	next--;
	upperBound = next;
	
	return 0;
}
//------------------------------------------------------------------------------

