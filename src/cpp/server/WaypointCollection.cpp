#include "WaypointCollection.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <iomanip>
#include <math.h>

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <fstream>

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
using namespace std;

WaypointCollection::WaypointCollection(){
   resetFromJsonFile("waypoints.json");
}

WaypointCollection::~WaypointCollection() {
   //cout << "WaypointCollection destructor.\n";
   wpts.clear();
}

bool WaypointCollection::resetFromJsonFile(string jsonFileName){
   bool r = false;
   ifstream jsonFile(jsonFileName.c_str());
   string line;
   cout << "The content of the Json file as a string: " << endl;
   if(jsonFile.is_open()){
      while(getline(jsonFile,line)){
         cout << line << endl;
      }
   }else{
      cout << "Json file not opened properly" << endl;
   }
   jsonFile.close();
   Json::Reader reader;
   Json::Value root;
   std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
   bool parseSuccess = reader.parse(json,root,false);
   if(parseSuccess){
      //cout << "successful parse" << endl;
      Json::Value::Members mbr = root.getMemberNames();
      for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
         //cout << *i << " " << endl;
         Json::Value jsonWpt = root[*i];
         string nameStr = "name";
         if(nameStr.compare(*i)==0){
            name = jsonWpt.asString();
            //cout << "found name " << *i << " value: " << name << endl;
         }else{
            //cout << "found " << *i << endl;
            string name = jsonWpt["name"].asString();
            string addr = jsonWpt["address"].asString();
            double lon = jsonWpt["lon"].asDouble();
            double lat = jsonWpt["lat"].asDouble();
	    double ele = jsonWpt["ele"].asDouble();
            //cout << " Waypoint: " << name << "lat: " << lat << "lon: " << lon << endl;
            Waypoint * wpt = new Waypoint(lat,lon,ele,name,addr);
            //waypoint->print();
            wpts[*i] = *wpt;
         }
      }
      r = true;
   }
   return r;
}

bool WaypointCollection::saveToJsonFile(string jsonFileName){
   bool r = false;
   Json::Value jsonLib;
   jsonLib["name"] = name;
   for(std::map<string,Waypoint>::iterator i = wpts.begin(); i!= wpts.end(); i++){
      string key = i->first;
      //cout << key << " " << endl;
      Json::Value jsonWpt;
      Waypoint wpt = wpts[key];
      jsonWpt["name"] = wpt.name;
      jsonWpt["address"] = wpt.addr;
      jsonWpt["lon"] = wpt.lon;
      jsonWpt["lat"] = wpt.lat;
      jsonWpt["ele"] = wpt.ele;
      jsonLib[key] = jsonWpt;
   }
   r = true;
   Json::StyledStreamWriter ssw("  ");
   std::ofstream jsonOutFile(jsonFileName.c_str(), std::ofstream::binary);
   ssw.write(jsonOutFile, jsonLib);
   return r;
}

bool WaypointCollection::add(Waypoint wpt){
   bool r = false;
   wpts[wpt.name]=wpt;
   r = true;
   return r;
}

bool WaypointCollection::remove(Waypoint wpt){
   bool r = false;
   wpts.erase(wpt.name);
   r = true;
   return r;
}

Waypoint WaypointCollection::get(string wptName){
   Waypoint ret = wpts[wptName];
   return ret;
}

bool WaypointCollection::mod(string wptName, double aLat, double aLon, double aEle, string aName, string aAddr){
   bool r = false;
   Waypoint modded = wpts[wptName];

   modded.lat = aLat;
   modded.lon = aLon;
   modded.ele = aEle;
   modded.name = aName;
   modded.addr = aAddr;

   wpts[wptName] = modded;
   return true;
}


double WaypointCollection::degreeToRadian (const double degree) { 
   return (degree * PI / 180); 
};

double WaypointCollection::radianToDegree (const double radian) {
   return (radian * 180 / PI); 
};
 

double WaypointCollection::bearing(string name1, string name2) {
   Waypoint fromWpt = wpts[name1];
   Waypoint toWpt = wpts[name2];
   double latitude1 = fromWpt.lat;
   double longitude1 = fromWpt.lon;
   double latitude2 = toWpt.lat;
   double longitude2 = toWpt.lon;

   const double longitudeDifference = degreeToRadian(longitude2 - longitude1);
   latitude1 = degreeToRadian(latitude1);
   latitude2 = degreeToRadian(latitude2);
 
   const double x = (cos(latitude1) * sin(latitude2)) -
                  (sin(latitude1) * cos(latitude2) * cos(longitudeDifference));
   const double y = sin(longitudeDifference) * cos(latitude2);
 
   const double degree = radianToDegree(atan2(y, x));
   return (degree >= 0)? degree : (degree + 360);

}

double WaypointCollection::distance(string name1, string name2) {
   Waypoint fromWpt = wpts[name1];
   Waypoint toWpt = wpts[name2];
   double latitude1 = fromWpt.lat;
   double longitude1 = fromWpt.lon;
   double latitude2 = toWpt.lat;
   double longitude2 = toWpt.lon;

   latitude1 = degreeToRadian(latitude1);
   longitude1 = degreeToRadian(longitude1);
   latitude2 = degreeToRadian(latitude2);
   longitude2 = degreeToRadian(longitude2);
 
   double x = sin((latitude2 - latitude1) / 2);
   double y = sin((longitude2 - longitude1) / 2);
   #if 1
   return earthDiameterMeters * asin(sqrt((x * x) + (cos(latitude1) * cos(latitude2) * y * y)));
   #else
   double value = (x * x) + (cos(latitude1) * cos(latitude2) * y * y);
   return earthDiameterMeters * atan2(sqrt(value), sqrt(1 - value));
   #endif
}

std::vector<string> WaypointCollection::getNames(){
   vector<string> myVec;
   for(map<string,Waypoint>::iterator it = wpts.begin();it!=wpts.end();++it){
      myVec.push_back(it->first);
      cout << it->first << "\n";
   }
   return myVec;
}

void WaypointCollection::printWptCol(){
   cout << "Waypoint Collection: " << name << " has waypoints" << endl;
   for(map<string,Waypoint>::iterator it = wpts.begin();it!=wpts.end();++it){
      cout << "key " << it->first << " is ";
      (it->second).print();
   }
   cout << endl;
}

