#ifndef __GAWRITER_H
#define __GAWRITER_H

#include <maya/MPlug.h>
#include <maya/MFnSet.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MIOStream.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MItDependencyGraph.h>

#include <iosfwd>

using std::ofstream;
using std::string;
using std::ostream;

class gaWriter {

public:
  /***************************************************************************/
  /**
  * Constructor and destructor.
  */
  /***************************************************************************/
  gaWriter(MDagPath dagPath, MStatus& status);

  virtual
  ~gaWriter();

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/
  virtual MStatus 
  extractGeometry();
  
  virtual MStatus 
  writeToFile(ostream& os) = 0;

protected:
  MObject
  findShader(const MObject& setNode);

  virtual MStatus
  outputSets(ostream& os);

  virtual	MStatus
  outputSingleSet(ostream& os,
                  MString setName,
                  MIntArray faces,
                  MString textureName) = 0;

  static void
  outputTabs(ostream& os, unsigned int tabCount);

  /***************************************************************************/
  /**
  * Members.
  */
  /***************************************************************************/

  MString m_fCurrentUVSetName;

  MPointArray m_fVertexArray;

  MColorArray m_fColorArray;

  MFloatVectorArray	m_fNormalArray;

  MFloatVectorArray	m_fTangentArray;

  MFloatVectorArray	m_fBinormalArray;

  MFnMesh* m_fMesh;

  MDagPath* m_fDagPath;

  MObjectArray m_fPolygonSets;

  MObjectArray m_fPolygonComponents;
};

#endif /*__GAWRITER_H*/