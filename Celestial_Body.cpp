/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CelestialBody_Simulate.cpp
 * Author: Michael DeMoor
 *
 * Created on May 2, 2017, 8:17 PM
 */

#include "TrackingSystem.h"

using namespace std;

Celestial_Body::Celestial_Body(const string Name, const float Mass, const Attributes Cel, const float Radius, Celestial* celestialptr){
    this->Name = Name;
    this->Mass = Mass;
    this->Radius = Radius;
    Values = Cel;
    Values.ID = celestialptr->SpaceCount;
    Status = "Intact";
    Celestial_Vec = celestialptr;
}

float Celestial_Body::RSmax = 0.0;
int Celestial_Body::SizeScale = 0;
float Celestial_Body::SpScale = 0.0;
int Celestial_Body::SpaceScale = 0;

void Celestial_Body::Display_Objects(){
    cout << "Displaying Objects: " << endl;
    for(Attributes i : Celestial_Vec->ObjectTracker){
        cout << "////////////////////////////////////////////////////////" << endl;
        print_attributes(i, Celestial_Vec->Celestial_Bodies[i.ID]->Name, Celestial_Vec->Celestial_Bodies[i.ID]->Mass, Celestial_Vec->Celestial_Bodies[i.ID]->Radius);
    }
    cout << "////////////////////////////////////////////////////////" << endl << endl;
}
void Celestial_Body::display_Object(){
    cout << Name << " Attributes: " << endl;
    if(Status == "Destroyed")//Status Use here/////////
        cout << "Sorry, " << Name << " was destroyed." << endl;     
    else
        print_attributes(Values, Name, Mass, Radius);
    cout << endl;
}

void Celestial_Body::print_attributes(Attributes V, string name, float Mass, float Radius){
    cout << "Name: " << name << endl;
    cout << "Mass (kg): " << Mass << endl;
    cout << "Current Position (m): [" << V.Rx << ", " << V.Ry << ", " << V.Rz << "]" << endl;
    cout << "Current Velocity (m/s): [" << V.Vx << ", " << V.Vy << ", " << V.Vz << "]" << endl;
    cout << "Radius (m): " << Radius << endl;   
}

void Celestial_Body::update_attributes(){
    Attributes V;
    bool Destroyed;
    for(CelestialPtr Body : Celestial_Vec->Celestial_Bodies){
        Destroyed = true;
        V = Body->Values;
        for(Attributes I : Celestial_Vec->ObjectTracker){
            if(V.ID == I.ID){
                Body->Values = I;
                Destroyed = false;
                break;
            }                
        }
        if(Destroyed){
            Body->Status = "Destroyed";
        }
    }
}

//Some Gets//
Attributes Celestial_Body::get_attributes(){
    return Values;
}

float Celestial_Body::get_Mass(){
    return Mass;
}

float Celestial_Body::get_Radius(){
    return Radius;
}

string Celestial_Body::get_Name(){
    return Name;
}

//Scale functions for the future//Will be used later in OpenGL//
void Celestial_Body::Set_Scale(){
    if(RScale < RSmax)
        RScale *= (pow(10, SizeScale)*Size_Constant);
    else
        RScale *= (pow(10, SizeScale));
}

void Celestial_Body::Set_Scalers(){//This code needs to be tested//    
    for(CelestialPtr Solar : Celestial_Vec->Celestial_Bodies){
        Solar->Set_Scale();
        if(Solar->RScale < Solar->Radius)
            Solar->RScale = Solar->Radius;
    }   
    for(CelestialPtr Solar : Celestial_Vec->Celestial_Bodies){
        if(Solar->Max)
            RSmax = Solar->RScale;
    }
    for(CelestialPtr Solar : Celestial_Vec->Celestial_Bodies){
        if(RSmax < Solar->RScale)
            Solar->RScale = RSmax;
    }    
}

void Celestial_Body::Inc_SpaceScale(){
    SpaceScale++;
    SpScale = pow(10,SpaceScale);
}

void Celestial_Body::Dec_SpaceScale(){
    SpaceScale--;
    SpScale = pow(10,SpaceScale);
}

void Celestial_Body::Inc_SizeScale(){
    SizeScale++;
    Set_Scalers();
}

void Celestial_Body::Dec_SizeScale(){
    SizeScale--;
    Set_Scalers();
}

void Celestial_Body::Initialize_Sim(vector<Attributes> &K1, vector<Attributes> &K1Copy, vector<Attributes> &K2, vector<Attributes> &K2Copy,
        vector<Attributes> &K3, vector<Attributes> &K3Copy, vector<Attributes> &K4, vector<Attributes> &K4Copy, vector<Attributes> &K5, 
        vector<Attributes> &K5Copy, vector<Attributes> &K6, vector<Attributes> &Input, vector<Attributes> &Result, vector<Attributes> &Correction, 
        vector<Attributes> &Tracker){
    //Prepare the simulation vectors by clearing them and deallocating all previously allocated memory//
    K1.clear();
    K1.shrink_to_fit();
    K1Copy.clear();
    K1Copy.shrink_to_fit();
    K2.clear();
    K2.shrink_to_fit();
    K2Copy.clear();
    K2Copy.shrink_to_fit();    
    K3.clear();
    K3.shrink_to_fit();
    K3Copy.clear();
    K3Copy.shrink_to_fit();    
    K4.clear();
    K4.shrink_to_fit();
    K4Copy.clear();
    K4Copy.shrink_to_fit();    
    K5.clear();
    K5.shrink_to_fit();
    K5Copy.clear();
    K5Copy.shrink_to_fit();    
    K6.clear();
    K6.shrink_to_fit();
    Input.clear();
    Input.shrink_to_fit();
    Result.clear();
    Result.shrink_to_fit();
    Correction.clear();
    Correction.shrink_to_fit();
    
    //Initialize each K vector 
    K1 = Tracker;
    K2 = Tracker;
    K3 = Tracker;
    K4 = Tracker;
    K5 = Tracker;
    K6 = Tracker;

    float Rmax = 0.0;
    for(CelestialPtr Solar : Celestial_Vec->Celestial_Bodies){
        Solar->RScale = Radius;
        if(Rmax < Solar->Radius)
            Rmax = Solar->Radius;
    }
    for(CelestialPtr Solar : Celestial_Vec->Celestial_Bodies)
        Solar->Size_Constant = Rmax/Radius;
    RSmax = Rmax;    
}

//Overload + operator to work across all the Attributes values in the vectors//
vector<Attributes>& operator+(vector<Attributes> &One, vector<Attributes> &Two){
    for(int i = 0; i<One.size(); i++){
        One[i].Rx = One[i].Rx + Two[i].Rx;
        One[i].Ry = One[i].Ry + Two[i].Ry;
        One[i].Rz = One[i].Rz + Two[i].Rz;
        One[i].Vx = One[i].Vx + Two[i].Vx;
        One[i].Vy = One[i].Vy + Two[i].Vy;
        One[i].Vz = One[i].Vz + Two[i].Vz;  
    }
    return One;
}
//Same with - operator//
vector<Attributes>& operator-(vector<Attributes> &One, vector<Attributes> &Two){
    for(int i = 0; i<One.size(); i++){
        One[i].Rx = One[i].Rx - Two[i].Rx;
        One[i].Ry = One[i].Ry - Two[i].Ry;
        One[i].Rz = One[i].Rz - Two[i].Rz;
        One[i].Vx = One[i].Vx - Two[i].Vx;
        One[i].Vy = One[i].Vy - Two[i].Vy;
        One[i].Vz = One[i].Vz - Two[i].Vz;  
    }
    return One;
}
//Same with * operator//
vector<Attributes>& operator*(const double C, vector<Attributes> &One){
    int num = 0;
    for(Attributes i : One){
        One[num].Rx = C*i.Rx;
        One[num].Ry = C*i.Ry;
        One[num].Rz = C*i.Rz;
        One[num].Vx = C*i.Vx;
        One[num].Vy = C*i.Vy;
        One[num].Vz = C*i.Vz;
        num++;
    }
    return One;
}

void Celestial_Body::Simulate_Motion(double Time,double h,double hmax,double hmin, double e){
vector<Attributes> K1, K1Copy, K2, K2Copy, K3, K3Copy, K4, K4Copy, K5, K5Copy, K6, Input, Result, Correction;
vector<Attributes> Tracker = Celestial_Vec->ObjectTracker;
double c[24]; c[0] = 0.25; c[1] = 3.0/32.0; c[2] = 9.0/32.0; c[3] = 1932.0/2197.0;
c[4]= 7200.0/2197.0; c[5] = 7296.0/2197.0; c[6] = 439.0/216.0; c[7] = 8.0;
c[8] = 3680.0/513.0; c[9] = 845.0/4104.0; c[10] = 8.0/27.0; c[11] = 2.0;
c[12] = 3544.0/2565.0; c[13] = 1859.0/4104.0; c[14] = 11.0/40.0; c[15] = 25.0/216.0;
c[16] = 1408.0/2565.0; c[17] = 2197.0/4104.0; c[18] = 0.2; c[19] = 16.0/135.0;
c[20] = 6656.0/12825.0; c[21] = 28561.0/56430.0; c[22] = 9.0/50.0; c[23] = 2.0/55.0; 
double R, D;
double hnext;
bool Trigger;
bool Last;
int Control, i, j, Size;
double Rad,Dist;
Attributes C, T;

cout << "Initial Values for Objects" << endl;
Display_Objects();
cout << endl;

double t = 0.0;
cout << "Simulating..." << endl;
is_collision(t, Tracker);//Check to see if any object starts inside another object//
Initialize_Sim(K1, K1Copy, K2, K2Copy, K3, K3Copy, K4, K4Copy, K5, K5Copy, K6, Input, Result, Correction, Tracker);//Initialize Simulation Vectors//

//Simulation Loop//
while(t<Time){//This code cannot be parallelized//
    Trigger = false;
    Last = false;
    //Runge-Kutta-Fehlberg Method. This is where the simulation magic happens.//
    while(true){
        if(Trigger)
            h = hnext;
        if(h == hmin)
            Last = true;
        K1 = RK_Functions(K1, Tracker);////////////Calculate K1/////////////////// 
        K1Copy = K1;
        Input = (c[0]*h)*K1 + Tracker;
        K2 = RK_Functions(K2, Input);///////////////Calculate K2/////////////////// 
        K2Copy = K2; K1 = K1Copy;          
        Input = (c[1]*h)*K1 + (c[2]*h)*K2 + Tracker;           
        K3 = RK_Functions(K3,Input);///////////////Calculate K3////////////////////
        K3Copy = K3; K1 = K1Copy; K2 = K2Copy;
        Input = (c[3]*h)*K1 - (c[4]*h)*K2 + (c[5]*h)*K3 + Tracker;                   
        K4 = RK_Functions(K4,Input);///////////////Calculate K4////////////////////
        K4Copy = K4; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy;
        Input = (c[6]*h)*K1 - (c[7]*h)*K2 + (c[8]*h)*K3 - (c[9]*h)*K4 + Tracker;
        K5 = RK_Functions(K5,Input);//////////////Calculate K5/////////////////////
        K5Copy = K5; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy; K4 = K4Copy; Result = Tracker;
        Input = Result - (c[10]*h)*K1 + (c[11]*h)*K2 - (c[12]*h)*K3 + (c[13]*h)*K4 - (c[14]*h)*K5;
        K6 = RK_Functions(K6,Input);///////////////Calculate K6////////////////////
        K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
        Result = (c[15]*h)*K1 + (c[16]*h)*K3 + (c[17]*h)*K4 - (c[18]*h)*K5 + Tracker;
        K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
        Correction = (c[19]*h)*K1 + (c[20]*h)*K3 + (c[21]*h)*K4 - (c[22]*h)*K5 + (c[23]*h)*K6 + Tracker;
        Correction = Correction - Result;
        R = Get_R(Correction, h);
        D = pow((e/(2*R)),0.25);
        hnext = D*h;
        if(hnext < hmin)
            hnext = hmin;
        if(R<=e)            
            break;
        if(Last)
            break;
        Trigger = true;        
    }
    if(hmax < hnext)
        hnext = hmax;
    t = t + h;
    h = hnext;
    Tracker = Result;
    
    //Check for collisions//
    i = 0;
    Size = Tracker.size();
    while(i < Size){//This code can be parallelized//
        Control = 1;
        C = Tracker[i];
        j = i + 1;
        while(j<Size){
            T = Tracker[j];
            Rad = Celestial_Vec->Celestial_Bodies[C.ID]->Radius + Celestial_Vec->Celestial_Bodies[T.ID]->Radius;
            Dist = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
            if(Dist<=Rad){//Collision Handling//
                if(Celestial_Vec->Celestial_Bodies[T.ID]->Mass <= Celestial_Vec->Celestial_Bodies[C.ID]->Mass){
                   cout << Celestial_Vec->Celestial_Bodies[T.ID]->Name << " crashed into " << Celestial_Vec->Celestial_Bodies[C.ID]->Name << " at time: " << t << endl << endl;
                   //Update the Attributes value for C using the resulting velocity from the collision momentum calculation//                   
                   C = Momentum(C,T);
                   Tracker[i] = C;
                   //Remove T and decrease size of vectors in algorithm by 1//
                   Tracker.erase(Tracker.begin() + j);
                   K1.erase(K1.begin() + j); K1Copy.pop_back(); K2Copy.pop_back(); K2.erase(K2.begin() + j); K3.erase(K3.begin() + j); K3Copy.pop_back();K4.erase(K4.begin() + j);
                   K4Copy.pop_back(); K5.erase(K5.begin() + j); K5Copy.pop_back(); K6.erase(K6.begin() + j); Input.pop_back(); Result.pop_back(); Correction.pop_back();
                   Size = Tracker.size(); 
                }
                else{
                   cout << Celestial_Vec->Celestial_Bodies[C.ID]->Name << " crashed into " << Celestial_Vec->Celestial_Bodies[T.ID]->Name << " at time: " << t << endl << endl;
                   //Update the Attributes value for T using the resulting velocity from the collision momentum calculation//
                   T = Momentum(T,C);
                   Tracker[j] = T;
                   //Remove C and decrease size of vectors in algorithm by 1//
                   Tracker.erase(Tracker.begin() + i);
                   K1.erase(K1.begin() + i); K1Copy.pop_back(); K2Copy.pop_back(); K2.erase(K2.begin() + i); K3.erase(K3.begin() + i); K3Copy.pop_back();K4.erase(K4.begin() + i);
                   K4Copy.pop_back(); K5.erase(K5.begin() + i); K5Copy.pop_back(); K6.erase(K6.begin() + i); Input.pop_back(); Result.pop_back(); Correction.pop_back();
                   Size = Tracker.size();
                   Control = 0;
                   break;
                }                 
            }
            else
               j++;
        }
        if(Control)
           i++;
    } 
}
Celestial_Vec->ObjectTracker.clear();
Celestial_Vec->ObjectTracker = Tracker;
update_attributes();
cout << "Final Simulation Result:" << endl;
Display_Objects();
}

double Celestial_Body::Get_R(const vector<Attributes>& Correction, const double h){
    double R = 0.0;
    double M1,M2;
    //Find and obtain the largest magnitude out of every position and velocity vector in the entire list//Max Norm 
    for(Attributes V : Correction){
        M1 = sqrt(pow(V.Rx,2) + pow(V.Ry,2) + pow(V.Rz,2));
        M2 = sqrt(pow(V.Vx,2) + pow(V.Vy,2) + pow(V.Vz,2));
        if(R < M1 || R < M2)
            R = (M1<M2)? M2:M1;
    }
    R = R/h;
    return R;//Return the result divided by the timestep//
}

vector<Attributes>& Celestial_Body::RK_Functions(vector<Attributes> &Temp, vector<Attributes> &Alt){//This code can be parallelized// 
    Position Result;
    //Implementing the functions calculating the components of the K values in the RKF45 algorithm// 
    for(int Num = 0; Num<Alt.size(); Num++){        
        Result = Acceleration(Alt, Num);
        Temp[Num].Vx = G*Result.Rx;
        Temp[Num].Vy = G*Result.Ry;
        Temp[Num].Vz = G*Result.Rz;
        Temp[Num].Rx = Alt[Num].Vx;
        Temp[Num].Ry = Alt[Num].Vy;
        Temp[Num].Rz = Alt[Num].Vz;
    }
    return Temp;
}


Position operator+(const Position &A, const Position &B){
    Position C;
    C.Rx = A.Rx + B.Rx;
    C.Ry = A.Ry + B.Ry;
    C.Rz = A.Rz + B.Rz;
    return C;
}

Position operator-(const Position &A, const Position &B){
    Position C;
    C.Rx = A.Rx - B.Rx;
    C.Ry = A.Ry - B.Ry;
    C.Rz = A.Rz - B.Rz;
    return C;
}


Position Celestial_Body::Acceleration(vector<Attributes>& Alt, int Num){
    int Curr = 0;
    Position Rnum, Rcurr, Result;
    //Initialize Position Data Structures: Result and Rnum//
    Result.Rx = 0.0;
    Result.Ry = 0.0;
    Result.Rz = 0.0;
    Rnum.Rx = Alt[Num].Rx;
    Rnum.Ry = Alt[Num].Ry;
    Rnum.Rz = Alt[Num].Rz;
    //Calculate the acceleration of Object Num due to the gravitational forces of all other objects// 
    while(Curr<Alt.size()){
        if(Curr != Num){
            Rcurr.Rx = Alt[Curr].Rx;
            Rcurr.Ry = Alt[Curr].Ry;
            Rcurr.Rz = Alt[Curr].Rz;
            Result = Result + Calculations(Rnum, Rcurr, Celestial_Vec->Celestial_Bodies[Alt[Curr].ID]->Mass);
            }
        Curr++;
    }       
    return Result;
}


Position Celestial_Body::Calculations(Position &Num,Position &Curr,const float Mass){
    Position R;
    double Distance;
    //Calculating the acceleration of Object Num due to the gravitational force from Object Curr on Object Num//
    R = Curr - Num;
    Distance = Distance_Calc(R.Rx,R.Ry,R.Rz);
    Distance = pow(Distance,3);
    R.Rx = (Mass/Distance)*R.Rx;
    R.Ry = (Mass/Distance)*R.Ry;
    R.Rz = (Mass/Distance)*R.Rz;
    return R;
}

double Celestial_Body::Distance_Calc(const double X, const double Y,const double Z){
   return sqrt(X*X + Y*Y + Z*Z);//Return the magnitude of a vector//    
}

Attributes Celestial_Body::Momentum(Attributes One, Attributes Two){
    double M1,M2;
    float Mass1 = Celestial_Vec->Celestial_Bodies[One.ID]->Mass;
    float Mass2 = Celestial_Vec->Celestial_Bodies[Two.ID]->Mass;
    //Calculating the resulting velocity of Object One due to the inelastic collision of Object Two by using conservation of momentum//
    M1 = Mass1/(Mass1 + Mass2);
    M2 = Mass2/(Mass1 + Mass2);
    One.Vx = M1*One.Vx + M2*Two.Vx;
    One.Vy = M1*One.Vy + M2*Two.Vy;
    One.Vz = M1*One.Vz + M2*Two.Vz;
    Celestial_Vec->Celestial_Bodies[One.ID]->Mass = Mass1 + Mass2;
    return One;    
}

void Celestial_Body::is_collision(const double t, vector<Attributes> &Tracker){
     int Control;
     double R,D;
     Attributes C, T;
     int i = 0;
     int j;
     int Size = Tracker.size();
     while(i < Size){
         Control = 0;
         C = Tracker[i];
         j = i + 1;
         while(j<Size){
             T = Tracker[j];
             R = Celestial_Vec->Celestial_Bodies[C.ID]->Radius + Celestial_Vec->Celestial_Bodies[T.ID]->Radius;
             D = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
             if(D<=R){
                 if(Celestial_Vec->Celestial_Bodies[T.ID]->Mass <= Celestial_Vec->Celestial_Bodies[C.ID]->Mass){
                    cout << Celestial_Vec->Celestial_Bodies[T.ID]->Name << " crashed into " << Celestial_Vec->Celestial_Bodies[C.ID]->Name << " at time: " << t << endl << endl;
                    //Update the Attributes value for C using the resulting velocity from the collision momentum calculation//                   
                    C = Momentum(C,T);
                    Tracker[i] = C;
                    //Remove T//
                    Tracker.erase(Tracker.begin() + j);
                    Size = Tracker.size(); 
                 }
                 else{
                    cout << Celestial_Vec->Celestial_Bodies[C.ID]->Name << " crashed into " << Celestial_Vec->Celestial_Bodies[T.ID]->Name << " at time: " << t << endl << endl;
                    //Update the Attributes value for T using the resulting velocity from the collision momentum calculation//
                    T = Momentum(T,C);
                    Tracker[j] = T;
                    //Remove C//
                    Tracker.erase(Tracker.begin() + i);
                    Size = Tracker.size();
                    Control = 1;
                    break;
                 }                 
             }
             else
                j++;
         }
         if(Control == 0)
            i++;
     }        
}

string Get_Status(CelestialPtr Celestial){
    return Celestial->Status;
}

//double no_nonsense();//Under Construction//Create Mass to Radius Correspondence algorithm
