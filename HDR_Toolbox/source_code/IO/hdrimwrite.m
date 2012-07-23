function ret=hdrimwrite(img, filename)
%
%       ret=hdrimwrite(img, filename)
%
%
%        Input:
%           -img: the image to write on the hard disk
%           -filename: the name of the image to write
%
%        Output:
%           -ret: a boolean value, it is set to 1 if the write succeed, 0 otherwise
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

%if it is a gray image we create three channels
[n,m,color]=size(img);
if(color==1)
    imgOut = zeros(size(img));
    for i=1:3
        imgOut(:,:,i)=img;
    end
    img = imgOut;
end

ret=0;

extension=lower(fileExtension(filename));

if(strcmpi(extension,'pic')==1)
    extension='hdr';
end

switch extension
    
    %PIC-HDR format by Greg Ward
    case 'hdr'
        try
            write_rgbe(img,filename);
        catch
            error('This PIC/HDR file can not be written.');
        end
        
    %Portable float map
    case 'pfm'
        try
            write_pfm(img,filename);
        catch
            error('This PFM file can not be written.');
        end
        
    otherwise
        %try to save as LDR image
        try
            imwrite(ClampImg(img,0,1),filename);
        catch
            error('This format is not supported.');
        end
end

ret=1;

end