#pragma once

#include <string>
#include <fstream>

using namespace std;

namespace SYNC_LIB
{
    class input_files
    {
    public:
        const string ins_file;
        const string fea_file;

        input_files(const string &_ins_file);
        input_files(const string &_ins_file, const string &_fea_file);
        ~input_files();
    };

    class output_streams
    {
    public:
        ofstream stat_s;
        ofstream out_s;
        ofstream log_s;
        ofstream sol_s;

        output_streams(const string &sta_file, const string &log_file, const string &out_file, const string &sol_file);
        ~output_streams();
    };
}
