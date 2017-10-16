/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*Define Matrix Type
 * Plan out Coordinate Conversions
 * Overload operators for matrix operations
 * Define proper macro headers for these functions and types
 * Write out the Random orbit generator function/random Initial Values generator function
 */

/*Random Orbit Generator Function:
 * 
 
 */

#include <time.h>
#include <cfloat>
#include "TrackingSystem.h"

using namespace std;

//Forward Declarations//
float Gen_Mass(float*);
float Gen_Radius();
float Gen_Eccent();
int Gen_Position(vector<CelestialPtr>&, float, double*, double*);
void Specify_Path(vector<double*>&, double*, float*, float, float&, double&);
int Point_Sampler(vector<double*>&, float*, float*);
float get_rotation(char, double*);
void Rotate(double*, const char, float);
int Least_Squares(double*, double*, vector<double*>&, float);
double SLRec_Calc(const float, const double*, const float);
float get_theta(double, double, double);
bool Verify_Mass(float*, float*);

Settings Set;
iMass Mass_Val;
iPosition Pos_Val;
iRadius Rad_Val;
int Ecc_Dec;
int Ecc_Range;
string Orbit;
char Axis;

//Default Values//
Settings Default;
iMass MassDefault;
iPosition Pos_Def;
iRadius Rad_Def;
int Ecc_Dec_Def;
int Ecc_Range_Def;
char Axis_Def;
string Body_Def;

void Set_Defaults(){
    print_settings();
    cout << endl;
    string Answer;
    while(true){
        cout << "Which default value would you like to change? (User, Mass, Position, Radius, Eccentricity, Axis, Body) Enter 'Print' to print the current settings and 'Done' when done" << endl;
        while(true){
        cin >> Answer;
            Answer = str_lower(Answer);
            if(Answer == "user" || Answer == "mass" || Answer == "position" || Answer == "radius" || Answer == "eccentricity" || Answer == "axis" || Answer == "body" || Answer == "print")
                break;
            if(Answer == "done")
                break;
            cout << "Error, invalid input." << endl;
        }
        if(Answer == "done")
            break;
        if(Answer == "user")
            Set_Settings(true);
        if(Answer == "mass")
            Set_Mass(true);
        if(Answer == "position")
            Set_Position(true);
        if(Answer == "radius")
            Set_Radius(true);
        if(Answer == "eccentricity")
            Set_Eccentric(true);
        if(Answer == "axis")
            Set_Axis(true);
        if(Answer == "body")
            Set_Body(true);
        if(Answer == "print")
            print_settings();    
    }
}

int Rand_Orbit_Gen(Attributes* Body, float &GenMass, float &GenRadius, vector<CelestialPtr> &Solar_Bodies, const string name){
    double Pos[3];
    double Vel[3];
    float Theta;
    float ObjMass;
    float ObjRadius;
    float OrbMass;
    double OrbVel[3];
    double OrbPos[3];
    vector<double*> Points;//User Sample Points//
    int Perihelion;    
    double SLRec;
    float Eccent;
    //Support Variables//
    double translate[3];
    double P1[2];
    srand(static_cast<unsigned int>(time(0)));
/////////////////////////////////////User Specification Block////////////////////////////////////
    while(true){
        string Answer;
        cout << "Would you like to use the default settings? Yes/No" << endl;
        Answer = yes_no();       
        Set = Default;
        if(Answer == "no")//Set Set//
            Set_Settings(false);
        
        if(Set.Body)//Set Body
            Set_Body(false);
        else
            Orbit = Body_Def;
        
        //Get the position of Celestial Body "Orbit"//
        Attributes V;
        while(true){
            bool Status = false;     
            for(CelestialPtr Celestial : Solar_Bodies){
                V = Celestial->get_attributes();
                if(Celestial->get_Name() == Orbit && Get_Status(Celestial) == "Intact"){
                    translate[0] = -V.Rx;
                    translate[1] = -V.Ry;
                    translate[2] = -V.Rz;
                    Status = true;
                    break;
                }
            }
            if(Status)
                break;
            cout << "\"" << Orbit 
            << "\" was either destroyed in a simulation or never existed to begin with.\n Enter another"
            << " Celestial body for " << name << " to orbit around please. 'Cancel' to cancel" << endl;
            cin >> Orbit;
            if(str_lower(Orbit) == "cancel")
                return 1;
        }
        OrbMass = Solar_Bodies[V.ID]->get_Mass();//We'll need this mass value later. Assign it to a new variable before V(retrieved above) goes out of scope//
        OrbVel[0] = V.Vx;//Same here//
        OrbVel[1] = V.Vy;//And here//
        OrbVel[2] = V.Vz;//And here too//  
        OrbPos[0] = V.Rx;
        OrbPos[1] = V.Ry;
        OrbPos[2] = V.Rz;
        
        if(Set.Axis)//Set Axis
            Set_Axis(false);
        else
            Axis = Axis_Def;
        
        if(Set.Perihelion){
            //Set Angle of Perihelion//
            cout << "Enter the angle (0 to 360 degrees) for the argument of perihelion." << endl;
            while(true){
                Answer = verify_pointdouble();
                double Test = convert(Answer);
                if(Answer == "done")
                    cout << "Error, enter a number" << endl;
                else if(Test < 0 || 360 < Test)
                    cout << "Error, number must be an integer in the range of 0 to 360" << endl;
                else{
                    if(Test - (int)Test == 0){
                        Perihelion = Test;
                        break;
                    }
                    cout << "Error, number must be an integer." << endl;
                }
            }
        }
        
        if(Set.IEccentric == Full){
            //Set Eccentricity
            cout << "Set the eccentricity value. (0-2)" << endl;
            while(true){
                Answer = verify_pointdouble();
                if(Answer == "done")
                    cout << "Error, enter a number" << endl;
                else if(2 < convert(Answer) || convert(Answer) < 0)
                    cout << "Error, Eccentricity value cannot be either negative or greater than 2." << endl;
                else{
                    Eccent = convert(Answer);
                    break;
                }
            }
        }
        else{
            Ecc_Range = Ecc_Range_Def;
            Ecc_Dec = Ecc_Dec_Def;
            if(Set.IEccentric == Partial)
                Set_Eccentric(false);
        }
        
        if(Set.IRadius == Full){
            //Set Radius
            cout << "Enter a new Radius value." << endl;            
            while(true){
                Answer = verify_pointdouble();
                if(Answer == "done")
                    cout << "Enter a number" << endl;
                else if(convert(Answer) <= 0)
                    cout << "Error. Radius value must be strictly positive." << endl;
                else{
                    ObjRadius = convert(Answer);
                    break;
                }
            }
            
        }
        else{
            Rad_Val = Rad_Def;
            if(Set.IRadius == Partial)
                Set_Radius(false);
        }
        
        if(Set.IPosition == Full){
            //Set Position
            cout << "Enter the coordinates for an initial orbital point." << endl;
            cout << "Enter the x-coordinate" << endl;
            while(true){
                Answer = verify_pointdouble();
                if(Answer == "done")
                    cout << "Invalid input. Enter a number" << endl;
                else{
                    Pos[0] = convert(Answer);
                    break;
                }
            }
            cout << "Enter the y-coordinate" << endl;
            while(true){
                Answer = verify_pointdouble();
                if(Answer == "done")
                    cout << "Invalid input. Enter a number" << endl;
                else{
                    Pos[1] = convert(Answer);
                    break;
                }
            }
            cout << "Enter the z-coordinate" << endl;
            while(true){
                Answer = verify_pointdouble();
                if(Answer == "done")
                    cout << "Invalid input. Enter a number" << endl;
                else{
                    Pos[2] = convert(Answer);
                    break;
                }
            }    
        }
        else{
            Pos_Val = Pos_Def;
            if(Set.IPosition == Partial)
                Set_Position(false);
        }             
        
        if(Set.IMass == Full){
            //Set IMass
            float Value;
            cout << "Enter a new Mass value." << endl;            
            while(true){
                Answer = verify_pointdouble();
                if(Answer == "done")
                    cout << "Enter a number" << endl;
                else if(convert(Answer) <= 0)
                    cout << "Error. Mass value must be strictly positive." << endl;
                else{
                    Value = convert(Answer);
                    if(Verify_Mass(&Value, &OrbMass)){
                        ObjMass = Value;
                        break;
                    }
                    cout << "Error. Mass value too large. Enter a new value" << endl;
                }
            }
        }
        else{
            Mass_Val = MassDefault;
            if(Set.IMass == Partial)
                Set_Mass(false);                              
        }            
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////RNG Block//////////////////////////////////////////////////
        if(Set.IMass != Full){
            if(ObjMass = Gen_Mass(&OrbMass) == -1)
                return 1;
        }
        if(Set.IRadius != Full)
            ObjRadius = Gen_Radius();       
        if(Set.IPosition != Full){
            if(Gen_Position(Solar_Bodies, ObjRadius, Pos, OrbPos))
                return 1;
        }
        if(!Set.Perihelion)
            Perihelion = rand() % 360;
        if(!Set.Sampler && Set.IEccentric != Full)    
            Eccent = Gen_Eccent();        
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////Information Processing Block////////////////////////////////////         
        //Apply Translation//
        Pos[0] += translate[0];//x
        Pos[1] += translate[1];//y
        Pos[2] += translate[2];//z
////////////////////////////////////////////////////////////////////////////////////////////////
        //Rotate Initial Position onto x-z(or whatever respective 2-D) plane, save the inverse transformation for later//
        Theta = get_rotation(Axis, Pos);//HERE
        Rotate(Pos, Axis, Theta);
////////////////////////////////////////////////////////////////////////////////////////////////
        //Translate into a 2-D "(x,y)" coordinate//
        if(Axis == 'Z' || Axis == 'X'){//x-z plane to x-y coordinate
            P1[0] = Pos[0];
            P1[1] = -Pos[2];
        }
        if(Axis == 'Y'){//x-y plane to x-y coordinate
            P1[0] = Pos[0];
            P1[1] = Pos[1];
        }
        
        //If necessary, apply Least Squares on specified points to obtain Eccentricity and Semi-Latus Rectum//
        float Peri = 2*pi*(Perihelion/360.0);   
        if(Set.Sampler){
            Specify_Path(Points, P1, &Theta, Peri, Eccent, SLRec);
        }
        else{//Calculate Semi-Latus Rectum//
            SLRec = SLRec_Calc(Eccent, P1, Peri);    
        }
        cout << "Semi-Latus Rectum: " << SLRec << endl;
        cout << "Eccentricity: " << Eccent << endl;            
        //Calculate Position at Argument of Perihelion to verify that it is stable//  
        if(0 < ((SLRec/(1+Eccent)) -  ObjRadius - Solar_Bodies[V.ID]->get_Radius()))//Might Have to Change//Will have to add onto this test later
            break;
        //If not, display Warning Message ... and Yes/No verification//
        cout << "Warning: " << name << " will likely crash into " << Orbit << ". Are you fine with this? Yes/No (No will make you restart the Orbit Customizer.)" << endl;
        Answer = yes_no();
        if(Answer == "yes")
            break;
    }
    
    if(!Points.empty()){//Deallocate all memory in "Points" and clear vector//Possible Memory Leak//
        for(vector<double*>::const_iterator Point=Points.begin(); Point!=Points.end(); ++Point)
            delete[] *Point;
        Points.clear();
        Points.shrink_to_fit();
    }
       
    //Use Eccentricity and Semi-Latus Rectum to calculate initial Velocity//
    float u = (ObjMass*OrbMass)/(ObjMass + OrbMass);
    double L = sqrt(u*G*OrbMass*ObjMass*SLRec);
    double E = G*OrbMass*ObjMass*(Eccent*Eccent - 1)/(2*SLRec);
    float Angle;
    
    Vel[1] = L/(u*(sqrt(P1[0]*P1[0] + P1[1]*P1[1])));//Vtan     
    Vel[0] = (2*((E + G*OrbMass*ObjMass/(sqrt(P1[0]*P1[0] + P1[1]*P1[1])))/u)) - Vel[1]*Vel[1];//Vrad
    if(Vel[0] < 0)
        Vel[0] = 0;
    Vel[0] = sqrt(Vel[0]);//Given the starting angle, whether the square root is positive or negative will determine whether the new orbit is clockwise or counter-clockwise//Just let it always be positive//
    if(1 < Eccent)    
        Vel[0] *= -1;
    
    cout << "VRad: " << Vel[0] << endl;
    cout << "VTan: " << Vel[1] << endl;
       
    //Translate Polar coordinates back to Cartesian coordinates//
    Angle = get_theta(P1[0], P1[1], sqrt(P1[0]*P1[0] + P1[1]*P1[1]));
    Vel[2] = Vel[0]*cos(Angle) - Vel[1]*sin(Angle);//Vx
    Vel[1] = Vel[0]*sin(Angle) + Vel[1]*cos(Angle);//Vy
    Vel[0] = Vel[2];
    Vel[2] = 0.0;
    
    Pos[0] = P1[0];
    //Make sure velocity vector is correct 3-D vector//
    if(Axis == 'Z' || Axis == 'X'){//x-y coordinate to x-z plane
        Vel[2] = -Vel[1];
        Vel[1] = 0.0;
        Pos[2] = -P1[1];
        Pos[1] = 0.0;        
    }
    else{
        Pos[1] = P1[1];
        Pos[2] = 0.0;
    }
           
    //Apply inverse rotational transformation to Initial velocity vectors//
    Rotate(Vel, Axis, -Theta);//HERE//
    Rotate(Pos, Axis, -Theta);
    
    //Calculate Initial Velocity with respect to origin in World Space(Rather than with respect to the body being orbited//
    Vel[0] += OrbVel[0];//HERE
    Vel[1] += OrbVel[1];
    Vel[2] += OrbVel[2];
    Pos[0] -= translate[0];
    Pos[1] -= translate[1];
    Pos[2] -= translate[2];
    
   //Set the attributes for new Celestial Object//
    GenMass = ObjMass;
    GenRadius = ObjRadius;
    Body->Rx = Pos[0];
    Body->Ry = Pos[1];
    Body->Rz = Pos[2];
    Body->Vx = Vel[0];
    Body->Vy = Vel[1];
    Body->Vz = Vel[2];
    
    return 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////        
}
            
int Gen_Position(vector<CelestialPtr> &Solar_Bodies, float Radius, double* Pos, double* OrbPos){
    double Dec;
    double Val;
    int Power;
    bool signx;
    bool signy;
    bool signz;
    bool Conflict;
    double r;
    Attributes Test;
    int Count = 0;
    while(Count < 10000){
        Conflict = false;
        if(Pos_Val.X == Maybe){
            if((rand() % 2) == 0)
                signx = true;//Positive//
            else
                signx = false;//Negative//
        }
        else if(Pos_Val.X == Yes)
            signx = true;//Positive
        else
            signx = false;//Negative

        if(Pos_Val.Y == Maybe){
            if((rand() % 2) == 0)
                signy = true;//Positive//
            else
                signy = false;//Negative//
        }
        else if(Pos_Val.Y == Yes)
            signy = true;//Positive
        else
            signy = false;//Negative    

        if(Pos_Val.Z == Maybe){
            if((rand() % 2) == 0)
                signz = true;//Positive//
            else
                signz = false;//Negative//
        }
        else if(Pos_Val.Z == Yes)
            signz = true;//Positive
        else
            signz = false;//Negative    

        Dec = (double)(rand() % (int)pow(10,Pos_Val.Decimalx))/(pow(10,Pos_Val.Decimalx));
        Val = (rand() % (Pos_Val.Maxx - Pos_Val.Minx + 1)) + Pos_Val.Minx;
        Power = (rand() % (Pos_Val.Powerxmax - Pos_Val.Powerxmin + 1)) + Pos_Val.Powerxmin;
        Val += Dec;
        if(signx)
            Pos[0] = Val*pow(10,Power);
        else
            Pos[0] = -Val*pow(10,Power);

        Dec = (double)(rand() % (int)pow(10,Pos_Val.Decimaly))/(pow(10,Pos_Val.Decimaly));
        Val = (rand() % (Pos_Val.Maxy - Pos_Val.Miny + 1)) + Pos_Val.Miny;
        Power = (rand() % (Pos_Val.Powerymax - Pos_Val.Powerymin + 1)) + Pos_Val.Powerymin;
        Val += Dec;
        if(signy)
            Pos[1] = Val*pow(10,Power);
        else
            Pos[1] = -Val*pow(10,Power);

        Dec = (double)(rand() % (int)pow(10,Pos_Val.Decimalz))/(pow(10,Pos_Val.Decimalz));
        Val = (rand() % (Pos_Val.Maxz - Pos_Val.Minz + 1)) + Pos_Val.Minz;
        Power = (rand() % (Pos_Val.Powerzmax - Pos_Val.Powerzmin + 1)) + Pos_Val.Powerzmin;
        Val += Dec;
        if(signz)
            Pos[2] = Val*pow(10,Power);    
        else
            Pos[2] = -Val*pow(10,Power);

        Pos[0] += OrbPos[0];
        Pos[1] += OrbPos[1];
        Pos[2] += OrbPos[2];
        
        for(CelestialPtr Body : Solar_Bodies){
            Test = Body->get_attributes();
            r = sqrt(pow(Pos[0] - Test.Rx,2) + pow(Pos[1] - Test.Ry,2) + pow(Pos[2] - Test.Rz,2));
            if(r < Body->get_Radius() + Radius){
                Conflict = true;
                break;
            }
        }
        if(!Conflict)
            return 0;
        Count++;
    }
    cout << "Error, failed to generate a proper orbital position far enough away from " << Orbit << ".\nRadius of " << Orbit << " might"
            "be too large and creating conflict.\nSuggestion: Change the Generator Settings to allow orbit generation farther away from " << Orbit << "." << endl;
    return 1;
}

float Gen_Mass(float *Orb){
    float Val;
    float Dec;
    float Power;
    int Count = 0;
    while(Count < 10000){
        Dec = (float)(rand() % (int)pow(10,Mass_Val.Decimal))/(pow(10,Mass_Val.Decimal));
        Val = (rand() % (Mass_Val.Max - Mass_Val.Min + 1)) + Mass_Val.Min;
        Power = (rand() % (Mass_Val.Powermax - Mass_Val.Powermin + 1)) + Mass_Val.Powermin;
        Val += Dec;    
        Val = Val*pow(10,Power);
        if(Verify_Mass(&Val, Orb))
            return Val;
        Count++;
    }
    cout << "Error, could not generate an acceptable mass value. Mass of " << Orbit << " might be too large." << endl;
    return -1;
}

bool Verify_Mass(float *Obj, float *Orb){
    if(*Obj > FLT_MAX/(*Orb))
        return false;
    return true;
}

float Gen_Radius(){
    float Dec = (float)(rand() % (int)pow(10,Rad_Val.Decimal))/(pow(10,Rad_Val.Decimal));
    float Val = (rand() % (Rad_Val.Max - Rad_Val.Min + 1)) + Rad_Val.Min;
    int Power = (rand() % (Rad_Val.Powermax - Rad_Val.Powermin + 1)) + Rad_Val.Powermin;
    Val += Dec;
    return Val*pow(10,Power);
}

float Gen_Eccent(){
    float Dec = (float)(rand() % (int)pow(10,Ecc_Dec))/(pow(10,Ecc_Dec));
    float Val = (rand() % (Ecc_Range + 1));
    if(Val < 2)
        Val += Dec;
    return Val;
}

double SLRec_Calc(const float Eccent, const double* P1, const float Peri){
    double r = sqrt(pow(P1[0],2) + pow(P1[1],2));
    float Theta = get_theta(P1[0], P1[1], r);
    return r*(1 + Eccent*cos(Theta - Peri));    
}

float get_rotation(char Axis, double* Pos){
    float Phi;
    switch(Axis){
        case 'Z':
            if(Pos[0] == 0){
                if(Pos[1] == 0)
                    Phi = 0.0;
                else
                    Phi = -pi/2;
            }
            else if(Pos[1] == 0)
                Phi = 0.0;
            else
                Phi = -atanf(Pos[1]/Pos[0]);
            break;
        case 'Y':
            if(Pos[0] == 0){
                if(Pos[2] == 0)
                    Phi = 0.0;
                else
                    Phi = -pi/2;
            }
            else if(Pos[2] == 0)
                Phi = 0.0;
            else
                Phi = atanf(Pos[2]/Pos[0]);
            break;
        case 'X':
            if(Pos[2] == 0){
                if(Pos[1] == 0)
                    Phi = 0.0;
                else
                    Phi = -pi/2;
            }
            else if(Pos[1] == 0)
                Phi = 0.0;
            else
                Phi = atanf(Pos[1]/Pos[2]);
    }    
    return Phi;
}


void Rotate(double* PS, const char Axis, float phi){
    float E = cos(phi);
    float F = sin(phi);
    double a, b;
    switch(Axis){
        case 'Z':
            a = PS[0];
            b = PS[1];
            PS[0] = a*E - b*F;
            PS[1] = a*F + b*E;
            break;
        case 'Y':
            a = PS[0];
            b = PS[2];
            PS[0] = a*E + b*F;
            PS[2] = -a*F + b*E;
            break;
        case 'X':
            a = PS[1];
            b = PS[2];
            PS[1] = a*E - b*F;
            PS[2] = a*F + b*E;
            break;
    } 
}

float get_theta(double x, double y, double r){
    float theta;   
    if(0 <= x && 0 <= y){
        if(x == 0)
            theta = pi/2;
        else
            theta = acosf(x/r);
    }
    if(x < 0 && 0 <= y){
        if(y == 0)
            theta = pi;
        else
            theta = pi - acosf(x/r);
    }
    if(x < 0 && y <0)
        theta = pi + acosf(x/r);
    if(0 <= x && y < 0){
        if(x == 0)
            theta = 3*pi/2;
        else
            theta = 2*pi - acosf(x/r);    
    }
    return theta;
}

void Specify_Path(vector<double*> &Points, double* P1, float* Theta, float Peri, float &Eccent, double &SLRec){
    int Status;
    string Answer;
    float ThetaCopy = *Theta;
    float StartAngle;
    double Parameters[2];
    while(true){                
        //Acquire Sample Points to apply Least Squares on//This is where a function call may be added to add a generator GUI in the future              
        Status = Point_Sampler(Points, &StartAngle, Theta);
        if(Status == POINT_EMPTY){
            cout << "No points were specified. Would you like to continue with just the one default point?" << endl;
            Answer = yes_no();
        }
        if(Status == POINT_FAIL){
            cout << "Would you like to continue with just the one default point? (Yes/No) No will result in a resample." << endl;
            Answer = yes_no();
        }           
        if(Status == POINT_SUCCESS){
            double P1Copy[2] = {P1[0], P1[1]};
            Status = Least_Squares(Parameters, P1, Points, StartAngle);
            if(Status == POINT_FAIL){
                P1[0] = P1Copy[0];
                P1[1] = P1Copy[1];                        
                cout << "Impossible to compute Eccentricity and Semi-Latus Rectum using Least Squares and the provided points. Would you like to respecify new points? (Yes/No)" << endl;
                string Ans;
                Ans = yes_no();
                if(Ans == "no"){                            
                    Eccent = Gen_Eccent();
                    *Theta = ThetaCopy;
                    SLRec = SLRec_Calc(Eccent, P1, Peri);
                    break;
                }
            }
            else{
                    SLRec = Parameters[0];               
                    Eccent = Parameters[1];
                    break;                            
            }
        }
        else{
            if(Answer == "yes"){
                Eccent = Gen_Eccent();
                *Theta = ThetaCopy;
                SLRec = SLRec_Calc(Eccent, P1, Peri);
                break;
            }
        } 
    }    
}

int Point_Sampler(vector<double*>& Points, float *SAngle, float *Theta){
    int Count = 1;
    vector<double> Inputs;// = {9923423432.0, -3249324.0, 3924.3294923, 32432499432.0, -32949324.0, 32943294943.0, 0.0, -3294923494.0};
    double Inputx, Inputy;
    string number;
    bool equal;    
    if(!Points.empty()){//Possible Memory Leak//
        for(vector<double*>::const_iterator Point=Points.begin(); Point!=Points.end(); ++Point)
            delete[] *Point;    
        Points.clear();
        Points.shrink_to_fit();
    }
    cout << endl;
    cout << "Pretend the orbital plane is the x-y plane. Specify the (x,y) points you would like the celestial orbit to go through." << endl;
    cout << "Enter 'Done' at any point when you're finished." << endl;
    while(true){
        equal = false;
        cout << "Point Number: " << Count << endl;
        cout << "Enter Rx" << endl;
        number = verify_pointdouble();
        if(number == "done")
            break;
        Inputx = convert(number);
        cout << "Enter Ry" << endl;
        number = verify_pointdouble();
        if(number == "done")   
            break;
        Inputy = convert(number);
        vector<double>::iterator Test;
        for(Test = Inputs.begin(); Test != Inputs.end(); Test+=2){   
            if(*Test == Inputx && *(Test+1) == Inputy){
                equal = true;
                break;
            }
        }
        if(equal)
            cout << "You've already added that point." << endl;
        else{
            Inputs.push_back(Inputx);
            Inputs.push_back(Inputy);
            Count++;
        }
    }
    
    float Angle;
    cout << "What would you like the angle of inclination to be (in degrees)?" << endl;
    while(true){
        number = verify_pointdouble();
        if(number == "done")
            cout << "Enter a number" << endl;
        else
            break;      
    }
    Angle = convert(number);
    Angle = ((int)Angle % 360) + (Angle - (int)Angle);
    *Theta = 2*pi*(Angle/360.0);
    
    cout << "Enter the angle that will specify the starting position in the orbit." << endl;
    while(true){
        number = verify_pointdouble();
        if(number == "done")
            cout << "Enter a number" << endl;
        else
            break;      
    }
    Angle = convert(number);
    Angle = ((int)Angle % 360) + (Angle - (int)Angle);
    *SAngle = 2*pi*(Angle/360.0);
    
    if(Inputs.empty())
        return POINT_EMPTY;
    double *P = new (nothrow) double[Inputs.size()]; 
    if(P == NULL){
        cout << "Error. Bad memory allocation. Try again but with fewer points or something I don't know. Good Luck." << endl;
        return POINT_FAIL;
    }
    for(int i=0; i<Inputs.size(); i++){
        P[i] = Inputs[i];
        if(i % 2 == 0)
            Points.push_back(P + i);
    }    
    
    return POINT_SUCCESS;
}

double get_Error(vector<double*> &Points, float E, float Peri, double SL){
    double r;
    double Diff;
    double Error = 0.0;
    for(double *Point : Points){
        r = sqrt(Point[0]*Point[0] + Point[1]*Point[1]);
        Diff = r - SL/(1 + E*cos(get_theta(Point[0], Point[1], r) - Peri));
        if(Diff < 0)
            Diff *= -1;
        Error += Diff;
    }
    
    return Error;
}

int LS_Calculations(vector<double*> &Points, double* Result, float Peri){
    long int Size = Points.size();
    double A[Size][2];
    double AT[2][Size];
    double ATA[2][2];
    double b[Size];
    double y[2];
    double* P;
    double r;
    float theta;//Argument of Latitude//    
    
    for(int i=0; i<Size; i++){
        A[i][0] = 1.0;
        P = Points[i];
        r = sqrt(pow(P[0],2) + pow(P[1],2));
        theta = get_theta(P[0],P[1],r);
        b[i] = r;
        A[i][1] = -r*cos(theta - Peri);
    }   

    //Initialize A Transpose//
    for(int i=0; i<Size; i++){
        AT[0][i] = 1.0;
        AT[1][i] = A[i][1];       
    }    
    
    //Multiply AT*A//
    for(int i=0; i<2; i++){       
        for(int j=0; j<2; j++){            
            ATA[i][j] = 0;
            for(int k=0; k<Size; k++)
                ATA[i][j] += AT[i][k]*A[k][j];            
        }
    }
    
    //Multiply AT*b//
    for(int i=0; i<2; i++){
        y[i] = 0.0;
        for(int k=0; k<Size; k++)
            y[i] += AT[i][k]*b[k];
    }

    //Invert ATA//
    double Det;
    double Sub;
    Det = ATA[0][0]*ATA[1][1] - ATA[0][1]*ATA[1][0];
    if(Det==0)//No dividing by zero//
        return POINT_FAIL;
    
    Det = 1.0/Det;
    Sub = ATA[0][0];
    ATA[0][0] = Det*ATA[1][1];
    ATA[1][1] = Det*Sub;
    ATA[0][1] = -Det*ATA[0][1];
    ATA[1][0] = -Det*ATA[1][0];
    
    //Multiply ATA(Inverted)*y//
    Sub = y[0];
    y[0] = ATA[0][0]*Sub + ATA[0][1]*y[1];//Semi-Latus Rectum//
    y[1] = ATA[1][0]*Sub + ATA[1][1]*y[1];//Eccentricity// 
    
    Result[0] = y[0];
    Result[1] = y[1];
    
    return POINT_SUCCESS;
}

int Least_Squares(double* Results, double* P1, vector<double*> &Points, float SAngle){
    double y[2];
    double r;
    int Status;
    float OptPeri;
    float OptE;
    double OptSL;
    double Error = DBL_MAX;
    double ErrorCpy;
    bool Fail = true;
    float Peri = 0.0;
    while(Peri <= 2*pi){       
        Status = LS_Calculations(Points, y, Peri);
        if(0 <= y[1] && y[1] <= 2 && 0 < y[0] && Status == POINT_SUCCESS){
            ErrorCpy = get_Error(Points, y[1], Peri, y[0]);
            Fail = false;
            if(ErrorCpy < Error){
                Error = ErrorCpy;
                OptPeri = Peri;
                OptE = y[1];
                OptSL = y[0];
            }
        }
    Peri+=0.001;
    }
    
    if(Fail)
        return POINT_FAIL; 
    
    Results[0] = OptSL;
    Results[1] = OptE;
    
    r = OptSL/(1 + OptE*cos(SAngle - OptPeri));
    P1[0] = r*cos(SAngle);
    P1[1] = r*sin(SAngle);

    cout << "Argument of Perihelion: " << OptPeri << endl;
    return POINT_SUCCESS;
}












/*struct iVelocity{//Is this Necessary?
    int Minx;
    int Miny;
    int Minz;
    int Maxx;
    int Maxy;
    int Maxz;
    int Powerxmax;
    int Powerxmin;
    int Powerymax;
    int Powerymin;
    int Powerzmax;
    int Powerzmin;
    int Decimalx;
    int Decimaly;
    int Decimalz;  
 *     User ISLRectum;//Necessary?
};*/
 //   if(Count == 10000)
 //       break;

  //  Count++;
/*   // double T0 = sqrt(P1[0]*P1[0] + P1[1]*P1[1]);
   // double T = G*OrbMass*ObjMass/(sqrt(P1[0]*P1[0] + P1[1]*P1[1]));
   // T = E + T;
  //  T = T/u;
  //  T = 2*T;
   // double T2 = Vel[1]*Vel[1];
 * //Sometimes Vel[1] squared might be a little (around 10^(-13) or something like thatish) bit larger than the rest of the equation. If so set it equal to 0.// 
        int Count = 1;
        string number;
        double P[2];
        bool equal;
        cout << "Pretend the orbital plane is the x-y plane. Specify the (x,y) points you would like the celestial orbit to go through." << endl;
        cout << "Enter 'Done' at any point when you're finished." << endl;
        while(true){
            equal = false;
            cout << "Point Number: " << Count << endl;
            cout << "Enter Rx" << endl;
            number = verify_pointdouble();
            if(number == "done")
                break;
            P[0] = convert(number);
            cout << "Enter Ry" << endl;
            number = verify_pointdouble();
            if(number == "done")
                break;
            P[1] = convert(number);
            //Still need to make sure points are not equal to each other//
            for(double *Test : Points){
                if(Test[0] == P[0] && Test[1] == P[1]){
                    equal = true;
                    break;
                }
            }
            if(equal)
                cout << "You've already added that point." << endl;
            else{
                Points.push_back(P);
                Count++;
            }
        }
 * 
 *         switch(Axis){
            case 'Z':
                if(Pos[0] == 0){
                    if(Pos[1] == 0)
                        Theta = 0.0;
                    else
                        Theta = -pi/2;
                }
                else if(Pos[1] == 0)
                    Theta = 0.0;
                else
                    Theta = -atanf(Pos[1]/Pos[0]);
                break;
            case 'Y':
                if(Pos[0] == 0){
                    if(Pos[2] == 0)
                        Theta = 0.0;
                    else
                        Theta = -pi/2;
                }
                else if(Pos[2] == 0)
                    Theta = 0.0;
                else
                    Theta = atanf(Pos[2]/Pos[0]);
                break;
            case 'X':
                if(Pos[2] == 0){
                    if(Pos[1] == 0)
                        Theta = 0.0;
                    else
                        Theta = -pi/2;
                }
                else if(Pos[1] == 0)
                    Theta = 0.0;
                else
                    Theta = atanf(Pos[1]/Pos[2]);
        }
 *             SLRec_Val.Decimal = 3;
            SLRec_Val.Max = 100;
            SLRec_Val.Min = 1;
            SLRec_Val.Powermax = 12;
            SLRec_Val.Powermin = 6;
 * int User_Samp_Def;//Necessary?//
 * 
 * 
 * struct iSLRectum{//Might Not be necessary//
    int Min;
    int Max;
    int Powermax;
    int Powermin;
    int Decimal;
};
iSLRectum SLRec_Val;
iSLRectum SLRec_Def = SLRec_Val;

struct iSampler{//Might not be necessary//
   int UserSampleNum;
   //int SampleDec;
  // int SamplePowMax;
  // int SamplePowMin;
};
   // A[0][0] = 1.0;
  //  r = sqrt(pow(P1[0],2) + pow(P1[1],2));
   // theta = get_theta(P1[0],P1[1],r);
   // b[0] = r;
   // A[0][1] = -r*cos(theta - Peri);  
 * 
 * float get_Peri(vector<double*> &Points){
    vector<double*> PointCpy;    
    double R;
    double Rmin;
    double* P;
    float Average = 0.0;
    
    P = Points[0];
    Rmin = sqrt(P[0]*P[0] + P[1]*P[1]);   
    for(double *Point : Points){
        R = sqrt(Point[0]*Point[0] + Point[1]*Point[1]);
        if(R < Rmin)
            Rmin = R;
    }     
    
    for(double* Point : Points){
        R = sqrt(Point[0]*Point[0] + Point[1]*Point[1]);
        if(R == Rmin)
            PointCpy.push_back(Point);
    }
    
    for(int i=0; i<PointCpy.size(); i++){
        P = PointCpy[i];
        Average += get_theta(P[0], P[1], Rmin);
    }
    
    Average /= PointCpy.size();
    
    PointCpy.clear();    
    return Average;    
}                        /*if(Parameters[0] <= 0){
                            cout << "Critical Warning: The points you entered make no sense whatsoever. The Semi-Latus Rectum is negative. What on Earth does this physically mean?. Please resample.\n"
                                    "If no, then the Semi-Latus Rectum will be clamped at one, and " << name << " will likely immediately crash into " << Orbit << ".  (Yes(Recommended)/No)" << endl;
                            Answer = yes_no();
                            if(Answer == "no"){
                                Parameters[0] = 1.0;
                                break;
                            }
                        }
                        else{*/
       // vector<double*> v;
       // for_each(v.begin(), v.end(), std::default_delete<Type>());
                /* for(double* Point : Points){
           // cout << *Point + 1 << endl;
           // cout << *(*Point + 1) << endl;
            /*delete (*Point + 1);
            cout << *(*Point + 1) << endl;
            cout << *Point << endl;
            delete (*Point);
            cout << "Point: " << **(Point) << endl;
            delete Point;
            cout << "Point: " << *(Point) << endl;
            cout << *(Point + 1) << endl;
        }*/
// * */