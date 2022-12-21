#include "gamma.hpp"


// -----/ Declaration /-----

struct Vertix {
	bool used = false;
	std::set<T> next_v;
	std::set<T> edges;

	bool operator [] (T v) {
		return next_v.count(v);
	}
};

struct Contact {
	std::set<T> contact_vertices;

	T get_gamma_contact(std::vector<Vertix>& g, T& edge) const {
		T ans = 0; bool flag;
		for (T curr_facet: g[*contact_vertices.begin()].edges) {
			flag = true;
			for (T v: contact_vertices) {
				if (!g[v].edges.count(curr_facet)) {
					flag = false;
					break;
				}
			}
			if (flag) {
				++ans;
				edge = curr_facet;
			}
		}

		return ans;
	}
};

bool is_graph_planar(graph_t& g);

void create_vertices(std::vector<Vertix>& vertices, graph_t& g);
bool gamma(std::vector<Vertix>& g, T start);
void make_edges(graph_t& edges, std::vector<Vertix>& g, T v, T u);
bool gamma_full_step(std::vector<Vertix>& g, graph_t& edges,
                     std::vector<T>& initial_cycle);
void get_new_contacts(std::vector<Contact>& contacts,
                      std::vector<Vertix>& g,
                      std::vector<T>& path);
void get_contact(Contact& contact, T v,
                 std::vector<Vertix>& g, std::vector<bool>& used);
T get_min_gamma(std::vector<Contact>& contacts,
                std::vector<Vertix>& g,
                T& cont_num, T& edge_min);
void stack_path(std::vector<Vertix>& g,
                graph_t& edges,
                std::vector<Contact>& contacts,
                T contact_num, T& contact_count,
                T edge, T& edges_count);
void fix_edges(std::vector<Vertix>& g,
               graph_t& edges,
               T from, T to, T old_edge,
               T new_edge);
bool get_path(std::vector<T>& path, std::vector<Vertix>& g,
              std::set<T>& contact_vertices, T v, T u);
T intersect_size(std::set<T> a, std::set<T> b);
bool get_edge_border(std::vector<T>& path, graph_t& edges,
                     std::vector<Vertix>& g, T edge,
                     T prev, T v, T finish);



// -----/ Function realizations /-----

bool is_graph_planar(graph_t& g) {
	std::vector<Vertix> vertices(g.size());
	create_vertices(vertices, g);

	for (T v = 0; v < g.size(); ++v) {
		if (!vertices[v].used && !gamma(vertices, v)) {
			return false;
        }
	}

	return true;
}

bool is_cycle(std::vector<Vertix>& g, T v,
              std::vector<T>& cycle,
              T prev = SSIZE_MAX) {
	static bool in_cycle = false;
	if (g[v].used) {
		cycle.push_back(v);
		in_cycle = true;
		return true;
	}

	g[v].used = true;
	for (T u: g[v].next_v) {
		if (prev != u && is_cycle(g, u, cycle, v)) {
			if (v != cycle[0]) {
				if (in_cycle) {
					cycle.push_back(v);
                } else {
					g[v].used = false;
                }
			} else {
				in_cycle = false;
            }

			return true;
		}
	}

    g[v].used = false;
	return false;
}

void create_vertices(std::vector<Vertix>& vertices, graph_t& g) {
	for (T v = 0; v < g.size(); ++v) {
		for (T next: g[v]) {
			vertices[v].next_v.insert(next);
        }
	}
}

bool gamma(std::vector<Vertix>& g, T start) {
	std::vector<T> cycle;
	graph_t edges(g.size());
	if (!is_cycle(g, start, cycle)) {
		return true;
    }

	g[cycle[0]].edges = {0, 1};
	make_edges(edges, g, cycle[0], cycle[cycle.size() - 1]);
	for (T i = 1; i < cycle.size(); ++i) {
		T v = cycle[i];
		T u = cycle[i - 1];
		g[v].edges = {0, 1};
		make_edges(edges, g, v, u);
	}
	
	return gamma_full_step(g, edges, cycle);
}

void make_edges(graph_t& edges, std::vector<Vertix>& g, T v, T u) {
	g[v].next_v.erase(u);
	g[u].next_v.erase(v);
	edges[v].push_back(u);
	edges[u].push_back(v);
}

bool gamma_full_step(std::vector<Vertix>& g, graph_t& edges,
                     std::vector<T>& initial_cycle) {
	std::vector<Contact> contacts;
	get_new_contacts(contacts, g, initial_cycle);

	T contact_count = initial_cycle.size();
	T edges_count = 2;
	while (contacts.size()) {
		T contact_num;
		T edge;
		if (get_min_gamma(contacts, g, contact_num, edge) > 0) {
			stack_path(g, edges, contacts, contact_num, contact_count, 
                       edge, edges_count);
        } else {
			return false;
        }
	}

	return true;
}

void get_new_contacts(std::vector<Contact>& contacts,
                      std::vector<Vertix>& g,
                      std::vector<T>& path) {
	std::set<T> path_set(path.begin() + 1, path.end() - 1);
	std::vector<bool> used(g.size());
	for (T i = 1; i < path.size() - 1; ++i) {
		T v = path[i];
		for (T u: g[v].next_v) {
			if (g[u].used) {
				if (u > v || !path_set.count(u)) {
					contacts.emplace_back();
					contacts[contacts.size() - 1].contact_vertices.insert(u);
					contacts[contacts.size() - 1].contact_vertices.insert(v);
				}
			} else 
            if (!used[u]) {
				Contact contact;
				get_contact(contact, u, g, used);
				contacts.push_back(contact);
			}
		}
	}
}

void get_contact(Contact& contact, T v,
                 std::vector<Vertix>& g, std::vector<bool>& used) {
	if (used[v]) {
		return;
    }

	if (g[v].used) {
		contact.contact_vertices.insert(v);
		return;
	}

	used[v] = true;
	for (T u: g[v].next_v) {
		get_contact(contact, u, g, used);
    }
}

T get_min_gamma(std::vector<Contact>& contacts,
                std::vector<Vertix>& g,
                T& cont_num, T& edge_min) {
	T min_num = 0;
	T min_gamma = SSIZE_MAX;
	for (T i = 0; i < contacts.size(); ++i) {
		T edge;
		T gamma = contacts[i].get_gamma_contact(g, edge);

		if (gamma < min_gamma || (gamma == min_gamma && 
                                  contacts[i].contact_vertices.size() == 2)) {
			min_num = i;
			min_gamma = gamma;
			edge_min = edge;
		}
	}

	cont_num = min_num;
	return min_gamma;
}

void stack_path(std::vector<Vertix>& g,
                graph_t& edges,
                std::vector<Contact>& contacts,
                T contact_num, T& contact_count,
                T edge, T& edges_count) {
	T v = *contacts[contact_num].contact_vertices.begin();
	if (contacts[contact_num].contact_vertices.size() == 1) {
		for (T u: g[v].next_v) {
			make_edges(edges, g, v, u);
        }
		contacts.erase(contacts.begin() + contact_num);
		return;
	}

	T u = *contacts[contact_num].contact_vertices.rbegin();
	T old_edge = edge;
	T new_edge = edges_count++;
	if (contacts[contact_num].contact_vertices.size() == 2 && g[v][u]) {
		g[v].edges.insert(new_edge);
		g[u].edges.insert(new_edge);

		fix_edges(g, edges, v, u, old_edge, new_edge);
		make_edges(edges, g, v, u);
		contacts.erase(contacts.begin() + contact_num);
		return;
	}

	std::vector<T> path;
	g[v].used = false;
	get_path(path, g, contacts[contact_num].contact_vertices, v, u);
	contact_count += path.size() - 2;
	
	g[v].edges.insert(new_edge);
	g[u].edges.insert(new_edge);
	fix_edges(g, edges, v, u, old_edge, new_edge);
	for (T i = 1; i < path.size(); ++i) {
		g[path[i]].edges.insert(old_edge);
		g[path[i]].edges.insert(new_edge);
		make_edges(edges, g, path[i - 1], path[i]);
	}

	contacts.erase(contacts.begin() + contact_num);
	get_new_contacts(contacts, g, path);
}

void fix_edges(std::vector<Vertix>& g,
               graph_t& edges,
               T from, T to, T old_edge,
               T new_edge) {
	std::vector<T> path;
	for (T v: edges[from]) {
		path.clear();
		if (g[v].edges.count(old_edge) && 
                            get_edge_border(path, edges, g, 
                                            old_edge, from, v, to)) {
			break;
        }
	}

	for (T i = 0; i < path.size(); ++i) {
		g[path[i]].edges.erase(old_edge);
		g[path[i]].edges.insert(new_edge);
	}
}

bool get_path(std::vector<T>& path, std::vector<Vertix>& g,
              std::set<T>& contact_vertices, T v, T u) {
	if (v == u) {
		path.push_back(v);
		return true;
	}
	
	if (g[v].used) {
		return false;
    }

	g[v].used = true;
	for (T w: g[v].next_v) {
		if (get_path(path, g, contact_vertices, w, u)) {
			path.push_back(v);
			return true;
		}
	}

    g[v].used = false;
	return false;
}

T intersect_size(std::set<T> a, std::set<T> b) {
	T ans = 0;
	auto a_it = a.begin();
	auto b_it = b.begin();
    
	while (a_it != a.end() && b_it != b.end()) {
		if (*a_it < *b_it) {
			++a_it;
        } else 
        if (*b_it < *a_it) {
			++b_it;
        } else {
			++ans;
			++a_it; ++b_it;
		}
	}

	return ans;
}

bool get_edge_border(std::vector<T>& path, graph_t& edges,
                     std::vector<Vertix>& g, T edge,
                     T prev, T v, T finish) {
	if (v == finish) {
		return true;
    }

	bool status = (edges[v].size() == 1);
	for (T u: edges[v]) {
		if (u == prev || intersect_size(g[v].edges, g[u].edges) > 2) {
			continue;
        }

		if (g[u].edges.count(edge)) {
			status |= get_edge_border(path, edges, g, 
                                       edge, v, u, finish);
        }
	}

	if (status) {
		path.push_back(v);
    }
	return status;
}
