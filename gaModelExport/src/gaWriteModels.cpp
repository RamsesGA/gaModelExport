#include "gaWriteModels.h"

gaWriterModels::gaWriterModels(MDagPath dagPath, MStatus& status) {
	m_fDagPath = new MDagPath(dagPath);
	m_fMesh = new MFnMesh(*m_fDagPath, &status);
}

/*
* 
*/

MStatus
gaWriterModels::extractGeometry() {
  if (MStatus::kFailure == gaWriterModels::extractGeometry()) {
    return MStatus::kFailure;
  }

  MStringArray uvSetNames;
  if (MStatus::kFailure == m_fMesh->getUVSetNames(uvSetNames)) {
    MGlobal::displayError("MFnMesh::getUVSetNames");
    return MStatus::kFailure;
  }

  unsigned int uvSetCount = uvSetNames.length();
  unsigned int i;

  UVSet* currUVSet = NULL;

  for (i = 0; i < uvSetCount; i++) {
    if (0 == i) {
      currUVSet = new UVSet;
      m_fHeadUVSet = currUVSet;
    }
    else {
      currUVSet->next = new UVSet;
      currUVSet = currUVSet->next;
    }

    currUVSet->name = uvSetNames[i];
    currUVSet->next = NULL;

    if (MStatus::kFailure == m_fMesh->getUVs(currUVSet->uArray, currUVSet->vArray, &currUVSet->name)) {
      return MStatus::kFailure;
    }
  }

  return MStatus::kSuccess;
}

MStatus 
gaWriterModels::writeToFile(ostream& os) {
  MGlobal::displayInfo("Exporting " + m_fMesh->partialPathName());

  os << SHAPE_DIVIDER;
  os << "Shape:  " << m_fMesh->partialPathName() << "\n";
  os << SHAPE_DIVIDER;
  os << "\n";

  if (MStatus::kFailure == outputFaces(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputVertices(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputVertexInfo(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputNormals(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputTangents(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputBinormals(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputColors(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputUVs(os)) {
    return MStatus::kFailure;
  }
  if (MStatus::kFailure == outputSets(os)) {
    return MStatus::kFailure;
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MObject
gaWriterModels::findShader(const MObject& setNode) {

  MFnDependencyNode fnNode(setNode);
  MPlug shaderPlug = fnNode.findPlug("surfaceShader", true);

  if (!shaderPlug.isNull()) {
    MPlugArray connectedPlugs;

    MStatus status;
    shaderPlug.connectedTo(connectedPlugs, true, false, &status);
    if (MStatus::kFailure == status) {
      MGlobal::displayError("MPlug::connectedTo");
      return MObject::kNullObj;
    }

    if (1 != connectedPlugs.length()) {
      MGlobal::displayError("Error getting shader for: " + m_fMesh->partialPathName());
    }
    else {
      return connectedPlugs[0].node();
    }
  }

  return MObject::kNullObj;
}

MStatus
gaWriterModels::outputSets(ostream& os) {
	MStatus status;

	unsigned int setCount = m_fPolygonSets.length();
	if (setCount > 1) {
		setCount--;
	}

	MIntArray faces;

	unsigned int i;
	for (i = 0; i < setCount; i++) {
		MObject set = m_fPolygonSets[i];
		MObject comp = m_fPolygonComponents[i];
		MFnSet fnSet(set, &status);

		if (MS::kFailure == status) {
			MGlobal::displayError("MFnSet::MFnSet");
			continue;
		}

		MItMeshPolygon itMeshPolygon(*m_fDagPath, comp, &status);

		if ((MS::kFailure == status)) {
			MGlobal::displayError("MItMeshPolygon::MItMeshPolygon");
			continue;
		}

		faces.setLength(itMeshPolygon.count());

		unsigned int j = 0;
		for (itMeshPolygon.reset(); !itMeshPolygon.isDone(); itMeshPolygon.next()) {
			faces[j++] = itMeshPolygon.index();
		}

		MObject shaderNode = findShader(set);
		if (MObject::kNullObj == shaderNode) {
			continue;
		}

		MPlug colorPlug = MFnDependencyNode(shaderNode).findPlug("color", true, &status);
		if (MS::kFailure == status) {
			MGlobal::displayError("MFnDependencyNode::findPlug");
			continue;
		}

		MItDependencyGraph itDG(colorPlug, 
			                      MFn::kFileTexture,
			                      MItDependencyGraph::kUpstream,
			                      MItDependencyGraph::kBreadthFirst,
			                      MItDependencyGraph::kNodeLevel,
			                      &status);

		if (MS::kFailure == status) {
			MGlobal::displayError("MItDependencyGraph::MItDependencyGraph");
			continue;
		}

		itDG.disablePruningOnFilter();

		MString textureName("");
		if (itDG.isDone()) {
			if (MStatus::kFailure == outputSingleSet(os, MString(fnSet.name()), faces, textureName)) {
				return MStatus::kFailure;
			}
		}
		else {
			MObject textureNode = itDG.currentItem();
			MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName", true);
			filenamePlug.getValue(textureName);
			if (MStatus::kFailure == outputSingleSet(os, MString(fnSet.name()), faces, textureName)) {
				return MStatus::kFailure;
			}
		}
	}

	return MStatus::kSuccess;
}

MStatus 
gaWriterModels::outputSingleSet(ostream& os, 
	                              MString setName, 
	                              MIntArray faces, 
	                              MString textureName) {
  unsigned int i;
  unsigned int faceCount = faces.length();

  os << "Set:  " << setName << "\n";
  os << HEADER_LINE;
  os << "Faces:  ";
  for (i = 0; i < faceCount; i++) {
    os << faces[i] << " ";
  }
  os << "\n";
  if (textureName == "") {
    textureName = "none";
  }
  os << "Texture File: " << textureName << "\n";
  os << "\n\n";
  return MStatus::kSuccess;
}

void
gaWriterModels::outputTabs(ostream& os, unsigned int tabCount) {
  unsigned int i;

  for (i = 0; i < tabCount; i++) {
    os << "\t";
  }
}

MStatus
gaWriterModels::outputFaces(ostream& os) {
  unsigned int faceCount = m_fMesh->numPolygons();

	if (0 == faceCount) {
		return MStatus::kFailure;
	}

	MStatus status;
	MIntArray indexArray;

	os << "Faces:  " << faceCount << "\n";
	os << HEADER_LINE;
	os << "Format:  Index|Vertex Indices\n";
	os << LINE;

	unsigned int i;
	for (i = 0; i < faceCount; ++i) {
		os << i << DELIMITER;

		unsigned int indexCount = m_fMesh->polygonVertexCount(i, &status);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MFnMesh::polygonVertexCount");
			return MStatus::kFailure;
		}

		status = m_fMesh->getPolygonVertices (i, indexArray);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MFnMesh::getPolygonVertices");
			return MStatus::kFailure;
		}

		unsigned int j;
		for (j = 0; j < indexCount; j++) {
			os << indexArray[j] << " ";
		}

		os << "\n";
	}
	os << "\n\n";

	return MStatus::kSuccess;
}

MStatus
gaWriterModels::outputVertices(ostream& os) {
  unsigned int vertexCount = m_fVertexArray.length();
  unsigned i;

  if (0 == vertexCount) {
    return MStatus::kFailure;
  }

  os << "Vertices:  " << vertexCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Vertex|(x, y, z)\n";
  os << LINE;

  for (i = 0; i < vertexCount; ++i) {
    os << i << DELIMITER << "("
       << m_fVertexArray[i].x << ", "
       << m_fVertexArray[i].y << ", "
       << m_fVertexArray[i].z << ")\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus
gaWriterModels::outputVertexInfo(ostream& os) {
  unsigned int faceCount = m_fMesh->numPolygons();
  unsigned i, j, indexCount;

  MStatus status;
  MIntArray indexArray;

  os << "Vertex Info:\n";
  os << HEADER_LINE;
  os << "Format:  Face|faceVertexIndex|vertexIndex|normalIndex|colorIndex|";

  UVSet* currUVSet;
  for (currUVSet = m_fHeadUVSet; currUVSet != NULL; currUVSet = currUVSet->next) {
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

      for (currUVSet = m_fHeadUVSet; currUVSet != NULL; currUVSet = currUVSet->next) {
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

MStatus
gaWriterModels::outputNormals(ostream& os) {
  unsigned int normalCount = m_fNormalArray.length();

  if (0 == normalCount) {
    return MStatus::kFailure;
  }

  os << "Normals:  " << normalCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|[x, y, z]\n";
  os << LINE;

  unsigned int i;
  for (i = 0; i < normalCount; ++i) {
    os << i << DELIMITER << "["
       << m_fNormalArray[i].x << ", "
       << m_fNormalArray[i].y << ", "
       << m_fNormalArray[i].z << "]\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus
gaWriterModels::outputTangents(ostream& os) {
  unsigned int tangentCount = m_fTangentArray.length();

  if (0 == tangentCount) {
    return MStatus::kFailure;
  }

  os << "Tangents:  " << tangentCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|[x, y, z]\n";
  os << LINE;

  unsigned int i;
  for (i = 0; i < tangentCount; ++i) {
    os << i << DELIMITER << "["
       << m_fTangentArray[i].x << ", "
       << m_fTangentArray[i].y << ", "
       << m_fTangentArray[i].z << "]\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus
gaWriterModels::outputBinormals(ostream& os) {
  unsigned int binormalCount = m_fBinormalArray.length();

  if (0 == binormalCount) {
    return MStatus::kFailure;
  }

  os << "Bi normals:  " << binormalCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|[x, y, z]\n";
  os << LINE;

  unsigned int i;
  for (i = 0; i < binormalCount; ++i) {
    os << i << DELIMITER << "["
       << m_fBinormalArray[i].x << ", "
       << m_fBinormalArray[i].y << ", "
       << m_fBinormalArray[i].z << "]\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus
gaWriterModels::outputColors(ostream& os) {
  unsigned int colorCount = m_fColorArray.length();

  if (0 == colorCount) {
    return MStatus::kFailure;
  }

  os << "Colors:  " << colorCount << "\n";
  os << HEADER_LINE;
  os << "Format:  Index|R G B A\n";
  os << LINE;

  unsigned int i;
  for (i = 0; i < colorCount; ++i) {
    os << i << DELIMITER
       << m_fColorArray[i].r << " "
       << m_fColorArray[i].g << " "
       << m_fColorArray[i].b << " "
       << m_fColorArray[i].a << "\n";
  }
  os << "\n\n";

  return MStatus::kSuccess;
}

MStatus
gaWriterModels::outputUVs(ostream& os) {
  UVSet* currUVSet;

  unsigned int i, uvCount;
  for (currUVSet = m_fHeadUVSet; currUVSet != NULL; currUVSet = currUVSet->next) {
    if (currUVSet->name == m_fCurrentUVSetName) {
      os << "Current ";
    }

    os << "UV Set:  " << currUVSet->name << "\n";
    uvCount = currUVSet->uArray.length();

    os << "UV Count:  " << uvCount << "\n";
    os << HEADER_LINE;
    os << "Format:  Index|(u, v)\n";
    os << LINE;

    for (i = 0; i < uvCount; ++i) {
      os << i << DELIMITER << "(" << currUVSet->uArray[i] << ", " << currUVSet->vArray[i] << ")\n";
    }
    os << "\n";
  }
  os << "\n";

  return MStatus::kSuccess;
}