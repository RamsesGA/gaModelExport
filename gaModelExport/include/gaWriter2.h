#ifndef __GAWRITER2_H
#define __GAWRITER2_H

#include <maya/MPlug.h>
#include <maya/MFnSet.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MIOStream.h>
#include <maya/MIntArray.h>
#include <maya/MIOStream.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItDependencyGraph.h>

#include "gaWriter.h"

#include <time.h>
#include <iosfwd>

#define DELIMITER "\t"
#define SHAPE_DIVIDER "***************************************************************\n"
#define HEADER_LINE "=================================================================\n"
#define LINE "------------------------------------------------------------------------\n"

struct UVSet {
  MFloatArray	uArray;
  MFloatArray	vArray;
  MString		name;
  UVSet* next;
};

using std::ofstream;
using std::string;
using std::ostream;

class gaWriter2 : public gaWriter {

public:
  /***************************************************************************/
  /**
  * Constructor and destructor.
  */
  /***************************************************************************/
  gaWriter2(const MDagPath& dagPath, MStatus& status);

  ~gaWriter2() override;

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  MStatus 
  extractGeometry() override;

  MStatus 
  writeToFile(ostream& os) override;

private:
  MStatus
  outputSingleSet(ostream& os,
                  MString setName,
                  MIntArray faces,
                  MString textureName) override;

  MStatus 
  outputFaces(ostream& os);

  MStatus 
  outputVertices(ostream& os);

  //MStatus	
  //outputVertexInfo(ostream& os);

  MStatus	
  outputNormals(ostream& os);

  MStatus	
  outputTangents(ostream& os);

  MStatus	
  outputBinormals(ostream& os);

  //MStatus
  //outputColors(ostream& os);

  MStatus
  outputUVs(ostream& os);

  /***************************************************************************/
  /**
  * Members.
  */
  /***************************************************************************/

  UVSet* m_fHeadUVSet;
};

#endif /*__GAWRITER2_H*/
