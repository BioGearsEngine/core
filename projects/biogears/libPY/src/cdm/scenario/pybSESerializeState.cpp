#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SESerializeState.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSESerializeState, m)
{

  py::class_<biogears::SESerializeState, biogears::SEAction>(m, "SESerializeState")
    .def(py::init<>())
    .def("TypeTag", &biogears::SESerializeState::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SESerializeState::Unload, py::const_))
    .def("Clear", &biogears::SESerializeState::Clear)
    .def("IsValid", py::overload_cast<>(&biogears::SESerializeState::IsValid, py::const_))
    .def("Load", &biogears::SESerializeState::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESerializeState::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SESerializeState::ToString, py::const_))

    .def("GetType", &biogears::SESerializeState::GetType)
    .def("SetType", &biogears::SESerializeState::SetType)
    .def("HasType", &biogears::SESerializeState::HasType)
    .def("InvalidateType", &biogears::SESerializeState::InvalidateType)
    .def("HasFilename", &biogears::SESerializeState::HasFilename)
    .def("GetFilename", &biogears::SESerializeState::GetFilename)
    .def("GetFilename_cStr", &biogears::SESerializeState::GetFilename_cStr)
    .def("SetFilename", py::overload_cast<const char*>(&biogears::SESerializeState::SetFilename))
    .def("SetFilename", py::overload_cast<const std::string&>(&biogears::SESerializeState::SetFilename))
    .def("InvalidateFilename", &biogears::SESerializeState::InvalidateFilename);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}