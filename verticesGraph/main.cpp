#include <iostream>
#include "verticesGraph.h"

using namespace std;

int main(int argc, char * argv[])
{
    verticesGraph cg(argv[1]);

    string option="-1";

    while(option=="-1")
    {
        cout<<"----------"<<endl;
        cout<<"Main Menu"<<endl;
        cout<<"----------"<<endl;
        cout<<"1. Play \"Mansion Escape\""<<endl;
        cout<<"2. Play \"Snakes and Ladders\""<<endl;
        cout<<"3. Graph operations"<<endl;
        cout<<"0. Quit"<<endl;
        cin>>option;
        if(option=="1")
        {
            if(argv[1]!=NULL)
            {
                cout<<"Please restart the program without using files"<<endl;
                option="-1";
            }
            else
            {
                cg.buildMansionEscape();
                cg.mansionEscape();
                return 0;
            }
        }
        else if(option=="2")
        {
            if(argv[1]==NULL)
            {
                cout<<"Please restart the program using a txt file for \"Snakes and Ladders\""<<endl;
                option="-1";
            }

            else
            {
                cg.snakesAndLadders();
                option="-1";
            }
        }
        else if(option=="3")
        {
            option="-3";
            while(option=="-3")
            {
                cout<<endl;
                cout<<"----------"<<endl;
                cout<<"1. Assign districts"<<endl;
                cout<<"2. Display"<<endl;
                cout<<"3. Find distance"<<endl;
                cout<<"4. Find shortest distance"<<endl;
                cout<<"5. Find a path with limited distance"<<endl;
                cout<<"0. Back"<<endl;
                cin>>option;
                if(option=="1")
                {
                    cg.assignDistricts();
                    option="-3";
                }
                else if(option=="2")
                {
                    cg.displayEverything();
                    option="-3";
                }
                else if(option=="3")
                {
                    string starting,destination;
                    cin.ignore();
                    cout<<"Enter a starting vertex: "<<endl;
                    getline(cin,starting);
                    cout<<"Enter an ending vertex: "<<endl;
                    getline(cin,destination);
                    cout<<cg.findDistance(starting,destination)<<endl;
                    option="-3";
                }
                else if(option=="4")
                {
                    string starting,destination;
                    cin.ignore();
                    cout<<"Enter a starting vertex: "<<endl;
                    getline(cin,starting);
                    cout<<"Enter an ending vertex: "<<endl;
                    getline(cin,destination);
                    cg.shortestDistance(starting,destination);
                    option="-3";
                }
                else if(option=="5")
                {
                    string starting,destination;
                    int distance;
                    cin.ignore();
                    cout<<"Enter a starting vertex: "<<endl;
                    getline(cin,starting);
                    cout<<"Enter a destination vertex: "<<endl;
                    getline(cin,destination);
                    cout<<"Enter distance limit: "<<endl;
                    cin>>distance;
                    cg.limitedDistancePath(starting,destination,distance);
                    option="-3";
                }
                else if(option=="0")
                {
                    option="-1";
                }
            }
        }
        else if(option=="0")
        {
            return 0;
        }
    }
    return 0;
}
