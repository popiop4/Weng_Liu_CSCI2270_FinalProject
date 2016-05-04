#include <iostream>
#include <stdio.h>
#include <string.h>
#include "graph.h"

using namespace std;

int main(int argc, char * argv[])
{
    graph cg(argv[1]);

    string option="0";
    string displayFile;
    if(argv[1]!=NULL)
    {
        displayFile=argv[1];
    }
    else
    {
        displayFile="NULL";
    }

    while(option=="0")
    {
        cout<<"---Main Menu---"<<endl;
        cout<<"You are using: "<<displayFile<<endl;
        cout<<"1. Display"<<endl;
        cout<<"2. Find shortest distance"<<endl;
        cout<<"3. Find a path"<<endl;
        cout<<"4. Play \"Room Escape\""<<endl;
        cout<<"5. Play \"Snakes and Ladders\""<<endl;
        cout<<"6. Enter in a new file to use"<<endl;
        cout<<"q. Quit"<<endl;
        cin>>option;
        if(option=="1")
        {
            cg.displayEverything();
            option="0";
        }
        else if(option=="2")
        {
            string starting,destination;
            cin.ignore();
            cout<<"Enter a starting vertex: "<<endl;
            getline(cin,starting);
            cout<<"Enter an ending vertex: "<<endl;
            getline(cin,destination);
            cg.shortestDistance(starting,destination);
            option="0";
        }
        else if(option=="3")
        {
            string starting,destination,limit;
            cin.ignore();
            cout<<"Enter a starting vertex: "<<endl;
            getline(cin,starting);
            cout<<"Enter a destination vertex: "<<endl;
            getline(cin,destination);
            cout<<"Enter a distance limit between two vertices: "<<endl;
            getline(cin,limit);
            cg.distanceLimitPath(starting,destination,limit);
            option="0";
        }
        else if(option=="4")
        {
            if(argv[1]!=NULL)
            {
                cout<<"Restart the program without using files"<<endl;
                option="0";
            }
            else
            {
                cg.buildRoomEscape();
                cg.roomEscape();
                return 0;
            }
        }
        else if(option=="5")
        {
            if(argv[1]==NULL&&displayFile == "NULL")
            {
                cout<<"Restart the program using a txt file for \"Snakes and Ladders\""<<endl;
                option="0";
            }

            else
            {
                cg.snakesAndLadders();
                option="0";
            }
        }
        else if(option =="6"){
            string filename;
            cin.ignore();
            cout<<"What file would you like to open?"<<endl;
            getline(cin,filename);

            char name[filename.size()+1];
            strcpy(name, filename.c_str());

            cg.~graph();
            cg.buildGraphFile(name);

            displayFile=filename;

            option ="0";


        }
        else if(option=="q")
        {
            return 0;
        }
        else
        {
            std::cout<<"Invalid input"<<std::endl;
            option="0";
        }
    }
    return 0;
}
