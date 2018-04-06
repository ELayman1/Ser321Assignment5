#include "Waypoint.hpp"

#include <iostream>
#include <stdlib.h>
#include <cmath>

/**
 * Copyright 2018 Tim Lindquist,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Purpose: demonstrate classes, alloc, init, in the context of
 * distance and direction calculations from one earth position (waypoint)
 * to another.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version January 2018
 */
Waypoint::Waypoint(){
   lat = lon = ele = 0;
   name = "";
   addr = "";
}

Waypoint::Waypoint(double aLat, double aLon, double anElevation, string aName, string aAddr) {
   lat = aLat;
   lon = aLon;
   ele = anElevation;
   name = aName;
   addr = aAddr;
}

Waypoint::~Waypoint() {
   //cout << "Waypoint destructor.\n";
   lat=lon=ele=0; name=""; addr="";
}

void Waypoint::setValues(double aLat, double aLon, double anElevation,
                         string aName, string aAddr) {
   lat = aLat;
   lon = aLon;
   ele = anElevation;
   name = aName;
   addr = aAddr;
}

double Waypoint::distanceGCTo(Waypoint wp, int scale){
   double ret = 0.0;
   // ret is in kilometers. switch to either Statute or Nautical?
   switch(scale) {
   case STATUTE:
      ret = ret * 0.62137119;
      break;
   case NAUTICAL:
      ret = ret * 0.5399568;
      break;
   }
   return ret;
}

double Waypoint::bearingGCInitTo(Waypoint wp, int scale){
   double ret = 0.0;
   return ret;
}

void Waypoint::print(){
   cout << "Waypoint " << name << " lat "
        << lat << " lon " << lon << "\n";
}
