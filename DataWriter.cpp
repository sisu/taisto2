#include "DataWriter.hpp"
char DataWriter::Data[1<<16];
char* const DataWriter::Buf = DataWriter::Data+4;
