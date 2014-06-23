%   Tao Du
%   taodu@stanford.edu
%   April 29, 2014

%   write data into a prb file
%   input:
%       data:
%       fileName:
%       type: 'rgb', 'depth'
%   this function has been tested
function [  ] = writePRBFile( data, fileName, type)
    data = double(data);
    fid = fopen(fileName, 'wb');
    if strcmp(type, 'rgb')
        fwrite(fid, 5, 'int8');
        [height, width, ~] = size(data);
        fwrite(fid, height, 'int32');
        fwrite(fid, width, 'int32');
        image = double(zeros(height, width * 3));
        image(:, 1 : 3 : end) = double(data(:, :, 1));
        image(:, 2 : 3 : end) = double(data(:, :, 2));
        image(:, 3 : 3 : end) = double(data(:, :, 3));
        fwrite(fid, image', 'double');
    elseif strcmp(type, 'depth')
        fwrite(fid, 4, 'int8');
        [height, width] = size(data);
        fwrite(fid, height, 'int32');
        fwrite(fid, width, 'int32');
        fwrite(fid, data', 'double');
    else
        disp('invalid type');
    end
    fclose(fid);
end