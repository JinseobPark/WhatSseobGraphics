#include "ofApp.h"

void BuildMesh(ofMesh& mesh, float w, float h, glm::vec3 pos)
{

	float verts[] = { -w + pos.x, -h + pos.y,  pos.z,
		-w + pos.x,  h + pos.y,  pos.z,
		w + pos.x,  h + pos.y,  pos.z,
		w + pos.x, -h + pos.y,  pos.z };

	float uvs[] = { 0, 0,  0, 1,  1, 1,  1, 0 };

	for (int i = 0; i < 4; ++i)
	{
		int idx = i * 3;
		int uvIdx = i * 2;
		mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
	mesh.addIndices(indices, 6);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();

	BuildMesh(charMesh, 0.25, 0.25, glm::vec3(0.0, -0.35, 0.0));
	BuildMesh(backgroundMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.5));
	BuildMesh(sunMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.4));
	BuildMesh(airMesh, 0.2, 0.2, glm::vec3(-0.7, 0.6, 0.0));

	charImg.load("jordy.png");
	backgroundImg.load("BG.jpg");
	airImg.load("jordy2.png");
	sunImg.load("sun.png");

	charShader.load("vertex.vert", "fragment.frag");
	alphaShader.load("vertex.vert", "alphaAir.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	charShader.begin();
	ofEnableDepthTest();

	charShader.setUniformTexture("tex", charImg, 0);
	charMesh.draw();

	charShader.setUniformTexture("tex", backgroundImg, 0);
	backgroundMesh.draw();

	charShader.end();


	ofDisableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

	alphaShader.begin();

	alphaShader.setUniformTexture("tex", airImg, 0);
	airMesh.draw();

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	alphaShader.setUniformTexture("tex", sunImg, 0);
	sunMesh.draw();

	alphaShader.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
