function ret = ConvHDRtoLDR(fmtIn, fmtOut, tonemapper, ldr_gamma)
%
%        iret = ConvHDRtoLDR(fmtIn, fmtOut)
%
%        This batch function converts LDR images in the current directory
%        from a format, fmtIn, to another LDR format, fmtOut.
%        
%        For example:
%           ConvLDRtoLDR('hdr', 'jpg', @ReinhardTMO, 2.2);
%
%        This lines tonemaps all the .hdr files in the folder using the 
%        Reinhard et al.'s operator and it saves them as .jpg files using
%        gamma 2.2
%
%        Input:
%           -fmtIn: an input string represeting the LDR format of the images
%           to be converted. This can be: 'hdr', 'pfm'
%           -fmtOut: an input string represeting the LDR format of
%           converted images. This can be: 'jpeg', 'jpg', 'png', etc.
%           -tonemapper: the tone mapping function to be used for tone
%           mapping the input HDR images. The default value is the
%           Reinhard et al.'s operator
%           -ldr_gamma: the encoding gamma for the LDR images. The default
%           value is 2.2
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

if(~exist('ldr_gamma'))
    ldr_gamma = 2.2;
end

if(~exist('tonemapper'))
    tonemapper = @ReinhardTMO;
end

lst = dir(['*.',fmtIn]);

for i=1:length(lst)
    tmpName = lst(i).name;
    disp(tmpName);
    
    img = hdrimread(tmpName);
    
    img_tmo = GammaTMO(tonemapper(img),ldr_gamma);
    
    tmpName_we = tmpName(1:(end-3));
    imwrite(img_tmo,[tmpName_we, fmtOut]);
end

ret = 1;

end