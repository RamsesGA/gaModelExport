#include "gaExportModels.h"

MTypeId gaExportModels::m_id(0x001386c3);

/*****************************************************************************/
/**
* Local Methods.
*/
/*****************************************************************************/

MStatus 
initializePlugin(MObject obj) {
  MStatus status;
  MFnPlugin plugin(obj, PLUGIN_GA_ENGINE, "1.0", "Any");

  status = plugin.registerFileTranslator(PLUGIN_GA_ENGINE,
                                         "",
                                         gaExportModels::creator,
                                         "", 
                                         "option1=1", 
                                         true);

  if (!status) {
    status.perror("registerFileTranslator");
    return status;
  }

  MGlobal::displayInfo("Class export models is working");

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

  MGlobal::displayInfo("Class export models is off");

  return status;
}

/*****************************************************************************/
/**
* Class Methods.
*/
/*****************************************************************************/

MStatus 
gaExportModels::initialize() {
  return MS::kFailure;
}

void* 
gaExportModels::creator() {
  return new gaExportModels();
}

MStatus
gaExportModels::compute(const MPlug& plug, MDataBlock& data) {
  MStatus returnStatus;

  return MS::kSuccess;
}

MStatus 
gaExportModels::reader(const MFileObject& file, 
                       const MString& options, 
                       FileAccessMode mode) {
  MGlobal::displayInfo("Reader is working");

  const MString fname = file.fullName();

  MStatus rval(MS::kSuccess);

  const int maxLineSize = 1024;
  char buf[maxLineSize];

  return MS::kFailure;

  /*
  ifstream inputfile(fname.asChar(), ios::in);
  if (!inputfile) {
    // open failed
    cerr << fname << ": could not be opened for reading\n";
    return MS::kFailure;
  }
  if (!inputfile.getline(buf, maxLineSize)) {
    cerr << "file " << fname << " contained no lines ... aborting\n";
    return MS::kFailure;
  }
  if (0 != strncmp(buf, magic.asChar(), magic.length())) {
    cerr << "first line of file " << fname;
    cerr << " did not contain " << magic.asChar() << " ... aborting\n";
    return MS::kFailure;
  }
  while (inputfile.getline(buf, maxLineSize)) {
    //processing each line of the file
    MString cmdString;
    cmdString.set(buf);
    if (!MGlobal::executeCommand(cmdString))
      rval = MS::kFailure;
  }
  inputfile.close();
  return rval;
  */
}

MStatus 
gaExportModels::writer(const MFileObject& file, 
                       const MString& optionsString,
                       FileAccessMode mode) {
  MGlobal::displayInfo("Writer is working");

  const MString fileName = file.fullName();

  ofstream newFile(fileName.asChar(), ios::out);

  if (!(newFile)) {
    MGlobal::displayError("\nError, trying to load " + fileName);
    return MS::kFailure;
  }

  newFile.setf(ios::unitbuf);

  if (MPxFileTranslator::kExportAccessMode == mode) {
    if (MS::kFailure == exportAll(newFile)) {
      MGlobal::displayError("\nError, trying to export All\n");
      return MS::kFailure;
    }
  }
  else {
    MGlobal::displayError("\nError\n");
    return MS::kFailure;
  }

  newFile.flush();
  newFile.close();

  MGlobal::displayInfo("\nExport to " + fileName + " successful\n");
  return MS::kSuccess;
}

bool
gaExportModels::haveReadMethod() const {
  return false;
  //return true;
}

bool
gaExportModels::haveWriteMethod() const {
  //return false;
  return true;
}

MPxFileTranslator::MFileKind 
gaExportModels::identifyFile(const MFileObject& file, 
                             const char* buffer, 
                             short size) const {
  MGlobal::displayInfo("Identify File is working");

  const char* name = file.name().asChar();
  int nameLength = (int)strlen(name);

  if ((nameLength > 5) && !strcasecmp(name + nameLength - 5, ".gam")) {
    return (kIsMyFileType);
  }

  return (kNotMyFileType);
}

MString
gaExportModels::filter() const {
  return "*.gam";
}

MStatus 
gaExportModels::exportAll(ofstream& file) {
  MGlobal::displayInfo("Export All is working");

  return MS::kFailure;
}

MString
gaExportModels::defaultExtension() const {
  return MString(GA_FORMAT);
}