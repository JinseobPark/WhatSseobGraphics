#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	quad.addVertex(glm::vec3(-0.5, -0.5, 0));
	quad.addVertex(glm::vec3(-0.5, 0.5, 0));
	quad.addVertex(glm::vec3(0.5, 0.5, 0));
	quad.addVertex(glm::vec3(0.5, -0.5, 0));

	quad.addColor(ofDefaultColorType(1, 0, 0, 1)); //red
	quad.addColor(ofDefaultColorType(0, 1, 0, 1)); //green
	quad.addColor(ofDefaultColorType(0, 0, 1, 1)); //blue
	quad.addColor(ofDefaultColorType(1, 1, 1, 1)); //white

	quad.addTexCoord(glm::vec2(0, 1));
	quad.addTexCoord(glm::vec2(0, 0));
	quad.addTexCoord(glm::vec2(1, 0));
	quad.addTexCoord(glm::vec2(1, 1));

	ofIndexType indices[6] = { 0,1,2,2,3,0 };
	quad.addIndices(indices, 6);

	shader.load("passthrough.vert", "uv_vis.frag");

	ofDisableArbTex();
	ofDisableAlphaBlending();
	img.load("jordy.png");
	checkerImg.load("checker.jpg");
	img.getTexture().setTextureWrap(GL_REPEAT, GL_CLAMP);

	brightness = 2.0f;
	timer = 0.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
	timer += ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw(){
	glm::vec4 mul = glm::vec4(0.5, 0.5, 0.5, 1);
	glm::vec4 add = glm::vec4(1.0, 0.0, 0.0, 0);
	shader.begin();
	shader.setUniformTexture("jordyTex",img, 0);
	shader.setUniformTexture("boardTex", checkerImg, 1);
	shader.setUniform1f("deltaTime", ofGetElapsedTimef());
	shader.setUniform1f("brightness", glm::sin(ofGetElapsedTimef()));
	shader.setUniform4f("multiply", mul);
	shader.setUniform4f("add", add);
	quad.draw();
	shader.end();
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
