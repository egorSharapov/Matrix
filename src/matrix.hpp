#include <algorithm>
#include <optional>
#include <ostream>
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

  public:
    Matrix() = default;
    Matrix(size_t rows, size_t columns, T val = {})
        : m_rows(rows), m_columns(columns) {
        m_data.reserve(m_rows * m_columns);
        std::fill_n(m_data.begin(), m_rows * m_columns, val);
    }

    Matrix(Matrix &&other)
        : m_data(std::move(other.m_data)), m_columns(other.m_columns),
          m_rows(other.m_rows) {
        other.m_columns = 0;
        other.m_rows = 0;
    }

    Matrix &operator=(const Matrix &other) {
        m_data = other.m_data;
        m_columns = other.m_columns;
        m_rows = other.m_rows;
        return *this;
    }

    Matrix &operator=(Matrix &&other) {
        m_data = std::move(other.m_data);
        m_columns = other.m_columns;
        m_rows = other.m_rows;
        other.m_rows = 0;
        other.m_columns = 0;
        return *this;
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

    Row operator[](size_t n) { return {m_data.begin() + m_rows * n}; }

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
    void dump(std::ostream &out) const {
        for (int i = 0; i < m_columns; ++i) {
            out << "| ";
            for (int j = 0; j < m_rows; ++j) {
                out << m_data[i * m_columns + j] << " ";
            }
            out << "|\n";
        }
    }

    T det() {
        /*
        soon
        */
    }
};
