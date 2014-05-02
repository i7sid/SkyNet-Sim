//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package skynet;

import org.mixim.modules.nic.NicCSMA;
//
// TODO auto-generated type
//
module Glider
{
    parameters:
          //type of the presentation layer
        string applicationType = default("GliderAppLayer");  //type of the application layer
        string mobilityType = default("GliderMobility"); //type of the mobility module
        string nicType;
        @display("bgb=210,450,white;i=device/palm;b=40,40,rect");
        @node();

    gates:
        input radioIn; // gate for sendDirect
    submodules:
        mobility: GliderMobility {
            parameters:
                @display("p=140,30;i=block/cogwheel");		//??????
        }
        nic: NicCSMA {
            parameters:
                @display("p=70,380;i=block/wrxtx,#008000");
                phyType = "PgPhyLayer";
        }
        appl: GliderApplLayer {
            parameters:
                @display("p=70,30;i=app");
        }
    connections:
        nic.upperLayerOut --> appl.lowerLayerIn;
        nic.upperLayerIn <-- appl.lowerLayerOut;
        nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> appl.lowerControlIn;
        nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- appl.lowerControlOut;

        //# connections of transport, session, and presentation layer depending of the
        //# existence


        radioIn --> nic.radioIn;
}
