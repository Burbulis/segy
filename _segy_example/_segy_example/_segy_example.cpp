// _segy_example.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "reader.hpp"

int main()
{
    assert(sizeof(headers::binHeader) == headers::CHK_SEGY_BINARY_HEADER_SIZE);
    assert(sizeof(headers::traceHeader) == headers::CHK_SEGY_TRACE_HEADER_SIZE);
    SGY::reader<SGY::noflip> rd;
    rd.open_segy("c:\\work\\2.sgy");
    rd.getTextHeader();
    headers::binHeader bh =  * rd.getBinaryHeader<headers::binHeader>();
    size_t couner = 0;
    std::vector<headers::traceHeader > tst;
    while (true)
    {
    
        headers::traceHeader th = *rd.getTraceHeader<headers::traceHeader>();;
        tst.push_back(th);
        bool test = rd.get_trData(bh.samples, rd.sample_size(static_cast<headers::SEGY_FORMAT>(bh.format)));
        if (!test)
            break;
        ++couner;
    }
    
    std::cout << "sz = " << sizeof(float) << ":" << sizeof(headers::traceHeader) << std::endl;
}
