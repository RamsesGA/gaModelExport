#include "gaExporter2.h"
#include "gaWriter2.h"

MTypeId gaExporter2::m_id(0x001386c3);

/***************************************************************************************/
/**
* .
*/
/***************************************************************************************/

MStatus
initializePlugin(MObject obj) {
  MStatus status;
  MFnPlugin plugin(obj, PLUGIN_GA_ENGINE, "1.0", "Any");

  status = plugin.registerFileTranslator(PLUGIN_GA_ENGINE,
                                         "",
                                         gaExporter2::creator,
                                         "",
                                         "option1=1",
                                         true);
  if (!status) {
    status.perror("registerFileTranslator");
    return status;
  }

  MGlobal::displayInfo("\nInitialize exporter 2 is working\n\n");

  return status;
}

MStatus
uninitializePlugin(MObject obj) {
  MStatus status;
  MFnPlugin plugin(obj);

  status = plugin.deregisterFileTranslator(PLUGIN_GA_ENGINE);

  if (!status) {
    status.perror("deregisterFileTranslator");
    return status;
  }

  MGlobal::displayInfo("\nUninitialize exporter 2 is ending\n\n");

  return status;
}

/***************************************************************************************/
/**
* Methods.
*/
/***************************************************************************************/

void* 
gaExporter2::creator() {
  return new gaExporter2();
}

MString 
gaExporter2::defaultExtension() const {
  return MString(GA_FORMAT);
}

gaWriter*
gaExporter2::createPolyWriter(const MDagPath dagPath, MStatus& status) {
  return new gaWriter2(dagPath, status);
}

void 
gaExporter2::writeHeader(ostream& os) {
  os << "Legend: How to understand the code\n"
     << "Delimiter = TAB\n"
     << "Coordinates = ( )\n"
     << "Vector = [ ]\n"
     << "**************************************************************************\n\n";
}

MString 
gaExporter2::filter() const {
  return "*.gam";
}

MPxFileTranslator::MFileKind 
gaExporter2::identifyFile(const MFileObject& file, 
                          const char* buffer, 
                          short size) const {
  const char* name = file.name().asChar();
  int nameLength = (int)strlen(name);

  if ((nameLength > 5) && !strcasecmp(name + nameLength - 5, ".gam")) {
    return (kIsMyFileType);
  }

  return (kNotMyFileType);
}