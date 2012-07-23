function img=hdrimread(filename)
%
%       img=hdrimread(filename)
%
%
%        Input:
%           -filename: the name of the file to open
%
%        Output:
%           -img: the opened image
%
%     Copyright (C) 2011  Francesco Banterle
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

extension=lower(fileExtension(filename));

if(strcmpi(extension,'pic')==1)
    extension='hdr';
end

img=[];

switch extension
    
    %PIC-HDR format by Greg Ward
    case 'hdr'
        try
		 %Uncompressed RGBE Image
            img=read_rgbe(filename);  
        catch
            %RLE compressed image
            img=double(hdrread(filename));
        end
        
    %Portable float map
    case 'pfm'
        try
            img=read_pfm(filename);
        catch
            disp('This PFM file can not be read.');
        end
       
    otherwise
        %try to open as LDR image
        try
            img=double(imread(filename))/255;
        catch
            disp('This format is not supported.');
        end
end

%Remove specials
img=RemoveSpecials(img);
end