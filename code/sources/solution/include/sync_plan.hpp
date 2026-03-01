#pragma once

#include "sync_heuristic_a_input.hpp"

#include <vector>
#include <iostream>
#include <utility>

#include "bitset.hpp"

using namespace std;

namespace SYNC_LIB
{
    typedef int customer_id;

    class plan_data
    {
    public:
        vector<vector<int>> routes_;                // Sequence of customers for each vehicle
        vector<vector<double>> time_;               // Time to travel between customers for each vehicle
        vector<vector<double>> starting_time_;      // Service time for each customer and vehicle

        vector<pair<double, double>> time_windows_; // Time window for each customer
        vector<double> slack_;                      // Slack time for each customer
        vector<double> average_starting_time_;      // Average starting time for each customer  

        vector<double> violations_;                 // Violations of time windows for each customer

    public:
        plan_data(const sync_heuristic_a_input &instance);
        virtual ~plan_data(void);
    };

    class sync_plan
    {
    private:
        const sync_heuristic_a_input &input_; 

        size_t n_depots_;
        size_t n_customers_;
        double tol_{1E-3};

        plan_data plan_;

    public:
        sync_plan(const sync_heuristic_a_input &input);
        virtual ~sync_plan(void);

        double get_makespan(vector<double> &routes_time) const;
        double get_makespan(void) const;

        bool set_routes(const vector<vector<int>> &routes);

        inline const vector<vector<double>> &get_starting_time(void) const { return plan_.starting_time_; }
        inline const vector<vector<double>> &get_time(void) const { return plan_.time_; }
        inline const vector<double> &get_slack(void) const { return plan_.slack_; }
        inline const vector<pair<double, double>> &get_time_windows(void) const { return plan_.time_windows_; }
        inline const vector<vector<int>> &get_routes(void) const { return plan_.routes_; }

        inline bool is_feasible(void) const { return is_feasible(plan_); }

        ostream &write(const sync_heuristic_a_input &input, ostream &os, const plan_data &plan) const;

    private:
        void init_sequence_time_(const sync_heuristic_a_input &input, plan_data &plan);
        void init_starting_time_(plan_data &plan);
        void update_time_windows_(const sync_heuristic_a_input &input, plan_data &plan);
        bool update_starting_time_(plan_data &plan);
        void update_violation_in_time_windows_(const sync_heuristic_a_input &input, plan_data &plan);
        void update_violated_customers_(const sync_heuristic_a_input &input, plan_data &plan);
        void update_slack_(const sync_heuristic_a_input &input, plan_data &plan);
        size_t get_latest_depot_(void) const;
        void insert_routes_(const vector<vector<int>> &routes, plan_data &plan);
        bool update_plan_(const sync_heuristic_a_input &input, plan_data &plan);

        bool is_feasible(const plan_data &plan) const;

        void starting_(const sync_heuristic_a_input &input, plan_data &plan, bool &tw_infeasible);
        void update_parameters_(const sync_heuristic_a_input &input, plan_data &plan, bool &cycle);

        void write_route_(ostream &os, const vector<int> &route) const;
    };
}