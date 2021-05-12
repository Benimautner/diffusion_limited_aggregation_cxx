#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
    // oF setup
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofSetWindowPosition(ofGetScreenWidth() / 4, ofGetScreenHeight() / 4);
    ofSetFrameRate(60);

    font.load("ubuntu.tff",40);

    ofxGuiEnableHiResDisplay();
    button_start.addListener(this, &ofApp::start_sim);

    gui.setup();
    gui.add(slider_n_nodes.setup("Number of Nodes",config.n_nodes, 2, 10000));
    gui.add(slider_iterations.setup("Iterations",config.iterations, 1, 300));
    gui.add(slider_horizontal_cells.setup("Horizontal Cells",config.horizontal_cells,4,100));
    gui.add(slider_vertical_cells.setup("Vertical Cells",config.vertical_cells,4,100));
    gui.add(slider_radius.setup("Radius",config.radius,1,10));
    gui.add(button_start.setup("Start"));

}
//--------------------------------------------------------------

float dstSqr(const float v1[], const float v2[]) {
    // if(v1_size < 2 || v2_size < 2) return MAXFLOAT;
    // float p1 = v2[0]-v1[0];
    // float p2 = v2[1]-v1[1];
    // return (   p1       *       p1      +       p2      *       p2      );
    return ( (v2[0] - v1[0]) * (v2[0] - v1[0]) + (v2[1] - v1[1]) * (v2[1] - v1[1]) );
}


void ofApp::update(){
    if(current_mode == STARTUP) {

    } else if(current_mode == RUNNING) {
        const auto win_width = (float) ofGetWidth();
        const auto win_height = (float) ofGetHeight();
        // int num_checking = 0;
        for (int j = 0; j < config.iterations; ++j) {
            vector<future<int>> thread_results;

            for (auto node : innocent_vector) {
                if (node->stuck)
                    continue;
                node->checking = false;

                float x = ofRandom(-rand_range, rand_range);
                float y = ofRandom(-rand_range, rand_range);

                node->vel[0] = x;
                node->vel[1] = y;
                node->pos[0] += node->vel[0];
                node->pos[0] = ofClamp(node->pos[0], node->radius * 2, win_width - node->radius * 2);

                node->pos[1] += node->vel[1];
                node->pos[1] = ofClamp(node->pos[1], node->radius * 2, win_height - node->radius * 2);


            if(dstSqr(node->pos, first_node->pos) > max_rad_of_tree + pow(node->radius * first_node->radius, 2) * 4)
                continue;
            node->checking = true;
            auto handle_evil_vectors = [&] () {
                //vector<node_t> cell_vector_part;
                int cell_vector_len = 0;
                //vector<node_t*> cell_vector_part;
                auto cell_vector_part = cell_mgr.get_nodes_to_check_by_position(node->pos, cell_vector_len);

                // auto cell_vector_part = cell_mgr.get_all_nodes();
                // int cell_vector_len = cell_vector_part.size();

                    for (int i = 0; i < cell_vector_len; i++) {
                        auto other = cell_vector_part[i];
                        if (other == nullptr) continue;
                        if (other->id == -1 || node->id == -1 || node->to_be_removed)
                            continue;
                        if (dstSqr(node->pos, other->pos) < node->radius * other->radius * 4 + node->radius * 4) {
                            cout << "Adding node: " << node->id << endl;
                            evil_vector_mutex.lock();
                            node->stuck = true;
                            node->to_be_removed = true;
                            cell_mgr.add_node(node);
                            max_rad_of_tree = MAX(max_rad_of_tree, dstSqr(node->pos, first_node->pos));
                            evil_vector_mutex.unlock();
                        }
                    }
                    return 0;
                };
                handle_evil_vectors();
                //thread_results.push_back(pool->enqueue(handle_evil_vectors));
            }
            //for(auto & thread_result : thread_results) {
            //    thread_result.wait();
            // }
        }

        for (long unsigned int i = 0; i < innocent_vector.size(); i++) {
            if (innocent_vector[i]->to_be_removed)
                innocent_vector.erase(innocent_vector.begin() + (long) i);
        }
        // cout << "Innocent length: " << innocent_vector.size() << endl << "Evil length: " << evil_vector.size() << endl;
        /*
        while(innocent_vector.size() < n_nodes) {
            auto new_node = make_shared<node_t>();
            new_node->id = 2;
            new_node->pos[0] = ofRandomWidth();
            new_node->pos[1] = ofRandomHeight();
            innocent_vector.emplace_back(new_node);
        }
         */
        if (innocent_vector.empty()) {
            if (duration == duration_t(chrono::milliseconds(0))) {
                end_time = custom_clock::now();
                duration = end_time - start_time;
            }
            cout << "It took " << duration.count() << " milliseconds." << endl;
        } else {
            cout << "FPS: " << ofGetFrameRate() << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(current_mode == STARTUP) {
        gui.draw();
    } else if(current_mode == RUNNING) {
        for (const auto &node : innocent_vector) {
            if (node->checking && debug_enabled)
                ofSetColor(0, 255, 0);
            else
                ofSetColor(255);
            ofDrawCircle(node->pos[0], node->pos[1], node->radius);
        }
        ofSetColor(255, 0, 0);
        for (const auto &node : cell_mgr.get_all_nodes()) {
            ofDrawCircle(node->pos[0], node->pos[1], node->radius);
        }
        if (debug_enabled) {
            ofNoFill();
            ofDrawCircle(first_node->pos[0], first_node->pos[1], sqrt(max_rad_of_tree));

            for (const auto &cell : cell_mgr.get_cell_vector()) {
                ofDrawRectangle(cell->pos[0], cell->pos[1], cell->size[0], cell->size[1]);
            }
            ofFill();
            ofSetColor(ofGetBackgroundColor());
            ofDrawRectangle(0, 0, 100, 50);
            ofSetColor(255);
            font.drawString(to_string((int) round(ofGetFrameRate())), 10, 45);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case(0x64): // d
            debug_enabled = !debug_enabled;
            break;
        case(0x71): // q
            ofExit();
            break;
        case(0x72): // r
            current_mode = STARTUP;
            break;
        default:
            cout << "Key: " << key << endl;
    }
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

void ofApp::start_sim() {
    config.n_nodes = slider_n_nodes;
    config.iterations = slider_iterations;
    config.horizontal_cells = slider_horizontal_cells;
    config.vertical_cells = slider_vertical_cells;
    config.radius = slider_radius;
    current_mode = RUNNING;

    initialize_sim();
}

void ofApp::initialize_sim() {

    start_time = custom_clock::now();

    cell_mgr.reset();
    innocent_vector.clear();
    first_node = nullptr;


    float vertical_cell_size = (float) ofGetHeight() / (float) config.vertical_cells;
    float horizontal_cell_size = (float) ofGetWidth() / (float) config.horizontal_cells;
    cell_mgr.set_width(config.vertical_cells);
    cell_mgr.set_height(config.horizontal_cells);
    int ctr = 0;
    for(int x = 0; x < config.horizontal_cells; x++) {
        for (int y = 0; y < config.vertical_cells; ++y) {
            auto new_cell = make_shared<cell_t>();
            new_cell->id = ctr;
            new_cell->pos[0] = (float) x * horizontal_cell_size;
            new_cell->pos[1] = (float) y * vertical_cell_size;
            new_cell->size[0] = horizontal_cell_size;
            new_cell->size[1] = vertical_cell_size;
            new_cell->pos_in_grid[0] = x;
            new_cell->pos_in_grid[1] = y;
            cell_mgr.add_cell(new_cell);
            ctr++;
        }
    }

    first_node = make_shared<node_t>();
    first_node->id = 0;
    first_node->pos[0] = (float) ofGetWidth() / 2.0f;
    first_node->pos[1] = (float) ofGetHeight() / 2.0f;
    first_node->stuck = true;
    first_node->radius = config.radius;

    cell_mgr.add_node(first_node);

    innocent_vector.reserve(config.n_nodes);
    for (int j = 1; j < config.n_nodes; ++j) {
        auto new_node = make_shared<node_t>();
        new_node->id = j;
        new_node->pos[0] = ofRandomWidth();
        new_node->pos[1] = ofRandomHeight();
        new_node->radius = config.radius;
        innocent_vector.emplace_back(new_node);
    }

    pool = new ThreadPool(thread::hardware_concurrency() - 2);
}
