/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "TrackingSystem.h"

using namespace std;

//To do list: replace globals with a singleton Settings Class object//
//Orbit Generator Settings//
extern Settings Set;
extern iMass Mass_Val;
extern iPosition Pos_Val;
extern iRadius Rad_Val;
extern int Ecc_Dec;
extern int Ecc_Range;
extern std::string Orbit;
extern char Axis;

//Default Values//
extern Settings Default;
extern iMass MassDefault;
extern iPosition Pos_Def;
extern iRadius Rad_Def;
extern int Ecc_Dec_Def;
extern int Ecc_Range_Def;
extern char Axis_Def;
extern std::string Body_Def;

void Initialize(){//Initialize Defaults//This function might not be necessary//
    Settings Seti;
    iMass Mass_Vali;
    iPosition Pos_Vali;
    iRadius Rad_Vali;
    int Ecc_Deci;
    int Ecc_Rangei;
    
    Seti.IMass = Zero;
    Seti.IPosition = Zero;
    Seti.Axis = false;
    Seti.IRadius = Zero;
    Seti.IEccentric = Zero;
    Seti.Perihelion = false;
    Seti.Body = false;
    Seti.Sampler = false;
    ////////////Mass///////////////
    Mass_Vali.Max = 1000;
    Mass_Vali.Min = 1;
    Mass_Vali.Powermax = 15;
    Mass_Vali.Powermin = 5;
    Mass_Vali.Decimal = 0;
    //////////Position/////////////
    Pos_Vali.Decimalx = 3;
    Pos_Vali.Decimaly = 3;//0;
    Pos_Vali.Decimalz = 3;
    Pos_Vali.Maxx = 100;
    Pos_Vali.Minx = 0;
    Pos_Vali.Maxy = 100;
    Pos_Vali.Miny = 0;
    Pos_Vali.Maxz = 100;
    Pos_Vali.Minz = 0;
    Pos_Vali.Powerxmax = 7;
    Pos_Vali.Powerxmin = 6;
    Pos_Vali.Powerymax = 7;
    Pos_Vali.Powerymin = 6;            
    Pos_Vali.Powerzmax = 7;
    Pos_Vali.Powerzmin = 6;
    Pos_Vali.X = Maybe;
    Pos_Vali.Y = Maybe;
    Pos_Vali.Z = Maybe;
    ///////////Radius//////////////
    Rad_Vali.Decimal = 3;
    Rad_Vali.Max = 10;
    Rad_Vali.Min = 1;
    Rad_Vali.Powermax = 8;
    Rad_Vali.Powermin = 4;
    Ecc_Deci = 3;
    Ecc_Rangei = 0;
    
    //Apply Defaults//
    Default = Seti;
    MassDefault = Mass_Vali;
    Pos_Def = Pos_Vali;
    Rad_Def = Rad_Vali;
    Ecc_Dec_Def = Ecc_Deci;
    Ecc_Range_Def = Ecc_Rangei;//No greater than 2//
    Axis_Def = 'Z';
    Body_Def = "Sun";
}

void print_User_Settings(){
    cout << "///////////////////Operational Settings/////////////////////////" << endl;
    cout << "Mass: ";
    if(Default.IMass == Zero)
        cout << "Zero User Specification\n";
    else if(Default.IMass == Partial)
        cout << "Partial User Specification (RNG Parameters are user specified)\n";
    else
        cout << "Full User Specification\n";
    cout << "Position: ";
    if(Default.IPosition == Zero)
        cout << "Zero User Specification\n";
    else if(Default.IPosition == Partial)
        cout << "Partial User Specification\n";
    else
        cout << "Full User Specification\n";
    cout << "Radius: ";
    if(Default.IRadius == Zero)
        cout << "Zero User Specification\n";
    else if(Default.IRadius == Partial)
        cout << "Partial User Specification\n";
    else
        cout << "Full User Specification\n";    
    cout << "Eccentricity: ";
    if(Default.IEccentric == Zero)
        cout << "Zero User Specification\n";
    else if(Default.IEccentric == Partial)
        cout << "Partial User Specification\n";
    else
        cout << "Full User Specification\n";
    cout << "Custom Orbit Sampler: ";
    if(Default.Sampler)
        cout << "Point Sampler On\n";
    else
        cout << "Point Sampler Off\n";
    cout << "Orbital Axis Specification: ";//This name might change//
    if(Default.Axis)
        cout  << "On\n";
    else
        cout << "Off\n";
    cout << "Argument of Perihelion Specification: ";
    if(Default.Perihelion)
        cout << "On\n";
    else
        cout << "Off\n";
    cout << "Orbital Body Specification: ";
    if(Default.Body)
        cout << "On\n";
    else
        cout << "Off\n";
    cout << "////////////////////////////////////////////////////////////////" << endl;    
}

void print_settings(){
    cout << "Default Settings:" << endl;
    print_User_Settings();
    cout << "/////////////////Default Mass Settings//////////////////////////" << endl;
    cout << "Number of decimal places: " << MassDefault.Decimal << endl;
    cout << "Maximum number value to be multiplied by a power of 10 (scaled): " << MassDefault.Max << endl;
    cout << "Minimum number value to scaled: " << MassDefault.Min << endl;
    cout << "Maximum power number to scale by: " << MassDefault.Powermax << endl;
    cout << "Minimum power number to scale by: " << MassDefault.Powermin << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "/////////////////Default Position Settings//////////////////////" << endl;
    cout << "Decimal places for X coordinate: " << Pos_Def.Decimalx << " Y Coordinate: " << Pos_Def.Decimaly << " Z Coordinate: " << Pos_Def.Decimalz << endl;
    cout << "Max value to be scaled for X, Y, and Z: " << Pos_Def.Maxx << ", " << Pos_Def.Maxy << ", and " << Pos_Def.Maxz << endl;
    cout << "Min value to be scaled for X, Y, and Z: " << Pos_Def.Minx << ", " << Pos_Def.Miny << ", and " << Pos_Def.Minz << endl;
    cout << "Max power number to scale by for X, Y, and Z: " << Pos_Def.Powerxmax << ", " << Pos_Def.Powerymax << ", and " << Pos_Def.Powerzmax << endl;
    cout << "Min power number to scale by for X, Y, and Z: " << Pos_Def.Powerxmin << ", " << Pos_Def.Powerymin << ", and " << Pos_Def.Powerzmin << endl;
    cout << "X Coordinate Sign: ";
    if(Pos_Def.X == Yes)
        cout << "Positive" << endl;
    else if(Pos_Def.X == No)
        cout << "Negative" << endl;
    else
        cout << "Random" << endl;
    cout << "Y Coordinate Sign: ";
    if(Pos_Def.Y == Yes)
        cout << "Positive" << endl;
    else if(Pos_Def.Y == No)
        cout << "Negative" << endl;
    else
        cout << "Random" << endl;    
    cout << "Z Coordinate Sign: ";
    if(Pos_Def.Z == Yes)
        cout << "Positive" << endl;
    else if(Pos_Def.Z == No)
        cout << "Negative" << endl;
    else
        cout << "Random" << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "///////////////////Default Radius Settings//////////////////////" << endl;
    cout << "Number of decimal places: " << Rad_Def.Decimal << endl;
    cout << "Max value to be scaled: " << Rad_Def.Max << endl;
    cout << "Min value to be scaled: " << Rad_Def.Min << endl;
    cout << "Max power number to scale by: " << Rad_Def.Powermax << endl;
    cout << "Min power number to scale by: " << Rad_Def.Powermin << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "///////////////Default Eccentricity Settings////////////////////" << endl;
    cout << "Number of decimal places (if less than 2): " << Ecc_Dec_Def << endl;
    cout << "Eccentricity Integer Range (0-0,1, or 2): 0-" << Ecc_Range_Def << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "///////////////////////Extra Settings///////////////////////////" << endl;
    cout << "Default Axis of Orbital Rotation: " << Axis_Def << endl;
    cout << "Default Body Being Orbited: " << Body_Def << endl;  
}

void Set_Settings(bool Def){//Change the Default Settings//
    string Answer;
    while(true){
        cout << "Which setting would you like to change.(Axis, Body,  Perihelion, IMass, IPosition, IRadius, IEccentric, Sampler). Enter 'done' if finished." << endl;
        while(true){
            cin >> Answer;
            Answer = str_lower(Answer);
            if(Answer == "axis" || Answer == "body"  || Answer == "perihelion"  || Answer == "imass"  || Answer == "iposition"  || Answer ==  "iradius" || Answer == "ieccentric")
                break;
            if(Answer == "sampler" || Answer == "done")
                break;
            cout << "Error, invalid input." << endl; 
        }
        if(Answer == "done")
            break;
        if(Answer == "axis"){
            cout << "Would you like to specify your own axis value? (Yes/No)" << endl;
            Answer = yes_no();
            if(Def){
                if(Answer == "yes")
                    Default.Axis = true;
                else
                    Default.Axis = false;
            }
            else{
                if(Answer == "yes")
                    Set.Axis = true;
                else
                    Set.Axis = false;
            }
        }
        else if(Answer == "body"){
            cout << "Would you like to specify the name of the celestial body being orbited? (Yes/No)" << endl;
            Answer = yes_no();
            if(Def){
                if(Answer == "yes")
                    Default.Body = true;
                else
                    Default.Body = false;                 
            }
            else{
                if(Answer == "yes")
                    Set.Body = true;
                else
                    Set.Body = false;
            }
        }
        else if(Answer == "perihelion"){
            cout << "Would you like to specify the argument of perihelion? (Yes/No)" << endl;
            Answer = yes_no();
            if(Def){
                if(Answer == "yes")
                    Default.Perihelion = true;
                else
                    Default.Perihelion = false;                 
            }
            else{
                if(Answer == "yes")
                    Set.Perihelion = true;
                else
                    Set.Perihelion = false;
            }
        }
        else if(Answer == "imass"){
            cout << "Would you like to fully specify the object's mass, partially specify the parameters to be used to generate it randomly,"
                    " or just use the default parameters for random mass generation?" << endl;
            cout << "(Fully, Partially, Default)" << endl;
            while(true){
                cin >> Answer;
                Answer = str_lower(Answer);
                if(Answer == "fully" || Answer == "partially" || Answer == "default")
                    break;
                cout << "Error, invalid input. (Fully, Partially, Default)" << endl;
            }
            if(Def){
                if(Answer == "fully")
                    Default.IMass = Full;
                if(Answer == "partially")
                    Default.IMass = Partial;
                if(Answer == "default")
                    Default.IMass == Zero;                
            }
            else{
                if(Answer == "fully")
                    Set.IMass = Full;
                if(Answer == "partially")
                    Set.IMass = Partial;
                if(Answer == "default")
                    Set.IMass == Zero;
            }
        }
        else if(Answer == "iposition"){
            cout << "Would you like to fully specify an initial default reference position, partially specify the parameters to be used to generate it randomly,"
                    " or just use the default parameters?" << endl;
            cout << "(Fully, Partially, Default)" << endl;
            while(true){
                cin >> Answer;
                Answer = str_lower(Answer);
                if(Answer == "fully" || Answer == "partially" || Answer == "default")
                    break;
                cout << "Error, invalid input. (Fully, Partially, Default)" << endl;
            }
            if(Def){
                if(Answer == "fully")
                    Default.IPosition = Full;
                if(Answer == "partially")
                    Default.IPosition = Partial;
                if(Answer == "default")
                    Default.IPosition == Zero;                 
            }
            else{
                if(Answer == "fully")
                    Set.IPosition = Full;
                if(Answer == "partially")
                    Set.IPosition = Partial;
                if(Answer == "default")
                    Set.IPosition == Zero;
            }
        }
        else if(Answer == "iradius"){
            cout << "Would you like to fully specify the object's radius, partially specify the parameters to be used to generate it randomly,"
                    " or just use the default parameters for random radius generation?" << endl;
            cout << "(Fully, Partially, Default)" << endl;
            while(true){
                cin >> Answer;
                Answer = str_lower(Answer);
                if(Answer == "fully" || Answer == "partially" || Answer == "default")
                    break;
                cout << "Error, invalid input. (Fully, Partially, Default)" << endl;
            }
            if(Def){
                if(Answer == "fully")
                    Default.IRadius = Full;
                if(Answer == "partially")
                    Default.IRadius = Partial;
                if(Answer == "default")
                    Default.IRadius == Zero;                 
            }
            else{
                if(Answer == "fully")
                    Set.IRadius = Full;
                if(Answer == "partially")
                    Set.IRadius = Partial;
                if(Answer == "default")
                    Set.IRadius == Zero;
            }
        }
        else if(Answer == "ieccentric"){
            cout << "Would you like to fully specify the object's orbital eccentricity, partially specify the parameters to be used to generate it randomly,"
                    " or just use the default parameters for random eccentricity generation?" << endl;
            cout << "(Fully, Partially, Default)" << endl;
            while(true){
                cin >> Answer;
                Answer = str_lower(Answer);
                if(Answer == "fully" || Answer == "partially" || Answer == "default")
                    break;
                cout << "Error, invalid input. (Fully, Partially, Default)" << endl;
            }
            if(Def){
                Default.Sampler = false;
                if(Answer == "fully")
                    Default.IEccentric = Full;
                if(Answer == "partially")
                    Default.IEccentric = Partial;
                if(Answer == "default")
                    Default.IEccentric == Zero;                 
            }
            else{
                Set.Sampler = false;
                if(Answer == "fully")
                    Set.IEccentric = Full;
                if(Answer == "partially")
                    Set.IEccentric = Partial;
                if(Answer == "default")
                    Set.IEccentric == Zero;
            }
        }
        else{
            cout << "Would you like to input your own sample points for the object's orbit to pass through (or come as close as possible to passing through)? (Yes/No)" << endl;
            Answer = yes_no();
            if(Def){
                Default.IEccentric = Zero;
                if(Answer == "yes")
                    Default.Sampler = true;
                else
                    Default.Sampler = false;                
            }
            else{
                Set.IEccentric = Zero;
                if(Answer == "yes")
                    Set.Sampler = true;
                else
                    Set.Sampler = false;
            }
        }
    }    
}

void Set_Body(bool Def){
    cout << "Enter the name of the Celestial Body being orbited" << endl;
    if(Def)
        cin >> Body_Def;
    else
        cin >> Orbit;        
}

void Set_Axis(bool Def){
    string A;
    cout << "Set an axis of rotation for the angle of inclination. (X, Y, Z)" << endl;
    while(true){
        cin >> A;
        A = str_lower(A);
        if(A == "x" || A == "y" || A == "z"){
            A[0] = toupper(A[0]);
            if(Def)
                Axis_Def = A[0];
            else
                Axis = A[0];
            break;
        }
        cout << "Error. Invalid input" << endl;
    }    
}

void Set_Eccentric(bool Def){
    string Answer;
    cout << "Enter the value range for the eccentricity to fall under. 0 to 0, 0 to 1, or 0 to 2 (Enter 0, 1, or 2)" << endl;                
    while(true){
        cin >> Answer;
        if(Answer == "0" || Answer == "1" || Answer == "2")
            break;
        cout << "Error, please enter 0, 1, or 2." << endl;
    }
    if(Def)
        Ecc_Range_Def = (int)convert(Answer);
    else
        Ecc_Range = (int)convert(Answer);
    cout << "Enter a value specifying the number of decimal places the Eccentricity will have." << endl;
    while(true){
        Answer = verify_pointdouble();
        if(Answer == "done")
            cout << "Error, please enter a number" << endl;
        else{
            double Test = convert(Answer);
            if(Test - (int)Test == 0 && 0 <= convert(Answer)){
                if(Def)
                    Ecc_Dec_Def = Test;
                else
                    Ecc_Dec = Test;
                break;
            }
            cout << "Error, please enter a positive integer value." << endl;
        }
    }    
}

void Set_Radius(bool Def){
    string Answer;
    while(true){
        cout << "What parameters would you like to specify? (ValMax, ValMin, PowerMax, PowerMin, Decimal) Enter 'done' when finished." << endl;
        while(true){
            cin >> Answer;
            Answer = str_lower(Answer);
            if(Answer == "valmax" || Answer == "valmin" || Answer == "powermax" || Answer == "powermin" || Answer == "decimal" || Answer == "done")
                break;
            cout << "Invalid input" << endl;
        }
        if(Answer == "done")
            break;
        Answer[0] = toupper(Answer[0]);
        cout << "Enter a new integer value for " << Answer << endl;
        double Test;
        int Result;
        string Ans;
        while(true){
            Ans = verify_pointdouble();
            if(Ans == "done")
                cout << "Invalid Input. Please enter an integer value." << endl;
            else{
                Test = convert(Ans);
                if(Test - (int)Test == 0 && 0 < convert(Ans)){
                    Result = (int)Test;
                    break;
                }
                cout << "Error, please enter a positive integer value." << endl;
            }
        }
        if(Def){
            if(Answer == "Valmax" && Rad_Def.Min <= Result)
                Rad_Def.Max = Result;
            else if(Answer == "Valmin" && Result <= Rad_Def.Max)
                Rad_Def.Min = Result;
            else if(Answer == "Powermax" && Rad_Def.Powermin <= Result)
                Rad_Def.Powermax = Result;
            else if(Answer == "Powermin" && Result <= Rad_Def.Powermax)
                Rad_Def.Powermin = Result;
            else if(Answer == "Decimal")
                Rad_Def.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;           
        }
        else{
            if(Answer == "Valmax" && Rad_Val.Min <= Result)
                Rad_Val.Max = Result;
            else if(Answer == "Valmin" && Result <= Rad_Val.Max)
                Rad_Val.Min = Result;
            else if(Answer == "Powermax" && Rad_Val.Powermin <= Result)
                Rad_Val.Powermax = Result;
            else if(Answer == "Powermin" && Result <= Rad_Val.Powermax)
                Rad_Val.Powermin = Result;
            else if(Answer == "Decimal")
                Rad_Val.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
        }
    }    
}

void Set_Position(bool Def){
    string Answer;
    while(true){
        cout << "What parameter would you like to specify? (Decimal, Val, Power, Sign)" << endl;
        while(true){
            cin >> Answer;
            Answer = str_lower(Answer);
            if(Answer == "val" || Answer == "power" || Answer == "decimal" || Answer == "sign" || Answer == "done")
                break;
            cout << "Invalid input" << endl;
        }
        if(Answer == "done")
            break;
        Answer[0] = toupper(Answer[0]);
        if(Answer == "Val"){
            while(true){
                cout << "What value would you like to change? (Maxx, Maxy, Maxz, Minx, Miny, Minz)" << endl;
                while(true){
                    cin >> Answer;
                    Answer = str_lower(Answer);
                    if(Answer == "maxx" || Answer == "maxy" || Answer == "maxz" || Answer == "minx" || Answer == "miny" || Answer == "minz" || Answer == "done")
                        break;
                    cout << "Invalid input" << endl;
                }
                if(Answer == "done")
                    break;
                Answer[0] = toupper(Answer[0]);
                cout << "Enter a new integer value for " << Answer << endl;
                double Test;
                int Result;
                string Ans;
                while(true){
                    Ans = verify_pointdouble();
                    if(Ans == "done")
                        cout << "Invalid Input. Please enter an integer value." << endl;
                    else{
                        Test = convert(Ans);
                        if(Test - (int)Test == 0 && 0 <= convert(Ans)){
                            Result = (int)Test;
                            break;
                        }
                        cout << "Error, please enter a positive integer value." << endl;
                    }
                }
                if(Def){
                    if(Answer == "Maxx" && Pos_Def.Minx <= Result)
                        Pos_Def.Maxx = Result;
                    else if(Answer == "Maxy" && Pos_Def.Miny <= Result)
                        Pos_Def.Maxy = Result;
                    else if(Answer == "Maxz" && Pos_Def.Minz <= Result)    
                        Pos_Def.Maxz = Result;
                    else if(Answer == "Minx" && Result <= Pos_Def.Maxx)
                        Pos_Def.Minx = Result;
                    else if(Answer == "Miny" && Result <= Pos_Def.Maxy)
                        Pos_Def.Miny = Result;
                    else if(Answer == "Minz" && Result <= Pos_Def.Maxz)
                        Pos_Def.Minz = Result;
                    else
                    cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
                }
                else{
                    if(Answer == "Maxx" && Pos_Val.Minx <= Result)
                        Pos_Val.Maxx = Result;
                    else if(Answer == "Maxy" && Pos_Val.Miny <= Result)
                        Pos_Val.Maxy = Result;
                    else if(Answer == "Maxz" && Pos_Val.Minz <= Result)    
                        Pos_Val.Maxz = Result;
                    else if(Answer == "Minx" && Result <= Pos_Val.Maxx)
                        Pos_Val.Minx = Result;
                    else if(Answer == "Miny" && Result <= Pos_Val.Maxy)
                        Pos_Val.Miny = Result;
                    else if(Answer == "Minz" && Result <= Pos_Val.Maxz)
                        Pos_Val.Minz = Result;
                    else
                    cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
                }
            }
        }
        else if(Answer == "Power"){
            while(true){
                cout << "What value would you like to change? (Maxx, Maxy, Maxz, Minx, Miny, Minz)" << endl;
                while(true){
                    cin >> Answer;
                    Answer = str_lower(Answer);
                    if(Answer == "maxx" || Answer == "maxy" || Answer == "maxz" || Answer == "minx" || Answer == "miny" || Answer == "minz" || Answer == "done")
                        break;
                    cout << "Invalid input" << endl;
                }
                if(Answer == "done")
                    break;
                Answer[0] = toupper(Answer[0]);
                cout << "Enter a new integer value for " << Answer << endl;
                double Test;
                int Result;
                string Ans;
                while(true){
                    Ans = verify_pointdouble();
                    if(Ans == "done")
                        cout << "Invalid Input. Please enter an integer value." << endl;
                    else{
                        Test = convert(Ans);
                        if(Test - (int)Test == 0 && 0 <= convert(Ans)){
                            Result = (int)Test;
                            break;
                        }
                        cout << "Error, please enter a positive integer value." << endl;
                    }
                }
                if(Def){
                    if(Answer == "Maxx" && Pos_Def.Powerxmin <= Result)
                        Pos_Def.Powerxmax = Result;
                    else if(Answer == "Maxy" && Pos_Def.Powerymin <= Result)
                        Pos_Def.Powerymax = Result;
                    else if(Answer == "Maxz" && Pos_Def.Powerzmin <= Result)    
                        Pos_Def.Powerzmax = Result;
                    else if(Answer == "Minx" && Result <= Pos_Def.Powerxmax)
                        Pos_Def.Powerxmin = Result;
                    else if(Answer == "Miny" && Result <= Pos_Def.Powerymax)
                        Pos_Def.Powerymin = Result;
                    else if(Answer == "Minz" && Result <= Pos_Def.Powerzmax)
                        Pos_Def.Powerzmin = Result;
                    else
                    cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;                     
                }
                else{
                    if(Answer == "Maxx" && Pos_Val.Powerxmin <= Result)
                        Pos_Val.Powerxmax = Result;
                    else if(Answer == "Maxy" && Pos_Val.Powerymin <= Result)
                        Pos_Val.Powerymax = Result;
                    else if(Answer == "Maxz" && Pos_Val.Powerzmin <= Result)    
                        Pos_Val.Powerzmax = Result;
                    else if(Answer == "Minx" && Result <= Pos_Val.Powerxmax)
                        Pos_Val.Powerxmin = Result;
                    else if(Answer == "Miny" && Result <= Pos_Val.Powerymax)
                        Pos_Val.Powerymin = Result;
                    else if(Answer == "Minz" && Result <= Pos_Val.Powerzmax)
                        Pos_Val.Powerzmin = Result;
                    else
                    cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl; 
                }
            }
        }
        else if(Answer == "Decimal"){
            while(true){
                cout << "What value would you like to change? (Decimalx, Decimaly, Decimalz)" << endl;
                while(true){
                    cin >> Answer;
                    Answer = str_lower(Answer);
                    if(Answer == "decimalx" || Answer == "decimaly" || Answer == "decimalz" || Answer == "done")
                        break;
                    cout << "Invalid input" << endl;
                }
                if(Answer == "done")
                    break;
                Answer[0] = toupper(Answer[0]);
                cout << "Enter a new integer value for " << Answer << endl;
                double Test;
                int Result;
                string Ans;
                while(true){
                    Ans = verify_pointdouble();
                    if(Ans == "done")
                        cout << "Invalid Input. Please enter an integer value." << endl;
                    else{
                        Test = convert(Ans);
                        if(Test - (int)Test == 0 && 0 <= convert(Ans)){
                            Result = (int)Test;
                            break;
                        }
                        cout << "Error, please enter a positive integer value." << endl;
                    }
                }
                if(Def){
                    if(Answer == "Decimalx")
                        Pos_Def.Decimalx = Result;
                    if(Answer == "Decimaly")
                        Pos_Def.Decimaly = Result;
                    if(Answer == "Decimalz")    
                        Pos_Def.Decimalz = Result;                     
                }
                else{
                    if(Answer == "Decimalx")
                        Pos_Val.Decimalx = Result;
                    if(Answer == "Decimaly")
                        Pos_Val.Decimaly = Result;
                    if(Answer == "Decimalz")    
                        Pos_Val.Decimalz = Result;
                }
            }
        }
        else{
            while(true){
                cout << "What sign value would you like to change? (Signx, Signy, Signz) ('Done' if done)" << endl;
                while(true){
                    cin >> Answer;
                    Answer = str_lower(Answer);
                    if(Answer == "signx" || Answer == "signy" || Answer == "signz" || Answer == "done")
                        break;
                    cout << "Invalid input" << endl;
                }
                if(Answer == "done")
                    break;
                Answer[0] = toupper(Answer[0]);
                cout << "Enter a new value for " << Answer << "(Yes, No, Maybe)" << endl;
                string Ans;
                while(true){
                    cin >> Ans;
                    Ans = str_lower(Ans);
                    if(Ans == "yes" || Ans == "no" || Ans == "maybe")
                        break;
                    cout << "Invalid input" << endl;
                }
                if(Def){
                    if(Answer == "signx"){
                        if(Ans == "yes")
                            Pos_Def.X = Yes;
                        else if(Ans == "no")
                            Pos_Def.X = No;
                        else
                            Pos_Def.X = Maybe;
                    }
                    else if(Answer == "signy"){
                        if(Ans == "yes")
                            Pos_Def.Y = Yes;
                        else if(Ans == "no")
                            Pos_Def.Y = No;
                        else
                            Pos_Def.Y = Maybe;
                    }
                    else{
                        if(Ans == "yes")
                            Pos_Def.Z = Yes;
                        else if(Ans == "no")
                            Pos_Def.Z = No;
                        else
                            Pos_Def.Z = Maybe;
                    }                    
                }
                else{
                    if(Answer == "signx"){
                        if(Ans == "yes")
                            Pos_Val.X = Yes;
                        else if(Ans == "no")
                            Pos_Val.X = No;
                        else
                            Pos_Val.X = Maybe;
                    }
                    else if(Answer == "signy"){
                        if(Ans == "yes")
                            Pos_Val.Y = Yes;
                        else if(Ans == "no")
                            Pos_Val.Y = No;
                        else
                            Pos_Val.Y = Maybe;
                    }
                    else{
                        if(Ans == "yes")
                            Pos_Val.Z = Yes;
                        else if(Ans == "no")
                            Pos_Val.Z = No;
                        else
                            Pos_Val.Z = Maybe;
                    }
                }
            }                        
        }                        
    }    
}

void Set_Mass(bool Def){
    string Answer;
    while(true){
        cout << "What parameters would you like to specify? (ValMax, ValMin, PowerMax, PowerMin, Decimal) Enter 'done' when finished." << endl;
        while(true){
            cin >> Answer;
            Answer = str_lower(Answer);
            if(Answer == "valmax" || Answer == "valmin" || Answer == "powermax" || Answer == "powermin" || Answer == "decimal" || Answer == "done")
                break;
            cout << "Invalid input" << endl;
        }
        if(Answer == "done")
            break;
        Answer[0] = toupper(Answer[0]);
        cout << "Enter a new integer value for " << Answer << endl;
        double Test;
        int Result;
        string Ans;
        while(true){
            Ans = verify_pointdouble();
            if(Ans == "done")
                cout << "Invalid Input. Please enter an integer value." << endl;
            else{
                Test = convert(Ans);
                Test = Test - (int)Test;
                if(Test == 0 && 0 < convert(Ans)){
                    Result = convert(Ans);
                    break;
                }
                cout << "Error, please enter a positive integer value." << endl;
            }
        }
        if(Def){
            if(Answer == "Valmax" && MassDefault.Min <= Result)
                MassDefault.Max = Result;
            else if(Answer == "Valmin" && Result <= MassDefault.Max)
                MassDefault.Min = Result;
            else if(Answer == "Powermax" && MassDefault.Powermin <= Result)
                MassDefault.Powermax = Result;
            else if(Answer == "Powermin" && Result <= MassDefault.Powermax)
                MassDefault.Powermin = Result;
            else if(Answer == "Decimal")
                MassDefault.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
        }
        else{
            if(Answer == "Valmax" && Mass_Val.Min <= Result)
                Mass_Val.Max = Result;
            else if(Answer == "Valmin" && Result <= Mass_Val.Max)
                Mass_Val.Min = Result;
            else if(Answer == "Powermax" && Mass_Val.Powermin <= Result)
                Mass_Val.Powermax = Result;
            else if(Answer == "Powermin" && Result <= Mass_Val.Powermax)
                Mass_Val.Powermin = Result;
            else if(Answer == "Decimal")
                Mass_Val.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
        }
    }    
}