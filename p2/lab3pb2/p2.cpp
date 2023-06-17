#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define MAXVAL 65537
using namespace std;

struct Node {
	int val, d, parent;
};
struct Edge {
	struct Node src;
	struct Node dest;
	int w;
};

struct Graph {
	vector<Edge> edges;
	vector<Node> nodes;
};

bool BellmanFordBool(Graph* g, int s) {
	(*g).nodes[s].d = 0;
	for (int i = 0; i < (*g).nodes.size() - 1; i++) {
		for (Edge e : (*g).edges) {
			if ((*g).nodes[e.dest.val].d > (*g).nodes[e.src.val].d + e.w) {
				(*g).nodes[e.dest.val].d = (*g).nodes[e.src.val].d + e.w;
				(*g).nodes[e.dest.val].parent = (*g).nodes[e.src.val].val;
			}
		}
	}
	for (Edge e : (*g).edges) {
		if ((*g).nodes[e.dest.val].d > (*g).nodes[e.src.val].d + e.w) {
			return false;
		}
	}
	return true;

}

vector<int> BellmanFordDist(Graph* g, int s) {
	Graph* gc = new Graph(*g);
	(*gc).nodes[s].d = 0;
	bool flag = true;
	for (int i = 0; i < (*gc).nodes.size() - 1; i++) {
		for (Edge e : (*gc).edges) {
			if ((*gc).nodes[e.dest.val].d > (*gc).nodes[e.src.val].d + e.w) {
				(*gc).nodes[e.dest.val].d = (*gc).nodes[e.src.val].d + e.w;
				(*gc).nodes[e.dest.val].parent = (*gc).nodes[e.src.val].val;
			}
		}
	}
	vector<int> retList;
	for (int i = 0; i < (*gc).nodes.size(); i++)
		retList.push_back((*gc).nodes[i].d);
	delete gc;
	gc = NULL;
	return retList;
}

string Johnson(Graph* g) {
	Graph* gc = new Graph(*g);
	Node n;
	n.val = (*g).nodes.size();
	n.d = MAXVAL;
	n.parent = -1;
	(*g).nodes.push_back(n);
	for (int i = 0; i < (*g).nodes.size() - 1; i++) {
		Edge e;
		e.src = (*g).nodes[(*g).nodes.size() - 1];
		e.dest = (*g).nodes[i];
		e.w = 0;
		(*g).edges.push_back(e);
	}
	if (!BellmanFordBool(g, (*g).nodes.size() - 1))
		return "-1";
	vector<int> h = BellmanFordDist(g, (*g).nodes.size() - 1);
	(*g).edges = (*gc).edges;
	(*g).nodes = (*gc).nodes;
	delete gc;
	gc = NULL;
	for (int i = 0; i < (*g).edges.size(); i++)
		(*g).edges[i].w = (*g).edges[i].w + h[(*g).edges[i].src.val] - h[(*g).edges[i].dest.val];
	string D = "";
	for (int i = 0; i < (*g).nodes.size(); i++) {
		vector<int> dist = BellmanFordDist(g, (*g).nodes[i].val);
		for (int j = 0; j < (*g).nodes.size(); j++)
			if (dist[j] == MAXVAL)
				D += "INF ";
			else
				D += (to_string(dist[j] - h[i] + h[j]) + " ");
		D += "\n";
	}
	return D;
}

int main(int argc, char** argv) {
	ifstream f(argv[1]);
	ofstream g(argv[2]);
	string line = "";
	getline(f, line);
	string sp = " ";
	int vertices = stoi(line.substr(0, line.find(sp)));
	int edges = stoi(line.substr(line.find(sp), line.length() - 1));
	int count = 0;
	Graph* graph = new Graph;
	for (int i = 0; i < vertices; i++) {
		Node n;
		n.val = i;
		n.d = MAXVAL;
		n.parent = -1;
		(*graph).nodes.push_back(n);
	}
	while (count < edges && getline(f, line)) {
		int srcNode = stoi(line.substr(0, line.find(sp)));
		line.erase(0, line.find(' ') + sp.length());
		int destNode = stoi(line.substr(0, line.find(sp)));
		int weight = stoi(line.substr(line.find(sp), line.length() - 1));
		Edge e;
		e.src = (*graph).nodes[srcNode];
		e.dest = (*graph).nodes[destNode];
		e.w = weight;
		(*graph).edges.push_back(e);
		count++;
	}
	string valToShow = Johnson(graph);
	if (valToShow != "-1") {
		for (Edge e : (*graph).edges)
			g << e.src.val << " " << e.dest.val << " " << e.w << endl;
		g << valToShow;
	}
	else
		g << valToShow;
	g.close();
	f.close();
	return 0;
}

