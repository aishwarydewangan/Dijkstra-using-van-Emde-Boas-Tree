#include <bits/stdc++.h>

using namespace std;

vector<int> *shortest_order;
int *distance_to_dest;
int num_vertices;


struct fibonacci_node {
	int degree;                 
	fibonacci_node *parent;     
	fibonacci_node *child;      
	fibonacci_node *left;       
	fibonacci_node *right;      
	bool mark;                  
	bool is_infinity;
	int key;             
};

struct fibonacci_heap {

	fibonacci_node *min_node;
	
	int num_nodes;

	fibonacci_heap()
	{
	    num_nodes = 0;
	    min_node = NULL;
	}
};

void fib_heap_insert(fibonacci_heap *fib_heap_obj, fibonacci_node *new_node, int key) {

	fibonacci_node *min_node = fib_heap_obj->min_node;
	new_node->key = key;
	new_node->degree = 0;
	new_node->parent = NULL;
	new_node->child = NULL;
	new_node->left = new_node;
	new_node->right = new_node;
	new_node->mark = false;
	new_node->is_infinity = false;
	if (min_node != NULL) {
		fibonacci_node* min_left_temp = min_node->left;
		min_node->left = new_node;
		new_node->right = min_node;
		new_node->left = min_left_temp;
		min_left_temp->right = new_node;
	}
	if (min_node == NULL || min_node->key > new_node->key) {
		fib_heap_obj->min_node = new_node;
	}

	fib_heap_obj->num_nodes = fib_heap_obj->num_nodes + 1;
}

void fib_heap_existing_to_root(fibonacci_heap *fib_heap_obj, fibonacci_node *new_node) {

	fibonacci_node *min_node = fib_heap_obj->min_node;
	new_node->parent = NULL;  //Updating the parent pointer to null.
	new_node->mark = false;   //Setting the mark value of the node to false.

	if (min_node != NULL) {
		fibonacci_node* min_left_temp = min_node->left;
		min_node->left = new_node;
		new_node->right = min_node;
		new_node->left = min_left_temp;
		min_left_temp->right = new_node;

		if (min_node->key > new_node->key) {
			fib_heap_obj->min_node = new_node;
		}
	} else { 
		fib_heap_obj->min_node = new_node;
		new_node->right = new_node;
		new_node->left = new_node;
	}
}

void fib_heap_add_child(fibonacci_node *parent_node, fibonacci_node *new_child_node) {

	if (parent_node->degree == 0) {
		parent_node->child = new_child_node;
		new_child_node->right = new_child_node;
		new_child_node->left = new_child_node;
		new_child_node->parent = parent_node;
	} else {
		fibonacci_node* first_child = parent_node->child;
		fibonacci_node* first_child_left_temp = first_child->left;
		first_child->left = new_child_node;
		new_child_node->right = first_child;
		new_child_node->left = first_child_left_temp;
		first_child_left_temp->right = new_child_node;
	}
	new_child_node->parent = parent_node;
	parent_node->degree = parent_node->degree + 1;
}

void fib_heap_remove_node_from_root(fibonacci_node *node) {

	if (node->right != node) {
		node->right->left = node->left;
		node->left->right = node->right;
	}

	if (node->parent != NULL) {
		int parent_degree = node->parent->degree;
		if (parent_degree == 1) {
			node->parent->child = NULL;
		} else {
			node->parent->child = node->right;
		}
		node->parent->degree = node->parent->degree - 1;
	}
}

void fib_heap_link(fibonacci_heap *heap_inst, fibonacci_node *high_node, fibonacci_node *low_node) {

	fib_heap_remove_node_from_root(high_node);
	fib_heap_add_child(low_node, high_node);
	high_node->mark = false;

}

void fib_heap_consolidate(fibonacci_heap *heap_inst) {

	int node_degree;
	int count = 0, root_count = 0; 

	if (heap_inst->num_nodes > 1) { 
		int degree_table_size = heap_inst->num_nodes;
		vector<fibonacci_node*> degree_table; 
		fibonacci_node *current_node = heap_inst->min_node, *start_node =
				heap_inst->min_node;
		fibonacci_node *existing_node_degree_array, *current_consolidating_node;

		fibonacci_node *temp_node = heap_inst->min_node, *iterating_node =
				heap_inst->min_node;
		do {
			root_count++;
			iterating_node = iterating_node->right;
		} while (iterating_node != temp_node);

		while (count < root_count) {
			current_consolidating_node = current_node;
			current_node = current_node->right; 
			node_degree = current_consolidating_node->degree;
			while (true) { 
				while (node_degree >= degree_table.size()) {
					degree_table.push_back(NULL); 
				}
				if (degree_table[node_degree] == NULL) { 
					degree_table[node_degree] = current_consolidating_node;
					break;
				} else {
					existing_node_degree_array = degree_table[node_degree];

					if (current_consolidating_node->key	> existing_node_degree_array->key) {
						fibonacci_node * temp_node = current_consolidating_node;
						current_consolidating_node = existing_node_degree_array;
						existing_node_degree_array = temp_node;
					}
					if (existing_node_degree_array == current_consolidating_node)
						break;
					fib_heap_link(heap_inst, existing_node_degree_array, current_consolidating_node);
					degree_table[node_degree] = NULL;
					node_degree++;
				}
			}
			count++;
		}

		heap_inst->min_node = NULL;
		for (int i = 0; i < degree_table.size(); i++) {
			if (degree_table[i] != NULL) {
				fib_heap_existing_to_root(heap_inst, degree_table[i]);
			}
		}
	}
}

fibonacci_node *fib_heap_extract_min(fibonacci_heap *heap_inst) {

	fibonacci_node *min_node = heap_inst->min_node;

	if (min_node != NULL) {
		int degree = min_node->degree;
		fibonacci_node *current_child = min_node->child;
		fibonacci_node *removed_child;
		int count = 0;
		while (count < degree) {
			removed_child = current_child;
			current_child = current_child->right;
			fib_heap_existing_to_root(heap_inst, removed_child);
			count++;
		}
		fib_heap_remove_node_from_root(min_node);
		heap_inst->num_nodes = heap_inst->num_nodes - 1;
		if (heap_inst->num_nodes == 0) {
			heap_inst->min_node = NULL;
		} else { 
			heap_inst->min_node = min_node->right; 
			fibonacci_node *min_node_left_temp = min_node->left;
			heap_inst->min_node->left = min_node_left_temp;
			min_node_left_temp->right = heap_inst->min_node;
			fib_heap_consolidate(heap_inst);
		}
	}
	return min_node;
}

int main(int argc, char *argv[]) {
	int nodes, edges, src, t, d;

	cout << "\nEnter vertices: ";
	cin >> nodes;
	cout << "\nEnter edges: ";
	cin >> edges;
	t = edges;


	vector< pair<int, int> > graph[nodes+1];
	vector<int> dist(nodes+1, INT_MAX);
	unordered_map<int, list<int> > distList;

	while(t--) {
		int a, b, w;
		cout << "\na\tb\td";
		cin >> a >> b >> w;
		graph[a].push_back(make_pair(b, w));
		graph[b].push_back(make_pair(a, w));
	}

	cout << "\nEnter source: ";
	cin >> src;
	dist[src] = 0;

	distList[0].push_back(src);

	fibonacci_heap *heap = new fibonacci_heap;
	fibonacci_node *node = new fibonacci_node;
	fibonacci_node *minimum;
	fib_heap_insert(heap, node, 0);

	minimum = fib_heap_extract_min(heap);

	while(minimum != NULL) {

		d = minimum->key;

		while(!distList[d].empty()) {
			int u = distList[d].front();
			distList[d].pop_front();

			for(int i = 0; i < graph[u].size(); i++) {
				pair<int, int> info = graph[u][i];

				int v = info.first;

				int w = info.second;

				if(dist[v] > dist[u] + w) {
					dist[v] = dist[u] + w;
					node = new fibonacci_node;
					fib_heap_insert(heap, node, dist[v]);
					distList[dist[v]].push_back(v);
				}
			}
		}

		minimum = fib_heap_extract_min(heap);
	}


	cout << "\nOutput: ";
	for(int i = 0; i < dist.size() - 1; i++) {
		cout << dist[i] << " ";
	}
	

	return 0;
}