function imgOut = CombineLDR(stack, stack_exposure, lin_type, lin_fun, weight_type)
%
%       imgOut = CombineLDR(stack, stack_exposure, lin_type, lin_fun, weight_type)
%
%
%        Input:
%           -stack: a sequence of LDR images with values in [0,1].
%           -stack_exposure: a sequence of exposure values associated to
%                            images in the stack
%           -lin_type: the linearization function:
%                      - 'linearized': images are already linearized
%                      - 'gamma2.2': gamma function 2.2 is used for
%                                    linearisation;
%                      - 'function': a spline for RGB is used for 
%                                    linearisation passed as input in
%                                    lin_fun
%                      - 'tabledDeb97': a tabled RGB function is used for
%                                       linearisation passed as input in
%                                       lin_fun using Debevec and Malik
%                                       method
%           -lin_fun: extra parameters for linearization, see lin_type
%           -weight_type:
%               - 'all': weight is set to 1
%               - 'hat': hat function 1-(2x-1)^12
%               - 'Deb97': Debevec and Malik 97 weight function
%
%        Output:
%           -imgOut: the combined HDR image from the stack
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

[r,c,col,n] = size(stack);

imgOut = zeros(r,c,col);

totWeight = zeros (r,c,col);

for i= 1: n
    tmpStack = [];
    switch lin_type
        case 'linearized'
            tmpStack = stack(:,:,:,i)/255;
        case 'gamma2.2'
            tmpStack = ((stack(:,:,:,i)/255).^2.2);
        case 'function'
            tmpStack = lin_fun(stack(:,:,:,i)/255);
        case 'tabledDeb97'
            tmpStack = tabledFunction(stack(:,:,:,i), lin_fun);
        otherwise
            tmpStack = [];
    end
   
    %Calculation of the weight function
    tmpWeight = WeightFunction(tmpStack, weight_type);
    
    imgOut = imgOut + (tmpWeight.*tmpStack) / stack_exposure(i) ;
    
    totWeight = totWeight + tmpWeight;
end

imgOut = RemoveSpecials(imgOut ./ totWeight);

end