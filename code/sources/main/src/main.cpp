#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "CTSP_instance.hpp"
#include "CTSP_2_heuristic_a_input.hpp"
#include "sync_heuristic.hpp"
#include "sync_solution.hpp"

#include "io_files.hpp"

using namespace std;

int ctsp2_heuristic(const SYNC_LIB::input_files &input_files,
                    SYNC_LIB::output_streams &output_streams)
{
    CTSP::instance I;

    I.read(input_files.ins_file);

    I.write_line(output_streams.stat_s);

    output_streams.stat_s << setw(15) << left << "CTSP_1" << '\t';
    output_streams.stat_s << setw(15) << left << "H CTSP1" << right << '\t';

    CTSP::CTSP_2_heuristic_a_input sync_heur_a_input(I);
    SYNC_LIB::sync_heuristic solver(sync_heur_a_input);

    SYNC_LIB::sync_solution solution;

    auto start = chrono::high_resolution_clock::now();
    solver.build_feasible_solution(solution, output_streams.log_s);
    auto end = chrono::high_resolution_clock::now();

    const double seconds{chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0};
    output_streams.stat_s << setw(10) << right << fixed << setprecision(4) << seconds << '\t';

    solution.write_statistics(output_streams.stat_s);

    if (solution.is_feasible())
    {
        solution.write(output_streams.sol_s);
    }

    return 0;
}

int main(int argc, char **argv)
{
    string input_file;

    if (argc == 1)
    {
        input_file = "../../instances/SubramanyamGounaris/burma14_p3_f70_lH.contsp";

        CTSP::instance I;

        I.read(input_file);

        // Heuristic
        CTSP::CTSP_2_heuristic_a_input sync_heur_a_input(I);
        SYNC_LIB::sync_heuristic initial_solution(sync_heur_a_input);

        // Measure time

        auto start = chrono::high_resolution_clock::now();

        SYNC_LIB::sync_solution solution;
        initial_solution.build_feasible_solution(solution, cout);

        auto end = chrono::high_resolution_clock::now();

        cout << "Time: " << setw(8) << fixed << setprecision(4) << chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0 << " seconds" << endl;

        // solution.write(cout);
        // cout << endl;
    }
    else if (argc == 8)
    {
        const string type(argv[1]);

        const string ins_file(argv[2]);
        const string sta_file(argv[3]);
        const string out_file(argv[4]);
        const string log_file(argv[5]);
        const string sol_file(argv[6]);

        SYNC_LIB::input_files input_files_instance(ins_file);

        SYNC_LIB::output_streams os_instance(sta_file, out_file, log_file, sol_file);

        if (type == "ctsp1")
        {
            ctsp2_heuristic(input_files_instance, os_instance);
        }
        else
        {
            cerr << "ERROR: Incorrect problem type" << endl;
            exit(1);
        }
    }

    return 0;
}
