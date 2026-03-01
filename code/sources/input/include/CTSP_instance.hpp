#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <utility>

#include "matrix.hpp"

using namespace std;

namespace CTSP
{
    class instance
    {
    protected:
        string id_;
        string comment_;
        string type_;

        GOMA::matrix<double> distances_;
        vector<vector<int>> demands_;

        size_t K_;

        vector<double> T_;

        size_t n_customers_;

        double max_distance_;

        vector<double> optimal_values_;

        bool triangle_inequality_;

    public:
        instance(void);
        virtual ~instance(void);

        virtual void read(const string &input_file);

        inline const vector<double> &get_optimal_values(void) const
        {
            return optimal_values_;
        }

        inline const string &get_instance_name(void) const
        {
            return id_;
        }

        inline const string &get_instance_comment(void) const
        {
            return comment_;
        }

        inline const string &get_instance_type(void) const
        {
            return type_;
        }

        inline const GOMA::matrix<double> &get_distances(void) const
        {
            return distances_;
        }

        inline const size_t &get_K(void) const
        {
            return K_;
        }

        inline size_t get_n_customers(void) const
        {
            return n_customers_;
        }

        inline const double &get_max_distance(void) const
        {
            return max_distance_;
        }

        inline const vector<double> &get_T(void) const
        {
            return T_;
        }

        inline const vector<vector<int>> &get_demands(void) const
        {
            return demands_;
        }

        size_t get_n_customer_operations(void) const;

        ostream &write_line(ostream &os) const;

        private:

        bool check_triangle_inequality_(void) const;
    };
}