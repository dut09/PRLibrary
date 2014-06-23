%   Tao Du
%   taodu@stanford.edu
%   June 22, 2014

function [  ] = writeCameraFile( fc, cc, kc, alpha_c, filename)
    %   write left camera
    fid = fopen(filename, 'wb');
    %   compute the height and width
    height = uint32(cc(2) * 2 + 1);
    width = uint32(cc(1) * 2 + 1);
    fwrite(fid, height, 'int32');
    fwrite(fid, width, 'int32');
    fwrite(fid, fc, 'double');
    fwrite(fid, cc, 'double');
    fwrite(fid, kc, 'double');
    fwrite(fid, alpha_c, 'double');
    fclose(fid);
end



