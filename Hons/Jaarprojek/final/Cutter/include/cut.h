

void Confirm(char* filename);

void copy(FILE* InFd,FILE* OutFd,offset from,offset to);

int BackwardFindBoundaries(offset from, offset* packetstart, offset* packetend);

int ReadTimeStamp(offset off,double* timestamp);

int ReadPacketTS(offset* off,double* pts,double* dts);

int CutPart(offset begin, offset end, char* filename);

void Offset(double time,offset off);

void TimeToOffsets();
