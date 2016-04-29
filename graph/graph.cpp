#include "graph.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <climits>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

graph::graph(char * fileName)
{
    buildGraphFile(fileName);
};

graph::graph(){};

void graph::buildGraphFile(char * fileName)
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

void graph::assignDistricts()
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

void graph::BFTraversalLabel(std::string starting,int distID)
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

void graph::displayEverything()
{
    assignDistricts();
    for(int x=0;x<vertices.size();x++)
    {
        std::cout<<"District: "<<vertices[x].district<<", "<<vertices[x].name<<"-->";
        for(int y=0;y<vertices[x].adj.size();y++)
        {
            std::cout<<vertices[x].adj[y].v->name;
            if(y!=vertices[x].adj.size()-1)
            {
                std::cout<<", ";
            }
        }
        std::cout<<std::endl;
    }
}

void graph::shortestDistance(std::string starting,std::string destination)
{
    if(findVertex(starting)==NULL||findVertex(destination)==NULL)
    {
        std::cout<<"At least one vertex doesn't exist"<<std::endl;
        return;
    }

    assignDistricts();
    vertex * sta=findVertex(starting);
    vertex * des=findVertex(destination);
    if(sta->district!=des->district)
    {
        std::cout<<"No path between two vertices"<<std::endl;
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

void graph::distanceLimitPath(std::string starting,std::string destination,std::string limit)
{
    if(findVertex(starting)==NULL||findVertex(destination)==NULL)
    {
        std::cout<<"At least one vertex doesn't exist"<<std::endl;
        return;
    }

    assignDistricts();
    vertex * sta=findVertex(starting);
    vertex * des=findVertex(destination);
    if(sta->district!=des->district)
    {
        std::cout<<"No path between two vertices"<<std::endl;
        return;
    }

    std::stringstream ss(limit);
    int d;
    if(!(ss>>d))
    {
        std::cout<<"Invalid distance limit"<<std::endl;
        return;
    }
    else
    {
        ss>>d;
    }

    if(d<=0)
    {
        std::cout<<"Distance limit must be greater than zero"<<std::endl;
        return;
    }

    int previous[vertices.size()];
    for(int x=0;x<vertices.size();x++)
    {
        previous[x]=-1;
        vertices[x].ID=x;
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
                if(v->adj[x].weight<=d)
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
            std::cout<<path[x]->name<<" >>> "<<findAdjVerticesDistance(path[x]->name,path[x-1]->name)<<" >>> ";
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

void graph::addVertex(std::string name)
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

void graph::addEdge(std::string v1,std::string v2,int weight)
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

void graph::snakesAndLadders()
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
    std::srand(time(0));
    std::cout<<"Get to "<<goal+1<<" to win!"<<std::endl;
    std::cout<<"Press enter to continue"<<std::endl;
    std::cin.get();
    while(player.position!=14&&cpp.position!=14)
    {
        std::cout<<"----------"<<std::endl;
        std::cout<<"You   "<<playerPosition+1<<" , "<<"Computer   "<<cppPosition+1;
        std::cin.get();
        die=rand()%maxNumber+minNumber;
        playerPosition+=die;
        if(playerPosition==goal)
        {
            std::cout<<"You roll a "<<die<<",   "<<"You   "<<playerPosition+1<<std::endl;
            std::cout<<"You get "<<playerPosition+1<<std::endl;
            std::cout<<"You win!"<<std::endl;
            return;
        }
        else if(playerPosition<goal)
        {
            player=vertices[playerPosition];
            std::cout<<"You roll a "<<die<<",   "<<"You   "<<playerPosition+1<<std::endl;
            if(player.adj.size()==1)
            {
                if(player.adj[0].weight==1&&player.position>player.adj[0].v->position)
                {
                    std::cout<<"You hit a snake! You   "<<player.adj[0].v->position+1<<std::endl;
                    playerPosition=player.adj[0].v->position;
                    player=vertices[playerPosition];
                }
                else if(player.adj[0].weight==2&&player.position<player.adj[0].v->position)
                {
                    std::cout<<"You hit a ladder! You   "<<player.adj[0].v->position+1<<std::endl;
                    playerPosition=player.adj[0].v->position;
                    player=vertices[playerPosition];
                }
            }
        }
        else
        {
            playerPosition=0;
            std::cout<<"You roll a "<<die<<",   "<<"You   "<<0<<std::endl;
        }
        std::cin.get();
        die=rand()%maxNumber+minNumber;
        cppPosition+=die;
        if(cppPosition==goal)
        {
            std::cout<<"Computer rolls a "<<die<<",   "<<"Computer   "<<cppPosition+1<<std::endl;
            std::cout<<"Computer gets "<<cppPosition+1<<std::endl;
            std::cout<<"Computer wins!"<<std::endl;
            return;
        }
        else if(cppPosition<goal)
        {
            cpp=vertices[cppPosition];
            std::cout<<"Computer rolls a "<<die<<",   "<<"Computer   "<<cppPosition+1<<std::endl;
            if(cpp.adj.size()==1)
            {
                if(cpp.adj[0].weight==1&&cpp.position>cpp.adj[0].v->position)
                {
                    std::cout<<"Computer hits a snake! Computer   "<<cpp.adj[0].v->position+1;
                    cppPosition=cpp.adj[0].v->position;
                    cpp=vertices[cppPosition];
                    std::cout<<std::endl;
                }
                else if(cpp.adj[0].weight==2&&cpp.position<cpp.adj[0].v->position)
                {
                    std::cout<<"Computer hits a ladder! Computer   "<<cpp.adj[0].v->position+1;
                    cppPosition=cpp.adj[0].v->position;
                    cpp=vertices[cppPosition];
                    std::cout<<std::endl;
                }
            }
        }
        else
        {
            cppPosition=0;
            std::cout<<"Computer rolls a "<<die<<",   "<<"Computer   "<<0<<std::endl;
        }
        std::cout<<std::endl;
    }
}

void graph::buildRoomEscape()
{
    std::string places[15]={"Room 1","Room 2","Room 3","Room 4","Room 5","Room 6","Room 7","Room 8","Room 9","Room 10","Room 11","Room 12","Room 13","Room 14","Room 15"};
    std::srand(time(0));
    for(int x=0;x<15;x++)
    {
        addVertex(places[x]);
    }
    std::vector<int> l1=randomNumberGenerate();
    std::vector<int> l2=randomNumberGenerate();
    std::vector<int> l3=randomNumberGenerate();
    for(int y=0;y<l1.size();y++)
    {
        if(l1[y]!=l2[y])
        {
            addEdge(places[l1[y]],places[l2[y]],1);
            addEdge(places[l2[y]],places[l1[y]],1);
        }
    }
    for(int z=0;z<vertices.size();z++)
    {
        if(vertices[z].adj.size()==0)
        {
            addEdge(vertices[z].name,places[l3[z]],1);
            addEdge(places[l3[z]],vertices[z].name,1);
            addEdge(vertices[z].name,places[l1[z]],1);
            addEdge(places[l3[z]],vertices[z].name,1);
        }
    }
}

std::vector<int> graph::randomNumberGenerate()
{
    std::vector<int> numbers;
    for(int x=0;x<15;x++)
    {
        numbers.push_back(x);
    }
    std::random_shuffle(numbers.begin(),numbers.end());
    return numbers;
}

int graph::findAdjVerticesDistance(std::string v1,std::string v2)
{
    assignDistricts();
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

void graph::roomEscape()
{
    std::cout<<"----------"<<std::endl;
    std::cout<<"Enter p to exit"<<std::endl;
    std::cout<<"You are trapped in a spooky place with 15 rooms. (Room 1, Room 2, ...)"<<std::endl;
    std::cout<<"You want to escape"<<std::endl;
    std::srand(time(0));
    int rn1=rand()%14+0;
    vertex * player=findVertex(vertices[rn1].name);
    std::cout<<"You look up, and find you are in "<<player->name<<std::endl;
    std::cout<<"Enter the room number you want to go"<<std::endl;
    std::string key,choice;
    std::cin.ignore();
    getline(std::cin,choice);
    std::stringstream ss(choice);
    int number;
    ss>>number;
    while(number-1==rn1||number<1||number>15)
    {
        if(choice=="p")
        {
            return;
        }
        std::cout<<"Invalid input"<<std::endl;
        std::cout<<"Enter the room number you want to go"<<std::endl;
        getline(std::cin,choice);
        std::stringstream ss(choice);
        ss>>number;
    }
    vertex * goal=findVertex(vertices[number-1].name);
    std::cout<<"You chose: "<<goal->name<<std::endl;
    int rn2=rand()%5+5;
    std::cout<<"You have "<<rn2<<" moves left to escape the place"<<std::endl;

    std::cout<<std::endl;
    while(player->name!=goal->name&&rn2!=0)
    {
        std::cout<<"You are at "<<player->name<<std::endl;
        int adjSize=player->adj.size();
        if(adjSize==2)
        {
            std::cout<<"q: "<<player->adj[0].v->name<<std::endl;
            std::cout<<"w: "<<player->adj[1].v->name<<std::endl;
        }
        else if(adjSize==3)
        {
            std::cout<<"q: "<<player->adj[0].v->name<<std::endl;
            std::cout<<"w: "<<player->adj[1].v->name<<std::endl;
            std::cout<<"e: "<<player->adj[2].v->name<<std::endl;
        }
        else if(adjSize==4)
        {
            std::cout<<"q: "<<player->adj[0].v->name<<std::endl;
            std::cout<<"w: "<<player->adj[1].v->name<<std::endl;
            std::cout<<"e: "<<player->adj[2].v->name<<std::endl;
            std::cout<<"r: "<<player->adj[3].v->name<<std::endl;
        }
        std::cout<<"Enter q/w/e/r"<<std::endl;
        getline(std::cin,key);
        if(key=="p")
        {
            return;
        }
        if(key=="q")
        {
            player=findVertex(player->adj[0].v->name);
            rn2-=1;
            std::cout<<"You have "<<rn2<<" moves left!"<<std::endl;
        }
        else if(key=="w")
        {
            player=findVertex(player->adj[1].v->name);
            rn2-=1;
            std::cout<<"You have "<<rn2<<" moves left!"<<std::endl;
        }
        else if(key=="e"&&player->adj.size()>=3)
        {
            player=findVertex(player->adj[2].v->name);
            rn2-=1;
            std::cout<<"You have "<<rn2<<" moves left!"<<std::endl;
        }
        else if(key=="r"&&player->adj.size()==4)
        {
            player=findVertex(player->adj[3].v->name);
            rn2-=1;
            std::cout<<"You have "<<rn2<<" moves left!"<<std::endl;
        }
        else
        {
            std::cout<<"Invalid input"<<std::endl;
        }
        std::cout<<"----------"<<std::endl;
    }
    if(player->name==goal->name&&rn2>=0)
    {
        std::cout<<"You successfully escaped the place! You win!"<<std::endl;
    }
    else
    {
        std::cout<<"You failed"<<std::endl;
    }
    std::cout<<"----------"<<std::endl;
}

vertex * graph::findVertex(std::string name)
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

vertex * graph::Dijkstra(std::string starting,std::string destination)
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
