function done=MedianCutAux(xMin,xMax,yMin,yMax,iter,cut)
%
%
%        done=MedianCutAux(xMin,xMax,yMin,yMax,iter,cut)
%       
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

global L;
global imgWork;
global limitSize;
global nLights;
global lights;

done=1;

lx=xMax-xMin;
ly=yMax-yMin;

if((lx>limitSize)&&(ly>limitSize)&&(iter<nLights))
    tot=sum(sum(L(yMin:yMax,xMin:xMax)));

    pivot=-1;
    if(cut==1)
        %Cut on the X-axis
        for i=xMin:xMax
            c=sum(sum(L(yMin:yMax,xMin:i)));
                if(c>=(tot-c)&&pivot==-1)
                pivot=i;
                end
        end

        if(lx>ly)
            MedianCutAux(xMin,pivot,yMin,yMax,iter+1,1);
            MedianCutAux(pivot+1,xMax,yMin,yMax,iter+1,1);
        else
            MedianCutAux(xMin,pivot,yMin,yMax,iter+1,0);
            MedianCutAux(pivot+1,xMax,yMin,yMax,iter+1,0);
        end
        
    else
        %Cut on the Y-axis
        for i=yMin:yMax
            c=sum(sum(L(yMin:i,xMin:xMax)));
            if(c>=(tot-c)&&pivot==-1)
                pivot=i;
            end
        end
        
        if(ly>lx)
            MedianCutAux(xMin,xMax,yMin,pivot,iter+1,0);
            MedianCutAux(xMin,xMax,pivot+1,yMax,iter+1,0);
        else
            MedianCutAux(xMin,xMax,yMin,pivot,iter+1,1);
            MedianCutAux(xMin,xMax,pivot+1,yMax,iter+1,1);    
        end
    end
else
    %Generation of the light source
    lights=[lights,CreateLight(xMin,xMax,yMin,yMax,L,imgWork)];
end

end