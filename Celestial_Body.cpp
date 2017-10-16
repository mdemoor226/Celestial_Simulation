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

vector<CelestialPtr> Celestial_Bodies;//Keep Global//
Celestial_Body::Celestial_Body(const string Name, const float Mass, const Attributes Celestial, const float Radius){
    this->Name = Name;
    this->Mass = Mass;
    this->Radius = Radius;
    Values.ID = SpaceCount;
    Values.Rx = Celestial.Rx;
    Values.Ry = Celestial.Ry;
    Values.Rz = Celestial.Rz;
    Values.Vx = Celestial.Vx;
    Values.Vy = Celestial.Vy;
    Values.Vz = Celestial.Vz;
    Status = "Intact";//Status Set Here//
}

vector<Attributes> Celestial_Body::ObjectTracker;
vector<float> Celestial_Body::Lightvector;
int Celestial_Body::SpaceCount = 0;
float Celestial_Body::RSmax = 0.0;
int Celestial_Body::SizeScale = 0;
float Celestial_Body::SpScale = 0.0;
int Celestial_Body::SpaceScale = 0;

void Celestial_Body::Display_Objects(){
    cout << "Displaying Objects: " << endl;
    for(Attributes i : ObjectTracker){
        cout << "////////////////////////////////////////////////////////" << endl;
        print_attributes(i, Celestial_Bodies[i.ID]->Name, Celestial_Bodies[i.ID]->Mass, Celestial_Bodies[i.ID]->Radius);
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
    for(CelestialPtr Body : Celestial_Bodies){
        Destroyed = true;
        V = Body->Values;
        for(Attributes I : ObjectTracker){
            if(V.ID == I.ID){
                Body->Values = I;
                Destroyed = false;
                break;
            }                
        }
        if(Destroyed){
            Body->Status = "Destroyed";//Status set here///////////////
        }
    }
}

//Sets and Gets//
Attributes Celestial_Body::get_attributes(){
    return Values;
}

void Celestial_Body::set_attributes(Attributes Set){
    Values = Set;
}

void Celestial_Body::set_Name(const string name){
    Name = name;
}

void Celestial_Body::set_ObjectTracker(int Place, Attributes Set){
    ObjectTracker[Place] = Set;
}

void Celestial_Body::set_Radius(float radius){
    Radius = radius;
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

Attributes Celestial_Body::get_ObjectTracker(int Place){
    return ObjectTracker[Place];
}

//Scale functions for the future//Will be used later in OpenGL//
void Celestial_Body::Set_Scale(){
    if(RScale < RSmax)
        RScale *= (pow(10, SizeScale)*Size_Constant);
    else
        RScale *= (pow(10, SizeScale));
}

void Celestial_Body::Set_Scalers(){//This code needs to be tested//    
    for(CelestialPtr Solar : Celestial_Bodies){
        Solar->Set_Scale();
        if(Solar->RScale < Solar->Radius)
            Solar->RScale = Solar->Radius;
    }   
    for(CelestialPtr Solar : Celestial_Bodies){
        if(Solar->Max)
            RSmax = Solar->RScale;
    }
    for(CelestialPtr Solar : Celestial_Bodies){
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
        vector<Attributes> &K5Copy, vector<Attributes> &K6, vector<Attributes> &Input, vector<Attributes> &Result, vector<Attributes> &Correction){
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
    
    for(int i=0; i<ObjectTracker.size(); i++){
      K1.push_back(Values);
      K2.push_back(Values);
      K3.push_back(Values);
      K4.push_back(Values);
      K5.push_back(Values);
      K6.push_back(Values);
    }
    
    float Rmax = 0.0;
    for(CelestialPtr Solar : Celestial_Bodies){
        Solar->RScale = Radius;
        if(Rmax < Solar->Radius)
            Rmax = Solar->Radius;
    }
    for(CelestialPtr Solar : Celestial_Bodies)
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
double c[24];
c[0] = 0.25;
c[1] = 3.0/32.0;
c[2] = 9.0/32.0;
c[3] = 1932.0/2197.0;
c[4]= 7200.0/2197.0;
c[5] = 7296.0/2197.0;
c[6] = 439.0/216.0;
c[7] = 8.0;
c[8] = 3680.0/513.0;
c[9] = 845.0/4104.0;
c[10] = 8.0/27.0;
c[11] = 2.0;
c[12] = 3544.0/2565.0;
c[13] = 1859.0/4104.0;
c[14] = 11.0/40.0;
c[15] = 25.0/216.0;
c[16] = 1408.0/2565.0;
c[17] = 2197.0/4104.0;
c[18] = 0.2;          
c[19] = 16.0/135.0;
c[20] = 6656.0/12825.0;
c[21] = 28561.0/56430.0;
c[22] = 9.0/50.0;
c[23] = 2.0/55.0; 
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
is_collision(t);//Check to see if any object starts inside another object//
Initialize_Sim(K1, K1Copy, K2, K2Copy, K3, K3Copy, K4, K4Copy, K5, K5Copy, K6, Input, Result, Correction);

//Simulation Loop//
while(t<Time){
    Trigger = false;
    Last = false;
    //Runge-Kutta-Fehlberg Method. This is where the simulation magic happens.//
    while(true){
        if(Trigger)
            h = hnext;
        if(h == hmin)
            Last = true;
        K1 = RK_Functions(K1, ObjectTracker);////////////Calculate K1/////////////////// 
        K1Copy = K1;
        Input = (c[0]*h)*K1 + ObjectTracker;
        K2 = RK_Functions(K2, Input);///////////////Calculate K2/////////////////// 
        K2Copy = K2; K1 = K1Copy;          
        Input = (c[1]*h)*K1 + (c[2]*h)*K2 + ObjectTracker;           
        K3 = RK_Functions(K3,Input);///////////////Calculate K3////////////////////
        K3Copy = K3; K1 = K1Copy; K2 = K2Copy;
        Input = (c[3]*h)*K1 - (c[4]*h)*K2 + (c[5]*h)*K3 + ObjectTracker;                   
        K4 = RK_Functions(K4,Input);///////////////Calculate K4////////////////////
        K4Copy = K4; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy;
        Input = (c[6]*h)*K1 - (c[7]*h)*K2 + (c[8]*h)*K3 - (c[9]*h)*K4 + ObjectTracker;
        K5 = RK_Functions(K5,Input);//////////////Calculate K5/////////////////////
        K5Copy = K5; K1 = K1Copy; K2 = K2Copy; K3 = K3Copy; K4 = K4Copy; Result = ObjectTracker;
        Input = Result - (c[10]*h)*K1 + (c[11]*h)*K2 - (c[12]*h)*K3 + (c[13]*h)*K4 - (c[14]*h)*K5;
        K6 = RK_Functions(K6,Input);///////////////Calculate K6////////////////////
        K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
        Result = (c[15]*h)*K1 + (c[16]*h)*K3 + (c[17]*h)*K4 - (c[18]*h)*K5 + ObjectTracker;
        K1 = K1Copy; K3 = K3Copy; K4 = K4Copy; K5 = K5Copy;
        Correction = (c[19]*h)*K1 + (c[20]*h)*K3 + (c[21]*h)*K4 - (c[22]*h)*K5 + (c[23]*h)*K6 + ObjectTracker;
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
    ObjectTracker = Result;
    
    //Check for collisions//
    i = 0;
    Size = ObjectTracker.size();
    while(i < Size){
        Control = 0;
        C = ObjectTracker[i];
        j = i + 1;
        while(j<Size){
            T = ObjectTracker[j];
            Rad = Celestial_Bodies[C.ID]->Radius + Celestial_Bodies[T.ID]->Radius;
            Dist = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
            if(Dist<=Rad){//Collision Handling//
                if(Celestial_Bodies[T.ID]->Mass <= Celestial_Bodies[C.ID]->Mass){
                   cout << Celestial_Bodies[T.ID]->Name << " crashed into " << Celestial_Bodies[C.ID]->Name << " at time: " << t << endl << endl;
                   //Update the Attributes value for C using the resulting velocity from the collision momentum calculation//                   
                   C = Momentum(C,T);
                   ObjectTracker[i] = C;
                   //Remove T//
                   ObjectTracker.erase(ObjectTracker.begin() + j);
                   K1.pop_back();
                   K1Copy.pop_back();
                   K2Copy.pop_back();
                   K2.pop_back();
                   K3.pop_back();
                   K3Copy.pop_back();
                   K4.pop_back();
                   K4Copy.pop_back();
                   K5.pop_back();
                   K5Copy.pop_back();
                   K6.pop_back();
                   Input.pop_back();
                   Result.pop_back();
                   Correction.pop_back();
                   Size = ObjectTracker.size(); 
                }
                else{
                   cout << Celestial_Bodies[C.ID]->Name << " crashed into " << Celestial_Bodies[T.ID]->Name << " at time: " << t << endl << endl;
                   //Update the Attributes value for T using the resulting velocity from the collision momentum calculation//
                   T = Momentum(T,C);
                   ObjectTracker[j] = T;
                   //Remove C//
                   ObjectTracker.erase(ObjectTracker.begin() + i);
                   K1.pop_back();
                   K1Copy.pop_back();
                   K2Copy.pop_back();
                   K2.pop_back();
                   K3.pop_back();
                   K3Copy.pop_back();
                   K4.pop_back();
                   K4Copy.pop_back();
                   K5.pop_back();
                   K5Copy.pop_back();
                   K6.pop_back();
                   Input.pop_back();
                   Result.pop_back();
                   Correction.pop_back();
                   Size = ObjectTracker.size();
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

update_attributes();
cout << "Final Simulation Result:" << endl;
Display_Objects();
}

double Celestial_Body::Get_R(const vector<Attributes>& Correction, const double h){
    double R = 0.0;
    double M1,M2;
    //Find and obtain the largest magnitude out of every position and velocity vector in the entire list// 
    for(Attributes V : Correction){
        M1 = sqrt(pow(V.Rx,2) + pow(V.Ry,2) + pow(V.Rz,2));
        M2 = sqrt(pow(V.Vx,2) + pow(V.Vy,2) + pow(V.Vz,2));
        if(R < M1 || R < M2)
            R = (M1<M2)? M2:M1;
    }
    R = R/h;
    return R;//Return the result divided by the timestep//
}

vector<Attributes>& Celestial_Body::RK_Functions(vector<Attributes> &Temp, vector<Attributes> &Alt){
    Position Result;
    //Implementing the functions calculating the components of the K values in the RKF45 algorithm// 
    for(int Num = 0; Num<Alt.size(); Num++){        
        Result = Acceleration(Alt, Num);
        Temp[Num].ID = Alt[Num].ID;
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
            Result = Result + Calculations(Rnum, Rcurr, Celestial_Bodies[Alt[Curr].ID]->Mass);
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
    float Mass1 = Celestial_Bodies[One.ID]->Mass;
    float Mass2 = Celestial_Bodies[Two.ID]->Mass;
    //Calculating the resulting velocity of Object One due to the inelastic collision of Object Two by using conservation of momentum//
    M1 = Mass1/(Mass1 + Mass2);
    M2 = Mass2/(Mass1 + Mass2);
    One.Vx = M1*One.Vx + M2*Two.Vx;
    One.Vy = M1*One.Vy + M2*Two.Vy;
    One.Vz = M1*One.Vz + M2*Two.Vz;
    Celestial_Bodies[One.ID]->Mass = Mass1 + Mass2;
    return One;    
}

void Celestial_Body::is_collision(const double t){//This function can still be improved a little//
     int Control;
     double R,D;
     Attributes C, T;
     int i = 0;
     int j;
     int Size = ObjectTracker.size();
     while(i < Size){
         Control = 0;
         C = ObjectTracker[i];
         j = i + 1;
         while(j<Size){
             T = ObjectTracker[j];
             R = Celestial_Bodies[C.ID]->Radius + Celestial_Bodies[T.ID]->Radius;
             D = Distance_Calc(C.Rx - T.Rx, C.Ry - T.Ry, C.Rz - T.Rz);
             if(D<=R){
                 if(Celestial_Bodies[T.ID]->Mass <= Celestial_Bodies[C.ID]->Mass){
                    cout << Celestial_Bodies[T.ID]->Name << " crashed into " << Celestial_Bodies[C.ID]->Name << " at time: " << t << endl << endl;
                    //Update the Attributes value for C using the resulting velocity from the collision momentum calculation//                   
                    C = Momentum(C,T);
                    ObjectTracker[i] = C;
                    //Remove T//
                    ObjectTracker.erase(ObjectTracker.begin() + j);
                    Size = ObjectTracker.size(); 
                 }
                 else{
                    cout << Celestial_Bodies[C.ID]->Name << " crashed into " << Celestial_Bodies[T.ID]->Name << " at time: " << t << endl << endl;
                    //Update the Attributes value for T using the resulting velocity from the collision momentum calculation//
                    T = Momentum(T,C);
                    ObjectTracker[j] = T;
                    //Remove C//
                    ObjectTracker.erase(ObjectTracker.begin() + i);
                    Size = ObjectTracker.size();
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
//double no_nonsense();//Under Construction//Create Mass to Radius Correspondence algorithm
int get_position(CelestialPtr Body, const string name){
    int Counter = 0;
    vector<Attributes> Track = Body->ObjectTracker;
    for(Attributes i : Track){
        if(Celestial_Bodies[i.ID]->Name == name)
            return Counter;//Unsafe Return?
        Counter++;
    }
    return -1;
}

string Get_Status(CelestialPtr Celestial){
    return Celestial->Status;
}

void Count_Decr(CelestialPtr Solar, int Decrement){
    int Element = 0;
    vector<Attributes> Track = Solar->ObjectTracker;
    for(int i=Decrement; i<Celestial_Bodies.size(); i++){
        Element = 0;
        for(Attributes Tracker : Track){
            if(Tracker.ID == Celestial_Bodies[i]->Values.ID){
                Tracker.ID--;
                Track[Element] = Tracker;                
                break;
            }
            Element++;
        }
        Celestial_Bodies[i]->Values.ID--;
    }
    Solar->ObjectTracker = Track;
}


////////////////////////////////////////Extra Code To Debug/Test Out Program/////////////////////////////////////////////////////////////////////////////
/* //   for(int i = j;i<ObjectTracker.size()-1; i++){
 //      ObjectTracker[i] = ObjectTracker[i+1];
  //  }
 * void remove_object(int position){
    
    for(int i = position; i < Celestial_Bodies.size()-1; i++)
        Celestial_Bodies[i] = Celestial_Bodies[i+1];
    Celestial_Bodies.pop_back();
}
 *     Count--;
 *    // ObjectTracker.pop_back();
    //If nonempty, decrease the size of the other vectors in the algorithm by one// 
    //if(Correction.size() > 0){
 * 
 * 
 * 
 * 
 * 
    Celestial_Body Sun("Sun",1000000000000000000000000000000000.0,0.0,0.0,0.0,0.0,0.0,0.0,10000000.0);
    Celestial_Body Jupiter("Earth", 10000000000000.0, 27361400000.0, -14292800000.0, -24503000000.0, 267875.0, -925483.0, -436952.0, 100000.0);
    Celestial_Bodies.push_back(Sun);
    Celestial_Bodies.push_back(Jupiter);
 */
    //Celestial_Body Sun("Sun",219041209481211000000000000000000000000.0,0.0,0.0,0.0,0.0,0.0,0.0,100000.0);
    //Celestial_Body Jupiter("Earth", 2143214235235.0, 1810000.0, 71211.0, 32423.1901293123, 1000000000000.0, 500000000.0, 7000000000.0, 1000.0);
    //Celestial_Bodies.push_back(Sun);
    //Celestial_Bodies.push_back(Jupiter);
    //Sun.Display_Objects();
    //Sun.Simulate_Motion(184372.72,0.01,0.25,0.001,0.0001, "NO");
   // Sun.Simulate_Motion(60372.72,0.01,0.25,0.001,0.0001, "NO");

    /*
    Celestial_Body Sun("Sun",5972200000000000000000000000000000.0,0.0,0.0,0.0,0.0,0.0,0.0,987654321);
    Celestial_Body Jupiter("Jupiter", 9237000000000000.0, 70000000000.0, 0.0, 0.0, 0.0, 2385509.050196923, 0.0, 10000000);
    Sun.Display_Objects();
    Sun.Simulate_Motion(184372.72,0.01,0.25,0.001,0.0001, "NO");
   // Sun.Display_Objects();
    */
    /*
    Attributes Temp, Temp2, Temp3, Temp4, Temp5, Temp6;
    
    Temp.Name = "Sun";
    Temp.Mass = 5972200000000000000000000000000000.0;
    Temp.Rx = 0.0; 
    Temp.Ry = 0.0;
    Temp.Rz = 0.0;
    Temp.Vx = 0.0;
    Temp.Vy = 0.0;
    Temp.Vz = 0.0;
    Temp.Radius = 987654321;
    
    Temp2.Name = "Earth";
    Temp2.Mass =  50000000000000000000000000000000000000.0;
    Temp2.Rx = 5000000000.0; 
    Temp2.Ry = -50000000000.0;
    Temp2.Rz = 0.0;
    Temp2.Vx = 0.0;
    Temp2.Vy = 50000.0;
    Temp2.Vz = 0.0;
    Temp2.Radius = 987321;
    
    Temp3.Name = "Mars";
    Temp3.Mass =  600000000.0;
    Temp3.Rx = 700000070.0; 
    Temp3.Ry = 0.0;
    Temp3.Rz = 800200000;
    Temp3.Vx = 0.0;
    Temp3.Vy = 0.0;
    Temp3.Vz = -50000;
    Temp3.Radius = 43553;
    
    Temp4.Name = "Moon";
    Temp4.Mass =  300000.0;
    Temp4.Rx = 5000000000.0; 
    Temp4.Ry = -50000000000;
    Temp4.Rz = 2000000.0;
    Temp4.Vx = 0.0;
    Temp4.Vy = 50000.0;
    Temp4.Vz = -5000.0;
    Temp4.Radius = 10000;
    
    Temp5.Name = "Venus";
    Temp5.Mass =  9500000000000.0;
    Temp5.Rx = 0.0; 
    Temp5.Ry = 0.0;
    Temp5.Rz = 4248934090.0;
    Temp5.Vx = 55632.0;
    Temp5.Vy = 77797.0;
    Temp5.Vz = -83932;
    Temp5.Radius = 800892;
    
    Temp6.Name = "Jupiter";
    Temp6.Mass =  9237000000000000.0;
    Temp6.Rx = 70000000000.0; 
    Temp6.Ry = 0.0;
    Temp6.Rz = 0.0;
    Temp6.Vx = 0.0;
    Temp6.Vy = 2385509.050196923;
    Temp6.Vz = 0.0;
    Temp6.Radius = 10000000;   
    
    vector<Attributes> Planets = {Temp, Temp2, Temp3, Temp4, Temp5, Temp6};
    vector<Celestial_Body> Celestials;
    for(Attributes i : Planets){
        Celestials.push_back(Celestial_Body(i.Name, i.Mass, i.Rx, i.Ry, i.Rz, i.Vx, i.Vy, i.Vz, i.Radius));        
    }
  //  cout << Celestials.size() << endl;
    Celestial_Body Sun = Celestials[0];
    Sun.display_Object();
    Sun.Simulate_Motion(60000.0,0.0001,0.5,0.000001,0.0000001, "no");
    */
    /*
    Celestial_Body Sun("Sun",5972200000000000000000000000000000.0,0.0,0.0,0.0,0.0,0.0,0.0,987654321);
    Celestial_Body Earth("Earth",50000000000000000000000000000000000000.0,5000000000.0,-50000000000.0,0.0,0.0,50000.0,0.0,987321);
    Celestial_Body Mars("Mars", 600000000.0, 700000070.0, 0.0, 800200000, 0.0, 0.0, -50000,43553);
    Celestial_Body Moon("Moon", 300000.0, 5000000000.0, -50000000000, 2000000.0, 0.0, 50000.0, -5000.0, 10000);
    Celestial_Body Venus("Venus", 9500000000000.0, 0.0, 0.0, 4248934090.0, 55632.0, 77797.0, -83932, 800892);
    Celestial_Body Jupiter("Jupiter", 9237000000000000.0, 70000000000.0, 0.0, 0.0, 0.0, 2385509.050196923, 0.0, 10000000);
    Sun.Display_Objects();
    Sun.Simulate_Motion(60000.0,0.0001,0.5,0.000001,0.0000001, "no");
    Moon.display_Object();
    Earth.display_Object();
    Jupiter.display_Object();
    Mars.Display_Objects();
    */
    /*
    Celestial_Body Sun("Sun",700000000000000000000.0,0.0,0.0,0.0,0.0,0.0,0.0,9821);
    Celestial_Body Earth("Earth",500000000000000000000.0,50000.0,-50000.0,0.0,0.0,500.0,0.0,5821);
    Celestial_Body Mars("Mars", 600000000000000000000.0, 70070.0, 0.0, 200200, 0.0, 0.0, -500,4353);
    Celestial_Body Moon("Moon", 300000000000000000000.0, 50000.0, -50000, 20000.0, 0.0, 500.0, -5000.0, 1000);
    Celestial_Body Jupiter("Jupiter", 923700000000000000000.0, 70000.0, 0.0, 0.0, 0.0, 25.3, 0.0, 1000);
    Sun.Display_Objects();
    Sun.Simulate_Motion(3600.0,0.01,0.5,0.0001,0.001, "no");
    Moon.display_Object();
    Earth.display_Object();
    Mars.display_Object();
    Jupiter.display_Object();*/


  /*  if(One.size() == 0){//Possibly Redundant Condition/Impossible case. Will verify/remove later//
        cout << "Hello MotherFucker" << endl;
        for(Attributes i : One){
            V.Mass = i.Mass;
            V.Name = i.Name;
            V.Radius = i.Radius;
            V.Rx = C*i.Rx;
            V.Ry = C*i.Ry;
            V.Rz = C*i.Rz;
            V.Vx = C*i.Vx;
            V.Vy = C*i.Vy;
            V.Vz = C*i.Vz;        
            One.push_back(V);
        }
    }
    else{*/


/*
double Celestial_Body::no_nonsense(){//Under Construction//
    

//Develop a standard to use to determine how close an object needs to be 
//to another object before adjusting the time step value//
    
//Create formula to calculate the optimal time step value once an object starts to approach anther object//
    
}
*/
/*
string Celestial_Body::get_status(){//Status Function Here//
    return Status;
}
 * void Celestial_Body::set_status(const string Update){//Status Function Here
    Status = Update;
}
*/
/*
void Just_Remove(CelestialPtr Body, int Pos){
    Body->remove_element(Pos);
}
*/
        //double no_nonsense();//Under Construction//