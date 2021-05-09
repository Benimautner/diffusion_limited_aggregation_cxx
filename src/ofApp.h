#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include <vector>
#include <random>

using namespace std;

struct node_t {
    float pos[2] = {0,0};
    float vel[2] = {0,0};

    bool stuck = false;
    bool to_be_removed = false;
    bool checking = false;
};

class ofApp : public ofBaseApp{

private:

    vector<node_t> innocent_vector;
    vector<node_t> evil_vector;
    node_t first_node;

    bool debug_enabled = false;
    float radius = 6;
    float min_dist = radius * radius * 4;
    int n_nodes = 2000;
    int iterations = 10;
    float rand_range = radius;
    float max_rad_of_tree = min_dist * min_dist;

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
};
