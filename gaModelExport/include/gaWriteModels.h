#pragma once

#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MPlug.h>
#include <maya/MItMeshPolygon.h>

#include <iosfwd>

using std::ofstream;
using std::string;
using std::ostream;

struct UVSet {
  MFloatArray	uArray;
  MFloatArray	vArray;
  MString		name;
  UVSet* next;
};

#define DELIMITER "\t"
#define SHAPE_DIVIDER "****************************************************************\n"
#define HEADER_LINE "=================================================================\n"
#define LINE "------------------------------------------------------------------------\n"

class gaWriterModels {

public:
  /***************************************************************************/
  /**
  * Constructor and destructor.
  */
  /***************************************************************************/
  gaWriterModels(MDagPath dagPath, MStatus& status);

  ~gaWriterModels() = default;

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  MStatus		
  extractGeometry();

  MStatus
  writeToFile(ostream& os);

  MObject
  findShader(const MObject& setNode);

  MStatus
  outputSets(ostream& os);

  MStatus 
  outputSingleSet(ostream& os,
                  MString setName,
                  MIntArray faces,
                  MString textureName);

  static void 
  outputTabs(std::ostream& os, unsigned int tabCount);

  MStatus 
  outputFaces(ostream& os);

  MStatus 
  outputVertices(ostream& os);

  MStatus	
  outputVertexInfo(ostream& os);

  MStatus	
  outputNormals(ostream& os);

  MStatus	
  outputTangents(ostream& os);

  MStatus	
  outputBinormals(ostream& os);

  MStatus	
  outputColors(ostream& os);

  MStatus	
  outputUVs(ostream& os);

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

  UVSet* m_fHeadUVSet;
};