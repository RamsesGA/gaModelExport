<<<<<<< Updated upstream
=======
#include <gaWriteModels.h>

>>>>>>> Stashed changes
#include "gaExportModels.h"

MTypeId gaExportModels::m_id(0x001386c3);

/*****************************************************************************/
/**
<<<<<<< Updated upstream
* Local Methods.
*/
/*****************************************************************************/

MStatus 
=======
* Methods EXPORTER.
*/
/*****************************************************************************/

MStatus
gaExportModels::writer(const MFileObject& file,
                       const MString& optionsString,
                       FileAccessMode mode) {
  MGlobal::displayInfo("Writer is working");

  const MString fileName = file.expandedFullName();

  ofstream newFile(fileName.asChar(), std::ios::out);

  if (!newFile) {
    MGlobal::displayError(fileName + ": could not be opened for reading");
    return MS::kFailure;
  }
  newFile.setf(std::ios::unitbuf);

  writeHeader(newFile);

  if (MPxFileTranslator::kExportAccessMode == mode) {
    if (MStatus::kFailure == exportAll(newFile)) {
      return MStatus::kFailure;
    }
  }
  else if (MPxFileTranslator::kExportActiveAccessMode == mode) {
    if (MStatus::kFailure == exportSelection(newFile)) {
      return MStatus::kFailure;
    }
  }
  else {
    return MStatus::kFailure;
  }

  writeFooter(newFile);
  newFile.flush();
  newFile.close();

  MGlobal::displayInfo("Export to " + fileName + " successful!");
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

bool 
gaExportModels::canBeOpened() const {
  return true;
}

MString
gaExportModels::defaultExtension() const {
  return MString(GA_FORMAT);
}

bool
gaExportModels::isVisible(MFnDagNode& fnDag, MStatus& status) {
  if (fnDag.isIntermediateObject()) {
    return false;
  }

  MPlug visPlug = fnDag.findPlug("visibility", true, &status);

  if (MStatus::kFailure == status) {
    MGlobal::displayError("MPlug::findPlug");
    return false;
  }
  else {
    bool visible;
    status = visPlug.getValue(visible);
    if (MStatus::kFailure == status) {
      MGlobal::displayError("MPlug::getValue");
    }
    return visible;
  }
}

MStatus
gaExportModels::exportAll(ostream& os) {
  MGlobal::displayInfo("Export All is working");

  MStatus status;

	MItDag itDag(MItDag::kDepthFirst, MFn::kMesh, &status);

	if (MStatus::kFailure == status) {
		MGlobal::displayError("MItDag::MItDag");
		return MStatus::kFailure;
	}

	for(;!itDag.isDone();itDag.next()) {
		MDagPath dagPath;
		if (MStatus::kFailure == itDag.getPath(dagPath)) {
			MGlobal::displayError("MDagPath::getPath");
			return MStatus::kFailure;
		}

		MFnDagNode visTester(dagPath);

		if(isVisible(visTester, status) && MStatus::kSuccess == status) {
			if (MStatus::kFailure == processPolyMesh(dagPath, os)) {
				return MStatus::kFailure;
			}
		}
	}
	return MStatus::kSuccess;
}

MStatus 
gaExportModels::exportSelection(ostream& os) {
  MStatus status;

  MSelectionList selectionList;
  if (MStatus::kFailure == MGlobal::getActiveSelectionList(selectionList)) {
    MGlobal::displayError("MGlobal::getActiveSelectionList");
    return MStatus::kFailure;
  }

  MItSelectionList itSelectionList(selectionList, MFn::kMesh, &status);
  if (MStatus::kFailure == status) {
    return MStatus::kFailure;
  }

  for (itSelectionList.reset(); !itSelectionList.isDone(); itSelectionList.next()) {
    MDagPath dagPath;

    if (MStatus::kFailure == itSelectionList.getDagPath(dagPath)) {
      MGlobal::displayError("MItSelectionList::getDagPath");
      return MStatus::kFailure;
    }

    if (MStatus::kFailure == processPolyMesh(dagPath, os)) {
      return MStatus::kFailure;
    }
  }
  return MStatus::kSuccess;
}

void 
gaExportModels::writeFooter(ostream& os) {
  os << "";
}

MStatus 
gaExportModels::processPolyMesh(const MDagPath dagPath, ostream& os) {
  MStatus status;
  gaWriterModels* pWriter = createPolyWriter(dagPath, status);

  if (MStatus::kFailure == status) {
    delete pWriter;
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == pWriter->extractGeometry()) {
    delete pWriter;
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == pWriter->writeToFile(os)) {
    delete pWriter;
    return MStatus::kFailure;
  }
  delete pWriter;

  return MStatus::kSuccess;
}

gaWriterModels*
gaExportModels::createPolyWriter(const MDagPath dagPath, MStatus& status) {
  return new gaWriterModels(dagPath, status);
}

/*****************************************************************************/
/**
* Methods RAW EXPORTER.
*/
/*****************************************************************************/

void*
gaExportModels::creator() {
  return new gaExportModels();
}

MStatus
>>>>>>> Stashed changes
initializePlugin(MObject obj) {
  MStatus status;
  MFnPlugin plugin(obj, PLUGIN_GA_ENGINE, "1.0", "Any");

  status = plugin.registerFileTranslator(PLUGIN_GA_ENGINE,
                                         "",
                                         gaExportModels::creator,
<<<<<<< Updated upstream
                                         "", 
                                         "option1=1", 
=======
                                         "",
                                         "option1=1",
>>>>>>> Stashed changes
                                         true);

  if (!status) {
    status.perror("registerFileTranslator");
    return status;
  }

  MGlobal::displayInfo("Class export models is working");

  return status;
}

<<<<<<< Updated upstream
MStatus 
=======
MStatus
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
void
gaExportModels::writeHeader(ostream& os) {
  os << "Legend:\n"
     << "Delimiter = TAB\n"
     << "() = coordinates\n"
     << "[] = vector\n"
     << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}

>>>>>>> Stashed changes
/*****************************************************************************/
/**
* Class Methods.
*/
/*****************************************************************************/

<<<<<<< Updated upstream
MStatus 
gaExportModels::initialize() {
  return MS::kFailure;
}

void* 
gaExportModels::creator() {
  return new gaExportModels();
}

=======
>>>>>>> Stashed changes
MStatus
gaExportModels::compute(const MPlug& plug, MDataBlock& data) {
  MStatus returnStatus;

  return MS::kSuccess;
}

<<<<<<< Updated upstream
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
=======
MStatus
gaExportModels::reader(const MFileObject& file,
                       const MString& options,
                       FileAccessMode mode) {
  MGlobal::displayInfo("Reader is working");

  return MS::kFailure;
}

MPxFileTranslator::MFileKind
gaExportModels::identifyFile(const MFileObject& file,
                             const char* buffer,
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
}

MStatus 
gaExportModels::exportAll(ofstream& file) {
  MGlobal::displayInfo("Export All is working");

  return MS::kFailure;
}

MString
gaExportModels::defaultExtension() const {
  return MString(GA_FORMAT);
=======
>>>>>>> Stashed changes
}