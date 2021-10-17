#include <iostream>
#include <vector>
//#include <unordered_set> 
#include <fstream>
#include <set>
#include <string>
using namespace std ;

struct Vertex
{
	int vertex ;
	std::vector <int> neighbours ;
	set <int>  path_parents ; 
};
class Betweenness
{
public:
	int V ;
	int E ;
	std::vector<std::vector<int> > weight ;
	std::vector<Vertex> graph ;
	Betweenness (int n_vertices, int n_edges, string inputaddress){
	 	V = n_vertices ;
	 	E = n_edges ;
	 	int A, B ;
	 	ifstream infile ;
	 	for(int i=0 ; i<V ; i++){
	 		std::vector<int> temp;
	 		for(int j=0 ; j<V ; j++){
	 			temp.push_back(0) ;
	 		}
	 		weight.push_back(temp) ;
	 	}
	 	infile.open(inputaddress);
        for (int i=0 ; i < V ; i++){
        	Vertex temp ;
        	temp.vertex = i ;
        	graph.push_back(temp) ;
        }
        for (int i=0 ; i < E ; i++){
        	infile >> A >> B >> weight[A][B] ;
        	weight[B][A] = weight[A][B] ;
        	graph[A].neighbours.push_back(B) ;
        	graph[B].neighbours.push_back(A) ;
        }
        infile.close() ;
	}

	int minDistance(std::vector<int> &dist, std::vector<bool> &visited){ 
   		int min = INT_MAX ;
   		int min_index = -1 ;
   		for (int v = 0; v < V; v++) 
     		if (visited[v] == false && dist[v] <= min){ 
         		min = dist[v] ;
         		min_index = v ; 
         	}
   		return min_index; 
	} 
	void Dijkstra_shortestpath(int source, std::vector<int> &dist){
		std::vector<bool> visited ;
		int v ;
     	for (int i = 0; i < V; i++){
        	dist.push_back(INT_MAX) ;
        	visited.push_back(false) ;
     	}
   		dist[source] = 0; 
     	for (int count = 0; count < V-1; count++){ 
       		int u = minDistance(dist, visited) ;
       		visited[u] = true ;
       		for (int i = 0 ; i < graph[u].neighbours.size() ; i++){
       			v = graph[u].neighbours[i] ;
         		if (!visited[v] && dist[u] + weight[u][v] < dist[v]){
            		dist[v] = dist[u] + weight[u][v] ; 
         		}
     		}
		}
	}
	void path_parents(int source, std::vector<int> &dist){
		int v ;
		for(int i=0 ; i<V ; i++){
			for(int j=0 ; j<graph[i].neighbours.size(); j++){
				v = graph[i].neighbours[j] ;
				if(dist[i]+weight[i][v] == dist[v]){
					graph[v].path_parents.insert(i) ;
				}
			}
		}
	}
	void Dijkstra_betweenness(int source, std::vector<int> &Lambda,
								 std::vector<int> &sigma){
		std::vector<bool> visited;
		int v ;
		for (int i = 0; i < V; i++){ 
        	Lambda.push_back(INT_MAX) ;
        	sigma.push_back(0) ;
        	visited.push_back(false) ;
        }
        sigma[source] = 1 ;
   		Lambda[source] = 0 ; 

     	for (int count = 0; count < V-1; count++){ 
       		int u = minDistance(Lambda, visited) ;
       		visited[u] = true ;
       		for (int i = 0; i < graph[u].neighbours.size() ; i++){
       			v = graph[u].neighbours[i] ;
         		if (!visited[v] && Lambda[u] + weight[u][v] <= Lambda[v]){
         			Lambda[v] = Lambda[u] + weight[u][v] ;
         			if(graph[v].path_parents.find(u) != graph[v].path_parents.end()){
         				sigma[v] = sigma[v] + sigma[u] ;
         			}
            			 
         		} 
         	}
         	cout<<"----------------------------------------"<<endl ;
         	cout<<"sigma"<<"\t" ;
         	for(int i=0;i<V;i++){
         		cout<<sigma[i]<<"\t" ;
         	}
         	cout<<endl ;
         	cout<<"lambda"<<"\t" ;
         	for(int i=0;i<V;i++){
         		if(Lambda[i]==2147483647)
         			cout<<"inf"<<"\t" ;
         		else
         			cout<<Lambda[i]<<"\t" ;
         	}
         	cout<<endl ;
         	cout<<"----------------------------------------"<<endl ;
		}
		cout<<"***"<<endl ;
	}
	void Bellmans_criterion(int source, int sink, std::vector<float> &bellman, std::vector<int> &Lambda_source,
						 std::vector<int> &sigma_source, std::vector<int> &Lambda_sink,
						 std::vector<int> &sigma_sink){
		for(int v=0 ; v<V ; v++)
			if(Lambda_source[sink] < Lambda_source[v] + Lambda_sink[v]){
				bellman.push_back(0) ;
			}
			else{
				bellman.push_back(sigma_source[v] * sigma_sink[v]) ;
			}
	
	}
	void betweenness_values(int source, int sink, std::vector<float> &betweenness){
		std::vector<int> dist_source ;
		Dijkstra_shortestpath(source, dist_source) ;
		path_parents(source, dist_source) ;
		std::vector<int> Lambda_source ;
		std::vector<int> sigma_source;
		Dijkstra_betweenness(source, Lambda_source, sigma_source) ;

		std::vector<int> dist_sink ;
		Dijkstra_shortestpath(sink, dist_sink) ;
		path_parents(source, dist_sink) ;
		std::vector<int> Lambda_sink ;
		std::vector<int> sigma_sink ;
		Dijkstra_betweenness(sink, Lambda_sink, sigma_sink) ;
		
		std::vector<float> bellman ;
		Bellmans_criterion(source, sink, bellman, Lambda_source, sigma_source, Lambda_sink, sigma_sink) ;
		std::vector<float> SIGMA;
		float total_shortestpath = sigma_sink[source] ;
		cout<<"--- Belman criterion Values ---"<<endl ;
		for(int i=0 ; i<V ; i++)
			cout<<i<<":: "<<" "<<bellman[i]<<endl ;
		for(int i=0 ; i<V ; i++){
			SIGMA.push_back(bellman[i]/total_shortestpath) ;
		}
		cout<<"--- Values of Sigma over all vertices ---"<<total_shortestpath<<endl ;
		for(int i=0 ; i<V ; i++)
			cout<<i<<":: "<<" "<<SIGMA[i]<<endl ;

		for (int i = 0; i < V; ++i)
		{	
			if(i!=source && i!=sink)
				betweenness[i] += SIGMA[i] ;
		}
	}
};

int main(){  
	int V = 8 ;
	int E = 13 ;
	string inputaddress ;
	cout<<"Number of vertices :: " ;
	cin>>V ;
	cout<<"Number of edges :: " ;	
	cin>>E ;
	cout<<"Complete address of input file :: " ;
	cin>>inputaddress ;
	Betweenness G(V, E, inputaddress) ;
	std::vector<float> betweenness ;
	for (int i=0 ; i<V ; i++)
		betweenness.push_back(0) ;
	for (int i=0 ; i<V ; i++)
		for(int j=i+1 ; j<V ; j++)
			if(i!=j)
				G.betweenness_values(i,j,betweenness) ;

	cout<<"--- Betweenness ---"<<endl ;
		for(int i=0 ; i<V ; i++)
			cout<<i<<":: "<<" "<<betweenness[i]<<endl ;
	return 0;
}
