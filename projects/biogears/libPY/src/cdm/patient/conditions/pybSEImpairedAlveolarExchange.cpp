#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEImpairedAlveolarExchange(py::module_ &m)
{

  py::class_<biogears::SEImpairedAlveolarExchange, biogears::SEPatientCondition>(m, "SEImpairedAlveolarExchange")
    .def(py::init<>())
    .def("Clear", &biogears::SEImpairedAlveolarExchange::Clear)
    .def("IsValid", &biogears::SEImpairedAlveolarExchange::IsValid)
    .def("Load", &biogears::SEImpairedAlveolarExchange::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEImpairedAlveolarExchange::Unload, py::const_))
    .def("GetName", &biogears::SEImpairedAlveolarExchange::GetName)
    .def("GetName_cStr", &biogears::SEImpairedAlveolarExchange::GetName_cStr)
    .def("HasImpairedSurfaceArea", &biogears::SEImpairedAlveolarExchange::HasImpairedSurfaceArea)
    .def("GetImpairedSurfaceArea", py::overload_cast<>(&biogears::SEImpairedAlveolarExchange::GetImpairedSurfaceArea))
    .def("GetImpairedSurfaceArea", py::overload_cast<const biogears::AreaUnit&>(&biogears::SEImpairedAlveolarExchange::GetImpairedSurfaceArea, py::const_))
    .def("HasImpairedFraction", &biogears::SEImpairedAlveolarExchange::HasImpairedFraction)
    .def("GetImpairedFraction", py::overload_cast<>(&biogears::SEImpairedAlveolarExchange::GetImpairedFraction))
    .def("GetImpairedFraction", py::overload_cast<>(&biogears::SEImpairedAlveolarExchange::GetImpairedFraction, py::const_))

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEImpairedAlveolarExchange::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
