<<<<<<< HEAD
﻿// _segy_example.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
=======
// _segy_example.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <assert.h>
#include <sstream>
#include "reader.hpp"
#include "writer.hpp"
bool isFlipByteOrder(std::string fileName)
{
    SGY::reader<SEGY::flip> rd;
    rd.open(fileName);
    rd.getTextHeader();
    SEGY::headers::binHeader bh = *rd.getBinaryHeader<SEGY::headers::binHeader>();
    SEGY::headers::SEGY_FORMAT frm = static_cast<SEGY::headers::SEGY_FORMAT>(bh.format);
    return ( ((frm > -1) && (frm < SEGY::headers::SEGY_FORMAT::SEGY_NOT_IN_USE_2)) );
}

//std::vector<SEGY::headers::traceHeader > 
//gettraceHeaders();
//{


//}

int main()
{
    assert(sizeof(SEGY::headers::binHeader) == SEGY::headers::CHK_SEGY_BINARY_HEADER_SIZE);
    assert(sizeof(SEGY::headers::traceHeader) == SEGY::headers::CHK_SEGY_TRACE_HEADER_SIZE);
    //if (isFlipByteOrder("c:\\work\\1.sgy"))
   // {
   //     std::cout << "flipped " << std::endl;
   // }
    SGY::reader<SEGY::noflip> rd;
    SGY::writer<SEGY::noflip> wd("c:\\work\\test.sgy");
    // SGY::writer wd("c:\\work\\test.sgy");
    rd.open("c:\\work\\2.sgy");
    rd.getRawTextHeader();
   // rd.getInternalBuffer();
    std::ostringstream srtr;
    srtr << "only test file for any manipulations...";
    wd.setTextHeader(srtr);
    SEGY::headers::binHeader bh =  * rd.getBinaryHeader<SEGY::headers::binHeader>();
    bh.format = SEGY::headers::SEGY_FORMAT::SEGY_SIGNED_INTEGER_4_BYTE; // SEGY_SIGNED_INTEGER_4_BYTE
    wd.setBinaryHeader(bh);
    SEGY::headers::SEGY_FORMAT frm = static_cast<SEGY::headers::SEGY_FORMAT>(bh.format);
    bool test = ((frm > -1) && (frm < SEGY::headers::SEGY_FORMAT::SEGY_NOT_IN_USE_2));
    size_t couner = 0;
    while (true)
    {
    
        SEGY::headers::traceHeader th = *rd.getTraceHeader<SEGY::headers::traceHeader>();;
        wd.setTraceHeader(th);
        bool test = rd.get_trData(bh.samples, rd.sample_size(static_cast<SEGY::headers::SEGY_FORMAT>(bh.format)));
        if (!test)
            break;
        std::vector< char > buff =  rd.getInternalBuffer();
        std::vector< int > tmp;
        tmp.resize(buff.size() / sizeof(int));
        memcpy(tmp.data(), buff.data(), buff.size());
        std::random_shuffle(tmp.begin(), tmp.end(), [](int val) {
            return  (rand() % val);
            });
        std::vector< int > out;
        int t = 1000;
        std::transform(tmp.begin(), tmp.end(), std::back_inserter(out),[t](int i) {
            return i * t;
            });
        wd.setTrackBuffer<int>(out);
        ++couner;
    }
    
    std::cout << "sz = " << sizeof(float) << ":" << sizeof(SEGY::headers::traceHeader) << std::endl;
}
>>>>>>> 638de4e26690b7740fa85fd01a057843e3e22c45
