function imgOut = ChangeMapping(img, mappingIn, mappingOut)
%
%new
%        imgOut = ChangeMapping(img,mapping1,mapping2)
%
%
%        Input:
%           -img: an environment map encoded with mapping1
%           -mapping1: the mapping representation of img. mapping1 is a
%                      string which takes the following values:
%                      - 'Angular'
%                      - 'LongitudeLatitude' or 'LL'
%                      - 'CubeMap'
%                      - 'Spherical'
%           -mapping2: the desired output mapping for imgOut. This mapping
%           is a string and takes the same values of mapping1. Note that
%           the 'Spherical' is not supported yet.
%
%        Output:
%           -imgOut: img in the mapping2 format
%
%     Copyright (C) 2011-12  Francesco Banterle
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

%Mapping to be fixed
if(strcmpi(mappingOut,'Spherical'))
    error('This output mapping is not yet supported');
end

%First step generation of directions
[r,c,col] = size(img);

if(strcmpi(mappingIn,'LL')==1)
    mappingIn = 'LongitudeLatitude';
end

if(strcmpi(mappingOut,'LL')==1)
    mappingOut = 'LongitudeLatitude';
end

if(strcmpi(mappingIn,mappingOut)==1)
    %if it is the same mapping no work to be done
    imgOut = img;
else
    maxCoord = max([r,c])/2;
    switch mappingOut
        case 'LongitudeLatitude'        
            D = LL2Direction(maxCoord,maxCoord*2);        %TESTED OK   
        case 'Angular'
            D = Angular2Direction(maxCoord,maxCoord);     %TESTED OK   
        case 'CubeMap'
            D = CubeMap2Direction(maxCoord*4,maxCoord*3); %TESTED OK
        case 'Spherical'
            %D = Spherical2Direction(maxCoord,maxCoord);   
        otherwise
            error('ChangeMapping: The following mapping is not recognized.');
    end

    %Second step interpolation of values
    imgOut = [];
    
    switch mappingIn
        case 'LongitudeLatitude'
            [X1,Y1] = Direction2LL(D,r,c);            %TESTED OK
        case 'Spherical'
            [X1,Y1] = Direction2Spherical(D,r,c);     %TESTED OK
        case 'Angular'
            [X1,Y1] = Direction2Angular(D,r,c);       %TESTED OK               
        case 'CubeMap'
            [X1,Y1] = Direction2CubeMap(D,r,c);       %TESTED OK
        otherwise
            error('ChangeMapping: The following mapping is not recognized.');
    end

    %Interpolation
    [X,Y] = meshgrid(1:c,1:r);
    X1 = real(round(X1));
    Y1 = real(round(Y1));
    
    for i=1:col
        imgOut(:,:,i) = interp2(X,Y,img(:,:,i),X1,Y1,'*cubic');
    end

    [rM,cM,colM] = size(imgOut);
    switch mappingOut
        case 'CubeMap'
            imgOut = imgOut.*CrossMask(rM,cM);

        case 'Angular'
            imgOut = imgOut.*AngularMask(rM,cM);

        case 'Spherical'
            imgOut = imgOut.*AngularMask(rM,cM);
    end    

    imgOut = RemoveSpecials(imgOut);
end

end