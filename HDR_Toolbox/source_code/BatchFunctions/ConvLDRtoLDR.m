function ret = ConvLDRtoLDR(fmtIn, fmtOut)
%
%        iret = ConvLDRtoLDR(fmtIn, fmtOut)
%
%        This batch function converts LDR images in the current directory
%        from a format, fmtIn, to another LDR format, fmtOut.
%        
%        For example:
%           ConvLDRtoLDR('jpg','png');
%
%        This lines converts .jpg files in the folder into .png files
%
%        Input:
%           -fmtIn: an input string represeting the LDR format of the images
%           to be converted. This can be: 'jpeg', 'jpg', 'png', etc.
%           -fmtOut: an input string represeting the LDR format of
%           converted images. This can be: 'jpeg', 'jpg', 'png', etc.
%
%        Output:
%           -ret: a boolean value, true or 1 if the method succeeds
%
%     Copyright (C) 2012  Francesco Banterle
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

ret = 0;

lst = dir(['*.',fmtIn]);

for i=1:length(lst)
    tmpName = lst(i).name;
    disp(tmpName);
    
    img = imread(tmpName);
    tmpName_we = tmpName(1:(end-3));
    imwrite(img,[tmpName_we, fmtOut]);
end

ret = 1;

end