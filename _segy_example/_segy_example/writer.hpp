namespace SGY
{

    template
        <
        typename t_flip
        >
        struct writer
    {
        std::unique_ptr<SEGY::writer> seg;
        writer(std::string fileName) :seg(std::make_unique<SEGY::writer>())
        {
            seg->open(fileName);
        }

        void
            setTextHeader(const std::ostringstream& ostr)
        {
            seg->reinit(SEGY::headers::CHK_SEGY_TEXT_HEADER_SIZE);
            std::string tmp = ostr.str();
            memcpy(seg->buffer.data(), tmp.c_str(), tmp.length());
            seg->write(SEGY::headers::CHK_SEGY_TEXT_HEADER_SIZE);
        }

        void setBinaryHeader(const SEGY::headers::binHeader& binheader)
        {
            seg->reinit(SEGY::headers::CHK_SEGY_BINARY_HEADER_SIZE);
            memcpy(seg->buffer.data(), &binheader, sizeof(SEGY::headers::binHeader));
            seg->write(SEGY::headers::CHK_SEGY_BINARY_HEADER_SIZE);
        }

        void setTraceHeader(const SEGY::headers::traceHeader& trcHeader)
        {
            seg->reinit(SEGY::headers::CHK_SEGY_TRACE_HEADER_SIZE);
            memcpy(seg->buffer.data(), &trcHeader, sizeof(SEGY::headers::traceHeader));
            seg->write(SEGY::headers::CHK_SEGY_TRACE_HEADER_SIZE);
        }

        template
            <
            typename T
            >
            void setTrackBuffer(std::vector<T> buffer)
        {
            seg->reinit(buffer.size() * sizeof(T));
            memcpy(seg->buffer.data(), buffer.data(), buffer.size() * sizeof(T));
            seg->write(buffer.size() * sizeof(T));
        }
    };
}
