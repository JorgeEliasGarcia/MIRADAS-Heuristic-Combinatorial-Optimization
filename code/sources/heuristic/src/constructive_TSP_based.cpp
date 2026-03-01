#include "constructive_TSP_based.hpp"

#include "sync_plan.hpp"
#include <limits>
#include "bitset.hpp"

namespace SYNC_LIB
{

    constructive_TSP_based::constructive_TSP_based(const sync_heuristic_a_input &input)
        : input_(input),
          time_(input.get_time()),
          matrix_ptr_(time_.get_matrix_ptr()),
          n_col_(time_.get_n_cols())
    {
    }

    constructive_TSP_based::~constructive_TSP_based(void)
    {
    }

    void constructive_TSP_based::constructive_TSP_nearest_neighbor_(const vector<int> &items, vector<int> &route, double &route_cost) const
    {
        const size_t n_items{items.size()};

        route.clear();
        route.reserve(n_items);
        route_cost = 0.0;

        const double inf_distance{numeric_limits<double>::max()};

        GOMA::bitset visited(n_items);

        int s{0};

        for (size_t i = 0; i < n_items; ++i)
        {
            double min_time{inf_distance};
            int min_index{-1};

            for (size_t j = 0; j < n_items; ++j)
            {
                if (!visited.contains(j + 1))
                {
                    const int t{items[j]};
                    const double time_st{time_(s + 1, t + 1)};

                    if (time_st < min_time)
                    {
                        min_time = time_st;
                        min_index = j;
                    }
                }
            }

            visited.insert(min_index + 1);
            route_cost += min_time;
            s = items[min_index];

            if (s != 0)
            {
                route.push_back(s);
            }
        }

        route_cost = get_time_(route);
    }

    void constructive_TSP_based::constructive_TSP_cheapest_insertion_(const vector<int> &items, vector<int> &route, double &route_cost) const
    {
        const size_t n_items{items.size()};

        route.clear();
        route.reserve(n_items);
        route_cost = 0.0;

        const double inf_distance{numeric_limits<double>::max()};

        GOMA::bitset visited(n_items + 1);

        int s{0};

        route.push_back(s);

        for (size_t i = 0; i < n_items; ++i)
        {
            double min_time{inf_distance};
            int min_index{-1};

            for (size_t j = 0; j < n_items; ++j)
            {
                if (!visited.contains(j + 1))
                {
                    const int t{items[j]};

                    for (size_t k = 0; k < route.size(); ++k)
                    {
                        const int u{route[k]};
                        const int v{route[(k + 1) % route.size()]};

                        const double time_uv{time_(u + 1, v + 1)};
                        const double time_ut{time_(u + 1, t + 1)};
                        const double time_tv{time_(t + 1, v + 1)};

                        assert(time_ut >= 0);
                        assert(time_tv >= 0);

                        const double time_insertion{time_ut + time_tv};

                        double time_sum{0.0};

                        if (u == v)
                            time_sum = time_insertion;
                        else
                            time_sum = time_insertion - time_uv;

                        if (time_sum < min_time)
                        {
                            min_time = time_sum;
                            min_index = j;
                        }
                    }

                    assert(min_index != -1);
                }
            }

            assert(min_time >= 0);

            visited.insert(min_index + 1);
            route_cost += min_time;
            s = items[min_index];

            if (s != 0)
            {
                route.push_back(s);
            }
        }

        // Remove the first item
        route.erase(route.begin());

        route_cost = get_time_(route);
    }

    void constructive_TSP_based::compute_TSP_LS_(vector<int> &c_route) const
    {
        const size_t n_items{c_route.size()};
        //const int thrl{static_cast<int>(ceil((double)(n_items) * 0.7))};
        const size_t max_iterations{10};
        const int thrl{10};

        vector<int> best_route(c_route);
        double best_route_cost{get_time_(c_route)};

        //vector<int> best_route;
        //double best_route_cost{1E10};
        

        for (size_t i{0}; i < max_iterations; ++i)
        {
            vector<int> route(c_route);
            double route_cost{get_time_(route)};

            bool improvement{false};

            double prev_route_cost{1E10};
            vector<int> prev_route(n_items);

            do
            {
                improvement = false;

                two_opt_(route, route_cost);

                route_cost = get_time_(route);

                if (route_cost < prev_route_cost)
                {
                    improvement = true;

                    prev_route = route;
                    prev_route_cost = route_cost;
                }

                for (size_t j{0}; j < (i % thrl) + 1; ++j)
                {
                    make_perturbation_(route);
                }

            } while (improvement);

            if (prev_route_cost < best_route_cost)
            {
                best_route = prev_route;
                best_route_cost = prev_route_cost;
            }
        }

        if (c_route != best_route)
        {
            c_route = best_route;
        }

        // cout << "Best Route Cost: " << best_route_cost << endl;
    }

    void constructive_TSP_based::three_opt_(vector<int> &route, double &route_cost) const
    {
        // Performs a 3-opt move on the route

        const GOMA::matrix<double> &time{input_.get_time()};
        const double *matrix_ptr{time.get_matrix_ptr()};
        const size_t n_col{time.get_n_cols()};

        size_t matrix_pos{0};

        const size_t n{route.size()};

        int best_route[n + 2];

        for (size_t i = 0; i < n; ++i)
        {
            best_route[i + 1] = route[i];
        }

        best_route[0] = 0;
        best_route[n + 1] = 0;

        int new_route[n + 2];

        new_route[0] = 0;

        double new_cost{0.0};

        double best_cost{numeric_limits<double>::max()};
        bool improvement{true};

        while (improvement)
        {
            improvement = false;

            for (size_t i{1}; i < n - 2; ++i)
            {
                for (size_t j{i + 1}; j < n - 1; ++j)
                {
                    for (size_t k{j + 1}; k < n; ++k)
                    {
                        for (size_t l{0}; l < n + 2; ++l)
                        {
                            new_route[l] = best_route[l];
                        }

                        new_route[i] = best_route[j];
                        new_route[i + 1] = best_route[k];
                        new_route[i + 2] = best_route[i];

                        new_cost = 0.0;

                        for (size_t l{0}; l < n + 1; ++l)
                        {
                            matrix_pos = new_route[l] * n_col + new_route[l + 1];
                            new_cost += matrix_ptr[matrix_pos];
                        }

                        if (new_cost < best_cost)
                        {
                            best_cost = new_cost;

                            for (size_t l{0}; l < n + 2; ++l)
                            {
                                best_route[l] = new_route[l];
                            }

                            improvement = true;
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < n; ++i)
        {
            route[i] = best_route[i + 1];
        }

        route_cost = get_time_(route);
    }

    void constructive_TSP_based::two_opt_(vector<int> &route, double &route_cost) const
    {
        const GOMA::matrix<double> &time{input_.get_time()};
        const double *matrix_ptr{time.get_matrix_ptr()};
        const size_t n_col{time.get_n_cols()};

        size_t matrix_pos{0};

        const size_t n{route.size()};

        int best_route[n + 2];

        for (size_t i = 0; i < n; ++i)
        {
            best_route[i + 1] = route[i];
        }

        best_route[0] = 0;
        best_route[n + 1] = 0;

        int new_route[n + 2];

        new_route[0] = 0;
        new_route[n + 1] = 0;

        double new_cost{0.0};

        double best_cost{numeric_limits<double>::max()};
        bool improvement{true};

        while (improvement)
        {
            improvement = false;

            for (size_t i{1}; i < n - 1; ++i)
            {
                for (size_t j{i + 1}; j < n; ++j)
                {
                    for (size_t k{1}; k < i; ++k)
                    {
                        new_route[k] = best_route[k];
                    }

                    for (size_t k{i}, l{j}; k <= j; ++k, --l)
                    {
                        new_route[k] = best_route[l];
                    }

                    for (size_t k{j + 1}; k < n + 1; ++k)
                    {
                        new_route[k] = best_route[k];
                    }

                    new_cost = 0.0;

                    for (size_t k{0}; k < n + 1; ++k)
                    {
                        matrix_pos = new_route[k] * n_col + new_route[k + 1];
                        new_cost += matrix_ptr[matrix_pos];
                    }

                    if (new_cost < best_cost)
                    {
                        best_cost = new_cost;

                        for (size_t l{0}; l < n + 2; ++l)
                        {
                            best_route[l] = new_route[l];
                        }

                        improvement = true;
                    }
                }
            }
        }

        for (size_t i = 0; i < n; ++i)
        {
            route[i] = best_route[i + 1];
        }

        route_cost = best_cost;
    }

    void constructive_TSP_based::make_perturbation_(vector<int> &route) const
    {
        const size_t n{route.size()};

        const size_t i{rand() % n};
        const size_t j{rand() % n};

        if (i != j)
        {
            const int aux{route[i]};
            route[i] = route[j];
            route[j] = aux;
        }
    }

    double constructive_TSP_based::get_time_(const vector<int> &route) const
    {
        size_t matrix_pos{0};

        const size_t n{route.size()};

        double cost{0.0};

        assert(route[0] != 0);

        cost = time_(0 + 1, route[0] + 1);

        for (size_t i = 0; i < n - 1; ++i)
        {
            matrix_pos = route[i] * n_col_ + route[i + 1];
            cost += matrix_ptr_[matrix_pos];
        }

        cost += time_(route[n - 1] + 1, 0 + 1);

        return cost;
    }

    void constructive_TSP_based::build_sorting_map_(vector<int> &route, vector<int> &map) const
    {
        const size_t n_customers{input_.get_n_customers()};
        map.resize(n_customers + 1, -1);

        compute_TSP_LS_(route);

        for (size_t i{0}; i < n_customers; ++i)
        {
            map[route[i]] = i;
        }
    }

    void constructive_TSP_based::sort_routes_(const vector<int> &map, vector<vector<int>> &routes) const
    {
        const size_t n_depots{input_.get_n_depots()};
        routes.resize(n_depots);

        get_depot_items_(input_.get_demands(), routes);

        for (size_t i{0}; i < n_depots; ++i)
        {
            vector<int> &route(routes[i]);

            sort_(map, route);
        }
    }

    void constructive_TSP_based::sort_(const vector<int> &map, vector<int> &items) const
    {
        // Sorting the items from the vector "items" according to the precedence matrix
        const size_t n_items{items.size()};

        for (size_t i{0}; i < n_items - 1; ++i)
        {
            for (size_t j{0}; j < n_items - i - 1; ++j)
            {
                if (map[items[j]] > map[items[j + 1]])
                {
                    const int aux{items[j]};
                    items[j] = items[j + 1];
                    items[j + 1] = aux;
                }
            }
        }
    }

    void constructive_TSP_based::get_depot_items_(const vector<vector<int>> &demands, vector<vector<int>> &depot_items) const
    {
        const size_t n_depots{depot_items.size()};
        const size_t n_customers{demands.size()};

        for (size_t i = 1; i < n_customers; ++i)
        {
            const vector<int> &d{demands[i]};

            for (size_t j = 0; j < n_depots; ++j)
            {
                if (d[j] > 0)
                {
                    depot_items[j].push_back(i);
                }
            }
        }
    }

    bool constructive_TSP_based::build_tw_feasible_solution(vector<int> &base_sequence, sync_solution &tw_feasible_solution) const
    {
        
        vector<int> map;
        build_sorting_map_(base_sequence, map);

        vector<vector<int>> routes;
        sort_routes_(map, routes);

        sync_plan plan(input_);

        tw_feasible_solution.set(input_, routes);

        return tw_feasible_solution.is_feasible();
    }


    void constructive_TSP_based::build_base_sequence_nearest_neighbor(vector<int> &route) const
    {
        const size_t n_customers{input_.get_n_customers()};
        route.resize(n_customers);

        vector<customer_id> customers_sequence(n_customers);

        for (size_t i{1}; i < n_customers + 1; ++i)
        {
            customers_sequence[i - 1] = i;
        }

        double route_cost{0.0};

        constructive_TSP_nearest_neighbor_(customers_sequence, route, route_cost);
        //two_opt_(route, route_cost);
    }

    void constructive_TSP_based::build_base_sequence_cheapest_insertion(vector<int> &route) const
    {
        const size_t n_customers{input_.get_n_customers()};
        route.resize(n_customers);

        vector<customer_id> customers_sequence(n_customers);

        for (size_t i{1}; i < n_customers + 1; ++i)
        {
            customers_sequence[i - 1] = i;
        }

        double route_cost{0.0};

        constructive_TSP_cheapest_insertion_(customers_sequence, route, route_cost);
        //two_opt_(route, route_cost);
    }
}