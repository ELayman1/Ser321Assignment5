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
 * Purpose: C++ class which serves as server for a collection of students. This
 * class is part of a student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>

#include "waypointserverstub.h"
#include "WaypointCollection.hpp"

using namespace jsonrpc;
using namespace std;

class WaypointServer : public waypointserverstub {
public:
   WaypointServer(AbstractServerConnector &connector, int port);
   virtual bool saveToJsonFile();
   virtual bool resetFromJsonFile();
   virtual bool add(const Json::Value& wptName);
   virtual bool remove(const string&  wptName);
   virtual bool mod(double aLat, double aLon, double aEle, const std::string& aName, const std::string& aAddr);
   //virtual Json::Value get(const std::string& wptName);
   virtual double bearing(const std::string& name1, const std::string& name2);
   virtual double distance(const std::string& name1, const std::string& name2);
   
private:
   WaypointCollection * library;
   int portNum;
};

WaypointServer::WaypointServer(AbstractServerConnector &connector, int port) :
                             waypointserverstub(connector){
   library = new WaypointCollection();
   library->resetFromJsonFile("waypoints.json");
   portNum = port;
}

bool WaypointServer::saveToJsonFile() {
   cout << "Saving collection to waypoints.json " << endl;
   bool ret = library->saveToJsonFile("waypoints.json");
   return ret;
}

bool WaypointServer::resetFromJsonFile() {
   cout << "Restoring collection from waypoints.json " << endl;
   bool ret = library->resetFromJsonFile("waypoints.json");
   return ret;
}

bool WaypointServer::add(const Json::Value& wptName) {
   cout << "Adding " << wptName << endl;
   bool ret = library->add(wptName);
   return ret;
}

bool WaypointServer::remove(const string& wptName) {
   cout << "Removing " << wptName << endl;
   bool ret = library->remove(wptName);
   return ret;
}

bool WaypointServer::mod(double aLat, double aLon, double aEle, const string& aName, const string& aAddr) {
   cout << "Modding " << aName << endl;
   bool ret = library->mod(aLat, aLon, aEle, aName, aAddr);
   return ret;
}

double WaypointServer::bearing(const string& name1, const string& name2) {
   cout << "Bearing " << name1 << ", " << name2  << endl;
   double ret = library->bearing(name1,name2);
   return ret;
}

double WaypointServer::distance(const string& name1, const string& name2) {
   cout << "Distance " << name1 << ", " << name2 << endl;
   double ret = library->distance(name1,name2);
   return ret;
}

void exiting(){
   std::cout << "Server has been terminated. Exiting normally" << endl;
}

int main(int argc, char * argv[]) {
   // invoke with ./bin/waypointRPCServer 8080
   int port = 8080;
   if(argc > 1){
      port = atoi(argv[1]);
   }
   HttpServer httpserver(port);
   WaypointServer ws(httpserver, port);
   std::atexit(exiting);
   auto ex = [] (int i) {cout << "server terminating with signal " << i << endl;
                         // ws.StopListening();
                         exit(0);
                         //return 0;
                        };
   // ^C
   std::signal(SIGINT, ex);
   // abort()
   std::signal(SIGABRT, ex);
   // sent by kill command
   std::signal(SIGTERM, ex);
   // ^Z
   std::signal(SIGTSTP, ex);
   cout << "Waypoint collection server listening on port " << port
      //<< " press return/enter to quit." << endl;
        << " use ps to get pid. To quit: kill -9 pid " << endl;
   ws.StartListening();
   while(true){
   }
   //int c = getchar();
   ws.StopListening();
   return 0;
}
