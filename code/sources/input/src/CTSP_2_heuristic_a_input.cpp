#include "CTSP_2_heuristic_a_input.hpp"

namespace CTSP
{
    CTSP_2_heuristic_a_input::CTSP_2_heuristic_a_input(const CTSP::instance &ctsp_instance) : SYNC_LIB::sync_heuristic_a_input()
    {
        const string id{ctsp_instance.get_instance_name()};
        const string type{ctsp_instance.get_instance_type()};
        const string comment{ctsp_instance.get_instance_comment()};

        const size_t n_locations{ctsp_instance.get_n_customers() + 1};
        const size_t n_vehicles{1};
        const size_t n_depots{ctsp_instance.get_K()};

        const GOMA::matrix<double> &time{ctsp_instance.get_distances()};
        const GOMA::matrix<double> &cost{ctsp_instance.get_distances()};

        const vector<vector<int>> &demands{ctsp_instance.get_demands()};

        const vector<double> q_demand(n_locations, 0.0);
        const vector<double> service_time(n_locations, 0.0);

        vector<double> max_diff_time(n_locations, ctsp_instance.get_T()[0]);
        const double max_distance{1E100};

        max_diff_time[0] = max_distance;

        const vector<double> optimal_values{ctsp_instance.get_optimal_values()};

        set_id(id);
        set_comment(comment);
        set_type(type);
        set_n_locations(n_locations);
        set_n_vehicles(n_vehicles);
        set_n_depots(n_depots);
        set_time(time);
        set_cost(cost);
        set_demand(demands);
        set_q_demand(q_demand);
        set_max_diff_time(max_diff_time);
        set_max_distance(max_distance);
        set_optimal_values(optimal_values);
    }

    CTSP_2_heuristic_a_input::~CTSP_2_heuristic_a_input()
    {
    }
}