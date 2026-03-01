#pragma once

#include <vector>
#include "matrix.hpp"

using namespace std;

namespace SYNC_LIB
{
    class sync_heuristic_a_input
    {
    private:
        string id_;
        string comment_;
        string type_;

        size_t n_locations_;
        size_t n_customer_operations_;

        size_t n_vehicles_; // Number of vehicles per route
        size_t n_depots_;   // Number of routes

        GOMA::matrix<double> time_;
        GOMA::matrix<double> cost_;

        vector<vector<int>> demands_;
        vector<vector<size_t>> customer_depots_;
        vector<vector<int>> depot_customers_;

        vector<double> q_demand_;

        vector<double> max_diff_time_;
        double max_distance_;

        vector<double> optimal_values_;

    public:
        sync_heuristic_a_input(void);
        virtual ~sync_heuristic_a_input(void);

        ostream &write_line(ostream &os) const;

        inline const string &get_id(void) const { return id_; }
        inline const string &get_comment(void) const { return comment_; }
        inline const string &get_type(void) const { return type_; }
        inline size_t get_n_locations(void) const { return n_locations_; }
        inline size_t get_n_customers(void) const { return n_locations_ - 1; }
        inline size_t get_n_vehicles(void) const { return n_vehicles_; }
        inline size_t get_n_depots(void) const { return n_depots_; }
        inline size_t get_num_days(void) const { return n_depots_; }
        inline const GOMA::matrix<double> &get_time(void) const { return time_; }
        inline const GOMA::matrix<double> &get_cost(void) const { return cost_; }
        inline const vector<vector<int>> &get_demands(void) const { return demands_; }
        inline const vector<double> &get_q_demand(void) const { return q_demand_; }
        inline const vector<double> &get_max_diff_time(void) const { return max_diff_time_; }
        inline double get_max_allowed_differencial(void) const { return max_diff_time_[1]; }
        inline double get_max_distance(void) const { return max_distance_; }
        inline const vector<double> &get_optimal_values(void) const { return optimal_values_; }
        inline size_t get_n_customer_operations(void) const { return n_customer_operations_; }
        inline const vector<vector<size_t>> &get_customer_depots(void) const { return customer_depots_; }
        inline const vector<vector<int>> &get_depot_customers(void) const { return depot_customers_; }

        void set_id(const string &id);
        void set_comment(const string &comment);
        void set_type(const string &type);
        void set_n_locations(const size_t &n_locations);
        void set_n_vehicles(const size_t &n_vehicles);
        void set_n_depots(const size_t &n_routes);
        void set_time(const GOMA::matrix<double> &time);
        void set_cost(const GOMA::matrix<double> &cost);
        void set_q_demand(const vector<double> &q_demand);
        void set_demand(const vector<vector<int>> &demands);

        void set_max_diff_time(const vector<double> &max_diff_time);
        void set_max_distance(const double &max_distance);
        void set_optimal_values(const vector<double> &optimal_values);

        double get_cost(const vector<int> &route) const;
        double get_cost(const vector<vector<int>> &routes) const;
    };
}