#include "ofApp.h"

void buildMesh(ofMesh& mesh, float w, float h, glm::vec3 pos)
{
	float verts[] = { -w + pos.x, -h + pos.y,  pos.z,
		-w + pos.x,  h + pos.y,  pos.z,
		w + pos.x,  h + pos.y,  pos.z,
		w + pos.x, -h + pos.y,  pos.z };

	float uvs[] = { 0,0, 0,1, 1,1, 1,0 };

	for (int i = 0; i < 4; ++i) {
		int idx = i * 3;
		int uvIdx = i * 2;

		mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[6] = { 0,1,2,2,3,0 };
	mesh.addIndices(indices, 6);
}

glm::mat4 buildMatrix(glm::vec3 trans, float rot, glm::vec3 scale)
{
	using glm::mat4;
	mat4 translation = glm::translate(trans);
	mat4 rotation = glm::rotate(rot, glm::vec3(0.0, 0.0, 1.0));
	mat4 scaler = glm::scale(scale);
	return translation * rotation * scaler;
}

//--------------------------------------------------------------
void ofApp::setup(){

	ofDisableArbTex();
	ofEnableDepthTest();

	buildMesh(charMesh, 0.1, 0.2, glm::vec3(0.0, -0.2, 0.0));
	buildMesh(backgroundMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.5));
	buildMesh(sunMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.4));
	buildMesh(cloudMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.0));

	alienImg.load("jordyYo.png");
	backgroundImg.load("BG.jpg");
	cloudImg.load("jordy2.png");
	sunImg.load("sun.png");

	spritesheetShader.load("sprite.vert", "fragment.frag");
	alphaTestShader.load("vertex.vert", "fragment.frag");
	cloudShader.load("cloud.vert", "alphaAir.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

	if (walkRight)
	{
		float speed = 0.4 * ofGetLastFrameTime();
		charPos += glm::vec3(speed, 0, 0);
	}
	if (isUp)
	{
		float speed = 0.4 * ofGetLastFrameTime();
		cloudPos += glm::vec3(speed * 2, speed, 0);
		cloudScale += glm::vec3(speed, speed, 0);
	}
	rotation += ofGetLastFrameTime();
}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	static float frame = 0.0;
	frame = (frame > 10) ? 0.0 : frame += 0.1;
	glm::vec2 spriteSize = glm::vec2(0.2, 0.25);
	glm::vec2 spriteFrame = glm::vec2((int)frame % 5, (int)frame / 4);


	ofDisableBlendMode();
	ofEnableDepthTest();

	spritesheetShader.begin();
	spritesheetShader.setUniform2f("size", spriteSize);
	spritesheetShader.setUniform2f("offset", spriteFrame);
	spritesheetShader.setUniformTexture("tex", alienImg, 0);
	spritesheetShader.setUniform3f("translation", charPos);
	charMesh.draw();

	spritesheetShader.end();

	alphaTestShader.begin();
	alphaTestShader.setUniformTexture("tex", backgroundImg, 0);
	backgroundMesh.draw();
	alphaTestShader.end();

	ofDisableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

	static float rotation = 1.0f;
	rotation += 1.0f * ofGetLastFrameTime();

	//construct the transform for our un-rotated cloud
	mat4 translationA = translate(vec3(-0.55, 0.0, 0.0));
	mat4 scaleA = scale(vec3(1.5, 1, 1));
	mat4 transformA = translationA * scaleA;

	//apply a rotation to that
	mat4 ourRotation = rotate(rotation, vec3(0.0, 0.0, 1.0));
	mat4 newMatrix = translationA * ourRotation * inverse(translationA);
	mat4 finalMatrixA = newMatrix * transformA;

	mat4 transformB = buildMatrix(vec3(0.4, 0.2, 0.0), 1.0f, vec3(1, 1, 1));

	cloudShader.begin();

	cloudShader.setUniformTexture("tex", cloudImg, 0);

	cloudShader.setUniformMatrix4f("transform", finalMatrixA);
	cloudMesh.draw();

	cloudShader.setUniformMatrix4f("transform", newMatrix);
	cloudMesh.draw();

	//cloudShader.setUniform3f("translation", cloudPos);
	//cloudShader.setUniform3f("scale", cloudScale);
	//cloudShader.setUniform1f("rotation", rotation);


	//ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	//cloudShader.setUniformTexture("tex", sunImg, 0);
	//sunMesh.draw();

	cloudShader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == ofKey::OF_KEY_RIGHT) {
		walkRight = true;
	}
	if (key == ofKey::OF_KEY_UP)
		isUp = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (key == ofKey::OF_KEY_RIGHT) {
		walkRight = false;
	}
	if (key == ofKey::OF_KEY_UP)
		isUp = false;
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
