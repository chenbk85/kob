#ifndef KOB_KSL_MATRIX_H_
#define KOB_KSL_MATRIX_H_
#include <kob/profile/prof.h>
#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <iostream>

namespace ksl {

template <typename Type>
class matrix {
public:

    matrix()
      :_rows(0),
       _columns(0),
       _data()
    {

    }

    matrix(size_t rows, size_t columns, const Type& initial = Type())
        : _rows(rows), _columns(columns),
          _data(_rows * _columns, initial) { }

    explicit matrix(size_t rows_columns, const Type& initial = Type())
        : _rows(rows_columns), _columns(rows_columns),
          _data(_rows * _columns, initial) { }
#if KOB_COMPILER_CPP11_ENABLED
    matrix(size_t rows, size_t columns, std::vector<Type>&& data)
        : _rows(rows), _columns(columns),
          _data(std::move(data)) {
        assert(_data.size() == _rows * _columns);
    }
#endif

    size_t row_size() const { return _rows; }

    //! number of columns in matrix
    size_t column_size() const { return _columns; }

    //! raw data of matrix
    Type * data() const { return _data.data(); }

    //! size of matrix raw data (rows * columns)
    size_t size() const { return _data.size(); }

    //! return reference to element at cell
    const Type& at(size_t row, size_t column) const
    {
        assert(row < _rows);
        assert(column < _columns);
        return _data[_columns * row + column];
    }

    //! return reference to element at cell
    Type& at(size_t row, size_t column)
    {
        assert(row < _rows);
        assert(column < _columns);
        return _data[_columns * row + column];
    }

    //! return reference to element at cell
    const Type& operator () (size_t row, size_t column) const
    {
        return at(row, column);
    }

    //! return reference to element at cell
    Type& operator () (size_t row, size_t column)
    {
        return at(row, column);
    }

    //! add matrix to this one
    matrix& operator += (const matrix& b)
    {
        assert(rowSize() == b.rowSize() && columnSize() == b.columnSize());
        std::transform(b._data.begin(), b._data.end(),
                       _data.begin(), _data.begin(), std::plus<Type>());
        return *this;
    }

    //! add two matrix to this one
    matrix operator + (const matrix& b) const
    {
        assert(rowSize() == b.rowSize() && columnSize() == b.columnSize());
        Matrix n = *this;
        return (n += b);
    }

    //! multiple matrix with a scalar
    matrix& operator *= (const Type& s) {
        for(size_t i = 0; i < _data.size(); i++) {
          _data[i] *= s;
        }
        return *this;
    }

    //! equality operator
    bool operator == (const matrix& b) const
    {
        if (rowSize() != b.rowSize() || columnSize() != b.columnSize()) return false;
        return std::equal(_data.begin(), _data.end(), b._data.begin());
    }

    //! inequality operator
    bool operator != (const matrix& b) const
    {
        return !operator == (b);
    }

private:
    size_t            _rows ;
    size_t            _columns;
    std::vector<Type> _data;
};

/*multiple*/

template <typename Type>

struct TypeInit {
  void operator()(Type &t) {
    t = Type();
  }
};

template <typename Type, typename Initor>

inline matrix<Type> matrix_multiple(const matrix<Type> &l, const matrix<Type> &r, Initor init = TypeInit<Type>())
{
 size_t row = l.rowSize();
 size_t col = r.columnSize();
 size_t collimit = l.columnSize();
  assert(l.columnSize() == r.rowSize());
  matrix<Type> ret(row, col);
  Type tmp;
  for(size_t i = 0; i < row; i++) {
    for(size_t j = 0; j < col; j++) {
      init(tmp);
      for(size_t k = 0; k < collimit; k++) {
        tmp +=l.at(i,k) * r.at(k,j);
      }
      
      ret.at(i, j) += tmp;
    }
  }

  return ret;
}
}
#endif 
