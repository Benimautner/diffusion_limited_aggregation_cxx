#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include <vector>
#include <random>
#include <chrono>
#include "../libs/ThreadPool/ThreadPool.h"
#include "cell_mgr_t.h"

using namespace std;

typedef chrono::high_resolution_clock custom_clock;
typedef chrono::duration<float, std::milli> duration_t;


class ofApp : public ofBaseApp{

private:

    ThreadPool *pool;
    cell_mgr_t cell_mgr;
    ofTrueTypeFont font;

    vector<shared_ptr<node_t>> innocent_vector;
    // vector<node_t> evil_vector;
    std::mutex evil_vector_mutex;
    shared_ptr<node_t> first_node;

    const int n_nodes = 600;
    const int iterations = 200;
    const int horizontal_cells = 80;
    const int vertical_cells = 38;
    const float radius = 8;

    bool debug_enabled = false;
    bool done = false;
    float min_dist = radius * radius * 4;
    float rand_range = radius / 2;
    float max_rad_of_tree = min_dist * min_dist;
    custom_clock::time_point start_time;
    custom_clock::time_point end_time;
    duration_t duration = chrono::milliseconds(0);

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
