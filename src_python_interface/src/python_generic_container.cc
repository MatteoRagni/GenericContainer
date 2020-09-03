#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "GenericContainer.hh"

#include <algorithm>
#include <vector>

namespace py = pybind11;

using GenericContainerNamespace::bool_type;
using GenericContainerNamespace::real_type;
using GenericContainerNamespace::int_type;
using GenericContainerNamespace::long_type;
using GenericContainerNamespace::complex_type;
using GenericContainerNamespace::string_type;
using GenericContainerNamespace::pointer_type;
using GenericContainerNamespace::mat_real_type;
using GenericContainerNamespace::mat_complex_type;
using GenericContainerNamespace::GenericContainer;
using GenericContainerNamespace::TypeAllowed;

PYBIND11_MODULE(GenericContainer, m) {

  py::enum_<TypeAllowed>(m, "GenericContainerType")
  .value("NOTYPE", TypeAllowed::GC_NOTYPE)
  .value("POINTER", TypeAllowed::GC_POINTER)
  .value("BOOL", TypeAllowed::GC_BOOL)
  .value("INTEGER", TypeAllowed::GC_INTEGER)
  .value("LONG", TypeAllowed::GC_LONG)
  .value("REAL", TypeAllowed::GC_REAL)
  .value("COMPLEX", TypeAllowed::GC_COMPLEX)
  .value("STRING", TypeAllowed::GC_STRING)
  .value("VEC_POINTER", TypeAllowed::GC_VEC_POINTER)
  .value("VEC_BOOL", TypeAllowed::GC_VEC_BOOL)
  .value("VEC_INTEGER", TypeAllowed::GC_VEC_INTEGER)
  .value("VEC_REAL", TypeAllowed::GC_VEC_REAL)
  .value("VEC_COMPLEX", TypeAllowed::GC_VEC_COMPLEX)
  .value("VEC_STRING", TypeAllowed::GC_VEC_STRING)
  .value("MAT_REAL", TypeAllowed::GC_MAT_REAL)
  .value("MAT_COMPLEX", TypeAllowed::GC_MAT_COMPLEX)
  .value("VECTOR", TypeAllowed::GC_VECTOR)
  .value("MAP", TypeAllowed::GC_MAP);

  py::class_<GenericContainer>(m, "GenericContainer")
  .def(py::init([]() {
    GenericContainer gc;
    return gc;
  }))
  // Simple Types
  .def(py::init([](bool_type value) {
    GenericContainer gc;
    gc.set_bool(value);
    return gc;
  }))
  .def(py::init([](int_type value) {
    GenericContainer gc;
    gc.set_int(value);
    return gc;
  }))
  .def(py::init([](real_type value) {
    GenericContainer gc;
    gc.set_real(value);
    return gc;
  }))
  .def(py::init([](real_type value_r, real_type value_i) {
    GenericContainer gc;
    gc.set_complex(value_r, value_i);
    return gc;
  }))
  .def(py::init([](complex_type & value) {
    GenericContainer gc;
    gc.set_complex(value);
    return gc;
  }))
  .def(py::init([](string_type const & value) {
    GenericContainer gc;
    gc.set_string(value);
    return gc;
  }))
  .def(py::init([](GenericContainer & obj) {
    GenericContainer gc;
    auto target = gc.set_vector(1);
    target[0] = obj;
    return gc;
  }))
  .def(py::init([](py::handle & obj, int) {
    GenericContainer gc;
    gc.set_pointer(reinterpret_cast<void*>(obj.ptr()));
    obj.inc_ref();
    return gc;
  }))
  // Vector of simple types
  .def(py::init([](std::vector<bool_type> value) {
    GenericContainer gc;
    gc.set_vec_bool(value);
    return gc;
  }))
  .def(py::init([](std::vector<int_type> value) {
    GenericContainer gc;
    gc.set_vec_int(value);
    return gc;
  }))
  .def(py::init([](std::vector<real_type> value) {
    GenericContainer gc;
    gc.set_vec_real(value);
    return gc;
  }))
  .def(py::init([](std::vector<complex_type> value) {
    GenericContainer gc;
    gc.set_vec_complex(value);
    return gc;
  }))
  .def(py::init([](std::vector<string_type> value) {
    GenericContainer gc;
    gc.set_vec_string(value);
    return gc;
  }))
  // Disabled for coherence. A py::object MUST be inside a simple GenericContainer 
  // .def(py::init([](std::vector<py::handle> & objs, int) {
  //   GenericContainer gc;
  //   auto target = gc.set_vec_pointer((unsigned)objs.size());
  //   std::transform(std::begin(objs), std::end(objs), std::begin(target), [](py::handle & obj) {
  //     return reinterpret_cast<void *>(obj.ptr());
  //   });
  //   return gc;
  // }))
  .def(py::init([](std::vector<GenericContainer> value) {
    GenericContainer gc;
    auto target = gc.set_vector((unsigned)value.size());
    std::copy(std::begin(value), std::end(value), std::begin(target));
    return gc;
  }))
  // Matrix of real and complex type
  .def(py::init([](std::vector<std::vector<real_type>> value) {
    GenericContainer gc;
    auto n_rows = value.size();
    if (n_rows == 0) {
      py::index_error("Matrix cannot have zero rows");
    }
    auto n_cols = value[0].size();
    if (n_cols == 0){
      py::index_error("Matrix cannot have zero columns");
    }
    for (auto const & el: value) {
      if (el.size() != n_cols) {
        py::index_error("Inner domensions not valid");
      }
    }
    auto target = gc.set_mat_real((unsigned)n_rows, (unsigned)n_cols);
    std::vector<real_type> tmp_target;
    tmp_target.reserve(n_rows * n_cols);
    for (auto const & el: value) {
      tmp_target.insert(std::end(tmp_target), std::begin(el), std::end(el));
    }
    std::copy(std::begin(tmp_target), std::end(tmp_target), std::begin(target));
    return gc;
  }))
  .def(py::init([](std::vector<std::vector<complex_type>> value) {
    GenericContainer gc;
    auto n_rows = value.size();
    if (n_rows == 0) {
      py::index_error("Matrix cannot have zero rows");
    }
    auto n_cols = value[0].size();
    if (n_cols == 0){
      py::index_error("Matrix cannot have zero columns");
    }
    for (auto const & el: value) {
      if (el.size() != n_cols) {
        py::index_error("Inner domensions not valid");
      }
    }
    auto target = gc.set_mat_complex((unsigned)n_rows, (unsigned)n_cols);
    std::vector<complex_type> tmp_target;
    tmp_target.reserve(n_rows * n_cols);
    for (auto const & el: value) {
      tmp_target.insert(std::end(tmp_target), std::begin(el), std::end(el));
    }
    std::copy(std::begin(tmp_target), std::end(tmp_target), std::begin(target));
    return gc;
  }))
  // Insert methods
  .def("append", [](GenericContainer & gc, bool el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_bool(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_BOOL) {
      gc.push_bool(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("append", [](GenericContainer & gc, int el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_int(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_INTEGER) {
      gc.push_int(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("append", [](GenericContainer & gc, real_type el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_real(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_REAL) {
      gc.push_real(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("append", [](GenericContainer & gc, complex_type & el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_complex(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_COMPLEX) {
      gc.push_complex(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("append", [](GenericContainer & gc, string_type const & el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_string(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_STRING) {
      gc.push_string(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def_property_readonly("type", [](const GenericContainer & gc) {
    return gc.get_type();
  })
  .def_property_readonly("data", [](const GenericContainer & gc) -> py::object {
    switch (gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
        return py::none();
        break;
      case (TypeAllowed::GC_POINTER): 
        {
          auto obj_ptr = reinterpret_cast<PyObject *>(gc.get_pvoid());
          // from https://github.com/pybind/pybind11/issues/1141
          return py::reinterpret_borrow<py::object>(obj_ptr);
        }
        break;
      case (TypeAllowed::GC_BOOL):
        return py::cast(gc.get_bool());
        break;
      case (TypeAllowed::GC_INTEGER):
        return py::cast(gc.get_int());
        break;
      case (TypeAllowed::GC_LONG):
        return py::cast(gc.get_long());
        break;
      case (TypeAllowed::GC_REAL):
        return py::cast(gc.get_real());
        break;
      case (TypeAllowed::GC_COMPLEX):
        return py::cast(gc.get_complex());
        break;
      case (TypeAllowed::GC_STRING):
        return py::cast(gc.get_string());
        break;
      case (TypeAllowed::GC_VEC_POINTER):
        {
          std::vector<void *> origin = gc.get_vec_pointer();
          std::vector<py::object> target(origin.size());
          std::transform(std::begin(origin), std::end(origin), std::begin(target), [](void * obj_ptr) {
            auto pyobj_ptr = reinterpret_cast<PyObject *>(obj_ptr);
            // from https://github.com/pybind/pybind11/issues/1141
            return py::reinterpret_borrow<py::object>(pyobj_ptr);
          });
          return py::cast(target);
        }
        break;
      case (TypeAllowed::GC_VEC_BOOL):
        return py::cast(gc.get_vec_bool());
        break;
      case (TypeAllowed::GC_VEC_INTEGER):
        return py::cast(gc.get_vec_int());
        break;
      case (TypeAllowed::GC_VEC_REAL):
        return py::cast(gc.get_vec_real());
        break;
      case (TypeAllowed::GC_VEC_COMPLEX):
        return py::cast(gc.get_vec_complex());
        break;
      case (TypeAllowed::GC_VEC_STRING):
        return py::cast(gc.get_vec_string());
        break;
      case (TypeAllowed::GC_MAT_REAL):
        {
          const mat_real_type & origin = gc.get_mat_real();
          size_t rows = origin.numRows();
          size_t cols = origin.numCols();
          std::vector<std::vector<real_type>> target(rows, std::vector<real_type>(cols));
          for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
              target[r][c] = origin((unsigned)r, (unsigned)c);
            }
          }
          return py::cast(target);
        }
        break;
      case (TypeAllowed::GC_MAT_COMPLEX):
        {
          const mat_complex_type & origin = gc.get_mat_complex();
          size_t rows = origin.numRows();
          size_t cols = origin.numCols();
          std::vector<std::vector<complex_type>> target(rows, std::vector<complex_type>(cols));
          for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
              target[r][c] = origin((unsigned)r, (unsigned)c);
            }
          }
          return py::cast(target);
        }
        break;
      case (TypeAllowed::GC_VECTOR):
        return py::cast(gc.get_vector());
        break;
      case (TypeAllowed::GC_MAP):
        return py::cast(gc.get_map());
        break;
    }
    py::value_error("Cannot return inner object");
    return py::none();
  })
  .def("__getitem__", [](GenericContainer & gc, size_t index) -> py::object {
    switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't vectorial and cannot be accessed with single integer index");
        break;
      case (TypeAllowed::GC_VEC_POINTER):
        {
          std::vector<void *> origin = gc.get_vec_pointer();
          if (index >= origin.size())
            py::index_error("Index out of bound");
          auto pyobj_ptr = reinterpret_cast<PyObject *>(origin[index]);
          // from https://github.com/pybind/pybind11/issues/1141
          return py::reinterpret_borrow<py::object>(pyobj_ptr);
        }
        break;
      case (TypeAllowed::GC_VEC_BOOL):
        {
          bool_type value = gc.get_bool_at((unsigned)index);
          return py::cast(value);
        }
        break;
      case (TypeAllowed::GC_VEC_INTEGER):
        {
          int_type & value = gc.get_int_at((unsigned)index);
          return py::cast(value);
        }
        break;
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_MAT_REAL):
        {
          real_type & value = gc.get_real_at((unsigned)index);
          return py::cast(value);
        }
        break;
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_MAT_COMPLEX):
        {
          complex_type & value = gc.get_complex_at((unsigned)index);
          return py::cast(value);
        }
        break;
      case (TypeAllowed::GC_VEC_STRING):
        {
          std::string & value = gc.get_string_at((unsigned)index);
          return py::cast(value);
        }
        break;
      case (TypeAllowed::GC_VECTOR):
        {
          GenericContainer & value = gc.get_gc_at((unsigned)index);
          return py::cast(value);
        }
        break;
    }
    py::value_error("Cannot return inner object");
    return py::none();
  })
  .def("__getitem__", [](GenericContainer & gc, std::tuple<size_t, size_t> indexes) -> py::object {
    switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't matricial and cannot be accessed with a tuple of integer as index");
        break;
      case (TypeAllowed::GC_MAT_REAL):
        {
          size_t cols = gc.get_numCols();
          size_t rows = gc.get_numRows();
          if (std::get<0>(indexes) >= rows) {
            py::index_error("Row index out of bound");
          }
          if (std::get<1>(indexes) >= cols) {
            py::index_error("Col index out of bound");
          }
          size_t v_index = std::get<0>(indexes) * cols + std::get<1>(indexes);
          real_type & value = gc.get_mat_real()((unsigned)std::get<0>(indexes), (unsigned)std::get<1>(indexes));
          return py::cast(value);
        }
        break;
      case (TypeAllowed::GC_MAT_COMPLEX):
        {
          size_t cols = gc.get_numCols();
          size_t rows = gc.get_numRows();
          if (std::get<0>(indexes) >= rows) {
            py::index_error("Row index out of bound");
          }
          if (std::get<1>(indexes) >= cols) {
            py::index_error("Col index out of bound");
          }
          complex_type & value = gc.get_mat_complex()((unsigned)std::get<0>(indexes), (unsigned)std::get<1>(indexes));
          return py::cast(value);
        }
        break;
    }
    py::value_error("Cannot return inner object");
    return py::none();
  })
  .def("__getitem__", [](GenericContainer & gc, const std::string & element) -> py::object {
     switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAT_REAL):
      case (TypeAllowed::GC_MAT_COMPLEX):
        py::index_error("This container isn't a map and cannot be accessed with a string");
        break;
      case (TypeAllowed::GC_MAP):
        {
          GenericContainer & igc = gc(element); // this may fail
          switch(igc.get_type()) {
            case (TypeAllowed::GC_POINTER): 
              {
                auto obj_ptr = reinterpret_cast<PyObject *>(gc.get_pvoid());
                // from https://github.com/pybind/pybind11/issues/1141
                return py::reinterpret_borrow<py::object>(obj_ptr);
              }
              break;
            case (TypeAllowed::GC_BOOL):
              return py::cast(gc.get_bool());
              break;
            case (TypeAllowed::GC_INTEGER):
              return py::cast(gc.get_int());
              break;
            case (TypeAllowed::GC_LONG):
              return py::cast(gc.get_long());
              break;
            case (TypeAllowed::GC_REAL):
              return py::cast(gc.get_real());
              break;
            case (TypeAllowed::GC_COMPLEX):
              return py::cast(gc.get_complex());
              break;
            case (TypeAllowed::GC_STRING):
              return py::cast(gc.get_string());
              break;
            case (TypeAllowed::GC_NOTYPE):
            case (TypeAllowed::GC_VEC_POINTER):
            case (TypeAllowed::GC_VEC_BOOL):
            case (TypeAllowed::GC_VEC_INTEGER):
            case (TypeAllowed::GC_VEC_REAL):
            case (TypeAllowed::GC_MAT_REAL):
            case (TypeAllowed::GC_VEC_COMPLEX):
            case (TypeAllowed::GC_MAT_COMPLEX):
            case (TypeAllowed::GC_VEC_STRING):
            case (TypeAllowed::GC_VECTOR):
            case (TypeAllowed::GC_MAP):
              {
                return py::cast(igc);
              }
              break;
          }
        }
    }
    py::value_error("Cannot return inner object");
    return py::none();
  })
  .def("__setitem__", [](GenericContainer & gc, size_t index, bool value) {
     switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAT_REAL):
      case (TypeAllowed::GC_MAT_COMPLEX):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't vectorial and cannot be accessed with a tuple of integer as index");
        break;      
      case (TypeAllowed::GC_VEC_BOOL):
        if (index >= gc.get_num_elements())
          py::index_error("Index out of bound");
        gc.get_vec_bool()[index] = value;
     }
  })
  .def("__setitem__", [](GenericContainer & gc, size_t index, int_type value) {
     switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAT_REAL):
      case (TypeAllowed::GC_MAT_COMPLEX):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't vectorial and cannot be accessed with a tuple of integer as index");
        break;      
      case (TypeAllowed::GC_VEC_INTEGER):
        if (index >= gc.get_num_elements())
          py::index_error("Index out of bound");
        gc.get_vec_int()[index] = value;
     }
  })
  .def("__setitem__", [](GenericContainer & gc, size_t index, real_type value) {
     switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAT_COMPLEX):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't vectorial and cannot be accessed with a tuple of integer as index");
        break;      
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_MAT_REAL):
        if (index >= gc.get_num_elements())
          py::index_error("Index out of bound");
        gc.get_vec_real()[index] = value;
     }
  })
  .def("__setitem__", [](GenericContainer & gc, size_t index, complex_type value) {
     switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAT_REAL):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't vectorial and cannot be accessed with a tuple of integer as index");
        break;      
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_MAT_COMPLEX):
        if (index >= gc.get_num_elements())
          py::index_error("Index out of bound");
        gc.get_vec_complex()[index] = value;
     }
  })
  .def("__setitem__", [](GenericContainer & gc, std::tuple<size_t, size_t> indexes, real_type value) {
     switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAT_COMPLEX):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't matricial and cannot be accessed with a tuple of integer as index");
        break;      
      case (TypeAllowed::GC_MAT_REAL):
        if (std::get<0>(indexes) >= gc.get_numRows()) {
          py::index_error("Index out of bound");
        }
        if (std::get<1>(indexes) >= gc.get_numCols()) {
          py::index_error("Index out of bound");
        }
        gc.get_mat_real()((unsigned)std::get<0>(indexes), (unsigned)std::get<1>(indexes)) = value;
     }
  })
  .def("__setitem__", [](GenericContainer & gc, std::tuple<size_t, size_t> indexes, complex_type value) {
     switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAT_COMPLEX):
      case (TypeAllowed::GC_MAP):
        py::index_error("This container isn't matricial and cannot be accessed with a tuple of integer as index");
        break;      
      case (TypeAllowed::GC_MAT_REAL):
        if (std::get<0>(indexes) >= gc.get_numRows()) {
          py::index_error("Index out of bound");
        }
        if (std::get<1>(indexes) >= gc.get_numCols()) {
          py::index_error("Index out of bound");
        }
        gc.get_mat_complex()((unsigned)std::get<0>(indexes), (unsigned)std::get<1>(indexes)) = value;
     }
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, bool_type value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    tgc.set_bool(value);
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, int_type value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    tgc.set_int(value);
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, real_type value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    tgc.set_real(value);
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, complex_type value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    tgc.set_complex(value);
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<bool_type> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto tgc_vec = tgc.set_vec_bool((unsigned)value.size());
    std::copy(std::begin(value), std::end(value), std::begin(tgc_vec));
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<int_type> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto tgc_vec = tgc.set_vec_int((unsigned)value.size());
    std::copy(std::begin(value), std::end(value), std::begin(tgc_vec));
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<real_type> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto tgc_vec = tgc.set_vec_real((unsigned)value.size());
    std::copy(std::begin(value), std::end(value), std::begin(tgc_vec));
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<complex_type> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto tgc_vec = tgc.set_vec_complex((unsigned)value.size());
    std::copy(std::begin(value), std::end(value), std::begin(tgc_vec));
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<GenericContainer> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto tgc_vec = tgc.set_vector((unsigned)value.size());
    std::copy(std::begin(value), std::end(value), std::begin(tgc_vec));
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<std::string> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto tgc_vec = tgc.set_vec_string((unsigned)value.size());
    std::copy(std::begin(value), std::end(value), std::begin(tgc_vec));
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<std::vector<real_type>> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto n_rows = value.size();
    if (n_rows == 0) {
      py::index_error("Matrix cannot have zero rows");
    }
    auto n_cols = value[0].size();
    if (n_cols == 0){
      py::index_error("Matrix cannot have zero columns");
    }
    for (auto const & el: value) {
      if (el.size() != n_cols) {
        py::index_error("Inner domensions not valid");
      }
    }
    auto target = tgc.set_mat_real((unsigned)n_rows, (unsigned)n_cols);
    std::vector<real_type> tmp_target;
    tmp_target.reserve(n_rows * n_cols);
    for (auto const & el: value) {
      tmp_target.insert(std::end(tmp_target), std::begin(el), std::end(el));
    }
    std::copy(std::begin(tmp_target), std::end(tmp_target), std::begin(target));
  })
  .def("__setitem__", [](GenericContainer & gc, std::string & element, std::vector<std::vector<complex_type>> value) {
    if (gc.get_type() != TypeAllowed::GC_MAP) {
      gc.set_map();
    }
    auto tgc = gc[element];
    auto n_rows = value.size();
    if (n_rows == 0) {
      py::index_error("Matrix cannot have zero rows");
    }
    auto n_cols = value[0].size();
    if (n_cols == 0){
      py::index_error("Matrix cannot have zero columns");
    }
    for (auto const & el: value) {
      if (el.size() != n_cols) {
        py::index_error("Inner domensions not valid");
      }
    }
    auto target = tgc.set_mat_complex((unsigned)n_rows, (unsigned)n_cols);
    std::vector<complex_type> tmp_target;
    tmp_target.reserve(n_rows * n_cols);
    for (auto const & el: value) {
      tmp_target.insert(std::end(tmp_target), std::begin(el), std::end(el));
    }
    std::copy(std::begin(tmp_target), std::end(tmp_target), std::begin(target));
  })
  .def("__len__", [](const GenericContainer & gc) -> size_t {
    switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
        return 0;
        break;
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
        return 1;
        break;
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_MAT_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_MAT_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
      case (TypeAllowed::GC_MAP):
        return gc.get_num_elements();
        break;
    }
    return 0;
  })
  .def("__contains__", [](const GenericContainer & gc, const std::string & element) -> bool {
    switch(gc.get_type()) {
      case (TypeAllowed::GC_NOTYPE):
      case (TypeAllowed::GC_POINTER):
      case (TypeAllowed::GC_BOOL):
      case (TypeAllowed::GC_INTEGER):
      case (TypeAllowed::GC_LONG):
      case (TypeAllowed::GC_REAL):
      case (TypeAllowed::GC_COMPLEX):
      case (TypeAllowed::GC_STRING):
      case (TypeAllowed::GC_VEC_POINTER):
      case (TypeAllowed::GC_VEC_BOOL):
      case (TypeAllowed::GC_VEC_INTEGER):
      case (TypeAllowed::GC_VEC_REAL):
      case (TypeAllowed::GC_MAT_REAL):
      case (TypeAllowed::GC_VEC_COMPLEX):
      case (TypeAllowed::GC_MAT_COMPLEX):
      case (TypeAllowed::GC_VEC_STRING):
      case (TypeAllowed::GC_VECTOR):
        return false;
        break;
      case (TypeAllowed::GC_MAP):
        return gc.get_map().count(element) > 0;
        break;
    }
    return false;
  });
}