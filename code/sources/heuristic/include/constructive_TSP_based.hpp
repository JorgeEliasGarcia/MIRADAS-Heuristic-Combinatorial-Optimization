#pragma once

#include <vector>
#include "sync_heuristic_a_input.hpp"
#include "sync_solution.hpp"
#include "matrix.hpp"

using namespace std;

namespace SYNC_LIB
{
    class constructive_TSP_based
    {
    private:
        const double precision_{1E-3};
        const sync_heuristic_a_input &input_;

        const GOMA::matrix<double> &time_;
        double const *matrix_ptr_;
        const size_t n_col_;

        vector<int> base_sequence_;
        size_t n_max_iterations_{100};
        size_t it_;

    public:
        constructive_TSP_based(const sync_heuristic_a_input &input);
        virtual ~constructive_TSP_based(void);

        void build_base_sequence_nearest_neighbor(vector<int> &base_sequence) const;
        void build_base_sequence_cheapest_insertion(vector<int> &base_sequence) const;

        bool build_tw_feasible_solution(vector<int> &base_sequence, sync_solution &tw_feasible_solution) const;

    protected:
        void constructive_TSP_nearest_neighbor_(const vector<int> &items, vector<int> &route, double &route_cost) const;
        void constructive_TSP_cheapest_insertion_(const vector<int> &items, vector<int> &route, double &route_cost) const;
        void build_sorting_map_(vector<int> &route, vector<int> &map) const;
        void compute_TSP_LS_(vector<int> &c_route) const;
        void two_opt_(vector<int> &route, double &route_cost) const;
        void three_opt_(vector<int> &route, double &route_cost) const;
        void make_perturbation_(vector<int> &route) const;
        void sort_routes_(const vector<int> &map, vector<vector<int>> &routes) const;
        void get_depot_items_(const vector<vector<int>> &demands, vector<vector<int>> &depot_items) const;
        void sort_(const vector<int> &map, vector<int> &items) const;

        double get_time_(const vector<int> &route) const;
    };
}