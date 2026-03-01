#pragma once

#include <vector>

#include "sync_heuristic_a_input.hpp"
#include "sync_solution.hpp"
#include "bitset.hpp"

namespace SYNC_LIB
{
    class local_search_reduce_fitness
    {
    private:
        const double precision_{1E-3};
        const sync_heuristic_a_input &input_;

        const GOMA::matrix<double> &time_;
        double const *matrix_ptr_;
        const size_t n_col_;

        const size_t n_depots_;
        const size_t n_locations_;

    public:
        local_search_reduce_fitness(const sync_heuristic_a_input &input);
        virtual ~local_search_reduce_fitness(void);

        bool reduce_fitness(const sync_solution &initial_solution, sync_solution &solution) const;

    protected:
    bool reduce_fitness_(const sync_solution &initial_solution, sync_solution &solution) const;
    void find_candidates_(const sync_plan &plan, vector<int> &candidates) const;
    void locate_candidates_(const sync_plan &plan, const vector<int> &candidates, vector<pair<int, int>> &lower_customer, vector<pair<int, int>> &upper_customer) const;
    bool select_move_(const vector<vector<int>> &routes, const vector<pair<int, int>> &lower_customer, const vector<pair<int, int>> &upper_customer, vector<vector<int>> &best_route, vector<size_t> &best_vehicle) const;
    void get_quality_measures_(const sync_plan &plan, double &makespan, double &avg_routes_time) const;
    void evaluate_swap_(const vector<vector<int>> &routes, const size_t c_vehicle, const size_t i, const size_t j, vector<int> &c_route, bool &feasible) const;
    void evaluate_candidate_up_(const vector<vector<int>> &routes, const pair<int, int> &customer, const size_t pos, vector<int> &c_route, bool &feasible) const;
    void evaluate_candidate_down_(const vector<vector<int>> &routes, const pair<int, int> &customer, const size_t pos, vector<int> &c_route, bool &feasible) const;

    bool update_(const sync_plan &plan, sync_solution &solution) const;

    };
}