function expand_map = BanterleExpandMap(img, BEM_bColorRec, BEM_clamping_threshold, BEM_percent, BEM_density_estimation_kernel, BEM_bHighQuality)
%
%		 expand_map = BanterleExpandMap(img, BEM_percent)
%
%
%		 Input:
%			-img: an input image LDR image in the linear domain
%           -BEM_bColorRec: a boolean value. If it is set 1 the expand
%           map will be calculated for each color channel
%           -BEM_clamping_threshold: if it is greater than 0, this value
%           determines the threshold for clamping light sources. Otherwise
%           this parameter is estimated automatically.
%           -BEM_percent: values in (0,1]
%           -BEM_density_estimation_kernel: a string representing the
%           kernel for density estimation. The function takes the same
%           input of the fspecial.m MATLAB function.
%           The default value is 'gaussian'.
%           -BEM_bHighQuality: a boolean value. If it is set to 1,
%           LischinskiMinimization will be used for better quality. This
%           takes more than using the bilateral filter. You may need MATLAB
%           at 64-bit for running high quality edge transer at HD
%           resolution (1920x1080).
%
%		 Output:
%			-expand_map: the final expand map
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

if(~exist('BEM_percent','var'))
    BEM_percent = 0.95;
else
    BEM_percent = ClampImg(BEM_percent,0.01,1.0);
end

if(~exist('BEM_density_estimation_kernel'))
    BEM_density_estimation_kernel = 'gaussian';
end

if(~exist('BEM_bHighQuality'))
    BEM_bHighQuality = 1;
end

%Median-cut for sampling img
[r,c,col]=size(img);
L = lum(img);
nLights = 2.^(round(log2(min([r,c]))+2));
[imgOut,lights] = MedianCut(img,nLights,0);

%Determing the samples clamping
window = round(min([r,c])/(2*sqrt(nLights)));
Lout = lum(imgOut);

if(BEM_clamping_threshold>0)
    thresholdSamples = BEM_clamping_threshold;
else
    %Create the histogram
    H = zeros(length(lights),1);
    for i=1:length(lights)
        [X0,X1,Y0,Y1]=GenerateBBox(lights(i).x,lights(i).y,r,c,window);
        indx = find(Lout(Y0:Y1,X0:X1)>0);
        H(i) = length(indx);
    end

    %Sort H
    H = sort(H);
    Hcum = cumsum(H);
    percentile = round(nLights*BEM_percent);
    [val,indx] = min(abs(Hcum-percentile));
    thresholdSamples = H(indx);
end

if(thresholdSamples>0)
    %samples' clamping
    imgOut_tmp = imgOut;
    Lout_tmp = Lout;
    for i=1:length(lights)
        [X0,X1,Y0,Y1] = GenerateBBox(lights(i).x,lights(i).y,r,c,window*3);
        indx = find(Lout(Y0:Y1,X0:X1)>0);    
        if(length(indx)<thresholdSamples)
            X=ClampImg(round(lights(i).x*c),1,c);
            Y=ClampImg(round(lights(i).y*r),1,r);
            imgOut_tmp(Y,X,:) = 0;
            Lout_tmp(Y,X) = 0;
        end
    end
    Lout = Lout_tmp;
    imgOut = imgOut_tmp;
end

%Density estimation thorugh splatting
window_scale = 8;
scaled_widow = window * window_scale;
H = fspecial(BEM_density_estimation_kernel,scaled_widow,GKSigma(scaled_widow));

[y,x] = find(Lout>0.0);
splat_pos = [x';y'];

if(BEM_bColorRec)
    [rH,cH] = size(H);
    H_col = zeros(rH,cH,col);
    for i=1:col
        H_col(:,:,i) = H;        
    end
    
    splat_power = zeros(length(x),col);
    for i=1:length(x)
        for j=1:col
            splat_power(i,j) = imgOut(y(i),x(i),j);
        end
    end
    
    clear('imgOut');
    clear('Lout');
    
    [img_density,counter_map] = imSplat(r,c,H_col,splat_pos,splat_power); 
    
    expand_map_de = zeros(r,c,col);
    for i=1:col
        img_density(:,:,i) = img_density(:,:,i)./counter_map;
        expand_map_de(:,:,i) = GaussianFilterWindow(img_density(:,:,i), scaled_widow);
    end
    clear('img_density');
    clear('counter_map');
else
    splat_power = Lout(y,x);
    
    clear('imgOut');
    clear('Lout');

    [img_density,counter_map] = imSplat(r,c,H,splat_pos,splat_power); 
    img_density = img_density./counter_map;

    expand_map_de = GaussianFilterWindow(img_density, scaled_widow);
    clear('img_density');
    clear('counter_map');
end

%Edge transfer
expand_map = zeros(r,c,col);

if(BEM_bColorRec)
    for i=1:col
        if(BEM_bHighQuality)
            expand_map(:,:,i) = LischinskiMinimization(img(:,:,i),expand_map_de(:,:,i),0.07*ones(r,c));
        else
            expand_map(:,:,i) = bilateralfilter(img(:,:,i), expand_map_de(:,:,i),32,all);
        end
    end
else    
    if(BEM_bHighQuality)
        tmp_expand_map = LischinskiMinimization(L,expand_map_de,0.07*ones(r,c));
    else
        tmp_expand_map = bilateralfilter(L, expand_map_de);
    end
    
    for i=1:col
        expand_map(:,:,i) = tmp_expand_map;
    end      
end

%Expand map normalization
max_em = max(expand_map(:));
if(max_em>0)
    expand_map = expand_map/max_em;
end

disp('This implementaion of the Banterle et al. 2008 method is not meant for videos but oly images.');
disp('Please contact the author at f_banty@yahoo.it in case parameters are not clear.');
end