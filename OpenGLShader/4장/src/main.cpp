#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings glSettings;
	glSettings.setSize(1024, 768); //was 748 vertical
	glSettings.windowMode = OF_WINDOW;
	glSettings.setGLVersion(4, 1);
	ofCreateWindow(glSettings);

	printf("%s\n", glGetString(GL_VERSION));
	ofRunApp(new ofApp());

	//blending
	//vec4 finalColor = src * src.a + (1.0 - src.a) * dst;

	//additive blending. 
	//어둡게는 불가능하고 더할수록 밝아짐
	//vec4 finalColor = src + dst;
}
