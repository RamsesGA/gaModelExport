#include "gaWriter.h"

gaWriter::gaWriter(MDagPath dagPath, MStatus& status) {
	m_fDagPath = new MDagPath(dagPath);
	m_fMesh = new MFnMesh(*m_fDagPath, &status);
}

gaWriter::~gaWriter() {
	if (nullptr != m_fDagPath) delete m_fDagPath;
	if (nullptr != m_fMesh) delete m_fMesh;
}

MStatus 
gaWriter::extractGeometry() {
	if (MStatus::kFailure == m_fMesh->getPoints(m_fVertexArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getPoints");
		return MStatus::kFailure;
	}
	if (MStatus::kFailure == m_fMesh->getNormals(m_fNormalArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getNormals");
		return MStatus::kFailure;
	}
	if (MStatus::kFailure == m_fMesh->getCurrentUVSetName(m_fCurrentUVSetName)) {
		MGlobal::displayError("MFnMesh::getCurrentUVSetName");
		return MStatus::kFailure;
	}
	if (MStatus::kFailure == m_fMesh->getTangents(m_fTangentArray,
		                                            MSpace::kWorld, 
		                                            &m_fCurrentUVSetName)) {
		MGlobal::displayError("MFnMesh::getTangents");
		return MStatus::kFailure;
	}
	if (MStatus::kFailure == m_fMesh->getBinormals(m_fBinormalArray,
		                                             MSpace::kWorld, 
		                                             &m_fCurrentUVSetName)) {
		MGlobal::displayError("MFnMesh::getBinormals");
		return MStatus::kFailure;
	}

	m_fDagPath->extendToShape();

	int instanceNum = 0;
	if (m_fDagPath->isInstanced()) {
    instanceNum = m_fDagPath->instanceNumber();
	}

	if (!m_fMesh->getConnectedSetsAndMembers(instanceNum, 
		                                       m_fPolygonSets, 
		                                       m_fPolygonComponents, 
		                                       true)) {
		MGlobal::displayError("MFnMesh::getConnectedSetsAndMembers");
		return MStatus::kFailure;
	}

	return MStatus::kSuccess;

	/*
  if (MStatus::kFailure == m_fMesh->getFaceVertexColors(m_fColorArray)) {
    MGlobal::displayError("MFnMesh::getFaceVertexColors");
    return MStatus::kFailure;
  }
	*/
}

void 
gaWriter::outputTabs(ostream& os, unsigned int tabCount) {
	for (unsigned int i = 0; i < tabCount; ++i) {
		os << "\t";
	}
}

MObject 
gaWriter::findShader(const MObject& setNode) {

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
gaWriter::outputSets(ostream& os) {
	MStatus status;

	unsigned int setCount = m_fPolygonSets.length();
	if (setCount > 1) {
		setCount--;
	}

	MIntArray faces;

	for (unsigned int i = 0; i < setCount; ++i) {
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
			faces[++j] = itMeshPolygon.index();
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
			if (MStatus::kFailure == outputSingleSet(os, 
				                                       MString(fnSet.name()), 
				                                       faces, textureName)) {
				return MStatus::kFailure;
			}
		}
		else {
			MObject textureNode = itDG.currentItem();

			MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName", 
				                                                           true);
			filenamePlug.getValue(textureName);

			if (MStatus::kFailure == outputSingleSet(os, 
				                                       MString(fnSet.name()), 
				                                       faces, textureName)) {
				return MStatus::kFailure;
			}
		}
	}

	return MStatus::kSuccess;
}