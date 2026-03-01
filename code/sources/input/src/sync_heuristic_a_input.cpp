#include "sync_heuristic_a_input.hpp"

namespace SYNC_LIB
{

    sync_heuristic_a_input::sync_heuristic_a_input(void) : max_distance_(0.0) {}

    sync_heuristic_a_input::~sync_heuristic_a_input(void) {}

    ostream &sync_heuristic_a_input::write_line(ostream &os) const
    {
        os << setw(20) << left << id_ << '\t';
        os << setw(5) << right << get_max_allowed_differencial() << '\t';
        os << setw(5) << get_max_distance() << '\t';
        os << setw(5) << get_num_days() << '\t';
        os << setw(5) << get_n_locations() << '\t';
        os << setw(5) << get_n_customer_operations() << '\t';
        os << setw(9) << fixed << setprecision(1) << optimal_values_[0] << '\t';
        os << setw(9) << fixed << setprecision(1) << optimal_values_[1] << '\t';
        return os;
    }

    void sync_heuristic_a_input::set_id(const string &id)
    {
        id_ = id;
    }

    void sync_heuristic_a_input::set_comment(const string &comment)
    {
        comment_ = comment;
    }

    void sync_heuristic_a_input::set_type(const string &type)
    {
        type_ = type;
    }

    void sync_heuristic_a_input::set_n_locations(const size_t &n_locations)
    {
        n_locations_ = n_locations;
    }

    void sync_heuristic_a_input::set_n_vehicles(const size_t &n_vehicles)
    {
        n_vehicles_ = n_vehicles;
    }

    void sync_heuristic_a_input::set_n_depots(const size_t &n_depots)
    {
        n_depots_ = n_depots;
    }

    void sync_heuristic_a_input::set_time(const GOMA::matrix<double> &time)
    {
        time_ = time;
    }

    void sync_heuristic_a_input::set_cost(const GOMA::matrix<double> &cost)
    {
        cost_ = cost;
    }

    void sync_heuristic_a_input::set_demand(const vector<vector<int>> &demands)
    {
        demands_ = demands;

        const size_t n_locations{demands.size()};

        customer_depots_.resize(n_locations);
        depot_customers_.resize(n_depots_);

        n_customer_operations_ = 0;

        for (size_t i{1}; i < n_locations; i++)
        {
            const vector<int> &demand_i{demands[i]};

            for (size_t j{0}; j < demand_i.size(); j++)
            {
                if (demand_i[j] > 0)
                {
                    n_customer_operations_++;
                    customer_depots_[i].push_back(j);
                    depot_customers_[j].push_back(i);
                }
            }
        }
    }

    void sync_heuristic_a_input::set_q_demand(const vector<double> &demand)
    {
        q_demand_ = demand;
    }

    void sync_heuristic_a_input::set_max_diff_time(const vector<double> &max_diff_time)
    {
        max_diff_time_ = max_diff_time;
    }

    void sync_heuristic_a_input::set_max_distance(const double &max_distance)
    {
        max_distance_ = max_distance;
    }

    void sync_heuristic_a_input::set_optimal_values(const vector<double> &optimal_values)
    {
        optimal_values_ = optimal_values;
    }

    double sync_heuristic_a_input::get_cost(const vector<int> &route) const
    {
        const size_t n{route.size()};

        double cost{0.0};

        const bool is_depot{route[0] == 0};

        if (!is_depot)
        {
            cost += cost_(0 + 1, route[0] + 1);
            cost += cost_(route[n - 1] + 1, 0 + 1);
        }
        

        for (size_t i = 0; i < n - 1; ++i)
        {
            cost += cost_(route[i] + 1, route[i + 1] + 1);
        }

        return cost;
    }

    double sync_heuristic_a_input::get_cost(const vector<vector<int>> &routes) const
    {
        const size_t n_routes{routes.size()};
        double cost{0.0};

        for (size_t i = 0; i < n_routes; ++i)
        {
            cost += get_cost(routes[i]);
        }

        return cost;
    }

}