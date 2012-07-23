%
%     HDR Toolbox Installer
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

disp('Installing the HDR Toolbox...');
d1 = 'ColorSpace';
d2 = 'Compression';
d3 = 'EnvironmentMaps';
d4 = 'EO';
d5 = 'Formats';
d6 = 'Generation';
d7 = 'IBL';
d8 = 'IBL/util';
d9 = 'IO';
d10 = 'LaplacianPyramids';
d11 = 'NativeVisualization';
d12 = 'Tmo';
d13 = 'Tmo/util';
d14 = 'util';
d15 = 'BatchFunctions';

cp = pwd();

tmpStr = '/source_code/';
addpath([cp,tmpStr,d1],'-begin');
addpath([cp,tmpStr,d2],'-begin');
addpath([cp,tmpStr,d3],'-begin');
addpath([cp,tmpStr,d4],'-begin');
addpath([cp,tmpStr,d5],'-begin');
addpath([cp,tmpStr,d6],'-begin');
addpath([cp,tmpStr,d7],'-begin');
addpath([cp,tmpStr,d8],'-begin');
addpath([cp,tmpStr,d9],'-begin');
addpath([cp,tmpStr,d10],'-begin');
addpath([cp,tmpStr,d11],'-begin');
addpath([cp,tmpStr,d12],'-begin');
addpath([cp,tmpStr,d13],'-begin');
addpath([cp,tmpStr,d14],'-begin');
addpath([cp,tmpStr,d15],'-begin');
addpath([cp,'/demos/'],'-begin');

savepath
disp('done!');

clear('d1');
clear('d2');
clear('d3');
clear('d4');
clear('d5');
clear('d6');
clear('d7');
clear('d8');
clear('d9');
clear('d10');
clear('d11');
clear('d12');
clear('d13');
clear('d14');
clear('cp');

disp('Check demo1.m and demo2.m on basics for how using the HDR Toolbox!');