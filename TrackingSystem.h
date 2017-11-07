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
#include "Celestial_Vec.h"

class Celestial_Body{friend class Celestial;
    friend std::string Get_Status(CelestialPtr); 
    public:
        Celestial_Body(const std::string, const float, const Attributes, const float, Celestial*);
        virtual ~Celestial_Body(){}
        virtual void Set_Mass(float) = 0;
        virtual void Remove() = 0;
        //virtual Draw// = 0;
        //void Move(To be added later in OpenGL)//If constraints allow
              
        void Display_Objects();
        void display_Object();        
        void Simulate_Motion(double,double,double,double,double);
        //Some Gets//
        Attributes get_attributes();                       
        std::string get_Name();
        float get_Radius();
        float get_Mass();
        
    protected:                   
        //Data Members//
        Celestial *Celestial_Vec;        
        std::string Name;
        Attributes Values;
        std::string Status;//Change to bool//
        float Mass;
        float Radius;
        std::string Color;
        float RScale;        
        static float SpScale;        
        
    private:
        bool Max;//What was this for again?
        float Size_Constant;
        static float RSmax;
        static int SizeScale;
        static int SpaceScale;
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
        std::vector<Attributes>&, std::vector<Attributes>&, std::vector<Attributes>&
        );
        std::vector<Attributes>& RK_Functions(std::vector<Attributes>&,std::vector<Attributes>&);
        Position Acceleration(std::vector<Attributes>&, int);
        Position Calculations(Position&, Position&, const float);        
        double Distance_Calc(const double,const double,const double);
        double Get_R(const std::vector<Attributes>&, const double);
        void is_collision(const double, std::vector<Attributes>&);
        Attributes Momentum(Attributes, Attributes);        
};

class Star: public Celestial_Body{
    public:
        Star(const std::string, float, Attributes, float, Celestial*);
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
        Non_Star(const std::string, float, Attributes, float, Celestial*);
        virtual ~Non_Star(){}
        virtual void Set_Mass(float);
        virtual void Remove();
        //Virtual Draw// = 0;
};

class Black_Hole: public Non_Star{
    public:
        Black_Hole(const std::string, float, Attributes, float, Celestial*);
        virtual ~Black_Hole(){}
        //Virtual Draw//Light does not reflect off of or illuminate Black Hole//            
};


class Planet: public Non_Star{
    public:
        Planet(std::string, float, Attributes, float, Celestial*);
        virtual ~Planet(){}
        //Virtual Draw//Light Reflects off of and illuminates Planet//
    
    private://Color(s) of Planets will be determined randomly//For now at least
        std::vector<const char*> Colors;
};

class Moon: public Non_Star{
    public:
        Moon(std::string, float, Attributes, float, Celestial*);
        virtual ~Moon(){}
        //Virtual Draw//Light affects Moon//
};

class Other: public Non_Star{
    public:
        Other(std::string, float, Attributes, float, Celestial*);
        virtual ~Other(){}
        //Virtual Draw//Light affects Other//
};

#endif /* TRACKINGSYSTEM_H */
