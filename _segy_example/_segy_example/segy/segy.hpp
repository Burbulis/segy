#ifndef __SEGY__
#define __SEGY__

#include <vector>

namespace SEGY
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

    namespace headers
    {
        enum
        {
            CHK_SEGY_BINARY_HEADER_SIZE = 400,
            CHK_SEGY_TEXT_HEADER_SIZE = 3200,
            CHK_SEGY_TRACE_HEADER_SIZE = 240
        };

        template
            <
            typename IntA,
            typename IntB
            >
            struct binary_header {
            IntA job_identification = 0;//3201-3204 номер работы 
            IntA line = 0;//3205-3208 номер профиля
            IntA reel = 0;//3209-3212 номер ленты
            IntB traces = 0;//3213-3214 число трасс в сейсмограмме
            IntB auxiliary_traces = 0;//3215-3216 число дополнительных трасс
            IntB interval = 0;//3217-3218 шаг дискретизации (мкс) в файле
            IntB interval_orig = 0;//3219-3220 шаг дискретизации в поле
            IntB samples = 0;//3221-3222 длина трассы в дискретах, в файле
            IntB samples_orig = 0;//3223-3224 длина трассы в полевой записи
            IntB format = 0;//3225-3226 код формата данных: 1 - R*4, 2 - I*4,3 - I*2, 4 - I*4h
            IntB ensemble_fold = 0;//3227-3228 кратность ОГТ
            IntB sorting = 0;//3229-3230 код сортировки: 1 - ОТВ, 2 – ОГТ,3 - L=const, 4 – Сумма
            /*        -1 = Other(should be explained in user Extended Textual File Header stanza
                        0 = Unknown
                        1 = As recorded(no sorting)
                        2 = CDP ensemble
                        3 = Single fold continuous profile
                        4 = Horizontally stacked
                        5 = Common source point
                        6 = Common receiver point
                        7 = Common offset point
                        8 = Common mid - point
                        9 = Common conversion point
                        */
            IntB vertical_sum = 0;//3231-3232 накапливание: 1 - нет, 2,....N
            IntB sweep_freq_start = 0;//3233-3234 начало свип-сигнала (мс)
            IntB sweep_freq_end = 0;//3235-3236 конец свип-сигнала
            IntB sweep_length = 0;//3237-3238 длина свип-сигнала
            IntB sweep_type = 0;//3239-3240 тип свип-сигнала Sweep type code : 1 = linear / 2 = parabolic/3 = exponential/ 4 = other
            IntB sweep_channel = 0;//3241-3242номер канала свип-сигнала
            IntB sweep_taperlen_start = 0;//3243-3244 длина конуса
            IntB sweep_taperlen_end = 0;//3245-3246 длина трассы свип-сигнала
            IntB taper_type = 0;//3247-3248 тип конуса Taper type : 1 = linear / 2 = cos^2 / 3 = other
            IntB correlated = 0;//3249-3250 корреляция: есть – 1, нет - 2
            IntB binary_gain_recovery = 0;//двоичный код усиления: восстановлен – 2, нет – 1
            IntB amplitude_recovery = 0;//метод восстановления усиления
            IntB measurement_system = 0;//един. измерения: 1 - метp, 2 – фут
            IntB impulse_polarity = 0;// полярность свип-сигнала
            IntB vibratory_polarity = 0;// код полярности сигнала вибратора
            IntB Unassigned[170];
        };
#pragma pack(push, 1)
        template
            <
            typename IntA,
            typename IntB
            >
            struct trace_header {
            public:
                IntA sequence_line = 0; //Номер трассы на профиле
                IntA sequence_file = 0; //Номер трассы на ленте
                IntA field_record = 0; //Физический номер полевой магнитограммы
                IntA traceno_orig = 0; //номер трассы в полевой МГ
                IntA energy_source_point = 0; //номер ПВ
                IntA ensemble = 0; //номер выборки ОГТ
                IntA traceno = 0; //номер трассы в выборке ОГТ
                IntB trace_id = 0; //код трассы
                IntB summed_traces = 0; //число накоплений
                IntB stacked_traces = 0; //кратность сумммирования.
                IntB data_use = 0; //хаpактеp данных: 1 - рабочие, 2 – тест
                IntA offset = 0; //расстояние ПВ-ПП
                IntA elevation_receiver = 0; //альтитуда ПП
                IntA elevation_source = 0; //альтитуда ПВ
                IntA depth_source = 0; // глубина ПВ от поверхности
                IntA datum_receiver = 0; //рельеф в ПП
                IntA datum_source = 0; //рельеф в ПВ
                IntA depth_water_source = 0; //уровень грунтовых вод в ПВ
                IntA depth_water_group = 0; //уровень грунтовых вод в ПП
                IntB elevation_scalar = 0; //масштабный коэф. для 41-68 байтов + - умножать, - - делить
                IntB coord_scalar = 0;//масштабный коэф. для 73-88 байтов
                IntA source_x = 0;//координаты ПВ: X Y
                IntA source_y = 0;
                IntA group_x = 0;//координаты ПП: X Y
                IntA group_y = 0;
                IntB coord_units = 0;//тип координат: 1 – длина (м,фут)2 – сферические (сек)
                IntB weathering_velocity = 0;//скорость в ЗМС
                IntB subweathering_velocity = 0;//скорость в коренных поpодах
                IntB uphole_source = 0;//верт. время в ПВ
                IntB uphole_group = 0;//верт. время в ПП
                IntB static_source = 0;//статическая поправка в ПВ
                IntB static_group = 0;//статическая поправка в ПП
                IntB static_total = 0;//суммарная статика, 0 - не вводилась
                IntB lag_a = 0;// время задержки A(мс) от конца
                IntB lag_b = 0;// время задержки B(мс) от момента
                IntB delay = 0;// задержка начала записи относительно момента взрыва
                IntB mute_start = 0;// начало мьютинга
                IntB mute_end = 0;//конец мьютинга (114 byte)
                IntB samples = 0;//число отсчетов в трассе
                IntB sample_interval = 0;//шаг дискретизации (мкс)
                IntB gain_type = 0;//формат полевой записи: 1 – целочисленный, 2 – бинарный,3 – плавающей запятой, 4 - резерв
                IntB gain_constant = 0;// коэф. усиления сейсмостанции
                IntB gain_initial = 0;//предварительное усиление в дб
                IntB correlated = 0;// корелограмма: 1 - нет, 2 – да
                IntB sweep_freq_start = 0;//начальная частота свип-сигнала
                IntB sweep_freq_end = 0;// конечная частота свип-сигнала
                IntB sweep_length = 0;//длина свип-сигнала (мс)
                IntB sweep_type = 0;//тип свип-сигнала
                IntB sweep_taperlen_start = 0;//длина конуса трассы свипа (мс)
                IntB sweep_taperlen_end = 0;//длина свипа на магнитной ленте
                IntB taper_type = 0;//тип конуса свипа
                IntB alias_filt_freq = 0;//частота фильтра зеркальных частот
                IntB alias_filt_slope = 0;//крутизна фильтра зеркальных частот
                IntB notch_filt_freq = 0;// частота фильтра-пробки
                IntB notch_filt_slope = 0;// крутизна фильтра-пробки
                IntB low_cut_freq = 0;// нижняя частота полосового фильтра
                IntB high_cut_freq = 0;//верхняя частота полосового фильтра
                IntB low_cut_slope = 0;//нижняя крутизна среза фильтра
                IntB high_cut_slope = 0;//155-156 верхняя крутизна среза фильтра
                IntB year = 0;// 157-158 год записи
                IntB day = 0;// 159 -160 день года
                IntB hour = 0;// 161-162 часы
                IntB min = 0;// 163 - 164 минуты
                IntB sec = 0;// 165 - 166 секунды
                IntB timecode = 0;//167 - 168 код времени: 1 - местное, 2 – Гринвич
                IntB weighting_factor = 0;//169 - 170 весовой коэф.трассы
                IntB geophone_group_roll1 = 0;//171 -172количество сейсмоприемников
                IntB geophone_group_first = 0;//173 - 174 номер начального приемника
                IntB geophone_group_last = 0;//175 - 176  номер конечного приемника
                IntB gap_size = 0;//177 - 178  размер промежутка
                IntB over_travel = 0;//179 - 180- перебег на конусность
                IntA cdp_x = 0;//181 - 184
                IntA cdp_y = 0;//185 - 188
                IntA iline = 0;//189 - 192 inline number
                IntA xline = 0;//193 - 196 crossline number
                IntA shot_point = 0;//197 - 200 shot point number
                IntB shot_point_scalar = 0;//201 - 202 shot point scalar
                IntB trace_unit = 0;//203 - 204 measurement unit
                IntB transduction_mantissa = 0;//205 - 208
                IntB transduction_exponent = 0;//209 - 210
                IntB transduction_unit = 0;//211 - 212
                IntB device_trace_ident = 0;//213 - 214
                IntB scale_time = 0;//215 - 216
                IntB source_type_orient = 0;//217 - 218
                char src_dir[6] = { 0 };//219-224
                IntA  source_energy_dir_exp = 0;//225 - 228
                IntB  source_measure_mant = 0;//229 - 230
                IntB  source_measure_exp = 0;
                IntB  source_measure_unit = 0;//231 - 232
                char unassigned[8];
        };
#pragma pack(pop)
        typedef binary_header<int, short int> binHeader;
        typedef trace_header<int, short int> traceHeader;

        typedef enum {
            SEGY_IBM_FLOAT_4_BYTE = 1,
            SEGY_SIGNED_INTEGER_4_BYTE = 2,
            SEGY_SIGNED_SHORT_2_BYTE = 3,
            SEGY_FIXED_POINT_WITH_GAIN_4_BYTE = 4, // Obsolete
            SEGY_IEEE_FLOAT_4_BYTE = 5,
            SEGY_IEEE_FLOAT_8_BYTE = 6,
            SEGY_SIGNED_CHAR_3_BYTE = 7,
            SEGY_SIGNED_INTEGER_3_BYTE = 7,
            SEGY_SIGNED_CHAR_1_BYTE = 8,
            SEGY_SIGNED_INTEGER_8_BYTE = 9,
            SEGY_UNSIGNED_INTEGER_4_BYTE = 10,
            SEGY_UNSIGNED_SHORT_2_BYTE = 11,
            SEGY_UNSIGNED_INTEGER_8_BYTE = 12,
            SEGY_UNSIGNED_INTEGER_3_BYTE = 15,
            SEGY_UNSIGNED_CHAR_1_BYTE = 16,
            SEGY_NOT_IN_USE_1 = 19,
            SEGY_NOT_IN_USE_2 = 20,
        } SEGY_FORMAT;
        enum { READ_ONLY = 1, READ_WRITE = 3 };


        static
        binHeader
        make_binaryHeader(int interval, int traces, int samples, short format)
        {
            binHeader bh;
            bh.interval = interval;
            bh.traces = traces;
            bh.samples = samples;
            bh.format = format;
            return (bh);
        }


    }//headers;

    template
    <
        short int mode
    >
    struct segy_
    {
        enum {
            INIT_SIZE_BUFFER_PARAM = headers::CHK_SEGY_TEXT_HEADER_SIZE + headers::CHK_SEGY_BINARY_HEADER_SIZE + headers::CHK_SEGY_TRACE_HEADER_SIZE,
          
        };

        FILE* f;
        std::vector< char > buffer;
        bool result;
        unsigned int currentContext;
        segy_() :f(NULL), result(true), buffer(INIT_SIZE_BUFFER_PARAM), currentContext(0)
        {}

        segy_(std::string fileName) :f(NULL), result(true), buffer(INIT_SIZE_BUFFER_PARAM), currentContext(0)
        {
            open(fileName);
        }

        ~segy_()
        {
            close();
        }

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

        void read(size_t sizeOfBuffer)
        {
            auto sz = fread(buffer.data(), 1, sizeOfBuffer, f);
            result = (sz == sizeOfBuffer);
        }
 
        void write(size_t sizeOfBuffer)
        {
            auto sz = fwrite(buffer.data(), 1, sizeOfBuffer, f);
            result = (sz == sizeOfBuffer);
        }


        void open(std::string fileName)
        {
            auto error = fopen_s(&f, fileName.c_str(), (mode == headers::READ_ONLY)?"rb":"rb+");
            if (0 != error)
            {
                auto error = fopen_s(&f, fileName.c_str(),"wb+");
                  if (0 != error)
                      throw (std::runtime_error("can't open segy file" + fileName));
            }
        }
        void create(std::string fileName)
        {

        }

        void close(void)
        {
            if (NULL != f)
                fclose(f);
        }

    };
    typedef segy_<headers::READ_ONLY> reader;
    typedef segy_<headers::READ_WRITE> writer;
}
#endif