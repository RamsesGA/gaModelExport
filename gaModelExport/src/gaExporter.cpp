#include "gaExporter.h"
#include "gaWriter.h"

MStatus 
gaExporter::writer(const MFileObject& file,
                   const MString& optionsString,
                   MPxFileTranslator::FileAccessMode mode) {
  const MString fileName = file.expandedFullName();

  ofstream newFile(fileName.asChar(), ios::out);

  MGlobal::displayInfo("Writer is working\n\n");

  if (!newFile) {
    MGlobal::displayError(fileName + ": could not be opened for reading");
    return MS::kFailure;
  }
  newFile.setf(ios::unitbuf);

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

  MGlobal::displayInfo("Export to " + fileName + " successful!\n\n");
  return MS::kSuccess;
}

bool
gaExporter::haveWriteMethod() const {
  return true;
}

bool
gaExporter::haveReadMethod() const{
  return false;
}

bool 
gaExporter::canBeOpened() const {
  return true;
}

MStatus 
gaExporter::exportAll(ostream& os) {
  MStatus status;

  MGlobal::displayInfo("Export All is working\n\n");

  MItDag itDag(MItDag::kDepthFirst, MFn::kMesh, &status);

  if (MStatus::kFailure == status) {
    MGlobal::displayError("MItDag::MItDag");

    return MStatus::kFailure;
  }

  for (; !itDag.isDone(); itDag.next()) {
    MDagPath dagPath;
    if (MStatus::kFailure == itDag.getPath(dagPath)) {
      MGlobal::displayError("MDagPath::getPath");

      return MStatus::kFailure;
    }

    MFnDagNode visTester(dagPath);

    if (isVisible(visTester, status) && MStatus::kSuccess == status) {
      if (MStatus::kFailure == processPolyMesh(dagPath, os)) {
        return MStatus::kFailure;
      }
    }
  }

  return MStatus::kSuccess;
}

MStatus 
gaExporter::exportSelection(ostream& os) {
  MStatus status;

  MGlobal::displayInfo("Export Selection is working\n\n");

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

MStatus 
gaExporter::processPolyMesh(const MDagPath dagPath, ostream& os) {
  MStatus status;
  gaWriter* pWriter = createPolyWriter(dagPath, status);

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

bool 
gaExporter::isVisible(MFnDagNode& fnDag, MStatus& status) {
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

void
gaExporter::writeHeader(ostream& os) {
  os << "";
}

void 
gaExporter::writeFooter(ostream& os) {
  os << "";
}