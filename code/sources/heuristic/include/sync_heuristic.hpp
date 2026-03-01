#pragma once

#include "sync_heuristic_a_input.hpp"
#include "sync_solution.hpp"
#include "sync_plan.hpp"
#include "constructive_TSP_based.hpp"
#include "reduce_fitness.hpp"

#include <vector>
#include <iostream>

using namespace std;

namespace SYNC_LIB
{
    class sync_heuristic
    {
    protected:
        const double precision_{1E-3};
        const sync_heuristic_a_input &input_;
        const size_t n_max_iterations_{650};

        constructive_TSP_based constructive_TSP_based_;
        local_search_reduce_fitness reduce_fitness_;

    public:
        sync_heuristic(const sync_heuristic_a_input &input);
        virtual ~sync_heuristic(void);

        bool build_feasible_solution(sync_solution &solution, ostream& log) const;

        private:
        bool build_feasible_solution_a_(sync_solution &solution, ostream& log) const;
        bool build_feasible_solution_b_(sync_solution &solution, ostream& log) const;
    };

}