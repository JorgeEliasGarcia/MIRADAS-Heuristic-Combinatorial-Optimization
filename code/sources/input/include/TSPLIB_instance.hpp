#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <cmath>
#include <algorithm>
#include "matrix.hpp"

using namespace std;

#define NCTYPE_NUM 3
#define WTYPE_NUM 10
#define WFORMAT_NUM 9
#define DTYPE_NUM 3
#define KEY_NUM 25

namespace CTSP
{

    typedef double coordItemType;
    typedef pair<coordItemType, coordItemType> coordType;

    class TSPLIB_instance
    {
        typedef double (TSPLIB_instance::*distanceType)(const coordType &, const coordType &) const;
        typedef void (TSPLIB_instance::*readType)(istream &is, ostream &os);

    private:
        const vector<string> keywords = {
            /* 0*/ "NAME",
            /* 1*/ "TYPE",
            /* 2*/ "COMMENT",
            /* 3*/ "DIMENSION",
            /* 4*/ "CAPACITY",
            /* 5*/ "EDGE_WEIGHT_TYPE",
            /* 6*/ "EDGE_WEIGHT_FORMAT",
            /* 7*/ "DISPLAY_DATA_TYPE",
            /* 8*/ "EDGE_WEIGHT_SECTION",
            /* 9*/ "DISPLAY_DATA_SECTION",
            /*10*/ "NODE_COORD_SECTION",
            /*11*/ "NODE_COORD_TYPE",
            /*12*/ "DEPOT_SECTION",
            /*13*/ "CAPACITY_VOL",
            /*14*/ "DEMAND_SECTION",
            /*15*/ "TIME_WINDOW_SECTION",
            /*16*/ "STANDTIME_SECTION",
            /*17*/ "PICKUP_SECTION",
            /*18*/ "EOF",
            /*19*/ "NUMBER_OF_TRUCKS",
            /*20*/ "NUM_DAYS",
            /*21*/ "DISTANCE",
            /*22*/ "MAXIMUM_ALLOWABLE_DIFFERENTIAL"};

        const vector<string> wtypes = {
            "EXPLICIT",
            "EUC_2D",
            "EUC_3D",
            "MAX_2D",
            "MAX_3D",
            "MAN_2D",
            "MAN_3D",
            "CEIL_2D",
            "GEO",
            "ATT"};

        const vector<string> wformats = {
            "UPPER_ROW",
            "LOWER_ROW",
            "UPPER_DIAG_ROW",
            "LOWER_DIAG_ROW",
            "UPPER_COL",
            "LOWER_COL",
            "UPPER_DIAG_COL",
            "LOWER_DIAG_COL",
            "FULL_MATRIX"};

        const vector<string> dtypes = {
            "COORD_DISPLAY",
            "TWOD_DISPLAY",
            "NO_DISPLAY"};

        const vector<string> messages = {
            "--  Reading file  --:"};

    private:
        string name_;
        string type_;
        string comment_;
        int dimension_;
        int edge_weight_type_;
        int edge_weight_format_;
        int display_data_type_;
        int num_days_;
        int max_distance_;
        int maximum_allowable_differencial_;
        int depot_;

        double *distances_;

        vector<int> coord_id_;
        vector<coordType> coord_;

        vector<int> display_id_;
        vector<coordType> display_;

        vector<vector<int>> demand_;

        vector<distanceType> distance_function_;
        vector<readType> read_function_;
        vector<readType> edge_weight_reading_function_;

        vector<double> optimal_values_;

    public:
        TSPLIB_instance(void);
        virtual ~TSPLIB_instance(void);

        void read(const string &input_file);

        inline const string &get_instance_name(void) const
        {
            return name_;
        }

        inline const string &get_instance_type(void) const
        {
            return type_;
        }

        inline const string &get_instance_comment(void) const
        {
            return comment_;
        }

        inline const int &get_dimension(void) const
        {
            return dimension_;
        }

        inline const vector<double> &get_optimal_values(void) const
        {
            return optimal_values_;
        }

        inline const vector<coordType> &coord(void) const
        {
            return coord_;
        }

        void get_distances(GOMA::matrix<double> &distances_) const;

        inline const vector<vector<int>> &get_demands(void) const
        {
            return demand_;
        }

        inline const int &get_depot(void) const
        {
            return depot_;
        }

        inline const int &get_num_days(void) const
        {
            return num_days_;
        }

        inline const int &get_max_distance(void) const
        {
            return max_distance_;
        }

        inline const int &get_maximum_allowable_differencial(void) const
        {
            return maximum_allowable_differencial_;
        }

    private:
        void compute_implicit_distance_matrix_(void);

        void read_name_section_(istream &is, ostream &os);
        void read_type_section_(istream &is, ostream &os);
        void read_comment_section_(istream &is, ostream &os);
        void read_dimension_section_(istream &is, ostream &os);
        void read_capacity_section_(istream &is, ostream &os);
        void read_edge_weight_type_section_(istream &is, ostream &os);
        void read_edge_weigh_format_section_(istream &is, ostream &os);
        void read_edge_weight_section_(istream &is, ostream &os);
        void read_display_data_type_section_(istream &is, ostream &os);
        void read_node_coord_section_(istream &is, ostream &os);
        void read_node_coord_type_section_(istream &is, ostream &os);
        void read_depot_section_(istream &is, ostream &os);
        void read_capacity_vol_section_(istream &is, ostream &os);
        void read_demand_section_(istream &is, ostream &os);
        void read_time_window_section_(istream &is, ostream &os);
        void read_standtime_section_(istream &is, ostream &os);
        void read_pickup_section_(istream &is, ostream &os);
        void read_EOF_section_(istream &is, ostream &os);
        void read_number_of_trucks_section_(istream &is, ostream &os);
        void read_num_days_section_(istream &is, ostream &os);
        void read_distance_section_(istream &is, ostream &os);
        void read_display_data_section_(istream &is, ostream &os);
        void read_maximum_allowable_differencial_section_(istream &is, ostream &os);

        void read_edge_weight_section_upper_row_(istream &is, ostream &os);
        void read_edge_weight_section_lower_row_(istream &is, ostream &os);
        void read_edge_weight_section_upper_diag_row_(istream &is, ostream &os);
        void read_edge_weight_section_lower_diag_row_(istream &is, ostream &os);
        void read_edge_weight_section_upper_col_(istream &is, ostream &os);
        void read_edge_weight_section_lower_col_(istream &is, ostream &os);
        void read_edge_weight_section_upper_diag_col_(istream &is, ostream &os);
        void read_edge_weight_section_lower_diag_col_(istream &is, ostream &os);
        void read_edge_weight_section_full_matrix_(istream &is, ostream &os);

        double compute_euc_2d_distance_(const coordType &a, const coordType &b) const;
        double compute_max_2d_distance_(const coordType &a, const coordType &b) const;
        double compute_man_2d_distance_(const coordType &a, const coordType &b) const;
        double compute_ceil_2d_distance_(const coordType &a, const coordType &b) const;
        double compute_geo_distance_(const coordType &a, const coordType &b) const;
        double compute_att_distance_(const coordType &a, const coordType &b) const;


        void establish_dimension_(const int dimension);

        void clean_(const string &i_file, const string &o_file);
        void clean_(istream &is, ostream &os);

        void read_(istream &is, ostream &os);

        int find_key_(const string &token, const vector<string> keywords) const;

        int nint_(double x) const;
        double dtrunc_(double x) const;
        void radian_coords_(const coordType &a, double &longitude, double &latitude) const;
    };

}