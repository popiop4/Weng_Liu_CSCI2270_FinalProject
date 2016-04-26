#ifndef VERTICESGRAPH_H
#define VERTICESGRAPH_H
#include <vector>
#include <iostream>

struct vertex;

struct adjVertex
{
    vertex *v;
    int weight;
};

struct vertex
{
    std::string name;
    int district;
    int distance;
    int ID;
    int position;
    bool visited;
    vertex *previous;
    std::vector<adjVertex> adj;

};

class verticesGraph
{
    private:
        std::vector<vertex> vertices;
    public:
        verticesGraph(char * fileName);
        verticesGraph();
        void buildGraphFile(char * fileName);
        void assignDistricts();
        void BFTraversalLabel(std::string starting,int distID);
        void displayEverything();
        void shortestDistance(std::string starting,std::string destination);
        void limitedDistancePath(std::string starting,std::string destination,int distance);
        void addVertex(std::string name);
        void addEdge(std::string v1,std::string v2,int weight);
        void snakesAndLadders();
        void buildMansionEscape();
        void mansionEscape();
        std::vector<int> randomNumberGenerate();
        int findDistance(std::string v1,std::string v2);
        vertex * findVertex(std::string name);
        vertex * Dijkstra(std::string starting,std::string destination);
};

#endif
