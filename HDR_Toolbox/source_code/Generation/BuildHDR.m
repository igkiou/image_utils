function imgHDR = BuildHDR(format, lin_type, weightFun, stack, stack_exposure)
%
%       imgHDR = BuildHDR(format)
%
%
%        Input:
%           -format: an LDR format for reading LDR images in the current directory 
%           -lin_type: the linearization function:
%                      - 'linearized': images are already linearized
%                      - 'gamma2.2': gamma function 2.2 is used for
%                                    linearisation;
%                      - 'function': a spline for RGB is used for 
%                                    linearisation passed as input in
%                                    lin_fun
%                      - 'tabledDeb97': a tabled RGB function is used for
%                                       linearisation passed as input in
%                                       lin_fun using Debevec and Malik 97
%                                       method
%           -weight_type:
%               - 'all': weight is set to 1
%               - 'hat': hat function 1-(2x-1)^12
%               - 'Deb97': Debevec and Malik 97 weight function
%           -stack: a stack of LDR images; 4-D array where values are
%           -stack_exposure: exposure values of the stack in seconds
%
%        Output:
%           -imgHDR: the final HDR image
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

%is a weight function defined?
if(~exist('weightFun'))
    weightFun = 'all';
end

%is the linearization type of the images defined?
if(~exist('lin_type'))
    lin_type = 'gamma2.2';
end

if(~exist('stack')&&~exist('stack_exposure'))
    %Read images from the current directory
    [stack, stack_exposure] = ReadLDRStack(format);
else
    %
    maxStack = max(max(max(max(stack))));
    if(maxStack<=1.0)
        stack = stack * 255;
    end   
end

%CRF Calculation
lin_fun = [];
switch lin_type
    case 'tabledDeb97'
        %Weight function
        W = WeightFunction(0:1/255:1,weightFun);
        %Convert the stack into a smaller stack
        stack2 = StackLowRes(stack);
        %Linearization process using Debevec and Malik 1998's method
        lin_fun = zeros(256,3);
        log_stack_exposure = log(stack_exposure);
        for i=1:3
            g=gsolve(stack2(:,:,i),log_stack_exposure,10,W);
            lin_fun(:,i)=(g/max(g));
        end
    otherwise    
end

%Combine different exposure using linearization function
imgHDR = CombineLDR(stack, stack_exposure, lin_type, lin_fun, weightFun);

end