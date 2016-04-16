#include "verticesGraph.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <climits>
#include <stdlib.h>
#include <time.h>

verticesGraph::verticesGraph(char * fileName)
{
    buildGraph(fileName);
};

verticesGraph::verticesGraph(){};

//verticesGraph::~verticesGraph(){};
//array doesn't need to be destroyed?

void verticesGraph::buildGraph(char * fileName)
{
    std::string firstLine,lines;
    std::string itemArray[20][20];
    int x=0,quantity=0;
    std::ifstream inFile;
    inFile.open(fileName);
    if(inFile.good())
    {
        getline(inFile,firstLine);
        std::stringstream ss1(firstLine);
        std::string items1;
        while(getline(ss1,items1,','))
        {
            quantity+=1;
        }
        quantity-=1;
        while(getline(inFile,lines))
        {
            int y=0;
            std::stringstream ss2(lines);
            std::string items2;
            while(getline(ss2,items2,','))
            {
                itemArray[x][y]=items2;
                y+=1;
            }
            x+=1;
        }
    }

    for(int x=0;x<quantity;x++)
    {
        addVertex(itemArray[x][0]);
    }

    for(int x=0;x<quantity;x++)
    {
        for(int y=0;y<quantity;y++)
        {
            if(itemArray[x][y+1]!="-1"&&itemArray[x][y+1]!="0")
            {
                addEdge(itemArray[x][0],itemArray[y][0],atoi(itemArray[x][y+1].c_str()));
            }
        }
    }
}

void verticesGraph::assignDistricts()
{
    int y=1;
    for(int x=0;x<vertices.size();x++)
    {
        if(vertices[x].district==-1)
        {
            BFTraversalLabel(vertices[x].name,y);
            y+=1;
        }
    }
}

void verticesGraph::BFTraversalLabel(std::string starting,int distID)
{
    std::queue<vertex *> qv;
    vertex * v1=findVertex(starting);
    vertex * v2;
    v1->visited=true;
    qv.push(v1);
    while(!qv.empty())
    {
        v2=qv.front();
        qv.pop();
        for(int x=0;x<v2->adj.size();x++)
        {
            if(v2->adj[x].v->visited==false)
            {
                v2->adj[x].v->visited=true;
                v2->district=distID;
                v2->adj[x].v->district=distID;
                qv.push(v2->adj[x].v);
            }
        }
    }

    for(int x=0;x<vertices.size();x++)
    {
        vertices[x].visited=false;
    }
}

void verticesGraph::displayEverything()
{
    for(int x=0;x<vertices.size();x++)
    {
        std::cout<<vertices[x].district<<", "<<vertices[x].name<<"-->";
        for(int y=0;y<vertices[x].adj.size();y++)
        {
            std::cout<<vertices[x].adj[y].v->name;
            if(y!=vertices[x].adj.size()-1)
            {
                std::cout<<" , ";
            }
        }
        std::cout<<std::endl;
    }
}

void verticesGraph::shortestDistance(std::string starting,std::string destination)
{
    if(findVertex(starting)==NULL||findVertex(destination)==NULL)
    {
        std::cout<<"At least one vertex doesn't exist"<<std::endl;
        return;
    }

    vertex * sta=findVertex(starting);
    vertex * des=findVertex(destination);
    if(sta->district!=des->district)
    {
        std::cout<<"No path between two vertices"<<std::endl;
        return;
    }

    if(vertices[0].district==-1)
    {
        std::cout<<"Please assign vertices' districts first"<<std::endl;
        return;
    }

    vertex * startV=findVertex(starting);
    vertex * endV=Dijkstra(starting,destination);
    std::vector<vertex *> path;
    vertex * v1=endV;
    while(v1!=startV)
    {
        path.push_back(v1);
        v1=v1->previous;
    }
    std::cout<<std::endl;
    std::cout<<"Shortest distance: "<<starting<<" >>> "<<destination<<" is "<<endV->distance<<std::endl;
    std::cout<<"Path: ";
    std::cout<<startV->name;
    for(int x=path.size()-1;x>-1;x--)
    {
        std::cout<<" >>> "<<path[x]->name;
    }
    std::cout << std::endl;

    for(int x=0;x<vertices.size();x++)
    {
        vertices[x].visited=false;
    }
}

void verticesGraph::limitedDistancePath(std::string starting,std::string destination,int distance)
{
    if(findVertex(starting)==NULL||findVertex(destination)==NULL)
    {
        std::cout<<"At least one vertices doesn't exist"<<std::endl;
        return;
    }

    vertex * sta=findVertex(starting);
    vertex * des=findVertex(destination);
    if(sta->district!=des->district)
    {
        std::cout<<"No path between two vertices"<<std::endl;
        return;
    }

    if(vertices[0].district==-1)
    {
        std::cout<<"Please assign vertices' districts first"<<std::endl;
        return;
    }

    if(distance<=0)
    {
        std::cout<<"distance must be greater than zero"<<std::endl;
        return;
    }

    int x=vertices.size();

    int previous[x];
    for(int y=0;y<x;y++)
    {
        previous[y]=-1;
        vertices[y].ID=y;
    }

    sta->visited=true;
    vertex * v;
    std::queue<vertex *> qv;
    qv.push(sta);
    while(!qv.empty())
    {
        v=qv.front();
        qv.pop();
        for(int x=0;x<v->adj.size();x++)
        {
            if(v->adj[x].v->visited==false)
            {
                if(v->adj[x].weight<=distance)
                {
                    previous[v->adj[x].v->ID]=v->ID;
                    v->adj[x].v->visited=true;
                    qv.push(v->adj[x].v);
                }
            }
        }
    }

    std::vector<vertex *> path;
    int pos=des->ID;
    path.push_back(des);
    while(previous[pos]!=-1)
    {
        pos=previous[pos];
        path.push_back(findVertex(vertices[pos].name));
    }

    if(path[0]!=des||path[path.size()-1]!=sta)
    {
        std::cout<<"No path can be found"<<std::endl;
        return;
    }

    std::cout<<"One path: ";
    for(int x=path.size()-1;x>-1;x--)
    {
        if(x!=0)
        {
            std::cout<<path[x]->name<<" >>> "<<findDistance(path[x]->name,path[x-1]->name)<<" >>> ";
        }
        else
        {
            std::cout<<path[x]->name;
        }
    }
    std::cout << std::endl;

    for(int x=0;x<vertices.size();x++)
    {
        vertices[x].visited=false;
    }
}

void verticesGraph::vertexInformation(std::string v1)
{
    vertex * v=findVertex(v1);
    std::cout<<"Name: "<<v->name<<std::endl;
    std::cout<<"District: "<<v->district<<std::endl;
    std::cout<<"Number of connected vertices: "<<v->adj.size()<<std::endl;
    std::cout<<"Adjacent vertices: ";
    for(int x=0;x<v->adj.size();x++)
    {
        std::cout<<v->adj[x].v->name;
        if(x!=vertices[x].adj.size()-1)
        {
            std::cout<<" , ";
        }
    }
    std::cout<<std::endl;
}

void verticesGraph::graphInformation()
{
    std::cout<<"Number of vertices: "<<vertices.size()<<std::endl;
    int maxDistrict=vertices[0].district;
    int maxConnections=vertices[0].adj.size();
    for(int x=0;x<vertices.size();x++)
    {
        if(vertices[x].district>=maxDistrict)
        {
            maxDistrict=vertices[x].district;
        }
        if(vertices[x].adj.size()>=maxDistrict)
        {
            maxConnections=vertices[x].adj.size();
        }
    }
    if(maxDistrict==-1)
    {
        maxDistrict=0;
    }
    std::cout<<"Number of districts: "<<maxDistrict<<std::endl;
    std::vector<vertex> maxV;
    std::cout<<"Vertex with most connections: ";
    for(int x=0;x<vertices.size();x++)
    {
        if(vertices[x].adj.size()==maxConnections)
        {
            maxV.push_back(vertices[x]);
        }
    }
    for(int y=0;y<maxV.size();y++)
    {
        std::cout<<maxV[y].name;
        if(y!=maxV.size()-1)
        {
            std::cout<<" , ";
        }
    }
    std::cout<<std::endl;
    std::cout<<"with "<<maxConnections<<" connections"<<std::endl;
}

void verticesGraph::addVertex(std::string name)
{
    bool found=false;
    for(int x=0;x<vertices.size();x++)
    {
        if(vertices[x].name==name)
        {
            found=true;
            std::cout<<vertices[x].name<<" is found"<<std::endl;
        }
    }
    if(found==false)
    {
        vertex v;
        v.name=name;
        v.district=-1;
        v.visited=false;
        vertices.push_back(v);
    }
}

void verticesGraph::addEdge(std::string v1,std::string v2,int weight)
{
    for(int x=0;x<vertices.size();x++)
    {
        if(vertices[x].name==v1)
        {
            for(int y=0;y<vertices.size();y++)
            {
                if(vertices[y].name==v2&&x!=y)
                {
                    adjVertex av;
                    av.v=&vertices[y];
                    av.weight=weight;
                    vertices[x].adj.push_back(av);
                }
            }
        }
    }
}

void verticesGraph::snakesAndLadders()
{
    for(int x=0;x<vertices.size();x++)
    {
        vertices[x].position=x;
    }
    int playerPosition=0;
    int cppPosition=0;
    vertex player=vertices[playerPosition];
    vertex cpp=vertices[cppPosition];
    int maxNumber=6;
    int minNumber=1;
    int die=0;
    int goal=vertices.size()-1;
    srand(time(NULL));
    std::cout<<"Get to "<<goal+1<<" to win!"<<std::endl;
    std::cout<<"Press enter to continue"<<std::endl;
    std::cin.get();
    while(player.position<vertices[goal].position&&cpp.position<vertices[goal].position)
    {
        std::cout<<"You = "<<playerPosition+1<<" , "<<"Computer = "<<cppPosition+1;
        std::cin.get();
        die=rand()%maxNumber+minNumber;
        playerPosition+=die;
        if(playerPosition>=goal)
        {
            std::cout<<"You roll a "<<die<<",   "<<playerPosition-die+1<<" --> "<<playerPosition+1<<std::endl;
            std::cout<<"You get "<<playerPosition+1<<std::endl;
            std::cout<<"You win!"<<std::endl;
            return;
        }
        else
        {
            player=vertices[playerPosition];
            std::cout<<"You roll a "<<die<<",   "<<playerPosition-die+1<<" --> "<<playerPosition+1<<std::endl;
            if(player.adj.size()==1)
            {
                if(player.adj[0].weight==1&&player.position>player.adj[0].v->position)
                {
                    std::cout<<"You hit a snake! You will go to "<<player.adj[0].v->position+1<<std::endl;
                    playerPosition=player.adj[0].v->position;
                    player=vertices[playerPosition];
                }
                else if(player.adj[0].weight==2&&player.position<player.adj[0].v->position)
                {
                    std::cout<<"You hit a ladder! You will go to "<<player.adj[0].v->position+1<<std::endl;
                    playerPosition=player.adj[0].v->position;
                    player=vertices[playerPosition];
                }
            }
        }
        std::cin.get();
        die=rand()%maxNumber+minNumber;
        cppPosition+=die;
        if(cppPosition>=goal)
        {
            std::cout<<"Computer rolls a "<<die<<",   "<<cppPosition-die+1<<" --> "<<cppPosition+1<<std::endl;
            std::cout<<"Computer gets "<<cppPosition+1<<std::endl;
            std::cout<<"Computer wins!"<<std::endl;
            return;
        }
        else
        {
            cpp=vertices[cppPosition];
            std::cout<<"Computer rolls a "<<die<<",   "<<cppPosition-die+1<<" --> "<<cppPosition+1<<std::endl;
            if(cpp.adj.size()==1)
            {
                if(cpp.adj[0].weight==1&&cpp.position>cpp.adj[0].v->position)
                {
                    std::cout<<"Computer hits a snake! Computer will go to "<<cpp.adj[0].v->position+1;
                    cppPosition=cpp.adj[0].v->position;
                    cpp=vertices[cppPosition];
                    std::cout<<std::endl;
                }
                else if(cpp.adj[0].weight==2&&cpp.position<cpp.adj[0].v->position)
                {
                    std::cout<<"Computer hits a ladder! Computer will go to "<<cpp.adj[0].v->position+1;
                    cppPosition=cpp.adj[0].v->position;
                    cpp=vertices[cppPosition];
                    std::cout<<std::endl;
                }
            }
        }
        std::cout<<std::endl;
    }
}

int verticesGraph::findDistance(std::string v1,std::string v2)
{
    if(findVertex(v1)==NULL||findVertex(v2)==NULL)
    {
        return -1;
    }

    vertex * sta=findVertex(v1);
    vertex * des=findVertex(v2);
    if(sta->district!=des->district)
    {
        return -1;
    }
    int distance;
    for(int x=0;x<vertices.size();x++)
    {
        if(sta->adj[x].v==des)
        {
            distance=sta->adj[x].weight;
        }
    }
    return distance;
}

vertex * verticesGraph::findVertex(std::string name)
{
    for(int x=0;x<vertices.size();x++)
    {
        if(vertices[x].name==name)
        {
            return &vertices[x];
        }
    }
    return NULL;
}

vertex * verticesGraph::Dijkstra(std::string starting,std::string destination)
{
    vertex * startV=findVertex(starting);
    vertex * endV=findVertex(destination);
    startV->visited=true;
    startV->distance=0;
    std::vector<vertex *> solved;
    solved.push_back(startV);

    int minDistance,dist;
    vertex * solvedV, *s, *parent;
    while(endV->visited==false)
    {
        minDistance=INT_MAX;
        solvedV=NULL;
        for(int x=0;x<solved.size();x++)
        {
            s=solved[x];
            for(int y=0;y<s->adj.size();y++)
            {
                if(s->adj[y].v->visited==false)
                {
                    dist=s->distance+s->adj[y].weight;
                    if(dist<minDistance)
                    {
                        solvedV=s->adj[y].v;
                        minDistance=dist;
                        parent=s;
                    }
                }
            }
        }
        solvedV->distance=minDistance;
        solvedV->previous=parent;
        solvedV->visited=true;
        solved.push_back(solvedV);
    }

    for(int x=0;x<vertices.size();x++)
    {
        vertices[x].visited=false;
    }

    return endV;
}
