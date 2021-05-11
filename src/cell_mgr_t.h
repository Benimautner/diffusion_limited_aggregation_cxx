//
// Created by beni on 10.05.21.
//

#ifndef DIFFUSION_LIMITED_AGGREGATION_C_CELL_MGR_T_H
#define DIFFUSION_LIMITED_AGGREGATION_C_CELL_MGR_T_H

#include <vector>
#include <unordered_set>
#include "ofMain.h"
#include "ofxGui.h"

using namespace std;

typedef const float pos_t;

struct node_t {
    node_t() = default;
    ~node_t() = default;
    int id = -1;
    float pos[2] = {0,0};
    float vel[2] = {0,0};

    bool stuck = false;
    bool to_be_removed = false;
    bool checking = false;
};

struct cell_t {
    cell_t() = default;
    ~cell_t() = default;
    int id = -1;
    int pos_in_grid[2] = {0,0};
    float pos[2] = {0,0};
    float size[2] = {0,0};
    vector<node_t*> evil_vector_part;
};

class cell_mgr_t {
private:
    vector<cell_t*> cell_vector;
    int width;
    int height;
    const vector<vector<int>> indices_to_check = {{-1,-1}, {0, -1}, {1, -1},
                                            {-1, 0},          {1,  0},
                                            {-1, 1}, {0,  1}, {1,  1}};

public:
    cell_mgr_t();

    int get_id_by_position(pos_t pos[]);
    vector<node_t*> get_nodes_to_check_by_position(pos_t pos[], int& len);
    void add_cell(float x, float y, float width, float height);
    vector<cell_t*> get_cell_vector();


    vector<node_t*> get_all_nodes();

    int get_index(int x, int y);

    void set_width(int width);

    void set_height(int height);

    bool get_cell_by_position(pos_t *pos, cell_t*& return_cell);

    void add_cell(cell_t *cell);
    void add_node(node_t *node);
};


#endif //DIFFUSION_LIMITED_AGGREGATION_C_CELL_MGR_T_H
