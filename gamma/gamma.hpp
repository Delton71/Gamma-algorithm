#ifndef GAMMA_HEADER
#define GAMMA_HEADER

#include <vector>
#include <set>
#include <limits>
#include <climits>

typedef size_t T;
typedef std::vector<std::vector<T>>
        graph_t;

bool is_graph_planar(graph_t& g);

#endif /* GAMMA_HEADER */