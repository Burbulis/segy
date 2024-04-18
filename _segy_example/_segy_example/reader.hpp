#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include "segy.hpp"

namespace SGY
{


    struct flip
    {
        static
            void
            trace_header(char* xs) {
            std::vector< int > sizes;
            sizes.insert(sizes.end(), 7, 4);
            sizes.insert(sizes.end(), 4, 2);
            sizes.insert(sizes.end(), 8, 4);
            sizes.insert(sizes.end(), 2, 2);
            sizes.insert(sizes.end(), 4, 4);
            sizes.insert(sizes.end(), 46, 2);
            sizes.insert(sizes.end(), 5, 4);
            sizes.insert(sizes.end(), 2, 2);
            sizes.insert(sizes.end(), 1, 4);
            sizes.insert(sizes.end(), 5, 2);
            sizes.insert(sizes.end(), 1, 4);
            sizes.insert(sizes.end(), 1, 2);
            sizes.insert(sizes.end(), 1, 4);
            sizes.insert(sizes.end(), 2, 2);

            for (auto size : sizes) {
                std::reverse(xs, xs + size);
                xs += size;
            }
        }

        static
            void
            binary_header(char* xs) {
            std::vector< int > sizes;
            sizes.insert(sizes.end(), 3, 4);
            sizes.insert(sizes.end(), 24, 2);

            for (auto size : sizes) {
                std::reverse(xs, xs + size);
                xs += size;
            }

            xs += 240;

            for (auto size : { 2, 2, 2 }) {
                std::reverse(xs, xs + size);
                xs += size;
            }
        }

        static
            void
            trace_data(char* xs, int samples, int samplesize) {
            for (int i = 0; i < samples; ++i) {
                std::reverse(xs, xs + samplesize);
                xs += samplesize;
            }
        }
    };

    struct noflip
    {
        static void trace_header(char* xs) {}
        static void binary_header(char* xs) {}
        static void trace_data(char* xs, int samples, int samplesize) {}
    };

    template
        <
        typename t_flip
        >
        struct reader
    {
        struct segy_
        {
            enum {
                INIT_SIZE_BUFFER_PARAM = headers::CHK_SEGY_TEXT_HEADER_SIZE + headers::CHK_SEGY_BINARY_HEADER_SIZE + headers::CHK_SEGY_TRACE_HEADER_SIZE

            };
            FILE* f;
            std::vector< char > buffer;
            bool result;
            unsigned int currentContext;
            segy_() :f(NULL), result(true), buffer(INIT_SIZE_BUFFER_PARAM), currentContext(0)
            {}
            void reinit(void)
            {
                buffer.clear();
                buffer.resize(INIT_SIZE_BUFFER_PARAM);
                memset(buffer.data(), 0, buffer.size());
            }

            void reinit(size_t newBufferSize)
            {
                buffer.clear();
                buffer.resize(newBufferSize);
                memset(buffer.data(), 0, buffer.size());
            }

        };

        std::unique_ptr<segy_> seg;

        reader()
        {
            seg = std::make_unique<segy_>();
        }


        void open_segy(std::string fileName)
        {
            auto error = fopen_s(&seg->f, fileName.c_str(), "rb");
            if (0 != error)
                throw (std::runtime_error("can't open segy file" + fileName));
        }

        void close_segy(void)
        {
            if (NULL != seg->f)
                fclose(seg->f);
        }

        ~reader()
        {
            close_segy();
        }

        std::vector<std::string>
            getTextHeader()
        {
            std::ostringstream ostr = {};
            seg->result = true;
            seg->currentContext = headers::CHK_SEGY_TEXT_HEADER_SIZE;
            auto sz = fread(seg->buffer.data(), 1, headers::CHK_SEGY_TEXT_HEADER_SIZE, seg->f);
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
            seg->result = (sz == headers::CHK_SEGY_TEXT_HEADER_SIZE);
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
            seg->currentContext = headers::CHK_SEGY_BINARY_HEADER_SIZE;
            auto sz = fread(seg->buffer.data(), 1, headers::CHK_SEGY_BINARY_HEADER_SIZE, seg->f);
            seg->result = (sz == headers::CHK_SEGY_BINARY_HEADER_SIZE);
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
            seg->currentContext = headers::CHK_SEGY_TRACE_HEADER_SIZE;
            auto sz = fread(seg->buffer.data(), 1, headers::CHK_SEGY_TRACE_HEADER_SIZE, seg->f);
            seg->result = (sz == headers::CHK_SEGY_TRACE_HEADER_SIZE);
            t_flip::trace_header(seg->buffer.data());
            return (reinterpret_cast<outType*>(seg->buffer.data()));
        }

        int
            sample_size(headers::SEGY_FORMAT sf)
        {
            switch (sf)
            {
            case headers::SEGY_FORMAT::SEGY_IBM_FLOAT_4_BYTE:             return 4;
            case headers::SEGY_FORMAT::SEGY_SIGNED_INTEGER_4_BYTE:        return 4;
            case headers::SEGY_FORMAT::SEGY_SIGNED_INTEGER_8_BYTE:        return 8;
            case headers::SEGY_FORMAT::SEGY_SIGNED_SHORT_2_BYTE:          return 2;
            case headers::SEGY_FORMAT::SEGY_FIXED_POINT_WITH_GAIN_4_BYTE: return 4;
            case headers::SEGY_FORMAT::SEGY_IEEE_FLOAT_4_BYTE:            return 4;
            case headers::SEGY_FORMAT::SEGY_IEEE_FLOAT_8_BYTE:            return 8;
            case headers::SEGY_FORMAT::SEGY_SIGNED_CHAR_1_BYTE:           return 1;
            case headers::SEGY_FORMAT::SEGY_UNSIGNED_CHAR_1_BYTE:         return 1;
            case headers::SEGY_FORMAT::SEGY_UNSIGNED_INTEGER_4_BYTE:      return 4;
            case headers::SEGY_FORMAT::SEGY_UNSIGNED_SHORT_2_BYTE:        return 2;
            case headers::SEGY_FORMAT::SEGY_UNSIGNED_INTEGER_8_BYTE:      return 8;
            case headers::SEGY_FORMAT::SEGY_SIGNED_CHAR_3_BYTE:           return 3;
            case headers::SEGY_FORMAT::SEGY_UNSIGNED_INTEGER_3_BYTE:      return 3;
            case headers::SEGY_FORMAT::SEGY_NOT_IN_USE_1:
            case headers::SEGY_FORMAT::SEGY_NOT_IN_USE_2:
            default:
                return -1;
            }
        }

        bool
            get_trData(int samples, int samplesize)
        {
            size_t tr_size = samples * samplesize;
            seg->reinit(tr_size);
            seg->result = true;
            seg->currentContext = 0;
            auto sz = fread(seg->buffer.data(), 1, tr_size, seg->f);
            seg->result = (sz == tr_size);
            t_flip::trace_data(seg->buffer.data(), samples, samplesize);
            float tt[4001];
            memcpy(tt, seg->buffer.data(), tr_size);
            //   for (size_t i = 0; i < samples -1; ++i)
            //   {
                 //  unsigned long test = static_cast<unsigned long>(tt[i] * 1000);
                  // if (test > 0)
                  //   std::cout << tt[i] << std::endl;
            //   }
            return (seg->result);
        }

    };//reader

    struct writer
    {
        void
            setTextHeader(std::vector<std::string>& tesxt)
        {
            std::ostringstream ostr = {};

        }




    };


}
