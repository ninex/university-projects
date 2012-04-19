function [t] = getText(filename);
fid = fopen(filename, 'r');
t = fread(fid, '*char')'
fclose(fid);