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
  .def(py::init([](std::vector<py::handle> & objs, int) {
    GenericContainer gc;
    auto target = gc.set_vec_pointer(objs.size());
    std::transform(std::begin(objs), std::end(objs), std::begin(target), [](py::handle & obj) {
      return reinterpret_cast<void *>(obj.ptr());
    });
    return gc;
  }))
  .def(py::init([](std::vector<GenericContainer> value) {
    GenericContainer gc;
    auto target = gc.set_vector(value.size());
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
    auto target = gc.set_mat_real(n_rows, n_cols);
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
    auto target = gc.set_mat_complex(n_rows, n_cols);
    std::vector<complex_type> tmp_target;
    tmp_target.reserve(n_rows * n_cols);
    for (auto const & el: value) {
      tmp_target.insert(std::end(tmp_target), std::begin(el), std::end(el));
    }
    std::copy(std::begin(tmp_target), std::end(tmp_target), std::begin(target));
    return gc;
  }))
  // Insert methods
  .def("insert", [](GenericContainer & gc, bool el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_bool(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_BOOL) {
      gc.push_bool(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("insert", [](GenericContainer & gc, int el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_int(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_INTEGER) {
      gc.push_int(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("insert", [](GenericContainer & gc, real_type el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_real(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_REAL) {
      gc.push_real(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("insert", [](GenericContainer & gc, complex_type & el) {
    if (gc.get_type() == TypeAllowed::GC_NOTYPE) {
      gc.set_vec_complex(1);
    }
    if (gc.get_type() == TypeAllowed::GC_VEC_COMPLEX) {
      gc.push_complex(el);
      return;
    }
    py::value_error("Incompatible generic container");
  })
  .def("insert", [](GenericContainer & gc, string_type const & el) {
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
          std::transform(std::begin(origin), std::end(origin), std::begin(target))
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
              target[r][c] = origin(r, c);
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
              target[r][c] = origin(r, c);
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
  });/*
  .def("__getitem__", [](const GenericContainer & gc, size_t index) {
    switch(gc.get_type) {
      case(GC_NOTYPE):
      case(GC_POINTER):
      case(GC_BOOL):
      case(GC_INTEGER):
      case(GC_LONG):
      case(GC_REAL):
      case(GC_COMPLEX):
      case(GC_STRING):
      case(GC_MAP):
        py::index_error("This container isn't vectorial");
      case(VEC_POINTER):


    }
      
  })*/


}