#include "gaWriter2.h"

/***************************************************************************************/
/**
* Constructor and destructor.
*/
/***************************************************************************************/

gaWriter2::gaWriter2(const MDagPath& dagPath, MStatus& status) : gaWriter(dagPath, status), 
                                                                 m_fHeadUVSet(nullptr) { }

gaWriter2::~gaWriter2() {
  if (nullptr != m_fHeadUVSet) delete m_fHeadUVSet;
}

/***************************************************************************************/
/**
* Methods.
*/
/***************************************************************************************/

MStatus 
gaWriter2::extractGeometry() {
  MGlobal::displayInfo("Extract geometry is working\n\n");

  if (MStatus::kFailure == gaWriter::extractGeometry()) {
    return MStatus::kFailure;
  }

  MStringArray uvSetNames;
  if (MStatus::kFailure == m_fMesh->getUVSetNames(uvSetNames)) {
    MGlobal::displayError("MFnMesh::getUVSetNames");

    return MStatus::kFailure;
  }

  unsigned int uvSetCount = uvSetNames.length();
  unsigned int i;

  UVSet* currUVSet = nullptr;

  for (i = 0; i < uvSetCount; ++i) {
    if (0 == i) {
      currUVSet = new UVSet;
      m_fHeadUVSet = currUVSet;
    }
    else {
      currUVSet->next = new UVSet;
      currUVSet = currUVSet->next;
    }

    currUVSet->name = uvSetNames[i];
    currUVSet->next = nullptr;

    if (MStatus::kFailure == m_fMesh->getUVs(currUVSet->uArray, 
                                             currUVSet->vArray, 
                                             &currUVSet->name)) {
      return MStatus::kFailure;
    }
  }

  return MStatus::kSuccess;
}

MStatus 
gaWriter2::writeToFile(ostream& os) {
  MGlobal::displayInfo("Exporting " + m_fMesh->partialPathName() + "\n\n");

  os << SHAPE_DIVIDER;
  os << "Shape or mesh name:  " << m_fMesh->partialPathName() << "\n";
  os << SHAPE_DIVIDER;
  os << "\n";

  if (MStatus::kFailure == outputFaces(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputVertices(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputUVs(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputTangents(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputNormals(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputBinormals(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputSets(os)) {
    return MStatus::kFailure;
  }
  os << "\n\n";

  return MStatus::kSuccess;

  /*
  if (MStatus::kFailure == outputVertexInfo(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputColors(os)) {
    return MStatus::kFailure;
  }
  */
}

MStatus 
gaWriter2::outputFaces(ostream& os) {
  unsigned int faceCount = m_fMesh->numPolygons();
  if (0 == faceCount) {
    return MStatus::kFailure;
  }

  MStatus status;
  MIntArray indexArray;

  os << HEADER_LINE;
  os << "Faces:  " << faceCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|Vertex Indices\n";
  os << LINE;

  for (unsigned int i = 0; i < faceCount; ++i) {
    os << i << DELIMITER;

    unsigned int indexCount = m_fMesh->polygonVertexCount(i, &status);
    if (MStatus::kFailure == status) {
      MGlobal::displayError("MFnMesh::polygonVertexCount");

      return MStatus::kFailure;
    }

    status = m_fMesh->getPolygonVertices(i, indexArray);
    if (MStatus::kFailure == status) {
      MGlobal::displayError("MFnMesh::getPolygonVertices");

      return MStatus::kFailure;
    }

    for (unsigned int j = 0; j < indexCount; ++j) {
      os << indexArray[j] << " ";
    }

    os << "\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus 
gaWriter2::outputVertices(ostream& os) {
  unsigned int vertexCount = m_fVertexArray.length();

  if (0 == vertexCount) {
    return MStatus::kFailure;
  }

  os << HEADER_LINE;
  os << "Vertices:  " << vertexCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Vertex|(x, y, z)\n";
  os << LINE;

  for (unsigned int i = 0; i < vertexCount; ++i) {
    os << i << DELIMITER << "("
       << m_fVertexArray[i].x << ", "
       << m_fVertexArray[i].y << ", "
       << m_fVertexArray[i].z << ")\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus 
gaWriter2::outputNormals(ostream& os) {
  unsigned int normalCount = m_fNormalArray.length();
  if (0 == normalCount) {
    return MStatus::kFailure;
  }

  os << HEADER_LINE;
  os << "Normals:  " << normalCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|[x, y, z]\n";
  os << LINE;

  for (unsigned int i = 0; i < normalCount; ++i) {
    os << i << DELIMITER << "["
       << m_fNormalArray[i].x << ", "
       << m_fNormalArray[i].y << ", "
       << m_fNormalArray[i].z << "]\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus 
gaWriter2::outputTangents(ostream& os) {
  unsigned int tangentCount = m_fTangentArray.length();
  if (0 == tangentCount) {
    return MStatus::kFailure;
  }

  os << HEADER_LINE;
  os << "Tangents:  " << tangentCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|[x, y, z]\n";
  os << LINE;

  for (unsigned int i = 0; i < tangentCount; ++i) {
    os << i << DELIMITER << "["
       << m_fTangentArray[i].x << ", "
       << m_fTangentArray[i].y << ", "
       << m_fTangentArray[i].z << "]\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus
gaWriter2::outputBinormals(ostream& os) {
  unsigned int binormalCount = m_fBinormalArray.length();
  if (0 == binormalCount) {
    return MStatus::kFailure;
  }

  os << HEADER_LINE;
  os << "Bi normals:  " << binormalCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|[x, y, z]\n";
  os << LINE;

  for (unsigned int i = 0; i < binormalCount; ++i) {
    os << i << DELIMITER << "["
       << m_fBinormalArray[i].x << ", "
       << m_fBinormalArray[i].y << ", "
       << m_fBinormalArray[i].z << "]\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus 
gaWriter2::outputUVs(ostream& os) {
  UVSet* currUVSet;
  unsigned int uvCount;

  os << HEADER_LINE;
  for (currUVSet = m_fHeadUVSet; currUVSet != nullptr; currUVSet = currUVSet->next) {
    if (currUVSet->name == m_fCurrentUVSetName) {
      os << "Current ";
    }

    os << "UV Set:  " << currUVSet->name << "\n";
    uvCount = currUVSet->uArray.length();
    os << "UV Count:  " << uvCount << "\n";
    os << HEADER_LINE;
    os << "Format:  Index|(u, v)\n";
    os << LINE;

    for (unsigned int i = 0; i < uvCount; ++i) {
      os << i << DELIMITER << "(" << currUVSet->uArray[i] << ", " << currUVSet->vArray[i] << ")\n";
    }
    os << "\n";
  }
  os << "\n";

  return MStatus::kSuccess;
}

MStatus 
gaWriter2::outputSingleSet(ostream& os, 
                           MString setName, 
                           MIntArray faces, 
                           MString textureName) {
  unsigned int faceCount = faces.length();

  os << HEADER_LINE;
  os << "Set:  " << setName << "\n";
  os << "Faces:  ";

  for (unsigned int i = 0; i < faceCount; ++i) {
    os << faces[i] << " ";
  }
  os << "\n";

  if (textureName == "") {
    textureName = "none";
  }

  os << "Texture File: " << textureName << "\n";
  os << HEADER_LINE;
  os << "\n\n";

  return MStatus::kSuccess;
}

/*
MStatus
gaWriter2::outputVertexInfo(ostream& os) {
  unsigned int faceCount = m_fMesh->numPolygons();
  unsigned i, j, indexCount;

  MStatus status;
  MIntArray indexArray;

  os << HEADER_LINE;
  os << "Vertex Info:\n";
  os << HEADER_LINE;
  os << "Format:  Face|faceVertexIndex|vertexIndex|normalIndex|colorIndex|";

  UVSet* currUVSet;

  for (currUVSet = m_fHeadUVSet; currUVSet != nullptr; currUVSet = currUVSet->next) {
    os << "| UV_" << currUVSet->name;
  }
  os << "\n";

  os << LINE;

  MIntArray normalIndexArray;
  int colorIndex, uvID;

  for (i = 0; i < faceCount; ++i) {
    indexCount = m_fMesh->polygonVertexCount(i, &status);
    if (MStatus::kFailure == status) {
      MGlobal::displayError("MFnMesh::polygonVertexCount");
      return MStatus::kFailure;
    }

    status = m_fMesh->getPolygonVertices(i, indexArray);
    if (MStatus::kFailure == status) {
      MGlobal::displayError("MFnMesh::getPolygonVertices");
      return MStatus::kFailure;
    }

    status = m_fMesh->getFaceNormalIds(i, normalIndexArray);
    if (MStatus::kFailure == status) {
      MGlobal::displayError("MFnMesh::getFaceNormalIds");
      return MStatus::kFailure;
    }

    for (j = 0; j < indexCount; ++j) {
      status = m_fMesh->getFaceVertexColorIndex(i, j, colorIndex);

      os << i << DELIMITER << j << DELIMITER << indexArray[j] << DELIMITER
         << normalIndexArray[j] << DELIMITER << colorIndex << DELIMITER;


      for (currUVSet = m_fHeadUVSet; currUVSet != nullptr; currUVSet = currUVSet->next) {
        status = m_fMesh->getPolygonUVid(i, j, uvID, &currUVSet->name);

        if (MStatus::kFailure == status) {
          MGlobal::displayError("MFnMesh::getPolygonUVid");
          return MStatus::kFailure;
        }
        os << DELIMITER << uvID;
      }
      os << "\n";
    }

    os << "\n";
  }
  os << "\n";

  return MStatus::kSuccess;
}
*/

/*
MStatus
gaWriter2::outputColors(ostream& os) {
  unsigned int colorCount = m_fColorArray.length();
  if (0 == colorCount) {
    return MStatus::kFailure;
  }

  os << "Colors:  " << colorCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|R G B A\n";
  os << LINE;

  for (unsigned int i = 0; i < colorCount; ++i) {
    os << i << DELIMITER
       << m_fColorArray[i].r << " "
       << m_fColorArray[i].g << " "
       << m_fColorArray[i].b << " "
       << m_fColorArray[i].a << "\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}
*/