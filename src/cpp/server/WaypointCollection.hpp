/**
 * Copyright 2016 Tim Lindquist,
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
 * <p/>
 * Purpose: C++ class to represent a collection of students. This class is 
 * part of a student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */

#include "Waypoint.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;

class WaypointCollection {

public:
   WaypointCollection();
   ~WaypointCollection();
   std::map<std::string, Waypoint> wpts;
   string name;
   const double PI = 3.14;
   const double earthDiameterMeters = 12742000;

   bool resetFromJsonFile(string jsonFileName);
   bool saveToJsonFile(string jsonFileName);
   bool add(Waypoint wpt);
   bool remove(Waypoint wpt);
   Waypoint get(string wptName); //returns a waypoint's json
   bool mod(string wptName, double aLat, double aLon, double aEle, string aName, string aAddr);
   std::vector<string> getNames(); //returns a json array of strings
   double degreeToRadian(const double degree);
   double radianToDegree(const double radian);
   double bearing(string name1, string name2);
   double distance(string name1, string name2);
   void printWptCol();
};
