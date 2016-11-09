/*******************************************************************************
 * MC658 - Projeto e Análise de Algoritmos III - 2s2016
 * Prof: Flavio Keidi Miyazawa
 * PED: Mauro Henrique Mulati
 ******************************************************************************/

/* ATENÇÃO: NÃO ALTERE ESTE ARQUIVO */

#ifndef COLOR_EXACT_HEURISTIC_H
#define COLOR_EXACT_HEURISTIC_H

#include "color.h"

int colorExact(GraphData& gd, NodeIntMap& color, int& lowerBound, int& upperBound, int timeLimit);
int colorHeuristic(GraphData &gd, NodeIntMap &color, int &lowerBound, int &upperBound, int timeLimit);

#endif
