%   Tao Du
%   taodu@stanford.edu
%   April 29, 2014

%   write prsc files for stereo cameras
function [  ] = writeStereoFile( fc_left, cc_left, kc_left, alpha_c_left,...
	fc_right, cc_right, kc_right, alpha_c_right, R, T )
    %   write left camera
    writeCameraFile(fc_left, cc_left, kc_left, alpha_c_left, 'depthCamera.prc');
    %   write right camera
    writeCameraFile(fc_right, cc_right, kc_right, alpha_c_right, 'rgbCamera.prc');
    %   write stereo camera
    fid = fopen('stereo.prsc', 'wb');
    fwrite(fid, R', 'double');
    fwrite(fid, T, 'double');
    fclose(fid);
end

