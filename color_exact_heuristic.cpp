/*******************************************************************************
 * MC658 - Projeto e Análise de Algoritmos III - 2s2016
 * Prof: Flavio Keidi Miyazawa
 * PED: Mauro Henrique Mulati
 * Usa ideias e código de Rafael Arakaki e Flávio Keidi Miyazawa 
 ******************************************************************************/

/* IMPLEMENTE AS FUNCOES INDICADAS
 * DIGITE SEU RA: ______
 * SUBMETA SOMENTE ESTE ARQUIVO */

#include <iostream>
#include <float.h>
#include <lemon/list_graph.h>
#include "mygraphlib.h"
#include "color_exact_heuristic.h"

int colorNaive(GraphData& gd, NodeIntMap& color, int& lowerBound, int& upperBound, int timeLimit);

//------------------------------------------------------------------------------
int colorExact(GraphData& gd, NodeIntMap& color, int& lowerBound, int& upperBound, int timeLimit)
/* SUBSTITUA O CONTEÚDO DESTA FUNÇÃO POR SUA IMPLEMENTAÇÃO DO ALGORITMO EXATO.
 * ENTRETANTO, NÃO ALTERE A ASSINATURA DA FUNÇÃO. */
{
   return colorNaive(gd, color, lowerBound, upperBound, timeLimit);
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

