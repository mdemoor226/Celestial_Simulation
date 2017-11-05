/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Celestial_Vec.h
 * Author: Michael DeMoor
 *
 * Created on September 2, 2017, 3:10 AM
 */

#ifndef CELESTIAL_VEC_H
#define CELESTIAL_VEC_H
#include <memory>
#include "Attributes.h"

class Celestial_Body;
typedef std::shared_ptr<Celestial_Body> CelestialPtr;

class Celestial{
    friend void Init_Sim(Celestial*);
    std::vector<CelestialPtr> Celestial_Bodies;
    std::vector<Attributes> ObjectTracker;
    std::vector<float> Lightvector;        
    int SpaceCount;
    int StarCount;    
    public:
        Celestial(const std::string);
        void Simulate();
        void Add_Object(Celestial*);
        void Remove_Object();
        void Alter_Object();        
        void View_Object() const;
        void View_Objects() const;
        std::string get_name() const;        

    private:
        int get_position(const std::string);
        void Count_Decr(int);
        std::string Sim_Name;  
};

//Settings Class//

#endif /* CELESTIAL_VEC_H */

