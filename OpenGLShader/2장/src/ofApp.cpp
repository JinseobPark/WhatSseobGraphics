#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	triangle.addVertex(glm::vec3(-0.5f, 0.5f, 0.0f));
	triangle.addVertex(glm::vec3(-0.5f, -0.5f, 0.0f));
	triangle.addVertex(glm::vec3(0.5f, -0.5f, 0.0f));


	triangle.addColor(ofFloatColor(1.0f, 0.0f, 0.0f, 1.0f));
	triangle.addColor(ofFloatColor(0.0f, 1.0f, 0.0f, 1.0f));
	triangle.addColor(ofFloatColor(0.0f, 0.0f, 1.0f, 1.0f));

	shader.load("first_vertex.vert", "first_fragment.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	shader.begin();
	shader.setUniform4f("fragCol", glm::vec4(0, 0.5, 1, 1));
	triangle.draw();
	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//glm::vec3 curPos = triangle.getVertex(2);
	//triangle.setVertex(2, curPos + glm::vec3(0, -20, 0));
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
