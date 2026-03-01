#pragma once

#include <iostream>
#include <iomanip>
#include <cassert>

#define WIDE 6
#define PRECISION 1

using namespace std;

namespace GOMA
{

    template <class T>
    class matrix
    {
    private:
        size_t m_;
        size_t n_;

        T *v_;

    public:
        matrix(void) : m_(0),
                       n_(0),
                       v_(NULL) {}

        matrix(int m, int n) : m_(m),
                               n_(n),
                               v_(NULL)
        {
            build();
        }

        matrix(int m, int n, T data) : m_(m),
                                       n_(n),
                                       v_(NULL)
        {
            build();

            fill(data);
        }

        matrix(const matrix &M) : m_(M.m_),
                                  n_(M.n_),
                                  v_(NULL)
        {
            build();

            copy(M.v_, M.m_, M.n_, v_, m_, n_);
        }

        virtual ~matrix(void)
        {
            if (v_)
            {
                delete[] v_;
            }
        }

        void resize(size_t m, size_t n)
        {
            if (m == m_ && n == n_)
                return;

            m_ = m;
            n_ = n;

            build();

            init(0);
        }

        void resize_and_keep(size_t m, size_t n)
        {
            if (m == m_ && n == n_)
                return;

            T *v;

            build(v, m, n);

            copy(v_, m_, n_, v, m, n);

            v_ = v;

            m_ = m;
            n_ = n;
        }

        T const* get_matrix_ptr(void) const
        {
            return v_;
        }

        void copy(T *v, size_t m, size_t n, T *&w, size_t p, size_t q)
        {
            const size_t l_m = min(m, p);
            const size_t l_n = min(n, q);

            for (size_t i = 1; i <= l_m; i++)
                for (size_t j = 1; j <= l_n; j++)
                    w[pos(q, i, j)] = v[pos(i, j)];
        }

        void init(T data)
        {

            fill(data);
        }

        void fill(T data)
        {
            const size_t sz = m_ * n_;

            T *v_ptr{v_};

            for (size_t i{0}; i < sz; i++)
            {
                *v_ptr = data;
                v_ptr++;
            }
        }

        T &operator[](size_t i)
        {
            assert(i < m_ * n_);

            return v_[i];
        }

        const T &operator[](size_t i) const
        {
            assert(i < m_ * n_);

            return v_[i];
        }

        T &operator()(size_t i, size_t j)
        {
            assert(i >= 1);
            assert(i <= m_);

            assert(j >= 1);
            assert(j <= n_);

            return v_[pos(i, j)];
        }

        T &at(size_t i, size_t j)
        {
            assert(i >= 1);
            assert(i <= m_);

            assert(j >= 1);
            assert(j <= n_);

            return v_[pos(i, j)];
        }

        const T &operator()(size_t i, size_t j) const
        {
            assert(i >= 1);
            assert(i <= m_);

            assert(j >= 1);
            assert(j <= n_);

            return v_[pos(i, j)];
        }

        const T &at(size_t i, size_t j) const
        {
            assert(i >= 1);
            assert(i <= m_);

            assert(j >= 1);
            assert(j <= n_);

            return v_[pos(i, j)];
        }

        matrix &operator=(const matrix &M)
        {
            if (this == &M)
                return *this;

            if (m_ != M.m_ || n_ != M.n_)
            {
                resize(M.m_, M.n_);
            }

            copy(M.v_, M.m_, M.n_, v_, m_, n_);

            return *this;
        }

        inline size_t get_m(void) const
        {
            return m_;
        }

        inline size_t get_n(void) const
        {
            return n_;
        }

        inline size_t get_n_rows(void) const
        {
            return m_;
        }

        inline size_t get_n_cols(void) const
        {
            return n_;
        }

        void transpose(matrix &M) const
        {
            M.resize(n_, m_);

            for (size_t i = 1; i <= m_; i++)
                for (size_t j = 1; j <= n_; j++)
                    M(j, i) = (*this)(i, j);
        }

        istream &read_raw(istream &is)
        {

            for (size_t i = 1; i <= m_; i++)
                for (size_t j = 1; j <= n_; j++)
                    is >> v_[pos(i, j)];

            return is;
        }

        ostream &write_raw(ostream &os) const
        {

            for (size_t i = 1; i <= m_; i++)
            {
                for (size_t j = 1; j <= n_; j++)
                {
                    const double val = v_[pos(i, j)];

                    if (val > 1E+6)
                        os << setw(WIDE) << "   inf ";
                    else
                        os << setw(WIDE) << fixed << setprecision(PRECISION) << val << " ";
                }
                os << endl;
            }

            return os;
        }

        inline int pos(size_t i, size_t j) const
        {
            return (i - 1) * n_ + j - 1;
        }

        inline int pos(size_t n, size_t i, size_t j)
        {
            return (i - 1) * n + j - 1;
        }

    private:
        void build(void)
        {
            if (v_)
            {
                delete[] v_;
            }

            if (m_ * n_ > 0)
            {
                v_ = new T[m_ * n_];
            }
            else
            {
                v_ = NULL;
            }
        }

        void
        build(T *v, size_t m, size_t n)
        {
            if (v)
            {
                delete[] v;
            }

            if (m * n > 0)
            {
                v = new T[m * n];
            }
            else
            {
                v = NULL;
            }
        }
    };
}

template <class T>
istream &operator>>(istream &is, GOMA::matrix<T> &M)
{
    M.read_raw(is);
    return is;
}

template <class T>
ostream &operator<<(ostream &os, const GOMA::matrix<T> &M)
{
    M.write_raw(os);
    return os;
}
