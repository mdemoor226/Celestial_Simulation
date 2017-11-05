/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "TrackingSystem.h"

using namespace std;

const char* All_Colors[7] = {"Red", "Blue", "Green", "Ice", "Purple", "Orange", "Brown"};//Will Probably Change in the future

Star::Star(const std::string name, float mass, Attributes values, float radius, Celestial* celestialptr) : Celestial_Body(name, mass, values, radius, celestialptr){
    Brightness = Brightness_Calc(mass);    
    Celestial_Vec->Lightvector.push_back(Brightness);
    if(mass <= 10000000000)
        Color = "Red";
    else if(mass <= 250000000000000000000000000.0)
        Color = "Yellow";
    else
        Color = "Blue";
    
    Bright_ID = Celestial_Vec->StarCount;
    values.ID = Celestial_Vec->SpaceCount;
    Celestial_Vec->ObjectTracker.insert(Celestial_Vec->ObjectTracker.begin() + Celestial_Vec->StarCount/*+1?*/, values);
    Celestial_Vec->StarCount++;
    Celestial_Vec->SpaceCount++;
}

bool Star::Modified = false;

void Star::Set_Mass(float mass){
    Mass = mass;    
    if(mass <= 10000000000)
        Color = "Red";
    else if(mass <= 250000000000000000000000000.0)
        Color = "Yellow";
    else
        Color = "Blue";    
    Brightness = Brightness_Calc(mass);
    Celestial_Vec->Lightvector[Bright_ID] = Brightness;
}

void Star::Remove(){
    if(!Modified){
        Modified = true;
        for(int i=Bright_ID + 1; i<Celestial_Vec->StarCount; i++){
            Celestial_Vec->Celestial_Bodies[Celestial_Vec->ObjectTracker[i].ID]->Remove();
        }        
        Celestial_Vec->Lightvector.erase(Celestial_Vec->Lightvector.begin() + Bright_ID);
        Celestial_Vec->ObjectTracker.erase(Celestial_Vec->ObjectTracker.begin() + Bright_ID);//remove_element(Bright_ID);
        Celestial_Vec->StarCount--;
        Celestial_Vec->SpaceCount--;
        Modified = false;
    }
    else{
        Bright_ID--;
    }
}

float Star::Brightness_Calc(float &mass){
    //Develop Algorithm to relate Mass to Brightness//
    return 0.0;
}

/* To be implemented later in OpenGL////////////////////////////////////////
void Star::Draw(){
 * 
 * 
 * 
 *
 *    
}
*///////////////////////////////////////////////////////////////////////

Non_Star::Non_Star(const string name, float mass, Attributes values, float radius, Celestial* celestialptr) : Celestial_Body(name, mass, values, radius, celestialptr){
    values.ID = Celestial_Vec->SpaceCount;
    vector<Attributes> Test = Celestial_Vec->ObjectTracker;
    Celestial_Vec->ObjectTracker.push_back(values);
    Test = Celestial_Vec->ObjectTracker;
    Celestial_Vec->SpaceCount++;
}

void Non_Star::Set_Mass(float mass){
    Mass = mass;
}

void Non_Star::Remove(){
    int Count = 0;
    for(Attributes Track : Celestial_Vec->ObjectTracker){
        if(Track.ID == Values.ID){
            Celestial_Vec->ObjectTracker.erase(Celestial_Vec->ObjectTracker.begin() + Count);//remove_element(Count);
            break;
        }
        Count++;
    }
    Celestial_Vec->SpaceCount--;
}

Black_Hole::Black_Hole(const string name, float mass, Attributes values, float radius, Celestial* celestialptr) : Non_Star(name, mass, values, radius, celestialptr){
    Color = "Black";
}

/* To be implemented later in OpenGL///////////////////////////////////////////
void Black_Hole::Draw(){//Light does not reflect off of or illuminate Black Hole//
 * 
 * 
 * 
 *
 *    
}
*//////////////////////////////////////////////////////////////////////////////

Planet::Planet(std::string name, float mass, Attributes values, float radius, Celestial* celestialptr) : Non_Star(name, mass, values, radius, celestialptr){
    int New;
    if(rand() % 2 == 0){//Mixed Colors
        int Num = (rand() % 4) + 1;//Number of Colors (4 max)//This is arbitrary and can be changed if needed//
        int Nums[Num];
        memset(Nums, -1, sizeof(Nums));
        int i = 0;
        bool Diff;
        while(i < Num){
            New = rand() % 7;
            Diff = true;
            for(int j=0; j<Num; j++){
                if(Nums[j] == New){
                    Diff = false;
                    break;
                }
            }
            if(Diff){
                Nums[i] = New;
                i++;
            }
        }
        for(i=0; i<Num; i++)
            Colors.push_back(All_Colors[Nums[i]]);      
    }
    else{//Single Static Color
        New = rand() % 7;
        Colors.push_back(All_Colors[New]);
    }
}

/* To be implemented later in OpenGL///////////////////////////////////////////
void Planet::Draw(){//Light Reflects off of and illuminates Planet//
 * 
 * 
 * 
 *
 *    
}
*//////////////////////////////////////////////////////////////////////////////

Moon::Moon(string name, float mass, Attributes values, float radius, Celestial* celestialptr) : Non_Star(name, mass, values, radius, celestialptr){
    Color = "White";
}

/* To be implemented later in OpenGL///////////////////////////////////////////
void Moon::Draw(){//Light affects Moon//
 * 
 * 
 * 
 *
 *    
}
*//////////////////////////////////////////////////////////////////////////////

Other::Other(string name, float mass, Attributes values, float radius, Celestial* celestialptr) : Non_Star(name, mass, values, radius, celestialptr){
    Color = "Brown";
}

/* To be implemented later in OpenGL///////////////////////////////////////////
void Other::Draw(){//Light affects Other//
 * 
 * 
 * 
 *
 *    
}
*//////////////////////////////////////////////////////////////////////////////