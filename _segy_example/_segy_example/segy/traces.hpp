#ifndef __TRACES__
#define __TRACES__
namespace traces
{
    template
        <
        typename headerType,
        typename traceType
        >
        class tracesHolder
    {
        typedef std::vector<traceType> rawTraceSeqType;
        typedef std::pair < headerType, rawTraceSeqType > pairType;
        std::vector< pairType > _traces;
    public:
        tracesHolder() {};
        ~tracesHolder() {}

        void
            add(const headerType& th, const rawTraceSeqType& trace)
        {
            _traces.push_back(std::make_pair(th, trace));
        }

        size_t
            getCountTraces()
        {
            return (_traces.size());
        }

        headerType getHeaderByIndex(size_t index)
        {
            return (_traces[index].first);
        }

        rawTraceSeqType
            getTraceDataByIndex(size_t index)
        {
            return (_traces[index].second);
        }

        void clear()
        {
            _traces.clear();
        }
    };
};
#endif
