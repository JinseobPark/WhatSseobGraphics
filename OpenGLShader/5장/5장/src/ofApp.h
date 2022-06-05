#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofShader cloudShader;
		ofShader alphaTestShader;
		ofShader spritesheetShader;

		ofMesh charMesh;
		ofMesh backgroundMesh;
		ofMesh cloudMesh;
		ofMesh sunMesh;

		ofImage sunImg;
		ofImage alienImg;
		ofImage backgroundImg;
		ofImage cloudImg;

		bool walkRight;
		glm::vec3 charPos;

		bool isUp;
		glm::vec3 cloudPos = glm::vec3(-0.7, 0.0, 0.0);;
		glm::vec3 cloudScale = glm::vec3(0.2, 0.2, 1.0);

		float rotation = 0.0f;
};
