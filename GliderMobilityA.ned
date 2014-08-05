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

import org.mixim.base.modules.BaseMobility;

simple GliderMobilityA extends BaseMobility
{
    
    parameters:
        @class(GliderMobilityA);
        double angle @unit(deg);        
       	double airSpeed @unit(mps) = default(10.0mps); // speed of the host (in m/s)
       	double climbRate @unit(mps) = default(-1.3mps); // sinkrate of the host (in m/s)
       	double secondsPerRotation @unit("s") = default(20.0s); // seconds per rotation
        double acceleration @unit(mpss) = default(0.0mpss); // acceleration of linear motion (m/s2)

        double updateInterval @unit("s") = default(0.1s);
        string traceTest = default("");
        double traceInterval @unit("s") = default(0.25s);
        
        
}

