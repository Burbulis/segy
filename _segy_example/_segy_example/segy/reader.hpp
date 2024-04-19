#ifndef _SGY_READER_
#define _SGY_READER_

#include <sstream>
#include <vector>
#include <algorithm>
#include "segy.hpp"

namespace SGY
{




    template
        <
        typename t_flip
        >
    struct reader
    {

        std::unique_ptr<SEGY::reader> seg;

        reader()
        {
            seg = std::make_unique<SEGY::reader>();
        }

        void 
        open(std::string fileName)
        {
            seg->open(fileName);
        }
  
        ~reader()
        {
            seg->close();
        }

        void getRawTextHeader(void)
        {
            seg->result = true;
            seg->currentContext = SEGY::headers::CHK_SEGY_TEXT_HEADER_SIZE;
            seg->read(SEGY::headers::CHK_SEGY_TEXT_HEADER_SIZE);
        }

        const std::vector<char>& getInternalBuffer(void)
        {
            return (seg->buffer);
        }

        std::vector<std::string>
            getTextHeader()
        {
            std::ostringstream ostr = {};
            seg->result = true;
            seg->currentContext = SEGY::headers::CHK_SEGY_TEXT_HEADER_SIZE;
            seg->read(SEGY::headers::CHK_SEGY_TEXT_HEADER_SIZE);
            ostr << seg->buffer.data();
            std::string str = ostr.str();
            std::vector<std::string> str_;
            size_t start = 0;
            do
            {

                size_t end = str.find("\r\n", start);
                if (end == std::string::npos)
                    break;
                str_.push_back(str.substr(start, end - start));
                start = end + 2;
            } while (true);
            
            str_.erase(std::remove_if(str_.begin(), str_.end(), [](const std::string& str)
                {
                    return (str.empty());
                }), str_.end());
            if (str_.empty())
                str_.push_back(str);
            return str_;
        }

        template
            <
            typename outType
            >
            outType* getBinaryHeader()
        {
            seg->reinit();
            seg->result = true;
            seg->currentContext = SEGY::headers::CHK_SEGY_BINARY_HEADER_SIZE;
            seg->read(SEGY::headers::CHK_SEGY_BINARY_HEADER_SIZE);
            t_flip::binary_header(seg->buffer.data());
            return (reinterpret_cast<outType*>(seg->buffer.data()));
        }

        template
            <
            typename outType
            >
            outType* getTraceHeader()
        {
            seg->reinit();
            seg->result = true;
            seg->currentContext = SEGY::headers::CHK_SEGY_TRACE_HEADER_SIZE;
            seg->read(SEGY::headers::CHK_SEGY_TRACE_HEADER_SIZE);
            t_flip::trace_header(seg->buffer.data());
            return (reinterpret_cast<outType*>(seg->buffer.data()));
        }

        int
        sample_size(SEGY::headers::SEGY_FORMAT sf)
        {
            switch (sf)
            {
            case SEGY::headers::SEGY_FORMAT::SEGY_IBM_FLOAT_4_BYTE:             return 4;
            case SEGY::headers::SEGY_FORMAT::SEGY_SIGNED_INTEGER_4_BYTE:        return 4;
            case SEGY::headers::SEGY_FORMAT::SEGY_SIGNED_INTEGER_8_BYTE:        return 8;
            case SEGY::headers::SEGY_FORMAT::SEGY_SIGNED_SHORT_2_BYTE:          return 2;
            case SEGY::headers::SEGY_FORMAT::SEGY_FIXED_POINT_WITH_GAIN_4_BYTE: return 4;
            case SEGY::headers::SEGY_FORMAT::SEGY_IEEE_FLOAT_4_BYTE:            return 4;
            case SEGY::headers::SEGY_FORMAT::SEGY_IEEE_FLOAT_8_BYTE:            return 8;
            case SEGY::headers::SEGY_FORMAT::SEGY_SIGNED_CHAR_1_BYTE:           return 1;
            case SEGY::headers::SEGY_FORMAT::SEGY_UNSIGNED_CHAR_1_BYTE:         return 1;
            case SEGY::headers::SEGY_FORMAT::SEGY_UNSIGNED_INTEGER_4_BYTE:      return 4;
            case SEGY::headers::SEGY_FORMAT::SEGY_UNSIGNED_SHORT_2_BYTE:        return 2;
            case SEGY::headers::SEGY_FORMAT::SEGY_UNSIGNED_INTEGER_8_BYTE:      return 8;
            case SEGY::headers::SEGY_FORMAT::SEGY_SIGNED_CHAR_3_BYTE:           return 3;
            case SEGY::headers::SEGY_FORMAT::SEGY_UNSIGNED_INTEGER_3_BYTE:      return 3;
            case SEGY::headers::SEGY_FORMAT::SEGY_NOT_IN_USE_1:
            case SEGY::headers::SEGY_FORMAT::SEGY_NOT_IN_USE_2:
            default:
                return -1;
            }
        }

        bool
        get_trData(int samples, int samplesize)
        {
            if (-1 == samplesize)
                throw std::runtime_error("samplesize error.");
            size_t tr_size = samples * samplesize;
            seg->reinit(tr_size);
            seg->result = true;
            seg->currentContext = 0;
            seg->read(tr_size);
            t_flip::trace_data(seg->buffer.data(), samples, samplesize);
            return (seg->result);
        }

    };//reader
}
#endif
