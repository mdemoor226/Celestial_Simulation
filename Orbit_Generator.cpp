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

#include <cfloat>
#include "TrackingSystem.h"

using namespace std;

int Generator::Rand_Orbit_Gen(Attributes* Body, float &GenMass, float &GenRadius, vector<CelestialPtr> &Solar_Bodies, const string name){
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
/////////////////////////////////////User Specification Block////////////////////////////////////
    while(true){
        string Answer;
        cout << "Would you like to use the default settings? Yes/No" << endl;
        Answer = yes_no();       
        Active = Default;
        if(Answer == "no")//Set Set//
            Set_Settings(false);
        
        if(Active.Set.Body)//Set Body
            Set_Body(false);
        
        //Get the position of Celestial Body "Orbit"//
        Attributes V;
        while(true){
            bool Status = false;     
            for(CelestialPtr Celestial : Solar_Bodies){
                V = Celestial->get_attributes();
                if(Celestial->get_Name() == Active.Orbit && Get_Status(Celestial) == "Intact"){
                    translate[0] = -V.Rx;
                    translate[1] = -V.Ry;
                    translate[2] = -V.Rz;
                    Status = true;
                    break;
                }
            }
            if(Status)
                break;
            cout << "\"" << Active.Orbit 
            << "\" was either destroyed in a simulation or never existed to begin with.\n Enter another"
            << " Celestial body for " << name << " to orbit around please. 'Cancel' to cancel" << endl;
            cin >> Active.Orbit;
            if(str_lower(Active.Orbit) == "cancel")
                return 1;
        }
        OrbMass = Solar_Bodies[V.ID]->get_Mass();//We'll need this mass value later. Assign it to a new variable before V(retrieved above) goes out of scope//
        OrbVel[0] = V.Vx;//Same here//
        OrbVel[1] = V.Vy;//And here//
        OrbVel[2] = V.Vz;//And here too//  
        OrbPos[0] = V.Rx;
        OrbPos[1] = V.Ry;
        OrbPos[2] = V.Rz;
        
        if(Active.Set.Axis)//Set Axis
            Set_Axis(false);
        
        if(Active.Set.Perihelion){
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
        
        if(Active.Set.IEccentric == Full){
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
            if(Active.Set.IEccentric == Partial)
                Set_Eccentric(false);
        }
        
        if(Active.Set.IRadius == Full){
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
            if(Active.Set.IRadius == Partial)
                Set_Radius(false);
        }
        
        if(Active.Set.IPosition == Full){
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
            if(Active.Set.IPosition == Partial)
                Set_Position(false);
        }             
        
        if(Active.Set.IMass == Full){
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
            if(Active.Set.IMass == Partial)
                Set_Mass(false);                              
        }            
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////RNG Block//////////////////////////////////////////////////
        if(Active.Set.IMass != Full){
            ObjMass = Gen_Mass(&OrbMass);
            if(ObjMass == -1)
                return 1;
        }
        if(Active.Set.IRadius != Full)
            ObjRadius = Gen_Radius();       
        if(Active.Set.IPosition != Full){
            if(Gen_Position(Solar_Bodies, ObjRadius, Pos, OrbPos))
                return 1;
        }
        if(!Active.Set.Perihelion)
            Perihelion = rand() % 360;
        if(!Active.Set.Sampler && Active.Set.IEccentric != Full)    
            Eccent = Gen_Eccent();        
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////Information Processing Block////////////////////////////////////  
        //Apply Translation//
        Pos[0] += translate[0];//x
        Pos[1] += translate[1];//y
        Pos[2] += translate[2];//z

        //Rotate Initial Position onto x-z(or whatever respective 2-D) plane, save the inverse transformation for later//
        Theta = get_rotation(Pos);//HERE
        Rotate(Pos, Theta);

        //Translate into a 2-D "(x,y)" coordinate//
        if(Active.Axis == 'Z' || Active.Axis == 'X'){//x-z plane to x-y coordinate
            P1[0] = Pos[0];
            P1[1] = -Pos[2];
        }
        if(Active.Axis == 'Y'){//x-y plane to x-y coordinate
            P1[0] = Pos[0];
            P1[1] = Pos[1];
        }
        
        //If necessary, apply Least Squares on specified points to obtain Eccentricity and Semi-Latus Rectum//
        float Peri = 2*pi*(Perihelion/360.0);   
        if(Active.Set.Sampler){
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
        cout << "Warning: " << name << " will likely crash into " << Active.Orbit << ". Are you fine with this? Yes/No (No will make you restart the Orbit Customizer.)" << endl;
        Answer = yes_no();
        if(Answer == "yes")
            break;
    }
    
    if(!Points.empty()){//Deallocate all memory in "Points" and clear vector//Possible Memory Leak//
        vector<double*>::const_iterator Point=Points.begin();
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
    Vel[0] = sqrt(Vel[0]);//Given the starting angle, whether the square root is positive or negative will determine whether the new orbit is clockwise or counter-clockwise//
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
    if(Active.Axis == 'Z' || Active.Axis == 'X'){//x-y coordinate to x-z plane
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
    Rotate(Vel, -Theta);
    Rotate(Pos, -Theta);
    
    //Calculate Initial Velocity with respect to origin in World Space(Rather than with respect to the body being orbited//
    Vel[0] += OrbVel[0];
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
            
int Generator::Gen_Position(vector<CelestialPtr> &Solar_Bodies, float Radius, double* Pos, double* OrbPos){
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
        if(Active.Pos_Val.X == Maybe){
            if((rand() % 2) == 0)
                signx = true;//Positive//
            else
                signx = false;//Negative//
        }
        else if(Active.Pos_Val.X == Yes)
            signx = true;//Positive
        else
            signx = false;//Negative

        if(Active.Pos_Val.Y == Maybe){
            if((rand() % 2) == 0)
                signy = true;//Positive//
            else
                signy = false;//Negative//
        }
        else if(Active.Pos_Val.Y == Yes)
            signy = true;//Positive
        else
            signy = false;//Negative    

        if(Active.Pos_Val.Z == Maybe){
            if((rand() % 2) == 0)
                signz = true;//Positive//
            else
                signz = false;//Negative//
        }
        else if(Active.Pos_Val.Z == Yes)
            signz = true;//Positive
        else
            signz = false;//Negative    

        Dec = (double)(rand() % (int)pow(10,Active.Pos_Val.Decimalx))/(pow(10,Active.Pos_Val.Decimalx));
        Val = (rand() % (Active.Pos_Val.Maxx - Active.Pos_Val.Minx + 1)) + Active.Pos_Val.Minx;
        Power = (rand() % (Active.Pos_Val.Powerxmax - Active.Pos_Val.Powerxmin + 1)) + Active.Pos_Val.Powerxmin;
        Val += Dec;
        if(signx)
            Pos[0] = Val*pow(10,Power);
        else
            Pos[0] = -Val*pow(10,Power);

        Dec = (double)(rand() % (int)pow(10,Active.Pos_Val.Decimaly))/(pow(10,Active.Pos_Val.Decimaly));
        Val = (rand() % (Active.Pos_Val.Maxy - Active.Pos_Val.Miny + 1)) + Active.Pos_Val.Miny;
        Power = (rand() % (Active.Pos_Val.Powerymax - Active.Pos_Val.Powerymin + 1)) + Active.Pos_Val.Powerymin;
        Val += Dec;
        if(signy)
            Pos[1] = Val*pow(10,Power);
        else
            Pos[1] = -Val*pow(10,Power);

        Dec = (double)(rand() % (int)pow(10,Active.Pos_Val.Decimalz))/(pow(10,Active.Pos_Val.Decimalz));
        Val = (rand() % (Active.Pos_Val.Maxz - Active.Pos_Val.Minz + 1)) + Active.Pos_Val.Minz;
        Power = (rand() % (Active.Pos_Val.Powerzmax - Active.Pos_Val.Powerzmin + 1)) + Active.Pos_Val.Powerzmin;
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
    cout << "Error, failed to generate a proper orbital position far enough away from " << Active.Orbit << ".\nRadius of " << Active.Orbit << " might"
            "be too large and creating conflict.\nSuggestion: Change the Generator Settings to allow orbit generation farther away from " << Active.Orbit << "." << endl;
    return 1;
}

float Generator::Gen_Mass(float *Orb){
    float Val;
    float Dec;
    float Power;
    int Count = 0;
    while(Count < 10000){
        Dec = (float)(rand() % (int)pow(10,Active.Mass_Val.Decimal))/(pow(10,Active.Mass_Val.Decimal));
        Val = (rand() % (Active.Mass_Val.Max - Active.Mass_Val.Min + 1)) + Active.Mass_Val.Min;
        Power = (rand() % (Active.Mass_Val.Powermax - Active.Mass_Val.Powermin + 1)) + Active.Mass_Val.Powermin;
        Val += Dec;    
        Val = Val*pow(10,Power);
        if(Verify_Mass(&Val, Orb))
            return Val;
        Count++;
    }
    cout << "Error, could not generate an acceptable mass value. Mass of " << Active.Orbit << " might be too large." << endl;
    return -1;
}

bool Generator::Verify_Mass(float *Obj, float *Orb){
    if(*Obj > FLT_MAX/(*Orb))
        return false;
    return true;
}

float Generator::Gen_Radius(){
    float Dec = (float)(rand() % (int)pow(10,Active.Rad_Val.Decimal))/(pow(10,Active.Rad_Val.Decimal));
    float Val = (rand() % (Active.Rad_Val.Max - Active.Rad_Val.Min + 1)) + Active.Rad_Val.Min;
    int Power = (rand() % (Active.Rad_Val.Powermax - Active.Rad_Val.Powermin + 1)) + Active.Rad_Val.Powermin;
    Val += Dec;
    return Val*pow(10,Power);
}

float Generator::Gen_Eccent(){
    float Dec = (float)(rand() % (int)pow(10,Active.Ecc_Dec))/(pow(10,Active.Ecc_Dec));
    float Val = (rand() % (Active.Ecc_Range + 1));
    if(Val < 2)
        Val += Dec;
    return Val;
}

double Generator::SLRec_Calc(const float Eccent, const double* P1, const float Peri){
    double r = sqrt(pow(P1[0],2) + pow(P1[1],2));
    float Theta = get_theta(P1[0], P1[1], r);
    return r*(1 + Eccent*cos(Theta - Peri));    
}

float Generator::get_rotation(double* Pos){
    float Phi;
    switch(Active.Axis){
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

void Generator::Rotate(double* PS, float phi){
    float E = cos(phi);
    float F = sin(phi);
    double a, b;
    switch(Active.Axis){
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

float Generator::get_theta(double x, double y, double r){
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

void Generator::Specify_Path(vector<double*> &Points, double* P1, float* Theta, float Peri, float &Eccent, double &SLRec){
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

int Generator::Point_Sampler(vector<double*>& Points, float *SAngle, float *Theta){
    int Count = 1;
    vector<double> Inputs;// = {9923423432.0, -3249324.0, 3924.3294923, 32432499432.0, -32949324.0, 32943294943.0, 0.0, -3294923494.0};
    double Inputx, Inputy;
    string number;
    bool equal;    
    if(!Points.empty()){//Possible Memory Leak//
        vector<double*>::const_iterator Point=Points.begin();
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

double Generator::get_Error(vector<double*> &Points, float E, float Peri, double SL){
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

int Generator::LS_Calculations(vector<double*> &Points, double* Result, float Peri){
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

int Generator::Least_Squares(double* Results, double* P1, vector<double*> &Points, float SAngle){
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
