#pragma once

#include "ofMain.h"
#include "soundEffects/hrtf2D.hpp"
#include "soundEffects/hrtf3D.hpp"

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
        int hrtfDataIndex;
        float index;
    
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
    
        hrtf2D* myHrtf2D;
        hrtf3D* myHrtf3D;
    private:
        ofBoxPrimitive soundSource;
        ofSpherePrimitive microphone;
        // ofCamera cam;
        ofEasyCam cam;
};
