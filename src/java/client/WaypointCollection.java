package waypoint.client;

import org.json.JSONString;
import org.json.JSONObject;
import org.json.JSONTokener;
import java.io.FileInputStream;
import java.util.Vector;
import java.util.Enumeration;
import java.io.Serializable;

/**
 * Copyright (c) 2015 Tim Lindquist,
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
 * Purpose:
 * A class implementing the operations for library management.
 * <p/>
 *
 * Ser321 Principles of Distributed Software Systems
 * @see <a href="http://pooh.poly.asu.edu/Ser321">Ser321 Home Page</a>
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering
 *                       Ira Fulton Schools of Engineering, ASU Polytechnic
 * @file    WaypointCollection.java
 * @date    Janurary, 2018
 * @license See above
 * @see ser321.Waypoint
 */
public class WaypointCollection extends Object implements Serializable {

   // Serial version UID is defined below. Its only needed if you want
   // to make changes to the class and still deserialize artifacts
   // generated from prior versions. Obtain this definition with:
   // serialver -classpath classes:lib/json.jar ser321.WaypointCollection
   private static final long serialVersionUID = 6982142948226029575L;

   private String name; // the name of the points
   private Vector<Waypoint> WptLib = new Vector<Waypoint>(); // the waypoints of this library

   public WaypointCollection() {
      this.name = "unknown";
   }

   public WaypointCollection(String fileName){
      try{
         FileInputStream in = new FileInputStream(fileName);
         JSONObject obj = new JSONObject(new JSONTokener(in));
         String [] names = JSONObject.getNames(obj);
         System.out.print("Waypoints are: ");
         for(int j=0; j< names.length; j++){
            System.out.print(names[j]+", ");
         }
         System.out.println("");
         name = obj.getString("name");
         WptLib = new Vector<Waypoint>();
         for (int i=0; i< names.length; i++){
            if(!names[i].equals("name")){
               Waypoint aWpt= new Waypoint((JSONObject)obj.getJSONObject(names[i]));
               WptLib.add(aWpt);
            }
         }
         
         in.close();
      }catch (Exception ex) {
         System.out.println("Exception importing from json: "+ex.getMessage());
      }
   }

   public String toJSONString(){
      String temp;
      JSONObject obj = new JSONObject();
      obj.put("name",name);
      for (Enumeration<Waypoint> e = WptLib.elements(); e.hasMoreElements();){
         Waypoint wpt = (Waypoint)e.nextElement();
         obj.put(wpt.getWptName(),wpt.toJSONObject());
      }
      temp = obj.toString();
      //System.out.println("group tojsonstring returning string: "+temp);
      return temp;
   }

   public String getName(){
      return name;
   }

   public void setName(String aName){
      name = aName;
   }

  /**
   * Associate a new waypoint with this Library.
   * Use addWaypointToLibrary to allow a user to access the Library.
   * @param name Is a String specifying waypoint.name to add
   * @param addr Is a String specifying waypoint.address for waypoint.
   * @param ele Is a String specifying waypoint.elevation for waypoint.
   * @param lat Is a String specifying waypoint.latitude for waypoint.
   * @param lon Is a String specifying waypoint.longitude for waypoint.
   */

   public void add(String name, String addr, double ele, double lat, double lon) {
      boolean found = false;
      for (int i = 0; i < WptLib.size(); i++) {
         if ((WptLib.elementAt(i)).getWptName().equals(name))
            found = true;
      }
      if (!found)
         WptLib.addElement(new Waypoint(name, addr, ele, lat, lon));
   }

  /**
   * Get the name strings of all waypoints associated in this Library.
   * Use getName to retrieve the name string property for the Library.
   * @return The vector of waypoint name strings.
   */
   public Vector<String> getNames() {
      Vector<String> temp = new Vector<String>();
      for (Enumeration e = WptLib.elements() ; e.hasMoreElements() ;) {
         temp.addElement(((Waypoint)e.nextElement()).getWptName());
      }
      return temp;
   }

  /**
   * Determine whether a waypoint is in the library.
   * Use exists to check a waypoint's attributes for this library
   * @param name Is a String specifying waypoint.name to add
   * @param addr Is a String specifying waypoint.address for waypoint.
   * @param ele Is a String specifying waypoint.elevation for waypoint.
   * @param lat Is a String specifying waypoint.latitude for waypoint.
   * @param lon Is a String specifying waypoint.longitude for waypoint.
   * @return true if all attributes are correct;
   * otherwise return false.
   */
   public boolean exists(String name, String addr, double ele, double lat, double lon) {
      boolean match = false;
      for (Enumeration e = WptLib.elements() ; e.hasMoreElements() ;) {
         if(((Waypoint)e.nextElement()).check(name, addr, ele, lat, lon)){
            match = true;
            break;
         }
      }
      return match;
   }

  /**
   * Print the group to standard out
   */
   public void printCollection(){
      System.out.print("Collection: "+ name +" has waypoints: ");
      for (Enumeration e = WptLib.elements() ; e.hasMoreElements() ;) {
         System.out.print(((Waypoint)e.nextElement()).getWptName()+", ");
      }
      System.out.println();
   }

}
