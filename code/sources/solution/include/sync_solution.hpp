#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <utility>

#include "sync_heuristic_a_input.hpp"
#include "sync_plan.hpp"


using namespace std;

namespace SYNC_LIB
{
    class sync_solution
    {
    private:
        string instance_name_;
        vector<vector<int>> routes_;

        double cost_;
        vector<double> route_times_;
        vector<double> route_costs_;
        double makespan_;
        bool feasible_;
    public:
        sync_solution(const string &instance_name);
        sync_solution(void);
        virtual ~sync_solution(void);

        void init(void);

        void set(const vector<vector<int>> &routes, const double cost, const vector<double> &route_times, const bool feasible);
        bool set(const sync_heuristic_a_input &input, const vector<vector<int>> &routes);

        void read(istream &is);
        void write(ostream &os) const;

        bool empty(void) const;

        size_t get_latest_route(void) const;
        size_t get_longest_route(void) const;

        void get_arcs(const size_t depot, vector<pair<int,int>> &arcs) const;

        void write_statistics(ostream &os) const;

        inline const vector<vector<int>> &get_routes(void) const
        {
            return routes_;
        }

        inline double get_cost(void) const
        {
            return cost_;
        }

        inline const vector<double> &get_route_times(void) const
        {
            return route_times_;
        }

        inline double get_makespan(void) const
        {
            return makespan_;
        }

        inline bool is_feasible(void) const
        {
            return feasible_;
        }

        inline size_t get_num_routes(void) const
        {
            return routes_.size();
        }

    };
}