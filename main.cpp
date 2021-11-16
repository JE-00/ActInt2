// Actividad Integradora 2	-	TC2038.4	-	ITESM-MTY
// José Carlos Esquerra Rivas - A00827879
// Lucas Eduardo Idigoras - A00827751
// Jose A. Kaun A01720829 - A01720829
// 12/11/2021 - 21/11/2021

#include <iostream>
#include <unordered_map>
#include <climits>
#include <vector>
#include <algorithm>
#define MAX 401

// Leer de entrada estandar  la información de un grafo que representa conexión de fibra óptica de colonias así como su representación del plano carteciano del mapa de la ciudad, así la indentificación de si es o no un una "central" (1 es central, 0 no es central), así como las conexiones entre ellas.

using namespace std;


struct Nodo {
    int id, x, y;
    bool central;
    string nombre;

    Nodo(){
        id = x = y = 0;
        nombre = "";
        central = false;
    }
    Nodo(int id, string nombre, int x, int y, bool central) {
        this -> x = x;
        this -> y = y;
        this -> central = central;
        this -> nombre = nombre;
        this -> id = id;
    }
};

unordered_map<string, Nodo> mapaStr;
unordered_map<int,Nodo> mapaInt;

struct Graph {
    int V, E;
    double costMSTKruskal;
    vector<pair<int, pair<int, int>>> edges; // Se utiliza para Kruskal

    vector<pair<int, int>> selectedEdgesK; // Los arcos seleccionados por Kruskal

    Graph(int V, int E){
        this->V = V;
        this->E = E;
        costMSTKruskal = 0;
    }

    void addEdge(int u, int v, int w) {
        edges.push_back({w, {u, v}});   // First mete el costo, second vertices de conexión
    }

    void kruskalMST();
};

// To represent Disjoint Sets
struct DisjointSets
{
    int *parent, *rnk;
    int n;

    DisjointSets(int n){
        this-> n = n;
        parent = new int[n+1];
        rnk = new int[n+1];
        for (int i = 0; i <= n; i++){
            rnk[i] = 0;
            parent[i] = i;
        }
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

// Complejidad: O(E logE)
void Graph::kruskalMST(){
    costMSTKruskal = 0;
    sort(edges.begin(), edges.end());
    DisjointSets ds(V);
    for (auto it : edges) {
        int u = it.second.first;
        int v = it.second.second;
        int set_u = ds.find(u);
        int set_v = ds.find(v);
        if (set_u != set_v) {
            ds.merge(u, v);
            costMSTKruskal += it.first;
            selectedEdgesK.emplace_back(u, v);
            auto itA = mapaInt.find(u);
            string sU = itA->second.nombre;
            auto itB = mapaInt.find(v);
            string sV = itB->second.nombre;
            cout << sU << " - " << sV << " " << it.first << endl;
        }
    }
}

int main() {

    int n; // cantidad nodos
    int m; // arcos
    int q; //
    string nom;
    int x, y;
    bool cent;

    cin >> n >> m >> q;

    Graph G(n, m);


// creacion de nodos y meterlos a un mapa
    for (int i = 0; i < n; i++) {
        cin >> nom >> x >> y >> cent;
        Nodo temp(i, nom, x, y, cent);
        mapaStr.emplace(nom, temp);
        mapaInt.emplace(i,temp);
    }

    string a, b;
    int c;
// lectura de arcos
    for(int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        auto itA = mapaStr.find(a);
        int idA = itA -> second.id;
        auto itB = mapaStr.find(b);
        int idB = itB -> second.id;
        G.addEdge(idA, idB, c);
    }

    cout << "-------------------" << endl;
    cout << "1 - Cableado optimo de nueva conexion." << endl << endl;
    G.kruskalMST();

    cout << endl;
    cout << "Costo Total: " << G.costMSTKruskal << endl << endl;

    cout << "-------------------" << endl;
    cout << "2 - La ruta optima." << endl;
    
}

// Entrada
// La primer linea llegan n (2 <= n <= 30), M (1 <= m <= 400) y q (1 <= q <= 10), n = cantidad de colonias, m = número de conexiones entre colonias, q = futuras nuevas colonias que se desean conectar.
//Luego vienen n lineas con el nombre de la colonia (no contienen espacios), su posición en el plano carteciano y si es o no una central.
// Posteriomente vienen m lineas con las conexiones entre colonias y su costo.
// Por último vienen q lineas con el nombre de la nueva colonia (no contiene espacios) y los puntos cartecianos de las nuevas colonias.

/*
5 8 2
LindaVista 200 120 1
Purisima 150 75 0
Tecnologico -50 20 1
Roma -75 50 0
AltaVista -50 40 0
LindaVista Purisima 48
LindaVista Roma 17
Purisima Tecnologico 40
Purisima Roma 50
Purisima AltaVista 80
Tecnologico Roma 55
Tecnologico AltaVista 15
Roma AltaVista 18
Independencia 180 -15
Roble 45 68
*/
