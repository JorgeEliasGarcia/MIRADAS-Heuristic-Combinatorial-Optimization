#include "gen_two_opt.hpp"

#include <iostream>
#include <limits>

namespace SYNC_LIB
{
    local_search_gen_two_opt::local_search_gen_two_opt(const sync_heuristic_a_input &input) : input_(input),
                                                                                              time_(input.get_cost()),
                                                                                              matrix_ptr_(time_.get_matrix_ptr()),
                                                                                              n_col_(time_.get_n_cols()),
                                                                                              n_depots_(input.get_n_depots()),
                                                                                              n_locations_(input.get_n_locations())
    {
    }

    local_search_gen_two_opt::~local_search_gen_two_opt(void)
    {
    }

    bool local_search_gen_two_opt::gen_two_opt(const sync_solution &initial_solution, sync_solution &solution) const
    {
        const vector<vector<int>> &initial_routes{initial_solution.get_routes()};

        if (initial_routes.size() == 0)
        {
            return false;
        }

        generalized_two_opt_(initial_solution, solution);

        return solution.is_feasible();
    }

    void local_search_gen_two_opt::swap_arcs_(const size_t i, const size_t j, const vector<int> &prev_route, vector<int> &new_route, double &new_cost) const
    {
        const size_t n{prev_route.size()};

        for (size_t k{0}; k < i; ++k)
        {
            new_route[k] = prev_route[k];
        }

        for (size_t k{i}, l = j; k <= j; ++k, --l)
        {
            new_route[k] = prev_route[l];
        }

        for (size_t k{j + 1}; k < n; ++k)
        {
            new_route[k] = prev_route[k];
        }

        new_cost = get_time_(new_route);
    }

    /*bool sync_heuristic::two_opt_(const vector<int> &route, size_t &ii, size_t &jj, vector<int> &t_best_route, double &t_best_cost) const
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

        double best_cost{numeric_limits<double>::max()};

        int new_route[n + 2];

        new_route[0] = 0;
        new_route[n + 1] = 0;

        double new_cost{0.0};

        bool improvement{true};


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
                        ii = i;
                        jj = j;

                        best_cost = new_cost;

                        for (size_t l{0}; l < n + 2; ++l)
                        {
                            best_route[l] = new_route[l];
                        }

                        improvement = true;
                    }
                }
            }


        for (size_t i = 0; i < n; ++i)
        {
            t_best_route[i] = best_route[i + 1];
        }

        cout << "Best Cost: " << best_cost << endl;

        t_best_cost = best_cost;

        return improvement;
    }*/

    bool local_search_gen_two_opt::two_opt_(const std::vector<int> &route, const double prev_route_time, vector<size_t> &ii, vector<size_t> &jj, std::vector<std::vector<int>> &best_routes, vector<double> &best_costs) const
    {
        const size_t n{route.size()};

        vector<int> new_route(n);
        double new_cost{0.0};

        bool improvement{false};

        for (size_t i{0}; i < n - 1; ++i)
        {
            const size_t max_j{min(i + 3, n)};

            for (size_t j{i + 1}; j < max_j; ++j)
            {
                new_cost = 0.0;

                swap_arcs_(i, j, route, new_route, new_cost);

                if (new_cost < prev_route_time)
                {
                    ii.push_back(i);
                    jj.push_back(j);
                    best_costs.push_back(new_cost);
                    best_routes.push_back(new_route);
                    improvement = true;
                }
            }
        }

        // cout << "Best Cost: " << best_cost << endl;

        return improvement;
    }

    bool local_search_gen_two_opt::two_opt_(const vector<int> &route, size_t &ii, size_t &jj, vector<int> &best_route, double &best_cost) const
    {
        const size_t n{route.size()};

        vector<int> new_route(n);
        double new_cost{0.0};

        bool improvement{false};

        for (size_t i{0}; i < n - 1; ++i)
        {
            for (size_t j{i + 1}; j < n; ++j)
            {
                new_cost = 0.0;

                swap_arcs_(i, j, route, new_route, new_cost);

                if (new_cost < best_cost)
                {
                    ii = i;
                    jj = j;
                    best_cost = new_cost;
                    best_route = new_route;
                    improvement = true;
                }
            }
        }

        // cout << "Best Cost: " << best_cost << endl;

        return improvement;
    }

    void local_search_gen_two_opt::get_quality_measures_(const sync_plan &plan, double &makespan, double &avg_routes_time) const
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

    void local_search_gen_two_opt::initialize_stack_(const sync_solution &initial_solution, stack<vector<vector<int>>> &new_routes_stack) const
    {
        vector<vector<int>> new_routes{initial_solution.get_routes()};

        for (vector<int> &c_route : new_routes)
        {
            if (c_route.front() == 0)
            {
                c_route.erase(c_route.begin());
                c_route.pop_back();
            }
        }

        new_routes_stack.push(new_routes);
    }

    void local_search_gen_two_opt::update_bitset_(const vector<int> &prev_route, const size_t ii, const size_t jj, GOMA::bitset &implied) const
    {
        implied.reset();

        //cout << "Fijando elementos: ";

        for (size_t i{ii}; i <= jj; ++i)
        {
            //cout << setw(3) << prev_route[i] << " ";
            implied.insert(prev_route[i] + 1);
        }

        //cout << endl;
    }

    void local_search_gen_two_opt::write_routes_(const vector<vector<int>> &t_routes) const
    {
        for (const vector<int> &route : t_routes)
        {
            for (const int &c_id : route)
            {
                cout << setw(3) << c_id << " ";
            }

            cout << endl;
        }
    }

    void local_search_gen_two_opt::update_aux_routes_(GOMA::bitset &implied, const size_t d, vector<vector<int>> &aux_routes) const
    {
        //write_routes_(aux_routes);
        //cout << endl;

        for (size_t i{0}; i < n_depots_ - 1; ++i)
        {
            const size_t pos{(d + i + 1) % n_depots_};

            const vector<int> &route{aux_routes[pos]};
            vector<int> new_route(route);

            reverse_fragment_(route, implied, new_route);

            aux_routes[pos] = new_route;
        }

        //write_routes_(aux_routes);

        //cout << "---------------------------------" << endl;
    }

    void local_search_gen_two_opt::get_quality_measures_(const vector<vector<int>> &aux_routes, double &makespan, double &avg_routes_time) const
    {
        sync_plan aux_plan(input_);

        const bool tw_feasible{aux_plan.set_routes(aux_routes)};

        if (tw_feasible)
        {
            get_quality_measures_(aux_plan, makespan, avg_routes_time);
        }
        else
        {
            //cout << "ERROR: TW not feasible" << endl;
            makespan = numeric_limits<double>::max();
            avg_routes_time = makespan;
        }
    }

    bool local_search_gen_two_opt::generalized_two_opt_(const sync_solution &initial_solution, sync_solution &solution) const
    {
        const vector<vector<int>> &initial_routes{initial_solution.get_routes()};

        if (initial_routes.size() == 0)
        {
            return false;
        }

        bool global_improvement{false};
        double global_makespan{initial_solution.get_makespan()};

        double best_makespan{numeric_limits<double>::max()};
        double best_avg_routing_time{numeric_limits<double>::max()};

        stack<vector<vector<int>>> new_routes_stack;

        initialize_stack_(initial_solution, new_routes_stack);

        GOMA::bitset implied(n_locations_);
        sync_plan aux_plan(input_);

        while (!new_routes_stack.empty())
        {
            const vector<vector<int>> c_routes{new_routes_stack.top()};
            new_routes_stack.pop();

            for (size_t d{0}; d < n_depots_; ++d)
            {
                vector<vector<int>> aux_routes{c_routes};

                const vector<int> &prev_route{aux_routes[d]};
                const double prev_route_time{get_time_(prev_route)};

                vector<int> new_route{prev_route};
                double new_route_time{prev_route_time};

                size_t ii{0};
                size_t jj{0};

                two_opt_(prev_route, ii, jj, new_route, new_route_time);

                if (new_route_time < prev_route_time)
                {
                    update_bitset_(prev_route, ii, jj, implied);

                    aux_routes[d] = new_route;

                    //cout << "Ruta: " << d << endl;

                    //cout << "Elementos a intercambiar: " << ii << " " << jj << endl;

                    update_aux_routes_(implied, d, aux_routes);

                    double aux_avg_routing_time{0.0};
                    double aux_makespan{0.0};

                    get_quality_measures_(aux_routes, aux_makespan, aux_avg_routing_time);

                    const double makespan_improvement{best_makespan - aux_makespan};
                    const double avg_routing_time_improvement{best_avg_routing_time - aux_avg_routing_time};
                    const bool improvement{(makespan_improvement > precision_) || ((makespan_improvement > -precision_) && (avg_routing_time_improvement > precision_))};

                    if (improvement)
                    {
                        new_routes_stack.push(aux_routes);
                        best_makespan = aux_makespan;
                        best_avg_routing_time = aux_avg_routing_time;

                        if (aux_makespan < global_makespan - precision_)
                        {
                            global_improvement = true;
                            global_makespan = aux_makespan;
                            solution.set(input_, aux_routes);
                        }
                    }
                }
            }
        }

        return global_improvement;
    }

    /*bool local_search_gen_two_opt::generalized_two_opt_(const sync_solution &initial_solution, sync_solution &solution) const
    {
        const vector<vector<int>> &initial_routes{initial_solution.get_routes()};

        if (initial_routes.size() == 0)
        {
            return false;
        }

        bool global_improvement{false};
        double global_makespan{initial_solution.get_makespan()};

        double best_makespan{numeric_limits<double>::max()};
        double best_avg_routing_time{numeric_limits<double>::max()};

        stack<vector<vector<int>>> new_routes_stack;

        initialize_stack_(initial_solution, new_routes_stack);

        GOMA::bitset implied(n_locations_);
        sync_plan aux_plan(input_);

        while (!new_routes_stack.empty())
        {
            const vector<vector<int>> c_routes{new_routes_stack.top()};
            new_routes_stack.pop();

            for (size_t d{0}; d < n_depots_; ++d)
            {
                vector<vector<int>> aux_routes{c_routes};

                const vector<int> &prev_route{aux_routes[d]};
                const double prev_route_time{get_time_(prev_route)};

                vector<vector<int>> v_new_route;
                vector<double> v_new_route_time;

                vector<size_t> v_ii;
                vector<size_t> v_jj;

                two_opt_(prev_route, prev_route_time, v_ii, v_jj, v_new_route, v_new_route_time);

                const size_t n{v_new_route.size()};

                cout << "N: " << n << endl;

                for (size_t i{0}; i < n; ++i)
                {
                    aux_routes = c_routes;

                    vector<int> new_route{v_new_route[i]};

                    size_t ii{v_ii[i]};
                    size_t jj{v_jj[i]};

                    update_bitset_(prev_route, ii, jj, implied);

                    aux_routes[d] = new_route;

                    update_aux_routes_(implied, d, aux_routes);

                    double aux_avg_routing_time{0.0};
                    double aux_makespan{0.0};

                    get_quality_measures_(aux_routes, aux_makespan, aux_avg_routing_time);

                    const double makespan_improvement{best_makespan - aux_makespan};
                    const double avg_routing_time_improvement{best_avg_routing_time - aux_avg_routing_time};
                    const bool improvement{(makespan_improvement > precision_) || ((makespan_improvement > -precision_) && (avg_routing_time_improvement > precision_))};

                    if (improvement)
                    {
                        new_routes_stack.push(aux_routes);
                        best_makespan = aux_makespan;
                        best_avg_routing_time = aux_avg_routing_time;

                        if (aux_makespan < global_makespan - precision_)
                        {
                            global_improvement = true;
                            global_makespan = aux_makespan;
                            solution.set(input_, aux_routes);
                        }
                    }
                }
            }
        }

        return global_improvement;
    }*/

    void local_search_gen_two_opt::reverse_fragment_(const vector<int> &route, GOMA::bitset &implied, vector<int> &new_route) const
    {
        const size_t n{route.size()};

        assert(n > 0);

        size_t ii{0};
        size_t jj{0};

        for (size_t i{0}; i < n; i++)
        {
            if (implied.contains(route[i] + 1))
            {
                ii = i;
                break;
            }
        }

        for (size_t i{n - 1}; i > ii; i--)
        {
            if (implied.contains(route[i] + 1))
            {
                jj = i;
                break;
            }
        }

        if (jj != ii)
        {

            for (size_t k{0}; k < ii; ++k)
            {
                new_route[k] = route[k];
            }

            {
                size_t k{ii};
                size_t l{jj};

                while (k <= l)
                {
                    if ((implied.contains(route[k] + 1) && implied.contains(route[l] + 1)))
                    {
                        new_route[k] = route[l];
                        new_route[l] = route[k];
                        k++;
                        l--;
                    }
                    else if (!implied.contains(route[k] + 1))
                    {
                        k++;
                    }
                    else if (!implied.contains(route[l] + 1))
                    {
                        l--;
                    }
                }
            }

            for (size_t k{jj + 1}; k < n; ++k)
            {
                new_route[k] = route[k];
            }
        }
    }

    double local_search_gen_two_opt::get_time_(const vector<int> &route) const
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
}