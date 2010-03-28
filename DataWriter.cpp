#include "DataWriter.hpp"
char DataWriter::Data[1<<20];
char* const DataWriter::Buf = DataWriter::Data+4;
