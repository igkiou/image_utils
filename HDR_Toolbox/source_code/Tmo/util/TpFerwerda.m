function val=TpFerwerda(x)
%
%       val=TpFerwerda(x)
%
%
%       The gamma function used in Ferwerda TMO for Photopic levels
%
%       Input:
%           -x: a value
%
%       Output:
%           -val: application of the gamma function
%
% 
%     This program is free software: you can redistribute it and/or modify
%     it under the terms of the GNU General Public License as published by
%     the Free Software Foundation, either version 3 of the License, or
%     (at your option) any later version.
% 
%     This program is distributed in the hope that it will be useful,
%     but WITHOUT ANY WARRANTY; without even the implied warranty of
%     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%     GNU General Public License for more details.
% 
%     You should have received a copy of the GNU General Public License
%     along with this program.  If not, see <http://www.gnu.org/licenses/>.
%

x2=log10(x);

if(x2<=-3.94)
    val=-2.86;
else
    if(x2>=-1.44)
        val=x2-0.395;
    else
        val=(0.405*x2+1.6)^2.18-2.86;
    end
end

val=10^val;

end