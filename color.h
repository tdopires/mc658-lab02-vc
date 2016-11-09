/*******************************************************************************
 * MC658 - Projeto e Análise de Algoritmos III - 2s2016
 * Prof: Flavio Keidi Miyazawa
 * PED: Mauro Henrique Mulati
 ******************************************************************************/

/* ATENÇÃO: NÃO ALTERE ESTE ARQUIVO */

#ifndef COLOR_H
#define COLOR_H

class GraphData
{
public:
   GraphData(ListGraph     &graph,
             NodeStringMap &nodename,
             NodePosMap    &posicaox,
             NodePosMap    &posy,
             EdgeValueMap  &eweight);
   ListGraph       &g;
   int             n,
                   m;
   NodeStringMap   &vname;
   EdgeStringMap   ename;
   NodeColorMap    vcolor;
   EdgeColorMap    ecolor;
   EdgeValueMap    &weight;
   NodePosMap      &posx;
   NodePosMap      &posy;
   AdjacencyMatrix AdjMat; // adjacency matrix
};

typedef enum E_SOLUTION_STATUS
{
  OK,
  INVALID_COLOR,
  INVALID_COLORS_VS_NUMBER,  // TESTED
  INVALID_NEIGHBORHOOD,
  INVALID_BOUNDS_OPT,
  INVALID_BOUNDS_NOT_OPT
} SOLUTION_STATUS;

void            showUsage();
SOLUTION_STATUS checkSolutionStatus(GraphData& gd, NodeIntMap& color, int lowerBound, int upperBound, int foundOpt);
string          graphAndColorsAsString(GraphData &gd, NodeIntMap &color);
string          nodesAndColorsAsString(GraphData &gd, NodeIntMap &color);
string          edgesAndColorsAsString(GraphData &gd, NodeIntMap &color);
string          colorsAsString(int lowerBound, int upperBound);
string          resultAsString(GraphData &gd,
                               string inputFile,
                               int exec,
                               NodeIntMap &color,
                               int lowerBound,
                               int upperBound,
                               int foundOpt,
                               SOLUTION_STATUS solutionStatus,
                               int elpasedTime,
                               int timeLimit);
string          decodeSolutionStatus(SOLUTION_STATUS solutionStatus);
void            graphAndColorsAsGraphical(GraphData &gd, NodeIntMap &color, string instance, int k);

#endif
