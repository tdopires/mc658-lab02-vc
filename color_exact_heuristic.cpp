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
#include <deque>

//------------------------------------------------------------------------------
// Auxiliar Functions for heuristic used for Cutoff on Exact Algorithm
int saturationDegree(GraphData& gd, NodeIntMap& color, Node v) {
	int degree = 0;
	for (IncEdgeIt e(gd.g,v); e != INVALID; ++e) {
		Node eu = gd.g.u(e);
		Node ev = gd.g.v(e);

		Node adj = (eu == v) ? ev : eu;

		if (color[adj] != 0)
			degree++;
	}
	return degree;
}

int vertexDegree(GraphData& gd, Node v) {
	int degree = 0;
	for (IncEdgeIt e(gd.g,v); e != INVALID; ++e)
		degree++;
	return degree;
}

int colorVertex(GraphData& gd, NodeIntMap& color, Node v) {
	int vcolor = 1;
	bool isColorUsed = false;
	do {
		isColorUsed = false;
		for (IncEdgeIt e(gd.g,v); e != INVALID; ++e) {
			Node eu = gd.g.u(e);
			Node ev = gd.g.v(e);

			Node adj = (eu == v) ? ev : eu;

			if (color[adj] == vcolor) {
				isColorUsed = true;
				vcolor++;
				continue;
			}
		}	
	} while (isColorUsed);

	color[v] = vcolor;

	return vcolor;
}

int calculateHeuristicCutoff(GraphData& gd) {
	NodeIntMap color(gd.g);	
	for (NodeIt v(gd.g); v != INVALID; ++v) {
		color[v] = 0;
	}

	int noOfColoredNodes = 0;
	while (noOfColoredNodes < gd.n) {
		
		int max = -1;
		Node target;
		for (NodeIt v(gd.g); v != INVALID; ++v) {
			if (color[v] == 0) {
				int d = saturationDegree(gd, color, v);

				if (d > max) {
					max = d;
					target = v;
				}
				if (d == max)
					if (vertexDegree(gd, v) > vertexDegree(gd, target))
						target = v;
			}
		}
		colorVertex(gd, color, target);
		noOfColoredNodes++;
	}

	int maxColor = 0;
	for (NodeIt v(gd.g); v != INVALID; ++v) {
		if (color[v] > maxColor) {
			maxColor = color[v];
		}
	}

	return maxColor;
}

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

		for (int k = 0; k < gd.n; k++) {
			y[k] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY, "y_" + to_string(k));

			for (NodeIt v(gd.g); v != INVALID; ++v) {
				x[v][k] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY, "x_" + gd.vname[v] + "," + to_string(k));
			}
		}
		model.update();

		for (NodeIt v(gd.g); v != INVALID; ++v) {
			GRBLinExpr expr = 0;
			for (int k = 0; k < gd.n; k++){
				expr += x[v][k];
			}
			model.addConstr(expr == 1.0);
		}
		model.update();

		for (EdgeIt e(gd.g); e != INVALID; ++e) {
			Node u = gd.g.u(e);
			Node v = gd.g.v(e);
			
			for (int k = 0; k < gd.n; k++){
				model.addConstr(x[u][k] + x[v][k] <= y[k]);
			}
		}
		model.update();

		
		for (int k = 0; k < gd.n; k++) {

			GRBLinExpr expr = 0;
			for (NodeIt v(gd.g); v != INVALID; ++v) {
				expr += x[v][k];
			}
			model.addConstr(expr >= y[k]);
		}
		model.update();
		

		GRBLinExpr total_k = 0;
		for (int k = 0; k < gd.n; k++) {
			total_k += y[k];
		}
		model.setObjective(total_k, GRB_MINIMIZE);
		model.update();

		if (timeLimit > 0) 
			model.getEnv().set(GRB_DoubleParam_TimeLimit, timeLimit);

		double cutoff = calculateHeuristicCutoff(gd);
		if (cutoff > 0)
			model.getEnv().set(GRB_DoubleParam_Cutoff, cutoff + 0.1);

		model.optimize();

		double k_total = 0.0;
		vector<int> newColors = vector<int>(gd.n, 0);
		for (int k = 0; k < gd.n; k++) {
			if (BinaryIsOne(y[k].get(GRB_DoubleAttr_X))) {
				k_total += 1.0;
				newColors[k] = k_total;
			}
		}

		for (int k = 0; k < gd.n; k++) {
			for (NodeIt v(gd.g); v != INVALID; ++v) {
				if (BinaryIsOne(x[v][k].get(GRB_DoubleAttr_X))) 
					color[v] = newColors[k];
			}
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
	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		vector<GRBVar> y(gd.n);
		ListGraph::NodeMap< vector<GRBVar> > x(gd.g, vector<GRBVar>(gd.n));

		for (int k = 0; k < gd.n; k++) {
			y[k] = model.addVar(0.0, 1.0, 1.0, GRB_CONTINUOUS, "y_" + to_string(k));

			for (NodeIt v(gd.g); v != INVALID; ++v) {
				x[v][k] = model.addVar(0.0, 1.0, 1.0, GRB_CONTINUOUS, "x_" + gd.vname[v] + "," + to_string(k));
			}
		}
		model.update();

		for (NodeIt v(gd.g); v != INVALID; ++v) {
			GRBLinExpr expr = 0;
			for (int k = 0; k < gd.n; k++){
				expr += x[v][k];
			}
			model.addConstr(expr == 1.0);
		}
		model.update();

		for (EdgeIt e(gd.g); e != INVALID; ++e) {
			Node u = gd.g.u(e);
			Node v = gd.g.v(e);
			
			for (int k = 0; k < gd.n; k++){
				model.addConstr(x[u][k] + x[v][k] <= y[k]);
			}
		}
		model.update();

		
		for (int k = 0; k < gd.n; k++){

			GRBLinExpr expr = 0;
			for (NodeIt v(gd.g); v != INVALID; ++v) {
				expr += x[v][k];
			}
			model.addConstr(expr >= y[k]);
		}
		model.update();
		

		GRBLinExpr total_k = 0;
		for (int k = 0; k < gd.n; k++) {
			total_k += y[k];
		}
		model.setObjective(total_k, GRB_MINIMIZE);
		model.update();

		model.optimize();

		deque<GRBVar*> intvars;
		GRBVar* modelvars = model.getVars();
		for (int j = 0; j < model.get(GRB_IntAttr_NumVars); ++j) {
			intvars.push_back(&modelvars[j]);
	    }


		for (int iter = 0; iter < 1000; ++iter) {

			deque<GRBVar*> fractional;
			for (size_t j = 0; j < intvars.size(); ++j) {
				if (NonBinary(intvars[j]->get(GRB_DoubleAttr_X))) {
					fractional.push_back(intvars[j]);
				}
			}

			if (fractional.size() == 0)
				break;

			bool isFeasible = false;
			do {

				double fractional_val = (*fractional.begin())->get(GRB_DoubleAttr_X);
				if (fractional_val > 0.5) {
					(*fractional.begin())->set(GRB_DoubleAttr_LB, 1.0);
					(*fractional.begin())->set(GRB_DoubleAttr_UB, 1.0);
				} else {
					(*fractional.begin())->set(GRB_DoubleAttr_LB, 0.0);
					(*fractional.begin())->set(GRB_DoubleAttr_UB, 0.0);
				}

				model.optimize();

				int status = model.get(GRB_IntAttr_Status);
				if (status == GRB_OPTIMAL)
					isFeasible = true;
				else
					fractional.pop_front();

			} while (!isFeasible);
			
		}

		double k_total = 0.0;
		vector<int> newColors = vector<int>(gd.n, 0);
		for (int k = 0; k < gd.n; k++) {
			if (BinaryIsOne(y[k].get(GRB_DoubleAttr_X))) {
				k_total += 1.0;
				newColors[k] = k_total;
			}
		}

		for (int k = 0; k < gd.n; k++) {
			for (NodeIt v(gd.g); v != INVALID; ++v) {
				if (BinaryIsOne(x[v][k].get(GRB_DoubleAttr_X))) 
					color[v] = newColors[k];
			}
		}

		upperBound = model.get(GRB_DoubleAttr_ObjVal);
		lowerBound = model.get(GRB_DoubleAttr_MinBound);
		
		if (upperBound == lowerBound) {
			return 1;
		}
		return 0;

	} catch(GRBException e) {
		cerr << "[GRBException] Code: "<< e.getErrorCode() << " getMessage: " << e.getMessage() << endl;
		exit(0);
	}

}
//------------------------------------------------------------------------------

