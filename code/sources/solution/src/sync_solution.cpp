#include "sync_solution.hpp"

#include <iostream>
#include <iomanip>
#include <limits>

namespace SYNC_LIB
{
    sync_solution::sync_solution(const string &instance_name) : instance_name_(instance_name), cost_(numeric_limits<double>::max()), makespan_(numeric_limits<double>::max()), feasible_(false)
    {
    }

    sync_solution::sync_solution(void) : instance_name_(""), cost_(numeric_limits<double>::max()), makespan_(numeric_limits<double>::max()), feasible_(false)
    {
    }

    sync_solution::~sync_solution(void)
    {
    }

    void sync_solution::init(void)
    {
        routes_.clear();
    }

    bool sync_solution::empty(void) const
    {
        return routes_.empty();
    }

    void sync_solution::write_statistics(ostream &os) const
    {
        if (cost_ < 1E10)
            os << setw(10) << right << fixed << setprecision(3) << cost_ << '\t';
        else
            os << setw(10) << right << "INF" << '\t';

        if (makespan_ < 1E10)
            os << setw(10) << right << fixed << setprecision(3) << makespan_ << '\t';
        else
            os << setw(10) << right << "INF" << '\t';

        os << setw(5) << right << feasible_ << '\t';
    }

    bool sync_solution::set(const sync_heuristic_a_input &input, const vector<vector<int>> &routes)
    {
        vector<vector<int>> new_routes(routes);

        const size_t n_routes{new_routes.size()};

        for (size_t i = 0; i < n_routes; ++i)
        {
            vector<int> &c_route{new_routes[i]};

            if (c_route[0] == 0)
            {
                c_route.erase(c_route.begin());
            }

            if (c_route.back() == 0)
            {
                c_route.pop_back();
            }
        }

        routes_ = new_routes;

        cost_ = input.get_cost(routes);

        route_costs_.resize(n_routes);

        for (size_t i = 0; i < n_routes; ++i)
        {
            route_costs_[i] = input.get_cost(routes[i]);
        }

        sync_plan plan(input);

        const bool tw_feasible{plan.set_routes(routes)};

        if (!tw_feasible)
        {
            feasible_ = false;
           
            return false;
        }
        else
        {
            vector<double> new_routes_time;
            const double new_makespan{plan.get_makespan(new_routes_time)};

            route_times_ = new_routes_time;
            makespan_ = new_makespan;

            feasible_ = true;
            return true;
        }

        
    }

    void sync_solution::set(const vector<vector<int>> &routes, const double cost, const vector<double> &route_times, const bool feasible)
    {
        vector<vector<int>> new_routes(routes);

        const size_t n_routes{new_routes.size()};

        for (size_t i = 0; i < n_routes; ++i)
        {
            vector<int> &c_route{new_routes[i]};

            if (c_route[0] == 0)
            {
                c_route.erase(c_route.begin());
            }

            if (c_route.back() == 0)
            {
                c_route.pop_back();
            }
        }

        routes_ = new_routes;
        cost_ = cost;
        route_times_ = route_times;

        double makespan{0.0};

        for (size_t i = 0; i < route_times_.size(); ++i)
        {
            if (route_times_[i] > makespan)
            {
                makespan = route_times_[i];
            }
        }

        makespan_ = makespan;

        feasible_ = feasible;
    }

    void sync_solution::read(istream &is)
    {
        size_t n_routes;
        is >> n_routes;

        routes_.resize(n_routes);

        for (size_t i = 0; i < n_routes; ++i)
        {
            size_t n_nodes;
            is >> n_nodes;

            routes_[i].resize(n_nodes);

            for (size_t j = 0; j < n_nodes; ++j)
            {
                is >> routes_[i][j];
            }
        }
    }

    void sync_solution::write(ostream &os) const
    {
        os << setw(4) << routes_.size() << endl;
        os << endl;

        for (size_t i = 0; i < routes_.size(); ++i)
        {
            os << setw(4) << routes_[i].size() << endl;
            os << endl;

            os << setw(4) << 1 << " ";
            for (size_t j = 0; j < routes_[i].size(); ++j)
            {
                os << setw(4) << routes_[i][j] << " ";
            }
            os << setw(4) << 1 << endl;

            os << endl;
        }

        os << setw(5) << cost_ << endl;
        os << setw(5) << makespan_ << endl;
    }

    void sync_solution::get_arcs(const size_t depot, vector<pair<int, int>> &arcs) const
    {
        arcs.clear();

        const vector<int> &route{routes_[depot]};

        for (size_t i = 0; i < route.size() - 1; ++i)
        {
            arcs.push_back(make_pair(route[i], route[i + 1]));
        }
    }

    size_t sync_solution::get_latest_route(void) const
    {
        size_t latest_route{0};
        double latest_time{0.0};

        for (size_t i = 0; i < route_times_.size(); ++i)
        {
            if (route_times_[i] > latest_time)
            {
                latest_time = route_times_[i];
                latest_route = i;
            }
        }

        return latest_route;
    }

    size_t sync_solution::get_longest_route(void) const
    {
        size_t longest_route{0};
        double longest_cost{0.0};

        for (size_t i = 0; i < route_costs_.size(); ++i)
        {
            if (route_costs_[i] > longest_cost)
            {
                longest_cost = route_times_[i];
                longest_route = i;
            }
        }

        return longest_route;
    }
}