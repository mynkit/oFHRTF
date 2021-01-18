#pragma once

#include "ofMain.h"
#include "soundEffects/hrtf.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
        // audio setting
        void audioIn(ofSoundBuffer &buffer);
        void audioOut(ofSoundBuffer &buffer);
        ofSoundStream sound_stream;
        vector<float> inputBuffer;
        int bpm;
        int bufferSize;
        int sampleRate;
        float azimuth;
    
        bool hrtfOn;

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
    
        hrtf* myHrtf;
    private:
        ofBoxPrimitive soundSource;
        ofSpherePrimitive microphone;
        ofCamera cam;
};
