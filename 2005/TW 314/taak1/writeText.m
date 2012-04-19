function writeText(filename,kt)
fid=fopen(filename,'w');
fwrite(fid,kt,'uchar');
fclose(fid);