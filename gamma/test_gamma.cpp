#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "gamma.hpp"


TEST_CASE("Gamma: simple planar graph") {
    graph_t graph = {
        {1, 2, 3, 4},
        {0, 2},
        {0, 1, 4},
        {0},
        {0, 2}
    };

    REQUIRE(is_graph_planar(graph) == true);
}

TEST_CASE("Gamma: K3_3 graph") {
    SECTION("Full K3_3 graph") {
        graph_t k3_3 = {
            {1, 3, 5},
            {0, 2, 4},
            {1, 3, 5},
            {0, 2, 4},
            {1, 3, 5},
            {0, 2, 4}
        };

        REQUIRE(is_graph_planar(k3_3) == false);
    }

    SECTION("K3_3 without 1 edge") {
        graph_t k3_3_minus_minus = {
            {1, 3, 5},
            {0, 2, 4},
            {1, 3},
            {0, 2, 4},
            {1, 3, 5},
            {0, 4}
        };

        REQUIRE(is_graph_planar(k3_3_minus_minus) == true);
    }
}

TEST_CASE("Gamma: K5 graph") {
    SECTION("Full K5 graph") {
        graph_t k5 = {
            {1, 2, 3, 4},
            {0, 2, 3, 4},
            {0, 1, 3, 4},
            {0, 1, 2, 4},
            {0, 1, 2, 3}
        };
        REQUIRE(is_graph_planar(k5) == false);
    }

    SECTION("K5 without 1 edge") {
        graph_t k5_minus_minus {
            {1, 2, 4},
            {0, 2, 3, 4},
            {0, 1, 3, 4},
            {1, 2, 4},
            {0, 1, 2, 3}
        };

        REQUIRE(is_graph_planar(k5_minus_minus) == true);
    }
}

TEST_CASE("Gamma: Star graph") {
    graph_t star = {
        {2, 3},
        {3, 4},
        {4, 0},
        {0, 1},
        {1, 2}
    };

    REQUIRE(is_graph_planar(star) == true);
}

TEST_CASE("Gamma: Graph with isolated nodes") {
    SECTION("Full K4 with isolated 1 node (index 2)") {
        graph_t k4_isolated = {
            {1, 3, 4},
            {0, 3, 4},
            {},
            {0, 1, 4},
            {0, 1, 3}
        };

        REQUIRE(is_graph_planar(k4_isolated) == true);
    }

    SECTION("Full K5 with isolated 1 node (index 2)") {
        graph_t k5_isolated = {
            {1, 3, 4, 5},
            {0, 3, 4, 5},
            {},
            {0, 1, 4, 5},
            {0, 1, 3, 5},
            {0, 1, 3, 4}
        };

        REQUIRE(is_graph_planar(k5_isolated) == false);
    }

    SECTION("Full empty graph with 5 nodes") {
        graph_t full_empty = {
            {},
            {},
            {},
            {},
            {}
        };

        REQUIRE(is_graph_planar(full_empty) == true);
    }
}

TEST_CASE("Gamma: Graphs with bridges") {
    SECTION("Only bridges") {
        graph_t only_bridges = {
            {4},
            {2},
            {1},
            {5},
            {0},
            {3}
        };

        REQUIRE(is_graph_planar(only_bridges) == true);
    }

    SECTION("Bridges & isolated") {
        graph_t bridges_plus_isolated = {
            {4},
            {},
            {},
            {5},
            {0},
            {3}
        };

        REQUIRE(is_graph_planar(bridges_plus_isolated) == true);
    }

    SECTION("K4 <-> bridge <-> K3 <-> bridge <-> K4") {
        graph_t bridges_plus_isolated_k3 = {
            {1, 2, 3},      //  0
            {0, 2, 3},      //  1
            {0, 1, 3, 4},   //  2
            {0, 1, 2},      //  3

            {5, 6, 2},      //  4
            {4, 6},         //  5
            {4, 5, 7},      //  6

            {8, 9, 10, 6},  //  7
            {7, 9, 10},     //  8
            {7, 8, 10},     //  9
            {7, 8, 9}       // 10
        };

        REQUIRE(is_graph_planar(bridges_plus_isolated_k3) == true);
    }

    SECTION("K4 <-> bridge <-> K5 <-> bridge <-> K4") {
        graph_t bridges_plus_isolated_k5 = {
            {1, 2, 3},          //  0
            {0, 2, 3},          //  1
            {0, 1, 3, 4},       //  2
            {0, 1, 2},          //  3

            {5, 6, 7, 8, 2},    //  4
            {4, 6, 7, 8},       //  5
            {4, 5, 7, 8},       //  6
            {4, 5, 6, 8, 10},   //  7
            {4, 5, 6, 7},       //  8

            {10, 11, 12},       //  9
            {9, 11, 12, 7},     // 10
            {9, 10, 12},        // 11
            {9, 10, 11}         // 12
        };

        REQUIRE(is_graph_planar(bridges_plus_isolated_k5) == false);
    }
}

TEST_CASE("Gamma: Big circle graphs") {
    SECTION("Simple circle") {
        graph_t big_circle_simple = {
            {1, 9},
            {0, 2},
            {1, 3},
            {2, 4},
            {3, 5},
            {4, 6},
            {5, 7},
            {6, 8},
            {7, 9},
            {8, 0}
        };

        REQUIRE(is_graph_planar(big_circle_simple) == true);
    }

    SECTION("Circle of circles") {
        graph_t circles_circle = {
            {1, 9, 10},
            {0, 2},
            {1, 3},
            {2, 4, 11},
            {3, 5, 14},
            {4, 6},
            {5, 7, 13},
            {6, 8},
            {7, 9, 12},
            {8, 0, 12},
            {0, 11},
            {3, 10},
            {8, 9, 13},
            {12, 14},
            {4, 13}
        };

        REQUIRE(is_graph_planar(circles_circle) == true);
    }
}

TEST_CASE("Gamma: Graph in graph in graph...") {
    SECTION("Circle in circle in circle") {
        graph_t circle_in_circle_in_circle = {
            {1, 9, 10},
            {0, 2},
            {1, 3},
            {2, 4},
            {3, 5},
            {4, 6},
            {5, 7},
            {6, 8},
            {7, 9},
            {8, 0},

            {11, 16, 0},
            {10, 12},
            {11, 13},
            {12, 14},
            {13, 15},
            {14, 16},
            {15, 10, 17},

            {18, 20, 16},
            {17, 19},
            {18, 20},
            {19, 17}
        };

        REQUIRE(is_graph_planar(circle_in_circle_in_circle) == true);
    }

    SECTION("Circle in circle in circle") {
        graph_t circle_in_circle_in_circle = {
            {1, 9},
            {0, 2},
            {1, 3},
            {2, 4},
            {3, 5},
            {4, 6},
            {5, 7},
            {6, 8},
            {7, 9},
            {8, 0},

            {11, 16},
            {10, 12},
            {11, 13},
            {12, 14},
            {13, 15},
            {14, 16},
            {15, 10},

            {18, 20},
            {17, 19},
            {18, 20},
            {19, 17}
        };

        REQUIRE(is_graph_planar(circle_in_circle_in_circle) == true);
    }
}

TEST_CASE("Gamma: Big star (circle, square, cross connected)") {
    SECTION("No planar star") {
        graph_t no_planar_star_circle_square = {
            {1, 7, 12},
            {0, 2, 9},
            {1, 3, 12},
            {2, 4, 10},
            {3, 5, 12},
            {4, 6, 11},
            {5, 7, 12},
            {0, 6, 8},
            {7, 9, 11, 12},
            {1, 8, 10, 12},
            {3, 9, 11, 12},
            {5, 8, 10, 12},
            {0, 2, 4, 6, 8, 9, 10, 11}
        };

        REQUIRE(is_graph_planar(no_planar_star_circle_square) == false);
    }

    SECTION("Planar star") {
        graph_t planar_star_circle_square = {
            {1, 7, 12},
            {0, 2, 9},
            {1, 3, 12},
            {2, 4, 10},
            {3, 5, 12},
            {4, 6, 11},
            {5, 7, 12},
            {0, 6, 8},
            {7, 9, 11},
            {1, 8, 10},
            {3, 9, 11},
            {5, 8, 10},
            {0, 2, 4, 6}
        };

        REQUIRE(is_graph_planar(planar_star_circle_square) == true);
    }
}
