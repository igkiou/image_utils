function Lav=logMean(img)
%
%
%        Lav=logMean(img)
%
%
%        Input:
%           -img: an single channel image
%
%        Output:
%           -Lav: the logarithmic mean of img
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

delta=1e-6;

Lav=exp(mean(mean(log(img+delta))));

end