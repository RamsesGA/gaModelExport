#ifndef __GAEXPORT_H
#define __GAEXPORT_H

#include <maya/MPlug.h>
#include <maya/MItDag.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MPxFileTranslator.h>

#include <iosfwd>
#include <fstream>
#include <ios>

class gaWriter;

using std::ofstream;
using std::string;
using std::ostream;

class gaExporter : public MPxFileTranslator {

public:
  /***************************************************************************/
  /**
  * Constructor and destructor.
  */
  /***************************************************************************/
  gaExporter() = default;

  ~gaExporter() = default;

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  MStatus
  writer(const MFileObject& file,
         const MString& optionsString,
         MPxFileTranslator::FileAccessMode mode) override;

  bool
  haveWriteMethod() const override;

  bool
  haveReadMethod() const override;

  bool
  canBeOpened() const override;

  MString
  defaultExtension() const override = 0;

protected:
  virtual	bool
  isVisible(MFnDagNode& fnDag, MStatus& status);

  virtual	MStatus
  exportAll(ostream& os);

  virtual	MStatus
  exportSelection(ostream& os);

  virtual void
  writeHeader(ostream& os);

  virtual void
  writeFooter(ostream& os);

  virtual MStatus
  processPolyMesh(const MDagPath dagPath, ostream& os);

  virtual gaWriter*
  createPolyWriter(const MDagPath dagPath, MStatus& status) = 0;
};

#endif /*__GAEXPORT_H*/