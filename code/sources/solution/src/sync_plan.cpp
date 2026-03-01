#include "sync_plan.hpp"

#include <iomanip>
#include <limits>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "bitset.hpp"

#define INF_SLACK 1E4

namespace SYNC_LIB
{
    plan_data::plan_data(const sync_heuristic_a_input &instance) : routes_(instance.get_n_depots()),
                                                                   time_(instance.get_n_depots()),
                                                                   starting_time_(instance.get_n_customers() + 1),
                                                                   time_windows_(instance.get_n_customers() + 1),
                                                                   slack_(instance.get_n_customers() + 1),
                                                                   average_starting_time_(instance.get_n_customers() + 1),
                                                                   violations_(instance.get_n_customers() + 1)
    {
        const size_t n_locations{instance.get_n_locations()};

        for (size_t i{0}; i < n_locations; ++i)
        {
            slack_[i] = 0;
        }
    }

    plan_data::~plan_data(void)
    {
    }


    sync_plan::sync_plan(const sync_heuristic_a_input &instance) : input_(instance),
                                                                   n_depots_(instance.get_n_depots()),
                                                                   n_customers_(instance.get_n_customers()),
                                                                   plan_(instance)

    {
    }

    sync_plan::~sync_plan(void)
    {
    }

    double sync_plan::get_makespan(vector<double> &routes_time) const
    {
        routes_time.resize(n_depots_);
        const vector<vector<double>> &starting_time{plan_.starting_time_};

        double makespan{0.0};

        const vector<double> &c_starting_time{starting_time[0]};

        for (size_t i = 0; i < n_depots_; ++i)
        {
            const double &c_time{c_starting_time[i]};

            routes_time[i] = c_time;

            if (c_time > makespan)
            {
                makespan = c_time;
            }
        }

        return makespan;
    }

    double sync_plan::get_makespan(void) const
    {
        const vector<vector<double>> &starting_time{plan_.starting_time_};

        double makespan{0.0};

        const vector<double> &c_starting_time{starting_time[0]};

        for (size_t i = 0; i < n_depots_; ++i)
        {
            const double &c_time{c_starting_time[i]};

            if (c_time > makespan)
            {
                makespan = c_time;
            }
        }

        return makespan;
    }

    void sync_plan::insert_routes_(const vector<vector<int>> &routes, plan_data &plan)
    {
        const size_t n_routes{routes.size()};
        plan.routes_.clear();
        plan.routes_.resize(n_routes);

        for (size_t i = 0; i < n_routes; ++i)
        {
            const vector<int> &route{routes[i]};
            vector<int> &t_route{plan.routes_[i]};

            if (route.front() != 0)
            {

                t_route.reserve(route.size() + 2);

                t_route.push_back(0);

                for (const int &c_id : route)
                {
                    t_route.push_back(c_id);
                }

                t_route.push_back(0);
            }
            else
            {
                t_route = route;
            }
        }
    }

    bool sync_plan::update_plan_(const sync_heuristic_a_input &input, plan_data &plan)
    {
        bool is_feasible{true};

        starting_(input, plan, is_feasible);

        return is_feasible;
    }

    bool sync_plan::set_routes(const vector<vector<int>> &routes)
    {
        bool tw_feasible{false};

        plan_data aux_plan(this->plan_);
        insert_routes_(routes, aux_plan);
        tw_feasible = update_plan_(input_, aux_plan);

        plan_ = aux_plan;

        return tw_feasible;

        // write(cout, plan_);
    }

    void sync_plan::starting_(const sync_heuristic_a_input &input, plan_data &plan, bool &tw_feasible)
    {
        init_sequence_time_(input, plan);
        init_starting_time_(plan);

        update_parameters_(input, plan, tw_feasible);
    }

    void sync_plan::update_parameters_(const sync_heuristic_a_input &input, plan_data &plan, bool &tw_feasible)
    {
        tw_feasible = true;

        update_time_windows_(input, plan);
        update_violation_in_time_windows_(input, plan);
        update_violated_customers_(input, plan);

        if (tw_feasible)
        {
            tw_feasible = is_feasible(plan);
        }
    }

    void sync_plan::init_sequence_time_(const sync_heuristic_a_input &input, plan_data &plan)
    {
        const GOMA::matrix<double> &time{input.get_time()};

        plan.time_.resize(n_depots_);

        for (size_t i = 0; i < n_depots_; ++i)
        {
            const vector<int> &route{plan.routes_[i]};

            const size_t n_customers{route.size()};

            vector<double> &t_time{plan.time_[i]};
            t_time.resize(n_customers);

            for (size_t j = 0; j < n_customers; ++j)
            {
                t_time[j] = 0.0;
            }

            for (size_t j = 1; j < n_customers; ++j)
            {
                t_time[j] = time(route[j - 1] + 1, route[j] + 1);
            }
        }
    }

    void sync_plan::init_starting_time_(plan_data &plan)
    {
        plan.starting_time_.resize(n_customers_ + 1);

        for (size_t i{0}; i < n_customers_ + 1; ++i)
        {
            vector<double> &c_starting_time{plan.starting_time_[i]};
            c_starting_time.resize(n_depots_);

            for (size_t j = 0; j < n_depots_; ++j)
            {
                c_starting_time[j] = 0.0;
            }
        }

        vector<vector<double>> &starting_time{plan.starting_time_};

        for (size_t i{0}; i < n_depots_; ++i)
        {
            const vector<int> &c_route{plan.routes_[i]};
            vector<double> &c_time{plan.time_[i]};

            const size_t c_n_customers{c_route.size()};

            for (size_t j{1}; j < c_n_customers; ++j)
            {
                const int c_u_custom{c_route[j]};
                const int p_u_custom{c_route[j - 1]};

                vector<double> &c_u_starting_time{starting_time[c_u_custom]};
                vector<double> &p_u_starting_time{starting_time[p_u_custom]};

                c_u_starting_time[i] = p_u_starting_time[i] + c_time[j];
            }
        }
    }

    bool sync_plan::update_starting_time_(plan_data &plan)
    {
        bool change{false};

        for (size_t i = 0; i < n_depots_; ++i)
        {
            const vector<int> &c_route{plan.routes_[i]};
            vector<double> &c_time{plan.time_[i]};

            const size_t c_n_customers{c_route.size()};
            vector<vector<double>> &starting_time{plan.starting_time_};

            double t{0.0};

            for (size_t j = 1; j < c_n_customers; ++j)
            {
                const double c_u_time{c_time[j]};
                const int c_u_custom{c_route[j]};

                const vector<double> &c_u_starting_time_v{starting_time[c_u_custom]};

                const double c_u_starting_time{c_u_starting_time_v[i]};

                t += c_u_time;

                if (t > c_u_starting_time)
                {
                    plan.starting_time_[c_u_custom][i] = t;
                    change = true;
                }
                else if (t <= c_u_starting_time)
                {
                    t = c_u_starting_time;
                }
            }
        }

        return change;
    }

    void sync_plan::update_slack_(const sync_heuristic_a_input &input, plan_data &plan)
    {
        const vector<double> &max_diff_time{input.get_max_diff_time()};
        const vector<pair<double, double>> &time_windows{plan.time_windows_};
        vector<double> &slack_v{plan.slack_};

        for (size_t j = 1; j < n_customers_ + 1; ++j)
        {
            const pair<double, double> &c_time_windows{time_windows[j]};

            const double slack{max_diff_time[j] - (c_time_windows.second - c_time_windows.first)};
            slack_v[j] = slack;
        }
    }

    void sync_plan::update_time_windows_(const sync_heuristic_a_input &input, plan_data &plan)
    {
        const vector<vector<size_t>> &customer_depots{input.get_customer_depots()};

        vector<pair<double, double>> &time_windows{plan.time_windows_};
        vector<vector<double>> &starting_time{plan.starting_time_};

        // time_windows.resize(n_customers_ + 1);

        {
            double max_time{0.0};

            vector<double> &t_time{starting_time[0]};

            for (size_t j{0}; j < n_depots_; ++j)
            {
                const double &t{t_time[j]};

                if (t > max_time)
                {
                    max_time = t;
                }
            }

            time_windows[0] = make_pair(0.0, max_time);
        }

        for (size_t i{1}; i < n_customers_ + 1; ++i)
        {
            double min_time{numeric_limits<double>::max()};
            double max_time{0.0};

            const vector<size_t> &c_depots{customer_depots[i]};
            vector<double> &i_starting_time{starting_time[i]};

            for (const size_t j : c_depots)
            {
                const double &t{i_starting_time[j]};

                if (t < min_time)
                {
                    min_time = t;
                }

                if (t > max_time)
                {
                    max_time = t;
                }
            }

            time_windows[i] = make_pair(min_time, max_time);
        }
    }

    void sync_plan::update_violation_in_time_windows_(const sync_heuristic_a_input &input, plan_data &plan)
    {
        const vector<double> &max_diff_time{input.get_max_diff_time()};
        vector<pair<double, double>> &time_windows{plan.time_windows_};
        vector<double> &violation{plan.violations_};

        for (size_t i{0}; i < n_customers_ + 1; ++i)
        {
            const pair<double, double> &c_time_windows{time_windows[i]};
            const double &c_max_diff_time{max_diff_time[i]};

            if (c_time_windows.second - c_time_windows.first > c_max_diff_time)
            {
                violation[i] = c_time_windows.second - c_time_windows.first - c_max_diff_time;
            }
            else
            {
                violation[i] = 0.0;
            }
        }
    }

    size_t sync_plan::get_latest_depot_(void) const
    {
        const vector<double> &depot_starting_time{plan_.starting_time_[0]};

        size_t latest_depot{0};
        double latest_time{depot_starting_time[0]};

        for (size_t i{1}; i < n_depots_; ++i)
        {
            if (depot_starting_time[i] > latest_time)
            {
                latest_time = depot_starting_time[i];
                latest_depot = i;
            }
        }

        return latest_depot;
    }

    void sync_plan::update_violated_customers_(const sync_heuristic_a_input &input, plan_data &plan)
    {
        const vector<pair<double, double>> &time_windows{plan.time_windows_};
        const vector<double> &violations{plan.violations_};
        const vector<vector<size_t>> &customer_depots{input.get_customer_depots()};
        vector<vector<double>> &starting_time_v{plan.starting_time_};
        vector<double> &average_starting_time{plan.average_starting_time_};

        for (size_t i{1}; i < n_customers_ + 1; ++i)
        {
            const double &c_violation{violations[i]};
            const double &c_min_time{time_windows[i].first};
            const double increase{c_min_time + c_violation};
            vector<double> &i_starting_time{starting_time_v[i]};
            double &i_avg_time{average_starting_time[i]};

            const vector<size_t> &c_depots{customer_depots[i]};

            double avg_time{0.0};
            double count{0.0};

            for (const size_t j : c_depots)
            {
                double &starting_time{i_starting_time[j]};

                if (starting_time < increase)
                {
                    starting_time = increase;
                }

                avg_time += starting_time;

                if (starting_time > tol_)
                {
                    count += 1.0;
                }
            }

            if (count > tol_)
            {
                avg_time /= count;
            }

            i_avg_time = avg_time;
        }
    }

    bool sync_plan::is_feasible(const plan_data &plan) const
    {
        for (size_t i{1}; i < n_customers_ + 1; ++i)
        {
            if (plan.violations_[i] > tol_)
            {
                return false;
            }
        }

        return true;
    }

    /*double sync_plan::compute_cost_(const vector<int> &route) const
    {
        const GOMA::matrix<double> &time{instance_.get_time()};

        double cost{0.0};

        const size_t n{route.size()};

        for (size_t i{0}; i < n - 1; ++i)
        {
            cost += time(route[i] + 1, route[i + 1] + 1);
        }

        return cost;
    }*/

    void sync_plan::write_route_(ostream &os, const vector<int> &route) const
    {
        const size_t n{route.size()};

        for (size_t i{0}; i < n; ++i)
        {
            os << setw(3) << route[i] << " ";
        }
    }

    /*void sync_plan::write_routes_(ostream &os, const vector<vector<int>> &routes) const
    {
        const size_t n_routes{routes.size()};

        for (size_t i{0}; i < n_routes; ++i)
        {
            const vector<int> &route{routes[i]};

            os << setw(6) << compute_cost_(route) << ":";
            write_route_(os, route);
            os << endl;
        }

        os << endl;
    }*/

    /*ostream &sync_plan::write(ostream &os) const
    {
        write_routes_(os, plan_.routes_);

        return os;
    }*/

    ostream &sync_plan::write(const sync_heuristic_a_input &input, ostream &os, const plan_data &plan) const
    {
        const size_t n_depots{plan.routes_.size()};
        const vector<double> &max_diff_time{input.get_max_diff_time()};

        for (size_t i = 0; i < n_depots; ++i)
        {
            const vector<int> &route{plan.routes_[i]};

            const size_t c_n_customers{route.size()};

            os << setw(3) << setprecision(0) << route[0] << " ";

            for (size_t j = 1; j < c_n_customers; ++j)
            {
                const int c_customer{route[j]};
                const double slack{-(plan.time_windows_[c_customer].second - plan.time_windows_[c_customer].first - max_diff_time[c_customer])};
                os << setw(2) << route[j] << " [" << setw(4) << fixed << setprecision(0) << plan.starting_time_[route[j]][i] << "] <";

                if ((slack < tol_) && (j < c_n_customers - 1))
                {
                    os << setw(3) << "- " << "> | ";
                }
                else if (slack < INF_SLACK)
                {
                    os << setw(3) << fixed << setprecision(0) << slack << "> | ";
                }
                else
                {
                    os << setw(3) << "- " << "> | ";
                }
            }

            os << endl;
        }

        os << endl;

        const size_t n_customers{input.get_n_customers()};

        for (size_t i = 1; i < n_customers + 1; ++i)
        {
            if (plan.slack_[i] < tol_)
            {

                os << endl;
                os << "Customer: " << setw(3) << i << endl;

                for (size_t j = 0; j < n_depots; ++j)
                {
                    const double &t{plan.starting_time_[i][j]};

                    if (t < tol_)
                    {
                        os << setw(5) << "-" << " ";
                    }
                    else
                    {
                        os << setw(5) << fixed << setprecision(0) << plan.starting_time_[i][j] << " ";
                    }
                }

                os << endl;

                os << " < " << setw(5) << fixed << setprecision(0) << plan.time_windows_[i].first;
                os << " " << setw(5) << fixed << setprecision(0) << plan.average_starting_time_[i];
                os << " " << setw(5) << fixed << setprecision(0) << plan.time_windows_[i].second << " > [";

                if (fabs(plan.slack_[i]) > tol_)
                {
                    os << setw(5) << fixed << setprecision(0) << plan.slack_[i] << "]";
                }
                else
                {
                    os << setw(5) << "  -  " << "]";
                }

                // os << setw(5) << max_diff_time[i] << " ";

                if (plan.violations_[i] > tol_)
                {
                    os << "     * " << setw(5) << plan.violations_[i];
                }

                os << endl;
            }
        }

        {
            os << endl;
            os << "Depot: " << endl;

            for (size_t j = 0; j < n_depots; ++j)
            {
                os << setw(5) << fixed << setprecision(0) << plan.starting_time_[0][j] << " ";
            }

            os << endl;

            os << " < " << setw(5) << fixed << setprecision(0) << plan.time_windows_[0].first << " " << setw(5) << fixed << setprecision(0) << plan.time_windows_[0].second << " > [" << setw(5) << plan.time_windows_[0].second - plan.time_windows_[0].first << "]";

            os << setw(5) << max_diff_time[0] << " ";

            if (plan.violations_[0] > tol_)
            {
                os << "     * " << setw(5) << plan.violations_[0];
            }

            os << endl;
        }

        os << endl;
        return os;
    }

} // namespace SYNC_LIB
// namespace name
