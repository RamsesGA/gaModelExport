#pragma once

/*****************************************************************************/
/**
* Includes.
*/
/*****************************************************************************/
#include <math.h>
#include <maya/MPlug.h>
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
#include <maya/MDataBlock.h>
#include <maya/MPlugArray.h>
#include <maya/MPxCommand.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatArray.h>
#include <maya/MItGeometry.h>
#include <maya/MObjectArray.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnGeometryFilter.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MItDependencyNodes.h>
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

static MSpace::Space g_space = MSpace::Space::kObject;

using std::ofstream;

/*****************************************************************************/
/**
*/
/*****************************************************************************/

/*
* Struct with my models info.
*/
struct ModelVertexInfo {
  float position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

  float normal[3] = { 0.0f, 0.0f, 0.0f };
  float tangent[3] = { 0.0f, 0.0f, 0.0f };
  float biTangent[3] = { 0.0f, 0.0f, 0.0f };

  float texCoords[2] = { 0.0f, 0.0f };

  float boneWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  unsigned int boneIds[4] = { 0, 0, 0, 0 };
};

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
  * Methods.
  */
  /***************************************************************************/

  static MStatus 
  initialize();

  static void* 
  creator();

  virtual MStatus 
  compute(const MPlug& plug, MDataBlock& data);

  MStatus
  reader(const MFileObject& file,
         const MString& options,
         FileAccessMode mode) override;

  MStatus
  writer(const MFileObject& file,
         const MString& optionsString,
         FileAccessMode mode) override;

  bool
  haveReadMethod() const override;

  bool
  haveWriteMethod() const override;

  MFileKind 
  identifyFile(const MFileObject& file,
               const char* buffer,
               short size) const override;

  MString
  filter() const override;

  MStatus
  exportAll(ofstream& file);

  MString
  defaultExtension() const;

  /***************************************************************************/
  /**
  * Members.
  */
  /***************************************************************************/

  static MTypeId m_id;
};