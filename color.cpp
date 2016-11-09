/*******************************************************************************
 * MC658 - Projeto e Análise de Algoritmos III - 2s2016
 * Prof: Flavio Keidi Miyazawa
 * PED: Mauro Henrique Mulati
 * Usa ideias e código de Rafael Arakaki e Flávio Keidi Miyazawa 
 ******************************************************************************/

/* ATENÇÃO: NÃO ALTERE ESTE ARQUIVO */

#include <climits>  // For INT_MAX
#include <set>
#include <ctime>  // For CLOCKS_PER_SEC
#include <lemon/list_graph.h>
#include "mygraphlib.h"
#include "myutils.h"
#include "color.h"
#include "color_exact_heuristic.h"

using namespace lemon;
using namespace std;

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
   // Parameters
   int    exec       = 0;  // 0: not set; 1: EXACT; 2: HEURISTIC
   int    timeLimit  = 0;  // 0: not set
   bool   verbose    = false;
   string inputFile;
   string outputFile;

   // Reading parameters
   for(int i = 1; i < argc; i++){
      const string arg(argv[i]);
      string next;
      if((i+1) < argc){
         next = string(argv[i+1]);
      }
      else{
         next = string("");
      }
      
      if(exec != 0 && (arg.find("-e") == 0 || arg.find("-h") == 0)){
         cerr << "Erro ao ler parametro \"" << arg << "\": pode haver somente um parametro de modo de execucao" << endl;
         showUsage();
         exit(1);
      }
      else if(arg.find("-e") == 0){
         exec = 1;
      }
      else if(arg.find("-h") == 0){
         exec = 2;
      }
      else if(arg.find("-v") == 0){
         verbose = true;
      }
      else if(arg.find("-t") == 0 && next.size() > 0){
         timeLimit = atoi(next.c_str()); 
         i++; 
         continue;
      }
      else if(arg.find("-i") == 0 && next.size() > 0){
         inputFile = next; 
         i++; 
         continue;
      }
      else if( arg.find("-o") == 0 && next.size() > 0){
         outputFile = next; 
         i++; 
         continue;
      }
      else{
         cerr << "Parametro invalido: \"" << arg << "\"" << " (ou parametro faltando)" << endl;
         showUsage();
         exit(1);
      }
   }

   // Check parameters
   if(exec == 0){
      cerr << "Deve ser selecionado exatamente um modo de execucao dentre -e ou -h" << endl;
      showUsage(); 
      exit(1);
   }
   if(inputFile.size() < 1){
      cerr << "Nome de arquivo de entrada invalido" << endl;
      showUsage(); 
      exit(1);
   }
   if(outputFile.size() < 1){
      cerr << "Nome de arquivo de saida invalido" << endl;
      showUsage(); 
      exit(1);
   }
   if(timeLimit == 0){
      timeLimit = 30; 
   }

   // int seed = 1;   // mhmulati
   // srand48(seed);  // mhmulati
   
   // Variables that represent the weighted undirected graph
   ListGraph     g;
   NodeStringMap vname(g);
   EdgeValueMap  weight(g);  // We will just ignore the weights
   NodePosMap    posx(g), 
                 posy(g);

   // Read the input. Note that we will just ignore the weights.
   if(!ReadListGraph(inputFile, g, vname, weight, posx, posy)){
     cerr << "Erro na leitura do arquivo de entrada " << inputFile << endl;
     exit(1);
   }
   
   // Declare and initialize the graph data structure with the read values
   GraphData gd(g, vname, posx, posy, weight);
   
   // Variables that represent the solution
   NodeIntMap color(gd.g);                // Declare a map in nodes of type ListGraph::NodeMap<int>
   for(NodeIt i(gd.g); i!=INVALID; ++i){  // Initialize all vertex with invalid color
      color[i] = 0;
   }
   int lowerBound = 1;
   int upperBound = gd.n;
   
   if(verbose){
      cout << "INPUT" << endl;
      cout << graphAndColorsAsString(gd, color) << endl;
   }   
   
   double  elapsedTime = DBL_MAX;
   clock_t before  = clock();
   
   int optimal = 0;
   
   switch(exec){
      case 1:{
         optimal = colorExact(gd, color, lowerBound, upperBound, timeLimit);
         break;
      }
      case 2:{
         optimal = colorHeuristic(gd, color, lowerBound, upperBound, timeLimit);
         break;
      }
   }
   
   clock_t after = clock();
   elapsedTime = (double) (after - before) / CLOCKS_PER_SEC;

   // Check if it is indeed a solution for the instance
   SOLUTION_STATUS solutionStatus = checkSolutionStatus(gd, color, lowerBound, upperBound, optimal);
   
   // Print the result in the outputFile
   ofstream ofsOutputFile;
   ofsOutputFile.open(outputFile);
   ofsOutputFile << resultAsString(gd, inputFile, exec, color, lowerBound, upperBound, optimal, solutionStatus, elapsedTime, timeLimit) << flush;
   ofsOutputFile.close();
   
   // Print the result in the screen
   if(verbose){
      cout << "OUTPUT" << endl;
      cout << resultAsString(gd, inputFile, exec, color, lowerBound, upperBound, optimal, solutionStatus, elapsedTime, timeLimit) << flush;
      // if(solutionStatus == OK){
         cout << edgesAndColorsAsString(gd, color) << flush;
         graphAndColorsAsGraphical(gd, color, inputFile, upperBound);
      // }
      cout << endl;
   }
   
   return solutionStatus;
}
//------------------------------------------------------------------------------
GraphData::GraphData(ListGraph     &graph,
                     NodeStringMap &nodename,
                     NodePosMap    &posicaox,
                     NodePosMap    &posicaoy,
                     EdgeValueMap  &eweight):
                     g(graph),
                     vname(nodename),
                     ename(graph),
                     vcolor(graph),
                     ecolor(graph),
                     weight(eweight),
                     posx(posicaox),
                     posy(posicaoy),
                     AdjMat(graph, eweight, MY_INF)
{
   n = countNodes(g);
   m = countEdges(g);
}
//------------------------------------------------------------------------------
void showUsage()
{
   cout << "Uso: \n"
        << "./color.e (-e|-h) -i <in> -o <out> [-t <time>] [-v]\n"
        << "Onde:\n"
        << "(-e|-h)    Seleciona algoritmo exato ou algoritmo heuristico\n"
        << "-i <in>    Informa arquivo de entrada <in>\n"
        << "-o <out>   Informa arquivo de saida <out>\n"
        << "-t <time>  Informa o tempo limite em segundos dado por <time>. Caso não seja informado, considera-se 30s.\n"
        << "-v         Adicionalmente produz saídas detalhada e gráfica\n"
        << flush;
}
//------------------------------------------------------------------------------
SOLUTION_STATUS checkSolutionStatus(GraphData &gd, NodeIntMap &color, int lowerBound, int upperBound, int optimal)
/* (1) Verificar se todos os vértices receberam cores válidas;
 * (2) Verificar quantas cores foram de fato usadas e se o valor da maior cor usada é o número de cores usadas (upperBound);
 * (3) Verificar cores dos extremos de todas as arestas; e
 * (4) Verificar se é compatível com o lower bound e o upper bound. */
{
   set<int> colorsUsed;
   int greater = 0;   
   for(NodeIt o(gd.g); o!=INVALID; ++o){
      if(color[o] <= 0){  // Check condition (1)
         return INVALID_COLOR;
      }
      colorsUsed.insert(color[o]);  // Remember, it is a set in the mathematical sense
      if(color[o] > greater){
         greater = color[o];
      }
   }
   
   if((int)colorsUsed.size() != greater){  // Check condition (2)
      return INVALID_COLORS_VS_NUMBER;
   }
   
   for(EdgeIt e(gd.g); e != INVALID; ++e){  // Check condition (3)
      if(color[gd.g.u(e)] == color[gd.g.v(e)]){
         return INVALID_NEIGHBORHOOD;
      }
   }
   
   
   if(optimal){  // Check condition (4)
      if (!(lowerBound == greater && greater == upperBound)){
         return INVALID_BOUNDS_OPT;
      }
   }
   else{
      if(!(lowerBound <= greater && greater == upperBound)){   
         return INVALID_BOUNDS_NOT_OPT;
      }
   }

   return OK;
}
//------------------------------------------------------------------------------
string graphAndColorsAsString(GraphData &gd, NodeIntMap &color){
   stringstream graphAndColorsAsString;
   graphAndColorsAsString << "n          : " << gd.n << endl;
   graphAndColorsAsString << "m          : " << gd.m << endl;
   graphAndColorsAsString << nodesAndColorsAsString(gd, color);
   graphAndColorsAsString << edgesAndColorsAsString(gd, color);
   return graphAndColorsAsString.str();
}

//------------------------------------------------------------------------------
string nodesAndColorsAsString(GraphData &gd, NodeIntMap &color)
{
   stringstream nodesAndColorsAsString;
   nodesAndColorsAsString << "nodes      :";
   for(NodeIt v(gd.g); v != INVALID; ++v){
      nodesAndColorsAsString << " " << gd.vname[v] << "." << color[v];
   }
   nodesAndColorsAsString << endl;
   return nodesAndColorsAsString.str();
}
//------------------------------------------------------------------------------
string edgesAndColorsAsString(GraphData &gd, NodeIntMap &color)
{
   stringstream edgesAndColorsAsString;
   edgesAndColorsAsString << "edges      :";
   for(EdgeIt e(gd.g); e != INVALID; ++e){
      edgesAndColorsAsString << " (" << gd.vname[gd.g.u(e)] << "." << color[gd.g.u(e)] << "," << gd.vname[gd.g.v(e)] << "." << color[gd.g.v(e)] << ")";
   }
   edgesAndColorsAsString << endl;
   return edgesAndColorsAsString.str();
}
//------------------------------------------------------------------------------
string colorsAsString(int lowerBound, int upperBound)
{
   stringstream coloringAsString;
   coloringAsString << "lowerBound : " << lowerBound << endl;
   coloringAsString << "upperBound : " << upperBound << endl;
   coloringAsString << "k          : " << upperBound << endl;
   return coloringAsString.str();
}
//------------------------------------------------------------------------------
string resultAsString(GraphData &gd,
                      string inputFile,
                      int exec,
                      NodeIntMap &color,
                      int lowerBound,
                      int upperBound,
                      int optimal,
                      SOLUTION_STATUS solutionStatus,
                      int elapsedTime,
                      int timeLimit)
{
   stringstream resultAsString;

   resultAsString << "algorithm  : " << (exec == 1 ? "EXACT" : "HEURISTIC") << endl;
   resultAsString << "instance   : " << inputFile << endl;
   resultAsString << "elapsedTime: " << elapsedTime << " s" << endl;
   resultAsString << "timeLimit  : " << timeLimit << " s" <<  endl;

   // if(solutionStatus == OK){
      resultAsString << "optimal    : " <<  (optimal?"Yes":"No") << endl;
      resultAsString << "n          : " << gd.n << endl;
      resultAsString << colorsAsString(lowerBound, upperBound);
      resultAsString << nodesAndColorsAsString(gd, color);
   // }
   if(solutionStatus != OK){  // else{
      resultAsString << "Solution returned by the student's algorithm has not passed in the verification." << endl;
      resultAsString << "error      : " << decodeSolutionStatus(solutionStatus) << endl;
   }

   return resultAsString.str();
}
//------------------------------------------------------------------------------
string decodeSolutionStatus(SOLUTION_STATUS solutionStatus)
{
   stringstream decodeSolutionStatus;
   switch(solutionStatus){
      case OK:{
         decodeSolutionStatus << "OK";
         break;
      }
      case INVALID_COLOR:{
         decodeSolutionStatus << "INVALID_COLOR";
         break;
      }
      case INVALID_COLORS_VS_NUMBER:{
         decodeSolutionStatus << "INVALID_COLORS_VS_NUMBER";
         break;
      }
      case INVALID_NEIGHBORHOOD:{
         decodeSolutionStatus << "INVALID_NEIGHBORHOOD";
         break;
      }
      case INVALID_BOUNDS_OPT:{
         decodeSolutionStatus << "INVALID_BOUNDS_OPT";
         break;
      }
      case INVALID_BOUNDS_NOT_OPT:{
         decodeSolutionStatus << "INVALID_BOUNDS_NOT_OPT";
         break;
      }
   }
   return decodeSolutionStatus.str();
}
//------------------------------------------------------------------------------
void graphAndColorsAsGraphical(GraphData &gd, NodeIntMap &color, string instance, int k)
{
   ListGraph h;
   NodeStringMap h_vname(h);  // node names
   NodeNodeMap g2h(gd.g);  // maps a node of g to a node of h
   NodePosMap h_posx(h);
   NodePosMap h_posy(h);
   NodeColorMap vcolor(h);   // color of the vertices
   EdgeColorMap acolor(h);  // color of edges
   EdgeStringMap aname(h);  // name of edges

   for(NodeIt v(gd.g); v != INVALID; ++v){
      Node hv;
      hv = h.addNode();
      g2h[v] = hv;
      h_posx[hv] = gd.posx[v];
      h_posy[hv] = gd.posy[v];
      h_vname[hv] = gd.vname[v]+ "." + IntToString(color[v]);
      vcolor[hv] = BLUE; // color[v]+2;  // Assign the color of the vertex (in the solution color) to the graphical color (vcolor).
                                // +2 to get over colors NOCOLOR, WHITE, BLACK
   }
   
   for(EdgeIt e(gd.g); e != INVALID; ++e){
      Edge a;      
      a = h.addEdge(g2h[gd.g.u(e)] , g2h[gd.g.v(e)]);
      aname[a] = "";
      acolor[a] = BLACK;  // Color of the edges must be black
   }
   
   ViewListGraph(h, h_vname, aname, h_posx, h_posy, vcolor, acolor, "GCP instance " + instance + " colored with " + IntToString(k) + " colors");
}
//------------------------------------------------------------------------------
