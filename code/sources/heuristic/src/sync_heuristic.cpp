#include "sync_heuristic.hpp"

#include <cassert>
#include <limits>
#include <cmath>
#include <algorithm>
#include <stack>

#include <iterator>
#include <random>

#include <vector>
#include <utility>

#include "matrix.hpp"
#include "bitset.hpp"

#include <iostream>

using namespace std;

namespace SYNC_LIB
{
    sync_heuristic::sync_heuristic(const sync_heuristic_a_input &input)
        : input_(input),
          constructive_TSP_based_(input),
          reduce_fitness_(input)
    {
    }

    sync_heuristic::~sync_heuristic(void)
    {
    }

    bool sync_heuristic::build_feasible_solution(sync_solution &solution, ostream &log) const
    {
        bool is_feasible{false};

        log << "- Starting heuristic -" << endl;

        build_feasible_solution_b_(solution, log);
        build_feasible_solution_a_(solution, log);

        if (solution.is_feasible())
        {
            log << "Feasible solution found" << endl;
        }
        else
        {
            log << "No feasible solution found" << endl;
        }

        return is_feasible;
    }

    bool sync_heuristic::build_feasible_solution_a_(sync_solution &solution, ostream &log) const
    {

        vector<int> base_sequence;

        constructive_TSP_based_.build_base_sequence_nearest_neighbor(base_sequence);

        vector<int> prev_base_sequence(base_sequence);

        for (size_t i{0}; (i < n_max_iterations_); ++i)
        {
            sync_solution tw_feasible_solution;

            constructive_TSP_based_.build_tw_feasible_solution(base_sequence, tw_feasible_solution);

            if (!tw_feasible_solution.is_feasible())
            {
                sync_solution new_solution;
                reduce_fitness_.reduce_fitness(tw_feasible_solution, new_solution);

                if (new_solution.is_feasible())
                {
                    tw_feasible_solution = new_solution;
                }
            }

            if (!solution.is_feasible() && tw_feasible_solution.is_feasible())
            {
                solution = tw_feasible_solution;
                log << "TSP  Iteration:  " << setw(6) << i << " " << setw(6) << solution.get_cost() << endl;
            }
            else if ((tw_feasible_solution.is_feasible() && solution.is_feasible()) && (tw_feasible_solution.get_cost() < solution.get_cost()))
            {
                solution = tw_feasible_solution;
                log << "TSP  Iteration:  " << setw(6) << i << " " << setw(6) << solution.get_cost() << endl;
            }

            if (i % 500 == 0)
            {
                if (solution.is_feasible())
                    log << "---- Iteration:  " << setw(6) << i << " " << setw(6) << solution.get_cost() << endl;
                else
                    log << "---- Iteration:  " << setw(6) << i << " " << setw(6) << "INFEAS" << endl;
            }
        }

        return solution.is_feasible();
    }

    bool sync_heuristic::build_feasible_solution_b_(sync_solution &solution, ostream &log) const
    {

        vector<int> base_sequence;
        constructive_TSP_based_.build_base_sequence_cheapest_insertion(base_sequence);

        vector<int> prev_base_sequence(base_sequence);

        for (size_t i{0}; (i < n_max_iterations_); ++i)
        {
            sync_solution tw_feasible_solution;

            constructive_TSP_based_.build_tw_feasible_solution(base_sequence, tw_feasible_solution);

            if (!tw_feasible_solution.is_feasible())
            {
                sync_solution new_solution;
                reduce_fitness_.reduce_fitness(tw_feasible_solution, new_solution);

                if (new_solution.is_feasible())
                {
                    tw_feasible_solution = new_solution;
                }
            }

            if (!solution.is_feasible() && tw_feasible_solution.is_feasible())
            {
                solution = tw_feasible_solution;
                log << "TSP  Iteration:  " << setw(6) << i << " " << setw(6) << solution.get_cost() << endl;
            }
            else if ((tw_feasible_solution.is_feasible() && solution.is_feasible()) && (tw_feasible_solution.get_cost() < solution.get_cost()))
            {
                solution = tw_feasible_solution;
                log << "TSP  Iteration:  " << setw(6) << i << " " << setw(6) << solution.get_cost() << endl;
            }

            if (i % 500 == 0)
            {
                if (solution.is_feasible())
                    log << "---- Iteration:  " << setw(6) << i << " " << setw(6) << solution.get_cost() << endl;
                else
                    log << "---- Iteration:  " << setw(6) << i << " " << setw(6) << "INFEAS" << endl;
            }
        }

        return solution.is_feasible();
    }

}