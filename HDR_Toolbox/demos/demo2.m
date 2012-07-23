%
%       HDR Toolbox demo 2:
%	   1) Load "CS_Warwick.pfm" HDR image
%      2) Show the Tone mapped HDR image
%	   3) Calculate/Show/Save a Diffuse Map
%	   4) Calculate/Show/Save a Light sources
%
%       Author: Francesco Banterle
%       Copyright February 2011 (c)
%
%

disp('1) Load "CS_Warwick.hdr" HDR image');
img = hdrimread('CS_Warwick.hdr');

disp('2) Tone Mapping the HDR image');
imgTMO = ReinhardTMO(img, 0.15, 1e8, 0,8);
disp('3) Apply Color Correction');
imgTMO = ColorCorrection(imgTMO,0.8);
disp('4) Showing the tonemapped images with gamma 2.2');
figure(1);
GammaTMO(imgTMO, 2.2, 0, 1);

disp('5) Calculate/Show/Save a Diffuse Map');
[imgOut,SH] = DiffuseConvolutionSH(img, 1);
figure(2);
GammaTMO(imgOut*0.5,2.2,0,1);
hdrimwrite(imgOut,'diffuse_map.hdr');

disp('6) Calculate/Show/Save a Light sources');
[imgMC,lights]=MedianCut(img,256,1);
figure(3);
GammaTMO(GaussianFilter(imgMC*0.25,1.0),1.0,0,1);
ExportLights(lights,'light_sources');
