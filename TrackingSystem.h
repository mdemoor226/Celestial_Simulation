/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrackingSystem.h
 * Author: Michael DeMoor
 *
 * Created on March 28, 2017, 6:35 PM
 */

#ifndef TRACKINGSYSTEM_H
#define TRACKINGSYSTEM_H
#include <memory>
#include "Celestial_Vec.h"

class Celestial_Body;
typedef std::shared_ptr<Celestial_Body> CelestialPtr;

class Celestial_Body{
    friend std::string Get_Status(CelestialPtr);    
    friend int get_position(CelestialPtr, const std::string);
    friend void Count_Decr(CelestialPtr, int);  
    public:
        Celestial_Body(const std::string, const float, const Attributes, const float);
        virtual ~Celestial_Body(){}
        virtual void Set_Mass(float) = 0;
        virtual void Remove() = 0;
        //virtual Draw// = 0;
        //void Move(To be added later in OpenGL)//If constraints allow
              
        void Display_Objects();
        void display_Object();        
        void Simulate_Motion(double,double,double,double,double);
        //Some Sets and Gets//
        Attributes get_attributes();                
        void set_attributes(Attributes);    
        Attributes get_ObjectTracker(int);
        void set_ObjectTracker(int, Attributes);
        std::string get_Name();
        void set_Name(const std::string);  
        void set_Radius(float);
        float get_Radius();
        float get_Mass();         
        
    protected:                   
        //Data Members//
        std::string Name;
        Attributes Values;
        std::string Status;//Change to bool//
        float Mass;
        float Radius;
        std::string Color;
        float RScale;        
        //Static Members//
        static std::vector<Attributes> ObjectTracker;
        static std::vector<float> Lightvector;        
        static int SpaceCount;
        static int StarCount;
        static float SpScale;        
        
    private:
        float Size_Constant;
        static float RSmax;
        static int SizeScale;
        static int SpaceScale;
        bool Max;
        void Set_Scalers();
        void Set_Scale();
        void Inc_SpaceScale();
        void Dec_SpaceScale();
        void Inc_SizeScale();
        void Dec_SizeScale();        
        void update_attributes();
        void print_attributes(Attributes, std::string, float, float);              
        //Simulation Support Functions//
        void Initialize_Sim(
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, 
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&,
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&,
        std::vector<Attributes>&, std::vector<Attributes>&
        );
        std::vector<Attributes>& RK_Functions(std::vector<Attributes>&,std::vector<Attributes>&);
        Position Acceleration(std::vector<Attributes>&, int);
        Position Calculations(Position&, Position&, const float);        
        double Distance_Calc(const double,const double,const double);
        double Get_R(const std::vector<Attributes>&, const double);
        void is_collision(const double);
        Attributes Momentum(Attributes, Attributes);        
};

class Star: public Celestial_Body{
    public:
        Star(const std::string, float, Attributes, float);
        virtual ~Star(){}
        virtual void Set_Mass(float);
        virtual void Remove();
        //virtual Draw//
                
    private:
        float Brightness_Calc(float&);
        float Brightness;//Watts(I think?)//In reality this would probably be measured in Lumens, but I'm not sure yet how OpenGL measures "Brightness"//
        int Bright_ID;
        static bool Modified;        
};

class Non_Star: public Celestial_Body{
    public:
        Non_Star(const std::string, float, Attributes, float);
        virtual ~Non_Star(){}
        virtual void Set_Mass(float);
        virtual void Remove();
        //Virtual Draw// = 0;
};

class Black_Hole: public Non_Star{//Celestial_Body{
    public:
        Black_Hole(const std::string, float, Attributes, float);
        virtual ~Black_Hole(){}
        //Virtual Draw//Light does not reflect off of or illuminate Black Hole//            
};


class Planet: public Non_Star{//Celestial_Body{
    public:
        Planet(std::string, float, Attributes, float);
        virtual ~Planet(){}
        //Virtual Draw//Light Reflects off of and illuminates Planet//
    
    private://Color(s) of Planets will be determined randomly//For now at least
        std::vector<const char*> Colors;
};

class Moon: public Non_Star{//Celestial_Body{
    public:
        Moon(std::string, float, Attributes, float);
        virtual ~Moon(){}
        //Virtual Draw//Light affects Moon//
};

class Other: public Non_Star{//Celestial_Body{
    public:
        Other(std::string, float, Attributes, float);
        virtual ~Other(){}
        //Virtual Draw//Light affects Other//
};

int Rand_Orbit_Gen(Attributes*, float&, float&, std::vector<CelestialPtr>&,const std::string);

#endif /* TRACKINGSYSTEM_H */


        /*Future Data Members(Might opt for an an inheritance hierarchy)
         * Color
         * Celestial Type (Star, Gas Giant, Rocky Planet, Comet, Black Hole etc)
         * Rings
         * Tilt
         * Scale
         * Brightness
         * Any other Graphical/Non-Graphical property
         * //Store the Object Tracker ID's of all Stars//
         */
        //StarID to count the number of Stars in the program
        //Or Data Structure to keep track of star locations/Brightness levels
        //std::string get_status();
        //void set_status(const std::string);