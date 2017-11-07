/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "TrackingSystem.h"

using namespace std;

Generator::Generator(){Initialize();}

void Generator::Set_Defaults(){
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

void Generator::Initialize(){//Initialize Defaults//This function might not be necessary//   
    Default.Set.IMass = Zero;
    Default.Set.IPosition = Zero;
    Default.Set.Axis = false;
    Default.Set.IRadius = Zero;
    Default.Set.IEccentric = Zero;
    Default.Set.Perihelion = false;
    Default.Set.Body = false;
    Default.Set.Sampler = false;
    ////////////Mass///////////////
    Default.Mass_Val.Max = 1000;
    Default.Mass_Val.Min = 1;
    Default.Mass_Val.Powermax = 15;
    Default.Mass_Val.Powermin = 5;
    Default.Mass_Val.Decimal = 0;
    //////////Position/////////////
    Default.Pos_Val.Decimalx = 3;
    Default.Pos_Val.Decimaly = 3;//0;
    Default.Pos_Val.Decimalz = 3;
    Default.Pos_Val.Maxx = 100;
    Default.Pos_Val.Minx = 0;
    Default.Pos_Val.Maxy = 100;
    Default.Pos_Val.Miny = 0;
    Default.Pos_Val.Maxz = 100;
    Default.Pos_Val.Minz = 0;
    Default.Pos_Val.Powerxmax = 7;
    Default.Pos_Val.Powerxmin = 6;
    Default.Pos_Val.Powerymax = 7;
    Default.Pos_Val.Powerymin = 6;            
    Default.Pos_Val.Powerzmax = 7;
    Default.Pos_Val.Powerzmin = 6;
    Default.Pos_Val.X = Maybe;
    Default.Pos_Val.Y = Maybe;
    Default.Pos_Val.Z = Maybe;
    ///////////Radius//////////////
    Default.Rad_Val.Decimal = 3;
    Default.Rad_Val.Max = 10;
    Default.Rad_Val.Min = 1;
    Default.Rad_Val.Powermax = 8;
    Default.Rad_Val.Powermin = 4;
    
    //////////Other////////////////
    Default.Ecc_Dec = 3;
    Default.Ecc_Range = 0;
    Default.Axis = 'Z';
    Default.Orbit = "Sun";
}

void Generator::print_User_Settings(){
    cout << "///////////////////Operational Settings/////////////////////////" << endl;
    cout << "Mass: ";
    if(Default.Set.IMass == Zero)
        cout << "Zero User Specification\n";
    else if(Default.Set.IMass == Partial)
        cout << "Partial User Specification (RNG Parameters are user specified)\n";
    else
        cout << "Full User Specification\n";
    cout << "Position: ";
    if(Default.Set.IPosition == Zero)
        cout << "Zero User Specification\n";
    else if(Default.Set.IPosition == Partial)
        cout << "Partial User Specification\n";
    else
        cout << "Full User Specification\n";
    cout << "Radius: ";
    if(Default.Set.IRadius == Zero)
        cout << "Zero User Specification\n";
    else if(Default.Set.IRadius == Partial)
        cout << "Partial User Specification\n";
    else
        cout << "Full User Specification\n";    
    cout << "Eccentricity: ";
    if(Default.Set.IEccentric == Zero)
        cout << "Zero User Specification\n";
    else if(Default.Set.IEccentric == Partial)
        cout << "Partial User Specification\n";
    else
        cout << "Full User Specification\n";
    cout << "Custom Orbit Sampler: ";
    if(Default.Set.Sampler)
        cout << "Point Sampler On\n";
    else
        cout << "Point Sampler Off\n";
    cout << "Orbital Axis Specification: ";//This name might change//
    if(Default.Set.Axis)
        cout  << "On\n";
    else
        cout << "Off\n";
    cout << "Argument of Perihelion Specification: ";
    if(Default.Set.Perihelion)
        cout << "On\n";
    else
        cout << "Off\n";
    cout << "Orbital Body Specification: ";
    if(Default.Set.Body)
        cout << "On\n";
    else
        cout << "Off\n";
    cout << "////////////////////////////////////////////////////////////////" << endl;    
}

void Generator::print_settings(){
    cout << "Default Settings:" << endl;
    print_User_Settings();
    cout << "/////////////////Default Mass Settings//////////////////////////" << endl;
    cout << "Number of decimal places: " << Default.Mass_Val.Decimal << endl;
    cout << "Maximum number value to be multiplied by a power of 10 (scaled): " << Default.Mass_Val.Max << endl;
    cout << "Minimum number value to scaled: " << Default.Mass_Val.Min << endl;
    cout << "Maximum power number to scale by: " << Default.Mass_Val.Powermax << endl;
    cout << "Minimum power number to scale by: " << Default.Mass_Val.Powermin << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "/////////////////Default Position Settings//////////////////////" << endl;
    cout << "Decimal places for X coordinate: " << Default.Pos_Val.Decimalx << " Y Coordinate: " << Default.Pos_Val.Decimaly << " Z Coordinate: " << Default.Pos_Val.Decimalz << endl;
    cout << "Max value to be scaled for X, Y, and Z: " << Default.Pos_Val.Maxx << ", " << Default.Pos_Val.Maxy << ", and " << Default.Pos_Val.Maxz << endl;
    cout << "Min value to be scaled for X, Y, and Z: " << Default.Pos_Val.Minx << ", " << Default.Pos_Val.Miny << ", and " << Default.Pos_Val.Minz << endl;
    cout << "Max power number to scale by for X, Y, and Z: " << Default.Pos_Val.Powerxmax << ", " << Default.Pos_Val.Powerymax << ", and " << Default.Pos_Val.Powerzmax << endl;
    cout << "Min power number to scale by for X, Y, and Z: " << Default.Pos_Val.Powerxmin << ", " << Default.Pos_Val.Powerymin << ", and " << Default.Pos_Val.Powerzmin << endl;
    cout << "X Coordinate Sign: ";
    if(Default.Pos_Val.X == Yes)
        cout << "Positive" << endl;
    else if(Default.Pos_Val.X == No)
        cout << "Negative" << endl;
    else
        cout << "Random" << endl;
    cout << "Y Coordinate Sign: ";
    if(Default.Pos_Val.Y == Yes)
        cout << "Positive" << endl;
    else if(Default.Pos_Val.Y == No)
        cout << "Negative" << endl;
    else
        cout << "Random" << endl;    
    cout << "Z Coordinate Sign: ";
    if(Default.Pos_Val.Z == Yes)
        cout << "Positive" << endl;
    else if(Default.Pos_Val.Z == No)
        cout << "Negative" << endl;
    else
        cout << "Random" << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "///////////////////Default Radius Settings//////////////////////" << endl;
    cout << "Number of decimal places: " << Default.Rad_Val.Decimal << endl;
    cout << "Max value to be scaled: " << Default.Rad_Val.Max << endl;
    cout << "Min value to be scaled: " << Default.Rad_Val.Min << endl;
    cout << "Max power number to scale by: " << Default.Rad_Val.Powermax << endl;
    cout << "Min power number to scale by: " << Default.Rad_Val.Powermin << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "///////////////Default Eccentricity Settings////////////////////" << endl;
    cout << "Number of decimal places (if less than 2): " << Default.Ecc_Dec << endl;
    cout << "Eccentricity Integer Range (0-0,1, or 2): 0-" << Default.Ecc_Range << endl;
    cout << "////////////////////////////////////////////////////////////////" << endl;
    cout << "///////////////////////Extra Settings///////////////////////////" << endl;
    cout << "Default Axis of Orbital Rotation: " << Default.Axis << endl;
    cout << "Default Body Being Orbited: " << Default.Orbit << endl;  
}

void Generator::Set_Settings(bool Def){//Change the Default Settings//
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
                    Default.Set.Axis = true;
                else
                    Default.Set.Axis = false;
            }
            else{
                if(Answer == "yes")
                    Active.Set.Axis = true;
                else
                    Active.Set.Axis = false;
            }
        }
        else if(Answer == "body"){
            cout << "Would you like to specify the name of the celestial body being orbited? (Yes/No)" << endl;
            Answer = yes_no();
            if(Def){
                if(Answer == "yes")
                    Default.Set.Body = true;
                else
                    Default.Set.Body = false;                 
            }
            else{
                if(Answer == "yes")
                    Active.Set.Body = true;
                else
                    Active.Set.Body = false;
            }
        }
        else if(Answer == "perihelion"){
            cout << "Would you like to specify the argument of perihelion? (Yes/No)" << endl;
            Answer = yes_no();
            if(Def){
                if(Answer == "yes")
                    Default.Set.Perihelion = true;
                else
                    Default.Set.Perihelion = false;                 
            }
            else{
                if(Answer == "yes")
                    Active.Set.Perihelion = true;
                else
                    Active.Set.Perihelion = false;
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
                    Default.Set.IMass = Full;
                if(Answer == "partially")
                    Default.Set.IMass = Partial;
                if(Answer == "default")
                    Default.Set.IMass == Zero;                
            }
            else{
                if(Answer == "fully")
                    Active.Set.IMass = Full;
                if(Answer == "partially")
                    Active.Set.IMass = Partial;
                if(Answer == "default")
                    Active.Set.IMass == Zero;
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
                    Default.Set.IPosition = Full;
                if(Answer == "partially")
                    Default.Set.IPosition = Partial;
                if(Answer == "default")
                    Default.Set.IPosition == Zero;                 
            }
            else{
                if(Answer == "fully")
                    Active.Set.IPosition = Full;
                if(Answer == "partially")
                    Active.Set.IPosition = Partial;
                if(Answer == "default")
                    Active.Set.IPosition == Zero;
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
                    Default.Set.IRadius = Full;
                if(Answer == "partially")
                    Default.Set.IRadius = Partial;
                if(Answer == "default")
                    Default.Set.IRadius == Zero;                 
            }
            else{
                if(Answer == "fully")
                    Active.Set.IRadius = Full;
                if(Answer == "partially")
                    Active.Set.IRadius = Partial;
                if(Answer == "default")
                    Active.Set.IRadius == Zero;
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
                Default.Set.Sampler = false;
                if(Answer == "fully")
                    Default.Set.IEccentric = Full;
                if(Answer == "partially")
                    Default.Set.IEccentric = Partial;
                if(Answer == "default")
                    Default.Set.IEccentric == Zero;                 
            }
            else{
                Active.Set.Sampler = false;
                if(Answer == "fully")
                    Active.Set.IEccentric = Full;
                if(Answer == "partially")
                    Active.Set.IEccentric = Partial;
                if(Answer == "default")
                    Active.Set.IEccentric == Zero;
            }
        }
        else{
            cout << "Would you like to input your own sample points for the object's orbit to pass through (or come as close as possible to passing through)? (Yes/No)" << endl;
            Answer = yes_no();
            if(Def){
                Default.Set.IEccentric = Zero;
                if(Answer == "yes")
                    Default.Set.Sampler = true;
                else
                    Default.Set.Sampler = false;                
            }
            else{
                Active.Set.IEccentric = Zero;
                if(Answer == "yes")
                    Active.Set.Sampler = true;
                else
                    Active.Set.Sampler = false;
            }
        }
    }    
}

void Generator::Set_Body(bool Def){
    cout << "Enter the name of the Celestial Body being orbited" << endl;
    if(Def)
        cin >> Default.Orbit;
    else
        cin >> Active.Orbit;        
}

void Generator::Set_Axis(bool Def){
    string A;
    cout << "Set an axis of rotation for the angle of inclination. (X, Y, Z)" << endl;
    while(true){
        cin >> A;
        A = str_lower(A);
        if(A == "x" || A == "y" || A == "z"){
            A[0] = toupper(A[0]);
            if(Def)
                Default.Axis = A[0];
            else
                Active.Axis = A[0];
            break;
        }
        cout << "Error. Invalid input" << endl;
    }    
}

void Generator::Set_Eccentric(bool Def){
    string Answer;
    cout << "Enter the value range for the eccentricity to fall under. 0 to 0, 0 to 1, or 0 to 2 (Enter 0, 1, or 2)" << endl;                
    while(true){
        cin >> Answer;
        if(Answer == "0" || Answer == "1" || Answer == "2")
            break;
        cout << "Error, please enter 0, 1, or 2." << endl;
    }
    if(Def)
        Default.Ecc_Range = (int)convert(Answer);
    else
        Active.Ecc_Range = (int)convert(Answer);
    cout << "Enter a value specifying the number of decimal places the Eccentricity will have." << endl;
    while(true){
        Answer = verify_pointdouble();
        if(Answer == "done")
            cout << "Error, please enter a number" << endl;
        else{
            double Test = convert(Answer);
            if(Test - (int)Test == 0 && 0 <= convert(Answer)){
                if(Def)
                    Default.Ecc_Dec = Test;
                else
                    Active.Ecc_Dec = Test;
                break;
            }
            cout << "Error, please enter a positive integer value." << endl;
        }
    }    
}

void Generator::Set_Radius(bool Def){
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
            if(Answer == "Valmax" && Default.Rad_Val.Min <= Result)
                Default.Rad_Val.Max = Result;
            else if(Answer == "Valmin" && Result <= Default.Rad_Val.Max)
                Default.Rad_Val.Min = Result;
            else if(Answer == "Powermax" && Default.Rad_Val.Powermin <= Result)
                Default.Rad_Val.Powermax = Result;
            else if(Answer == "Powermin" && Result <= Default.Rad_Val.Powermax)
                Default.Rad_Val.Powermin = Result;
            else if(Answer == "Decimal")
                Default.Rad_Val.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;           
        }
        else{
            if(Answer == "Valmax" && Active.Rad_Val.Min <= Result)
                Active.Rad_Val.Max = Result;
            else if(Answer == "Valmin" && Result <= Active.Rad_Val.Max)
                Active.Rad_Val.Min = Result;
            else if(Answer == "Powermax" && Active.Rad_Val.Powermin <= Result)
                Active.Rad_Val.Powermax = Result;
            else if(Answer == "Powermin" && Result <= Active.Rad_Val.Powermax)
                Active.Rad_Val.Powermin = Result;
            else if(Answer == "Decimal")
                Active.Rad_Val.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
        }
    }    
}

void Generator::Set_Position(bool Def){
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
                    if(Answer == "Maxx" && Default.Pos_Val.Minx <= Result)
                        Default.Pos_Val.Maxx = Result;
                    else if(Answer == "Maxy" && Default.Pos_Val.Miny <= Result)
                        Default.Pos_Val.Maxy = Result;
                    else if(Answer == "Maxz" && Default.Pos_Val.Minz <= Result)    
                        Default.Pos_Val.Maxz = Result;
                    else if(Answer == "Minx" && Result <= Default.Pos_Val.Maxx)
                        Default.Pos_Val.Minx = Result;
                    else if(Answer == "Miny" && Result <= Default.Pos_Val.Maxy)
                        Default.Pos_Val.Miny = Result;
                    else if(Answer == "Minz" && Result <= Default.Pos_Val.Maxz)
                        Default.Pos_Val.Minz = Result;
                    else
                    cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
                }
                else{
                    if(Answer == "Maxx" && Active.Pos_Val.Minx <= Result)
                        Active.Pos_Val.Maxx = Result;
                    else if(Answer == "Maxy" && Active.Pos_Val.Miny <= Result)
                        Active.Pos_Val.Maxy = Result;
                    else if(Answer == "Maxz" && Active.Pos_Val.Minz <= Result)    
                        Active.Pos_Val.Maxz = Result;
                    else if(Answer == "Minx" && Result <= Active.Pos_Val.Maxx)
                        Active.Pos_Val.Minx = Result;
                    else if(Answer == "Miny" && Result <= Active.Pos_Val.Maxy)
                        Active.Pos_Val.Miny = Result;
                    else if(Answer == "Minz" && Result <= Active.Pos_Val.Maxz)
                        Active.Pos_Val.Minz = Result;
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
                    if(Answer == "Maxx" && Default.Pos_Val.Powerxmin <= Result)
                        Default.Pos_Val.Powerxmax = Result;
                    else if(Answer == "Maxy" && Default.Pos_Val.Powerymin <= Result)
                        Default.Pos_Val.Powerymax = Result;
                    else if(Answer == "Maxz" && Default.Pos_Val.Powerzmin <= Result)    
                        Default.Pos_Val.Powerzmax = Result;
                    else if(Answer == "Minx" && Result <= Default.Pos_Val.Powerxmax)
                        Default.Pos_Val.Powerxmin = Result;
                    else if(Answer == "Miny" && Result <= Default.Pos_Val.Powerymax)
                        Default.Pos_Val.Powerymin = Result;
                    else if(Answer == "Minz" && Result <= Default.Pos_Val.Powerzmax)
                        Default.Pos_Val.Powerzmin = Result;
                    else
                    cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;                     
                }
                else{
                    if(Answer == "Maxx" && Active.Pos_Val.Powerxmin <= Result)
                        Active.Pos_Val.Powerxmax = Result;
                    else if(Answer == "Maxy" && Active.Pos_Val.Powerymin <= Result)
                        Active.Pos_Val.Powerymax = Result;
                    else if(Answer == "Maxz" && Active.Pos_Val.Powerzmin <= Result)    
                        Active.Pos_Val.Powerzmax = Result;
                    else if(Answer == "Minx" && Result <= Active.Pos_Val.Powerxmax)
                        Active.Pos_Val.Powerxmin = Result;
                    else if(Answer == "Miny" && Result <= Active.Pos_Val.Powerymax)
                        Active.Pos_Val.Powerymin = Result;
                    else if(Answer == "Minz" && Result <= Active.Pos_Val.Powerzmax)
                        Active.Pos_Val.Powerzmin = Result;
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
                        Default.Pos_Val.Decimalx = Result;
                    if(Answer == "Decimaly")
                        Default.Pos_Val.Decimaly = Result;
                    if(Answer == "Decimalz")    
                        Default.Pos_Val.Decimalz = Result;                     
                }
                else{
                    if(Answer == "Decimalx")
                        Active.Pos_Val.Decimalx = Result;
                    if(Answer == "Decimaly")
                        Active.Pos_Val.Decimaly = Result;
                    if(Answer == "Decimalz")    
                        Active.Pos_Val.Decimalz = Result;
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
                            Default.Pos_Val.X = Yes;
                        else if(Ans == "no")
                            Default.Pos_Val.X = No;
                        else
                            Default.Pos_Val.X = Maybe;
                    }
                    else if(Answer == "signy"){
                        if(Ans == "yes")
                            Default.Pos_Val.Y = Yes;
                        else if(Ans == "no")
                            Default.Pos_Val.Y = No;
                        else
                            Default.Pos_Val.Y = Maybe;
                    }
                    else{
                        if(Ans == "yes")
                            Default.Pos_Val.Z = Yes;
                        else if(Ans == "no")
                            Default.Pos_Val.Z = No;
                        else
                            Default.Pos_Val.Z = Maybe;
                    }                    
                }
                else{
                    if(Answer == "signx"){
                        if(Ans == "yes")
                            Active.Pos_Val.X = Yes;
                        else if(Ans == "no")
                            Active.Pos_Val.X = No;
                        else
                            Active.Pos_Val.X = Maybe;
                    }
                    else if(Answer == "signy"){
                        if(Ans == "yes")
                            Active.Pos_Val.Y = Yes;
                        else if(Ans == "no")
                            Active.Pos_Val.Y = No;
                        else
                            Active.Pos_Val.Y = Maybe;
                    }
                    else{
                        if(Ans == "yes")
                            Active.Pos_Val.Z = Yes;
                        else if(Ans == "no")
                            Active.Pos_Val.Z = No;
                        else
                            Active.Pos_Val.Z = Maybe;
                    }
                }
            }                        
        }                        
    }    
}

void Generator::Set_Mass(bool Def){
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
            if(Answer == "Valmax" && Default.Mass_Val.Min <= Result)
                Default.Mass_Val.Max = Result;
            else if(Answer == "Valmin" && Result <= Default.Mass_Val.Max)
                Default.Mass_Val.Min = Result;
            else if(Answer == "Powermax" && Default.Mass_Val.Powermin <= Result)
                Default.Mass_Val.Powermax = Result;
            else if(Answer == "Powermin" && Result <= Default.Mass_Val.Powermax)
                Default.Mass_Val.Powermin = Result;
            else if(Answer == "Decimal")
                Default.Mass_Val.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
        }
        else{
            if(Answer == "Valmax" && Active.Mass_Val.Min <= Result)
                Active.Mass_Val.Max = Result;
            else if(Answer == "Valmin" && Result <= Active.Mass_Val.Max)
                Active.Mass_Val.Min = Result;
            else if(Answer == "Powermax" && Active.Mass_Val.Powermin <= Result)
                Active.Mass_Val.Powermax = Result;
            else if(Answer == "Powermin" && Result <= Active.Mass_Val.Powermax)
                Active.Mass_Val.Powermin = Result;
            else if(Answer == "Decimal")
                Active.Mass_Val.Decimal = Result;
            else
                cout << "Error, mins and maxes don't make sense (eg. max might be less than min)" << endl;
        }
    }    
}