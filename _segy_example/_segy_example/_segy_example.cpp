// _segy_example.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <assert.h>
#include <iostream>
#include <sstream>
#include <utility>
//#include "segy/reader.hpp"
#include "segy/writer.hpp"
#include "segy/segy.hpp"
#include "segy/traces.hpp"


/*

bool isFlipByteOrder(std::string fileName)
{
    SGY::reader<SEGY::flip> rd;
    rd.open(fileName);
    rd.getTextHeader();
    SEGY::headers::binHeader bh = *rd.getBinaryHeader<SEGY::headers::binHeader>();
    SEGY::headers::SEGY_FORMAT frm = static_cast<SEGY::headers::SEGY_FORMAT>(bh.format);
    return ( ((frm > -1) && (frm < SEGY::headers::SEGY_FORMAT::SEGY_NOT_IN_USE_2)) );
}
*/

template
<
    typename T
>

std::vector<T> generateChanell(int disxreetPerTrace)
{
    std::vector<T> ret;
    for (int i = 0; i < disxreetPerTrace; ++i)
    {
        int  tt = int( ((i < 2000)?sin(i * 100):cos(i * 100) )  * i);
        ret.push_back(tt);
    }
    return(ret);
}


int main()
{
    assert(sizeof(SEGY::headers::binHeader) == SEGY::headers::CHK_SEGY_BINARY_HEADER_SIZE);
    assert(sizeof(SEGY::headers::traceHeader) == SEGY::headers::CHK_SEGY_TRACE_HEADER_SIZE);
    traces::tracesHolder< SEGY::headers::traceHeader,  int  > tHolder;
    SGY::writer<SEGY::noflip> wd("c:\\work\\test.sgy");
    std::ostringstream srtr;
    srtr << "only test file for any manipulations...";
    wd.setTextHeader(srtr);
//    rd.getBinaryHeader<SEGY::headers::binHeader>();
    SEGY::headers::binHeader bh = SEGY::headers::make_binaryHeader(2000, 5, 1000, SEGY::headers::SEGY_FORMAT::SEGY_SIGNED_INTEGER_4_BYTE);
    wd.setBinaryHeader(bh);
    SEGY::headers::SEGY_FORMAT frm = static_cast<SEGY::headers::SEGY_FORMAT>(bh.format);
    bool test = ((frm > -1) && (frm < SEGY::headers::SEGY_FORMAT::SEGY_NOT_IN_USE_2));
    size_t counter = 0;
    SEGY::headers::traceHeader th;
    memset(&th, 0, sizeof(th));
     for (size_t i = 0; i < bh.traces;++i)
        tHolder.add(th, generateChanell< int  >(bh.samples));
    
    for (size_t i = 0; i < tHolder.getCountTraces(); ++i)
    {
        wd.setTraceHeader(tHolder.getHeaderByIndex(i));
        wd.setTrackBuffer(tHolder.getTraceDataByIndex(i));
    }
    std::cout << "sz = " << sizeof(float) << ":" << sizeof(SEGY::headers::traceHeader) << std::endl;
}
