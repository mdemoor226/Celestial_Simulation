/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <time.h>
#include "TrackingSystem.h"

using namespace std;

void Save_Sim(vector<Celestial> &Sims, Celestial &Active){
    int Count = 0;
    for(Celestial Sim : Sims){
        if(Sim.get_name() == Active.get_name()){
            Sims[Count] = Active;
            break;
        }
        Count++;
    }
    cout << "Saved" << endl;
}

void Load_Sim(vector<Celestial> &Sims, Celestial &Active){
    cout << "Warning, loading in a new simulation will overwrite the current one. Would you like to save the current one first?\n";
    string response = yes_no();
    if(response == "yes")Save_Sim(Sims, Active);
    
    cout << "Enter the name of the simulation you would like to load.\n";
    cin >> response;
    bool fake = true;
    for(Celestial Sim : Sims){
        if(Sim.get_name() == response){
            Active = Sim;
            fake = false;
            break;
        }
    }
    if(fake) cout << "Error, simulation profile does not exist." << endl;
    else cout << "Done." << endl;
}

void Add_Sim(vector<Celestial> &Sims){
    cout << "What is the name of the new simulation.\n";
    string Name;
    cin >> Name;
    Sims.push_back(Celestial(Name));
    cout << "Done." << endl;
}

void Remove_Sim(vector<Celestial> &Sims){
    cout << "Enter the name of the simulation you would like to remove.\n";
    string Name;
    cin >> Name;
    bool fake = true;
    int Count = 0;
    for(Celestial Sim : Sims){
        if(Sim.get_name() == Name){
            Sims.erase(Sims.begin() + Count);
            fake = false;
            break;
        }
        Count++;
    }
    if(fake) cout << "Error, simulation profile does not exist." << endl;
    else cout << "Done." << endl;
}

void View_Sims(vector<Celestial> &Sims){
    cout << "Current Simulations:\n";
    int Count = 1;
    for(Celestial Sim : Sims){
        cout << "Simulation #" << Count << ": " << Sim.get_name() << "\n";
        Count++;
    }
}

int main(){
    srand(static_cast<unsigned int>(time(0)));  
    Celestial Sim_Init("Default");
    Celestial Active_Sim = Sim_Init; 
    Init_Sim(&Active_Sim);
    vector<Celestial> Sims = {Active_Sim};  
    
    string In;
    cout << "Welcome to the Celestial Animator." << endl;  
    while(true){
        cin >> In;
        In = str_lower(In);
       
        if(In == "add")
            Active_Sim.Add_Object(&Active_Sim);        
        else if(In == "settings")
            Active_Sim.Change_Settings();
        else if(In == "remove")
            Active_Sim.Remove_Object();
        else if(In == "display")
            Active_Sim.View_Object();
        else if(In == "displayall")
            Active_Sim.View_Objects();
        else if(In == "alter")
            Active_Sim.Alter_Object();
        else if(In == "simulate")
            Active_Sim.Simulate();
        else if(In == "load")
            Load_Sim(Sims, Active_Sim);
        else if(In == "save")
            Save_Sim(Sims, Active_Sim);
        else if(In == "new")
            Add_Sim(Sims);
        else if(In == "delete")
            Remove_Sim(Sims);
        else if(In == "view")
            View_Sims(Sims);
        else if(In == "help")
            cout << "View       : Display a list of current simulation profiles.\nNew        : Create a new simulation profile.\n"
                    "Delete     : Delete a simulation profile.\nSave       : Save the current simulation profile.\nLoad       : Load in a different simulation profile.\n"
                    "Add        : Add objects to the system.\nRemove     : Remove an object from the system.\n"
                    "Alter      : Change the values/attributes of an object.\nDisplay    : Display the attributes of a specific object.\n"
                    "Displayall : Display the attributes of every object.\nSimulate   : Run a simulation."
                    "\nSettings   : View and alter the Orbit Customizer Settings.\nHelp       : List the commands.\nQuit       : Exit the program.\n" << endl;                 
        else if(In == "quit")
            break;
        else
            cout << "Error, Invalid input. Type 'Help' for a list of commands." << endl;
        cout << endl;
    }
}


