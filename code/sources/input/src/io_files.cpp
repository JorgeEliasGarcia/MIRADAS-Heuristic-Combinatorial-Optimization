#include "io_files.hpp"

namespace SYNC_LIB
{
    input_files::input_files(const string &_ins_file) : ins_file(_ins_file)
    {
    }

    input_files::input_files(const string &_ins_file, const string &_fea_file) : ins_file(_ins_file), fea_file(_fea_file)
    {
    }

    input_files::~input_files()
    {
    }

    output_streams::output_streams(const string &sta_file, const string &log_file, const string &out_file, const string &sol_file) : stat_s(sta_file, std::ios_base::app), out_s(out_file), log_s(log_file), sol_s(sol_file)
    {
    }

    output_streams::~output_streams()
    {

        stat_s << endl;
        stat_s.close();

        out_s << endl;
        out_s.close();

        log_s << endl;
        log_s.close();

        sol_s << endl;
        sol_s.close();
    }
}
