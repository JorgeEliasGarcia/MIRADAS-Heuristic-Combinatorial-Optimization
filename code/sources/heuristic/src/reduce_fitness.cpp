#include "reduce_fitness.hpp"

#include <iostream>
#include <limits>
#include <iomanip>
#include <cmath>
#include <stack>
#include <algorithm>

namespace SYNC_LIB
{
    bool compare_(const pair<pair<double, double>, size_t> &a, const pair<pair<double, double>, size_t> &b)
    {
        if (fabs(a.first.first - b.first.first) < 1E-3)
        {
            return a.first.second < b.first.second;
        }

        return a.first.first < b.first.first;
    }

    local_search_reduce_fitness::local_search_reduce_fitness(const sync_heuristic_a_input &input)
        : input_(input),
          time_(input.get_time()),
          matrix_ptr_(time_.get_matrix_ptr()),
          n_col_(time_.get_n_cols()),
          n_depots_(input.get_n_depots()),
          n_locations_(input.get_n_locations())
    {
    }

    local_search_reduce_fitness::~local_search_reduce_fitness(void)
    {
    }

    bool local_search_reduce_fitness::reduce_fitness(const sync_solution &initial_solution, sync_solution &solution) const
    {
        reduce_fitness_(initial_solution, solution);

        return solution.is_feasible();
    }

    bool local_search_reduce_fitness::update_(const sync_plan &plan, sync_solution &solution) const
    {
        vector<double> new_routes_time;
        const double new_makespan{plan.get_makespan(new_routes_time)};

        const vector<vector<int>> &new_routes{plan.get_routes()};

        const double new_cost{input_.get_cost(new_routes)};

        const double ref_makespan{input_.get_max_distance()};

        bool is_feasible{new_makespan < ref_makespan + precision_};

        solution.set(new_routes, new_cost, new_routes_time, is_feasible);

        return is_feasible;
    }

    void local_search_reduce_fitness::find_candidates_(const sync_plan &plan, vector<int> &candidates) const
    {
        const vector<double> &slack{plan.get_slack()};
        const size_t n_customers{slack.size()};

        for (size_t i{1}; i < n_customers; ++i)
        {
            if (slack[i] < -precision_)
            {
                candidates.push_back(i);
            }
        }

        // cout << "Number of candidates: " << candidates.size() << endl;
    }

    void local_search_reduce_fitness::locate_candidates_(const sync_plan &plan, const vector<int> &candidates, vector<pair<int, int>> &lower_customer, vector<pair<int, int>> &upper_customer) const
    {
        const size_t n_candidates{candidates.size()};
        const vector<pair<double, double>> &time_windows{plan.get_time_windows()};

        const size_t n_depots{input_.get_n_depots()};
        const vector<vector<double>> starting_time{plan.get_starting_time()};
        vector<vector<int>> routes{plan.get_routes()};

        for (size_t i{0}; i < n_candidates; ++i)
        {
            const int c_customer{candidates[i]};
            const pair<double, double> &c_time_window{time_windows[c_customer]};

            const double lower{c_time_window.first};
            const double upper{c_time_window.second};

            int lower_depot{-1};
            int upper_depot{-1};

            for (size_t j{0}; j < n_depots; ++j)
            {
                const double &c_starting_time{starting_time[c_customer][j]};

                if (c_starting_time > 0)
                {

                    if (fabs(c_starting_time - lower) < precision_)
                    {
                        lower_depot = j;
                    }

                    if (fabs(c_starting_time - upper) < precision_)
                    {
                        upper_depot = j;
                    }
                }
            }

            // Find the position within the route
            {
                const vector<int> &c_route{routes[lower_depot]};

                const size_t n_c_customers{c_route.size()};

                int lower_position{-1};

                for (size_t j{0}; j < n_c_customers; ++j)
                {
                    if (c_route[j] == c_customer)
                    {
                        lower_position = j - 1;
                        break;
                    }
                }

                lower_customer[i] = make_pair(lower_depot, lower_position);
            }

            {
                const vector<int> &c_route{routes[upper_depot]};

                const size_t n_c_customers{c_route.size()};

                int upper_position{-1};

                for (size_t j{0}; j < n_c_customers; ++j)
                {
                    if (c_route[j] == c_customer)
                    {
                        upper_position = j - 1;
                        break;
                    }
                }

                upper_customer[i] = make_pair(upper_depot, upper_position);
            }
        }
    }

    void local_search_reduce_fitness::get_quality_measures_(const sync_plan &plan, double &makespan, double &avg_routes_time) const
    {
        const size_t n_depots{input_.get_n_depots()};

        vector<double> routes_time(n_depots);

        makespan = plan.get_makespan(routes_time);

        avg_routes_time = 0.0;

        for (size_t i{0}; i < n_depots; ++i)
        {
            avg_routes_time += routes_time[i];
        }

        avg_routes_time /= n_depots;
    }

    void local_search_reduce_fitness::evaluate_swap_(const vector<vector<int>> &routes, const size_t c_vehicle, const size_t i, const size_t j, vector<int> &c_route, bool &feasible) const
    {
        c_route = routes[c_vehicle];

        swap(c_route[i], c_route[j]);

        vector<vector<int>> new_routes{routes};
        new_routes[c_vehicle] = c_route;

        sync_plan t_plan(input_);
        t_plan.set_routes(new_routes);

        feasible = t_plan.is_feasible();
    }


    void local_search_reduce_fitness::evaluate_candidate_up_(const vector<vector<int>> &routes, const pair<int, int> &customer, const size_t pos, vector<int> &c_route, bool &feasible) const
    {
        const size_t c_depot{(size_t)customer.first};
        const size_t c_position{(size_t)customer.second};

        if (c_position > 1)
        {
            evaluate_swap_(routes, c_depot, c_position, c_position - pos, c_route, feasible);
        }

        // cout << "Makespan: " << setw(7) << fixed << setprecision(0) << c_makespan << endl;
        // cout << "Average : " << setw(7) << fixed << setprecision(2) << c_avg_routing_time << endl;
        // cout << endl;
    }

    void local_search_reduce_fitness::evaluate_candidate_down_(const vector<vector<int>> &routes, const pair<int, int> &customer, const size_t pos, vector<int> &c_route, bool &feasible) const
    {
        const size_t c_depot{(size_t)customer.first};
        const size_t c_position{(size_t)customer.second};

        const size_t n_customers{routes[c_depot].size()};

        if ((n_customers > pos) && (c_position < (n_customers - (pos + 1))))
        {
            evaluate_swap_(routes, c_depot, c_position, c_position + pos, c_route, feasible);
        }
        

        // cout << "Makespan: " << setw(7) << fixed << setprecision(0) << c_makespan << endl;
        // cout << "Average : " << setw(7) << fixed << setprecision(2) << c_avg_routing_time << endl;
        // cout << endl;
    }

    bool local_search_reduce_fitness::select_move_(const vector<vector<int>> &routes, const vector<pair<int, int>> &lower_customer, const vector<pair<int, int>> &upper_customer, vector<vector<int>> &best_route, vector<size_t> &best_vehicle) const
    {
        const size_t n_candidates{lower_customer.size()};

        vector<pair<vector<int>, size_t>> new_routes(2 * n_candidates);
        vector<pair<bool, size_t>> quality_measures(2 * n_candidates);

        for (size_t i{0}; i < n_candidates; i++)
        {
            const pair<int, int> &c_upper_customer{upper_customer[i]};
            const pair<int, int> &c_lower_customer{lower_customer[i]};

            pair<bool, size_t> &c_quality_measure_upper{quality_measures[2 * i]};
            pair<bool, size_t> &c_quality_measure_lower{quality_measures[2 * i + 1]};

            bool &c_upper_quality{c_quality_measure_upper.first};
            bool &c_lower_quality{c_quality_measure_lower.first};

            c_quality_measure_upper.second = 2 * i;
            c_quality_measure_lower.second = 2 * i + 1;

            pair<vector<int>, size_t> &c_upper_route{new_routes[2 * i]};
            pair<vector<int>, size_t> &c_lower_route{new_routes[2 * i + 1]};

            c_upper_route.second = c_upper_customer.first;
            c_lower_route.second = c_lower_customer.first;

            evaluate_candidate_up_(routes, c_upper_customer, 1, c_upper_route.first, c_upper_quality);
            evaluate_candidate_down_(routes, c_lower_customer, 1, c_lower_route.first, c_lower_quality);
        }

        bool improvement{false};

        for (size_t candidate{0}; (candidate < 2 * n_candidates); ++candidate)
        {
            const size_t candidate_pos{quality_measures[candidate].second};
            const size_t candidate_sz{new_routes[candidate_pos].first.size()};

            if (candidate_sz > 0)
            {
                const bool feasible{quality_measures[candidate].first};

                if (feasible)
                {
                    const size_t best_inx{quality_measures[candidate].second};

                    best_route.push_back(new_routes[best_inx].first);
                    best_vehicle.push_back(new_routes[best_inx].second);

                    improvement = true;
                }
            }
        }

        return improvement;
    }

    bool local_search_reduce_fitness::reduce_fitness_(const sync_solution &initial_solution, sync_solution &solution) const
    {
        bool global_improvement{false};
        bool improvement{false};

        vector<vector<int>> routes{initial_solution.get_routes()};

        sync_plan plan(input_);
        plan.set_routes(routes);

        // Remove depot from new_routes

        for (vector<int> &c_route : routes)
        {
            if (c_route.front() == 0)
            {
                c_route.erase(c_route.begin());
                c_route.pop_back();
            }
        }

        vector<vector<int>> feasible_routes;

        stack<vector<vector<int>>> new_routes_stack;

        new_routes_stack.push(routes);

        while (!new_routes_stack.empty())
        {
            vector<vector<int>> t_routes{new_routes_stack.top()};
            new_routes_stack.pop();

            plan.set_routes(t_routes);

            improvement = false;

            vector<int> candidates;

            find_candidates_(plan, candidates);

            const size_t n_candidates{candidates.size()};

            if (n_candidates > 0)
            {
                vector<pair<int, int>> lower_customer(n_candidates);
                vector<pair<int, int>> upper_customer(n_candidates);

                locate_candidates_(plan, candidates, lower_customer, upper_customer);

                vector<vector<int>> best_route;
                vector<size_t> best_vehicle;

                improvement = select_move_(t_routes, lower_customer, upper_customer, best_route, best_vehicle);

                if (improvement)
                {
                    const size_t n_best_route{best_route.size()};

                    for (size_t i{0}; i < n_best_route; ++i)
                    {
                        const size_t c_vehicle{best_vehicle[i]};
                        const vector<int> c_best_route{best_route[i]};

                        vector<vector<int>> c_routes{t_routes};
                        c_routes[c_vehicle] = c_best_route;

                        const bool feasible{plan.set_routes(c_routes)};

                        if (feasible)
                        {
                           feasible_routes = c_routes;
                           global_improvement = true;
                           break;
                        }
                    }
                }
            }
        }

        if (global_improvement)
        {
            solution.set(input_, feasible_routes);           
        }

        return global_improvement;
    }

}