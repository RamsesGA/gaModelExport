#pragma once

/*****************************************************************************/
/**
* Includes.
*/
/*****************************************************************************/
#include <math.h>
#include <maya/MPlug.h>
<<<<<<< Updated upstream
=======
#include <maya/MItDag.h>
>>>>>>> Stashed changes
#include <maya/MFnSet.h>
#include <maya/MPxNode.h> 
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MVector.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MIOStream.h>
#include <maya/MFnPlugin.h>
<<<<<<< Updated upstream
=======
#include <maya/MDistance.h>
>>>>>>> Stashed changes
#include <maya/MDataBlock.h>
#include <maya/MPlugArray.h>
#include <maya/MPxCommand.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatArray.h>
#include <maya/MItGeometry.h>
<<<<<<< Updated upstream
#include <maya/MObjectArray.h>
=======
#include <maya/MItMeshEdge.h>
#include <maya/MPointArray.h>
#include <maya/MObjectArray.h>
#include <maya/MItMeshVertex.h>
>>>>>>> Stashed changes
#include <maya/MSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnGeometryFilter.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPxFileTranslator.h>
<<<<<<< Updated upstream
#include <maya/MItDependencyNodes.h>
=======
#include <maya/MFloatVectorArray.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItDependencyGraph.h>
>>>>>>> Stashed changes
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnWeightGeometryFilter.h>
#include <maya/MFnSingleIndexedComponent.h>

//No Maya includes
#include <vector>
#include <fstream>

/*****************************************************************************/
/**
* Defines and global statics.
*/
/*****************************************************************************/
#define	PLUGIN_GA_ENGINE "gaModels"
#define GA_FORMAT "gam"

<<<<<<< Updated upstream
static MSpace::Space g_space = MSpace::Space::kObject;

using std::ofstream;

/*****************************************************************************/
/**
*/
/*****************************************************************************/

/*
* Struct with my models info.
*/
=======
using std::ofstream;
using std::string;
using std::vector;
using std::ostream;

class gaWriterModels;

/*****************************************************************************/
/*****************************************************************************/

/*
>>>>>>> Stashed changes
struct ModelVertexInfo {
  float position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

  float normal[3] = { 0.0f, 0.0f, 0.0f };
  float tangent[3] = { 0.0f, 0.0f, 0.0f };
  float biTangent[3] = { 0.0f, 0.0f, 0.0f };

  float texCoords[2] = { 0.0f, 0.0f };

  float boneWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  unsigned int boneIds[4] = { 0, 0, 0, 0 };
};

<<<<<<< Updated upstream
/*
* Struct with general info of one mesh.
*/
struct MeshInfo {
  std::string meshName;
  std::vector<ModelVertexInfo> vModelVertexInfo;
  std::vector<unsigned int> vIndexInfo;
};

/*
* Struct with all models in Maya's scene.
*/
struct MayaModels {

};
=======
struct MeshInfo {
  string meshName;
  vector<ModelVertexInfo> vModelVertexInfo;
  vector<unsigned int> vIndexInfo;
};

struct MayaModels {
};
*/
>>>>>>> Stashed changes

/*
* 
*/
class gaExportModels : public MPxFileTranslator
{
public:
  /***************************************************************************/
  /**
  * Constructor and destructor.
  */
  /***************************************************************************/
  gaExportModels() = default;

  ~gaExportModels() = default;

  /***************************************************************************/
  /**
<<<<<<< Updated upstream
  * Methods.
  */
  /***************************************************************************/

  static MStatus 
  initialize();
=======
  * Methods EXPORTER.
  */
  /***************************************************************************/

  MStatus
  writer(const MFileObject& file,
         const MString& optionsString,
         FileAccessMode mode) override;

  bool
  haveReadMethod() const override;

  bool
  haveWriteMethod() const override;

  bool
  canBeOpened() const override;

  MString
  defaultExtension() const;

  bool
  isVisible(MFnDagNode& fnDag, MStatus& status);

  MStatus
  exportAll(ostream& os);

  MStatus
  exportSelection(ostream& os);

  void
  writeFooter(ostream& os);

  MStatus
  processPolyMesh(const MDagPath dagPath, ostream& os);

  gaWriterModels*
  createPolyWriter(const MDagPath dagPath, MStatus& status);

  /***************************************************************************/
  /**
  * Methods RAW EXPORTER.
  */
  /***************************************************************************/
>>>>>>> Stashed changes

  static void* 
  creator();

<<<<<<< Updated upstream
=======
  void
  writeHeader(ostream& os);

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

>>>>>>> Stashed changes
  virtual MStatus 
  compute(const MPlug& plug, MDataBlock& data);

  MStatus
  reader(const MFileObject& file,
         const MString& options,
         FileAccessMode mode) override;

<<<<<<< Updated upstream
  MStatus
  writer(const MFileObject& file,
         const MString& optionsString,
         FileAccessMode mode) override;

  bool
  haveReadMethod() const override;

  bool
  haveWriteMethod() const override;

=======
>>>>>>> Stashed changes
  MFileKind 
  identifyFile(const MFileObject& file,
               const char* buffer,
               short size) const override;

  MString
  filter() const override;

<<<<<<< Updated upstream
  MStatus
  exportAll(ofstream& file);

  MString
  defaultExtension() const;

=======
>>>>>>> Stashed changes
  /***************************************************************************/
  /**
  * Members.
  */
  /***************************************************************************/

  static MTypeId m_id;
};