#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>

template <typename T = int>
class Matrix {
  private:
    size_t m_rows;
    size_t m_columns;
    std::vector<T> m_data;

    struct Row {
      private:
        using it = typename std::vector<T>::iterator;
        it m_row;

      public:
        /*constexpr*/ Row(it other) : m_row(other) {}
        /*constexpr*/ const T &operator[](size_t n) const {
            return *(m_row + n);
        }
        /*constexpr*/ T &operator[](size_t n) { return *(m_row + n); }
    };

    void swap(Matrix &rhs) {
        std::swap(m_columns, rhs.m_columns);
        std::swap(m_rows, rhs.m_rows);
        m_data.swap(rhs.m_data);
    }

  public:
    /*constexpr*/ Matrix() = default;
    /*constexpr*/ Matrix(const Matrix &other) = default;
    Matrix(Matrix &&other) noexcept
        : m_data(std::move(other.m_data)),
          m_columns(std::exchange(other.m_columns, 0)),
          m_rows(std::exchange(other.m_rows, 0)) {}

    /*constexpr*/ Matrix(size_t rows, size_t columns, T val = {})
        : m_rows(rows), m_columns(columns) {
        m_data.resize(m_rows * m_columns, val);
    }

    /*constexpr*/ Matrix(std::initializer_list<std::initializer_list<T>> matrix)
        : m_rows(matrix.size()), m_columns(matrix.begin()->size()) {

        for (const auto row : matrix) {
            for (const auto elem : row) {
                m_data.push_back(elem);
            }
        }
    }

    static Matrix<T> identity(size_t rows) {
        Matrix<T> m{rows, rows};
        for (size_t i = 0; i < rows; ++i) {
            m[i][i] = 1;
        }
        return m;
    }

    /*constexpr*/ Matrix &operator=(const Matrix &other) {
        if (this == std::addressof(other)) {
            return *this;
        }

        Matrix copy{other};
        copy.swap(*this);
        return *this;
    }

    /*constexpr*/ Matrix &operator=(Matrix &&other) noexcept {
        if (this == std::addressof(other)) {
            return *this;
        }

        Matrix move{std::move(other)};
        move.swap(*this);
        return *this;
    }

    /*constexpr*/ bool equivalency(const Matrix &rhs) const {
        return (m_columns == rhs.m_columns) && (m_rows == rhs.m_rows);
    }

    /*constexpr*/ Matrix &operator+=(const Matrix &other) {
        if (equivalency(other)) {
            for (int i = 0; i < m_data.size(); ++i) {
                m_data[i] += other.m_data[i];
            }
        }
        return *this;
    }

    /*constexpr*/ Matrix &operator-=(const Matrix &other) {
        if (equivalency(other)) {
            for (int i = 0; i < m_data.size(); ++i) {
                m_data[i] -= other.m_data[i];
            }
        }
        return *this;
    }

    /*constexpr*/ Row operator[](size_t n) {
        return {m_data.begin() + m_rows * n};
    }

    /*constexpr*/ size_t ncolumns() const { return m_columns; }
    /*constexpr*/ size_t nrows() const { return m_rows; }

    std::optional<T> trace() const {
        if (m_rows != m_columns) {
            return std::nullopt;
        }
        T trace_val = 0;
        for (int i = 0; i < m_rows; ++i) {
            trace_val += m_data[i + i * m_columns];
        }
        return {trace_val};
    }

    Matrix &negate() & {
        for (T &val : m_data) {
            val = -val;
        }
        return *this;
    }

    Matrix &transpose() & {
        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_columns; ++j) {
                std::swap(m_data[i * m_rows + j], m_data[j * m_rows + i]);
            }
        }
        return *this;
    }

    bool equal(const Matrix &rhs) const { return m_data == rhs.m_data; }

    /*constexpr*/ void swap_rows(size_t lhs_idx, size_t rhs_idx) {
        auto lhs_begin = m_data.begin() + lhs_idx * m_rows;
        auto rhs_begin = m_data.begin() + rhs_idx * m_rows;

        std::swap_ranges(lhs_begin, lhs_begin + m_columns, rhs_begin);
    }

    template <typename CharT>
    void dump(std::basic_ostream<CharT> &out) const {
        for (int i = 0; i < m_rows; ++i) {
            out << "[ ";
            auto row = m_data.begin() + i * m_rows;
            std::for_each(row, row + m_columns, [&out](T cell) mutable {
                out << std::setw(2) << cell << " ";
            });
            out << "]\n";
        }
    }

    /*constexpr*/ std::optional<T> det() const {
        if (m_rows != m_columns) {
            return std::nullopt;
        }

        Matrix<T> clone{*this};
        size_t N = clone.nrows();
        char sign = 1;

        // Bareiss algotithm
        for (size_t i = 0; i < N - 1; ++i) {

            if (clone[i][i] == 0) {
                size_t pivot = 0;
                for (pivot = i + 1; pivot < N; ++pivot) {
                    if (clone[pivot][i]) {
                        clone.swap_rows(pivot, i);
                        sign = -sign;
                        break;
                    }
                }
                if (pivot == N) {
                    return {{}};
                }
            }

            for (size_t j = i + 1; j < N; ++j) {
                for (size_t k = i + 1; k < N; ++k) {
                    clone[j][k] =
                        (clone[j][k] * clone[i][i] - clone[j][i] * clone[i][k]);

                    if (i != 0) {
                        clone[j][k] /= clone[i - 1][i - 1];
                    }
                }
            }
        }
        return {clone[N - 1][N - 1]};
    }
};

template <typename T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    return lhs.equal(rhs);
}

template <typename T>
bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs) {
    return !lhs.equal(rhs);
}

template <typename T>
Matrix<T> operator+(Matrix<T> lhs, const Matrix<T> &rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
Matrix<T> operator-(Matrix<T> lhs, const Matrix<T> &rhs) {
    lhs += rhs;
    return lhs;
}

template <typename CharT, typename T>
std::basic_ostream<CharT> &operator<<(std::basic_ostream<CharT> &os,
                                      const Matrix<T> &m) {
    m.dump(os);
    return os;
}
