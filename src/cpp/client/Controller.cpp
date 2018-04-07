#include "WaypointGUI.cpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cmath>

#include <sstream>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <jsoncpp/json/json.h>
#include "waypointcollectionstub.h"

#include "../server/WaypointCollection.hpp"

using namespace jsonrpc;
using namespace std;

/**
 * Copyright (c) 2018 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Waypoint management.
 * This class extends the Gui component class WaypointGUI and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2018
 **/
static WaypointCollection aLib;

class Controller : public WaypointGUI {

   /** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */

   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      exit(1);
   }

   static void ClickedRemoveWP(Fl_Widget * w, void * userdata) {
      Controller* anInstance = (Controller*)userdata;
      Fl_Input_Choice * theWPChoice = anInstance->frWps;
      Fl_Input_Choice * theWPChoice2 = anInstance->toWps;
      std::string selected(theWPChoice->value());
      std::cout << "You clicked the remove waypoint button with "
                << selected
                << std::endl;
      for (int i=0; i < theWPChoice->menubutton()->size(); i++ ) {
         const Fl_Menu_Item &item = theWPChoice->menubutton()->menu()[i];
         if(!selected.compare(item.label())){  // if they are equal
            theWPChoice->menubutton()->remove(i);
	    theWPChoice2->menubutton()->remove(i);
            cout << "Removed " << selected << std::endl;
	    string wptName = theWPChoice->value();
            aLib.remove(wptName);
            break;
         }
	 
      }
      if(theWPChoice->menubutton()->size()>0){
         theWPChoice->value(theWPChoice->menubutton()->menu()[0].label());
      }else{
         theWPChoice->value("");
      }
  }

   static void ClickedAddWP(Fl_Widget * w, void * userdata) {
      Controller* anInstance = (Controller*)userdata;
      Fl_Input_Choice * fromWPChoice = anInstance->frWps;
      Fl_Input_Choice * toWPChoice = anInstance->toWps;

      Fl_Input * theLat = anInstance->latIn;
      Fl_Input * theLon = anInstance->lonIn;
      Fl_Input * theEle = anInstance->eleIn;
      Fl_Input * theName = anInstance->nameIn;
      Fl_Input * theAddr = anInstance->addrIn;
      std::string lat(theLat->value());
      std::string lon(theLon->value());
      std::string ele(theEle->value());

      // what follows is not expedient, but shows how to convert to/from
      // double and formatted C and C++ strings.
      double latNum = atof(lat.c_str());  //convert from string to double
      double lonNum = atof(lon.c_str());  //convert from string to double
      double eleNum = atof(ele.c_str());  //convert from string to double
      char latFormat[10];
      char lonFormat[10];
      char eleFormat[10];

      sprintf(latFormat,"%4.4f",latNum);  //format the double into a C string
      sprintf(lonFormat,"%4.4f",lonNum);  //format the double into a C string
      sprintf(eleFormat,"%4.4f",eleNum);  //format the double into a C string

      std::string latCppStr(latFormat);   //convert formatted C str to C++ str

      std::string name(theName->value());
      std::string addr(theAddr->value());
      std::cout << "You clicked the add waypoint button lat: "<< latCppStr
                << " lon: " << lon << " ele: " << ele << " name: "
                << name << "addr: " << addr << std::endl;

      Waypoint temp;
      temp.name = name;
      temp.addr = addr;
      temp.lon = lonNum;
      temp.lat = latNum;
      temp.ele = eleNum;

      aLib.mod(latNum,lonNum,eleNum,name,addr);

      fromWPChoice->add(name.c_str());
      toWPChoice->add(name.c_str());
      fromWPChoice->value(name.c_str());
      toWPChoice->value(name.c_str());
   }

   static void ClickedModWP(Fl_Widget * w, void * userdata) {
      Controller* anInstance = (Controller*)userdata;
      Fl_Input_Choice * fromWPChoice = anInstance->frWps;
      

      Fl_Input * theLat = anInstance->latIn;
      Fl_Input * theLon = anInstance->lonIn;
      Fl_Input * theEle = anInstance->eleIn;
      Fl_Input * theAddr = anInstance->addrIn;
      std::string lat(theLat->value());
      std::string lon(theLon->value());
      std::string ele(theEle->value());

      // what follows is not expedient, but shows how to convert to/from
      // double and formatted C and C++ strings.
      double latNum = atof(lat.c_str());  //convert from string to double
      double lonNum = atof(lon.c_str());  //convert from string to double
      double eleNum = atof(ele.c_str());  //convert from string to double

      char latFormat[10];
      char lonFormat[10];
      char eleFormat[10];
      sprintf(latFormat,"%4.4f",latNum);  //format the double into a C string
      sprintf(lonFormat,"%4.4f",lonNum);  //format the double into a C string
      sprintf(eleFormat,"%4.4f",eleNum);  //format the double into a C string

      std::string latCppStr(latFormat);   //convert formatted C str to C++ str

      std::string name(fromWPChoice->value());
      std::string addr(theAddr->value());

      std::string selected(fromWPChoice->value());
      std::cout << "You clicked the modify waypoint button with "
                << selected
                << std::endl;

      string wptName = fromWPChoice->value();

      aLib.mod(latNum,lonNum,eleNum,wptName,addr);
   }

   static void ClickedDistanceAndBearing(Fl_Widget * w, void * userdata) {
      Controller* anInstance = (Controller*)userdata;
      Fl_Input_Choice * fromWPChoice = anInstance->frWps;
      Fl_Input_Choice * toWPChoice = anInstance->toWps;
      Fl_Input * theDB = anInstance->distBearIn;

      std::string selected(fromWPChoice->value());
      std::string selected2(toWPChoice->value());
      std::cout << "You clicked the distance and bearing waypoint button from "
                << selected 
                << " to "
                << selected2
                << std::endl;

      string wptName1 = fromWPChoice->value();
      string wptName2 = toWPChoice->value();

      double distance = aLib.distance(wptName1, wptName2);
      double bearing = aLib.bearing(wptName1, wptName2);

      char distFormat[10];
      char bearFormat[10];
      sprintf(distFormat,"%4.4f",distance);  //format the double into a C string
      sprintf(bearFormat,"%4.4f",bearing);  //format the double into a C string
      std::string distCppStr(distFormat);   //convert formatted C str to C++ str
      std::string bearCppStr(bearFormat);   //convert formatted C str to C++ str
      std::string distBear = "D: " + distCppStr + ", B:" + bearCppStr; 
      cout << distBear << endl;

      theDB->value(distBear.c_str());

   }

   static void ClickedImportJson(Fl_Widget * w, void * userdata) {
      Controller* anInstance = (Controller*)userdata;
      Fl_Input_Choice * fromWPChoice = anInstance->frWps;
      Fl_Input_Choice * toWPChoice = anInstance->toWps;

      for (int i=0; i < fromWPChoice->menubutton()->size(); i++ ) {
 	 string str1 = "from waypoint";
	 string str2 = "to waypoint";
         const Fl_Menu_Item &item = fromWPChoice->menubutton()->menu()[i];
         const Fl_Menu_Item &item2 = toWPChoice->menubutton()->menu()[i];
         if(!str1.compare(item.label())) {  // if they are equal
            fromWPChoice->menubutton()->remove(i);
	    if(!str2.compare(item2.label())) {
	       toWPChoice->menubutton()->remove(i);
	    }
            break;
	 }
	 
      }
  
      aLib.resetFromJsonFile("waypoints.json");
      std::cout << endl << "Done initializing from waypoints.json" << endl; 

      for(map<string,Waypoint>::iterator it = aLib.library.begin();it!=aLib.library.end();++it){
	  //cout << "key " << it->first << endl;
	  string wptName = it->first;
          fromWPChoice->add(wptName.c_str());
          toWPChoice->add(wptName.c_str());
          cout << "Waypoint Name: " << wptName << " has been added." << std::endl;
      }
      
   }

   static void ClickedExportJson(Fl_Widget * w, void * userdata) {
      aLib.saveToJsonFile("waypoints.json");
      std::cout << "Done writing a json file." << endl;
   }

   static void SelectedFromWP(Fl_Widget * w, void * userdata) {
      Controller* anInstance = (Controller*)userdata;
      Fl_Input_Choice * frWps = anInstance->frWps;
      std::string selected(frWps->value());
      std::cout << "You changed the selection in the Fl_Input_Choice from to the value "
                << selected
                << std::endl;
   }

public:
   Controller(const char * name = 0) : WaypointGUI(name) {
      removeWPButt->callback(ClickedRemoveWP, (void*)this);
      addWPButt->callback(ClickedAddWP, (void*)this);
      modWPButt->callback(ClickedModWP, (void*)this);
      distBearButt->callback(ClickedDistanceAndBearing, (void*)this);
      importButt->callback(ClickedImportJson, (void*)this);
      exportButt->callback(ClickedExportJson, (void*)this);
      frWps->callback(SelectedFromWP, (void*)this);
      callback(ClickedX);
   }
};


int main(int argc, char*argv[]) {
   string host = "http://127.0.0.1:8080";
   if(argc>1){
      host = string(argv[1]);
   }
   HttpClient httpclient(host);
   waypointcollectionstub sc(httpclient);
   cout << "Connecting to host " << host << endl;
   try {
      string inLine;
      while(true){
      	Controller cm("Elizabeth's Waypoint Browser");
   	return (Fl::run());
      }
   } catch (JsonRpcException e) {
      cerr << e.what() << endl;
   }
}
