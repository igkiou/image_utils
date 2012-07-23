function [img_cur_exp, exposure] = InteractiveHDRVis(img)
%
%        exposure = InteractiveHDRVis(img)
%
%        This function allows to explore the dynamic range in an image,
%        img. This is achieved by clicking with the left mouse button into
%        the image to change its exposure. The program ends when the user
%        presses the key ENTER or right mouse button.
%
%        Input:
%           -img: an HDR image
%
%        Output:
%           -img_cur_exp: the input image with the last selected exposure
%           -exposure: the last click exposure
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

%get(0,'Screensize') 

L = lum(img);

bFlag = 1;

invGamma = 1.0/2.2;
exposure = 1.0;
kernelSize = 7;

img_cur_exp = (img*exposure);

while(bFlag)
    imshow(img_cur_exp.^invGamma);
    [x,y,button] = ginput(1);
    
    if(isempty(x)==0)
        block = L((y-kernelSize):(y+kernelSize),(x-kernelSize):(x+kernelSize));
        exposure = 0.25/mean(block(:));
        disp(exposure);
        img_cur_exp = (img*exposure);
        
        if(button==3)
            bFlag = 0;
        end
    else
        bFlag = 0;
    end
end

end