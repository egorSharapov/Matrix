#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <ostream>
#include <cmath>
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
        Row(const it other) : m_row(other) {}
        const T &operator[](size_t n) const { return *(m_row + n); }
        T &operator[](size_t n) { return *(m_row + n); }
    };

    void swap(Matrix &rhs) {
        std::swap(m_columns, rhs.m_columns);
        std::swap(m_rows, rhs.m_rows);
        m_data.swap(rhs.m_data);
    }

  public:
    Matrix() = default;
    Matrix(size_t rows, size_t columns, T val = {})
        : m_rows(rows), m_columns(columns) {
        m_data.reserve(m_rows * m_columns);
        std::fill_n(m_data.begin(), m_rows * m_columns, val);
    }

    Matrix(Matrix &&other) noexcept
        : m_data(std::move(other.m_data)),
          m_columns(std::exchange(other.m_columns, 0)),
          m_rows(std::exchange(other.m_rows, 0)) {}

    Matrix(const Matrix &other) = default;

    Matrix(std::initializer_list<std::initializer_list<T>> matrix) {
        m_rows = matrix.size();
        m_columns = matrix.begin()->size();
        for (const auto row : matrix) {
            for (const auto elem : row) {
                m_data.push_back(elem);
            }
        }
    }

    static Matrix<T> identity(size_t rows) {
        Matrix<T> m{rows, rows};
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < rows; ++j) {
                if (i == j) {
                    m.m_data.push_back(1);
                } else {
                    m.m_data.push_back(0);
                }
            }
        }
        return m;
    }

    Matrix &operator=(const Matrix &other) {
        if (this == std::addressof(other)) {
            return *this;
        }

        Matrix copy{other};
        copy.swap(*this);
        return *this;
    }

    Matrix &operator=(Matrix &&other) noexcept {
        if (this == std::addressof(other)) {
            return *this;
        }

        Matrix move{std::move(other)};
        move.swap(*this);
        return *this;
    }

    Matrix &operator+=(const Matrix &other) {
        if (equivalency(other)) {
            for (int i = 0; i < m_data.size(); ++i) {
                m_data[i] += other.m_data[i];
            }
        }
        return *this;
    }

    Matrix &operator-=(const Matrix &other) {
        if (equivalency(other)) {
            for (int i = 0; i < m_data.size(); ++i) {
                m_data[i] -= other.m_data[i];
            }
        }
        return *this;
    }

    Row operator[](size_t n) { return {m_data.begin() + m_rows * n}; }

    bool equivalency(const Matrix &rhs) const {
        return (m_columns == rhs.m_columns) && (m_rows == rhs.m_rows);
    }

    size_t ncolumns() const { return m_columns; }
    size_t nrows() const { return m_rows; }

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

    template <typename CharT>
    void dump(std::basic_ostream<CharT> &out) const {
        for (int i = 0; i < m_columns; ++i) {
            out << "[ ";
            for (int j = 0; j < m_rows; ++j) {
                out << std::setw(4) << m_data[i * m_rows + j] << " ";
            }
            out << "]\n";
        }
    }

    std::optional<long double> det() const {
        if (m_rows != m_columns) {
            return std::nullopt;
        }

        Matrix<long double> clone{m_rows, m_columns};
        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_rows; ++j) {
                clone[i][j] = m_data[i * m_rows + j];
            }
        }
        long double determine = 1;

        for (int k = 0; k < m_rows; ++k) {
            if (std::abs(clone[k][k]) <
                std::numeric_limits<long double>::epsilon()) {
                continue;
            }
            long double D = clone[k][k];
            determine *= D;

            for (int i = 0; i < m_columns; ++i) {
                clone[k][i] = clone[k][i] / D;
            }
            for (int i = k + 1; i < m_rows; ++i) {
                long double K = clone[i][k] / clone[k][k];
                for (int j = 0; j < m_columns; ++j) {
                    clone[i][j] -= clone[k][j] * K;
                }
            }
        }
        // for (int i = 0; i < clone.nrows(); ++i) {
        //     determine *= clone[i][i];
        // }
        return {determine};
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
