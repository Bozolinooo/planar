#include <bits/stdc++.h>

using namespace std;

void converte_inteiro_base(int num, int *resultado, int base){
	int j = 0;
	
	while(num != 0){
		resultado[j] = num % base;
		num = num / base;
		j++;
	}
	
}

void gerar_orientacao(char** mat_adj, char** mat_adj_orientada, int n_vertices, int num_orientacao, int n_arestas){
	int i, j, cont = 0;
	int orientacao[n_arestas];
	
	for(i = 0; i < n_arestas; i++)
		orientacao[i] = 0;
	
	for(i = 0; i < n_vertices; i++)
		for(j = 0;j < n_vertices; j++)
			mat_adj_orientada[i][j] = 0;
	
	converte_inteiro_base(num_orientacao, orientacao, 2);
	
	for(i = 0; i < n_vertices; i++){
		for(j = i + 1; j < n_vertices; j++){
			if(mat_adj[i][j] != 0){
				if (orientacao[cont] == 0){
					mat_adj_orientada[i][j] = mat_adj[i][j];
					mat_adj_orientada[j][i] = -mat_adj[i][j];
				} else{
					mat_adj_orientada[i][j] = -mat_adj[i][j];
					mat_adj_orientada[j][i] = mat_adj[i][j];
				}
				cont++;
			}
		}
	}
}

void limpa_memoria_matriz (char** mat_adj){
	free(*mat_adj);
	free(mat_adj);
}

char** cria_matriz (int n_vertices) {
	char* valores = (char *) malloc ((n_vertices * n_vertices) * sizeof(char));
	char** linhas = (char **) malloc (n_vertices * sizeof(char*));
	int i;
	
	if((valores == NULL) || (linhas == NULL))
		exit(EXIT_FAILURE);
	
	for(i = 0; i < n_vertices; i++)
		linhas[i] = valores + i*n_vertices;
	
	return linhas;
}

int n_arestas_matriz(char** mat_adj, int n_vertices, int orientado){
	int i, j, k = 0;
	
	for(i = 0; i < n_vertices; i++)
		for(j = 0; j < n_vertices; j++) {
			if(orientado == 1) {
				if((mat_adj[i][j] == 1) || (mat_adj[i][j] == -1))
					k++;
				
			} else {
				if(mat_adj[i][j] == 1)
					k++;
			}
		}
	
	return k / 2;
}

void app(char** mat_adj, int n_vertices, int *ordem, int *coloracao){
	int i, j, l, cor_atual, flag, size, size2, q;
	vector<int> coloridos;
	char** caminhoT2;
	char** mT2;
	char** g_cor;
	
	caminhoT2 = cria_matriz(n_vertices);
	mT2 = cria_matriz(n_vertices);
	g_cor = cria_matriz(n_vertices);
	
	for(i = 0; i < n_vertices; i++) {
		coloracao[i] = -1;
		for(j = 0; j < n_vertices; j++) {
			caminhoT2[i][j] = mat_adj[i][j];
			mT2[i][j] = g_cor[i][j] = 0;
			
			if(caminhoT2[i][j] == -1)
				caminhoT2[i][j] = 0;
		}
	}
		
	for(i = 0; i < n_vertices; i++) {
		for(j = 0; j < n_vertices; j++) {
			for(l = 0; l < n_vertices; l++){
				mT2[i][j] += caminhoT2[i][l] * caminhoT2[l][j];
			}
			
			if(mT2[i][j] != 0)
				mT2[j][i] = mT2[i][j];
		}
	}
	
	coloracao[ordem[0]] = 0;
	coloridos.push_back(ordem[0]);
	
	for(q = 1; q < n_vertices; q++){
		i = ordem[q];
		
		for(cor_atual = 0;; cor_atual++){
			flag = 0;
			size = coloridos.size();
			
			for(j = 0; j < size; j++){
				if(mat_adj[i][coloridos[j]] == -1 || mat_adj[i][coloridos[j]] == 1 || mT2[i][coloridos[j]] != 0){
					if(cor_atual == coloracao[coloridos[j]]){
						flag = 1; 
						break;
					}
				}
			}
			
			if(flag == 1)
				continue;
				
			size2 = coloridos.size();
			
			for(j = 0; j < size2; j++){
				if(mat_adj[i][coloridos[j]] == 1){	
					for(l = 0; l < size2; l++){
						if(g_cor[coloracao[coloridos[j]]][coloracao[coloridos[l]]] == 1)
							if(coloracao[coloridos[l]] ==  cor_atual){									
								flag = 1;
								break;
							}
					}
				} else if(mat_adj[i][coloridos[j]] == -1){
					for(l = 0; l < size2; l++){
						if(g_cor[coloracao[coloridos[l]]][coloracao[coloridos[j]]] == 1)
							if(coloracao[coloridos[l]] ==  cor_atual){
								flag = 1;
								break;
							}
					}
				}
				
				if(flag == 1)
					break;
			}
				
				
			if(flag == 1)
				continue;
			
			coloracao[i] = cor_atual;
			coloridos.push_back(i);
			size2 = coloridos.size();
			
			for(j = 0; j < size2; j++) {
				if(mat_adj[i][coloridos[j]] == 1 && coloracao[coloridos[j]] != -1){
					g_cor[coloracao[i]][coloracao[coloridos[j]]] = 1;						
				} else if(mat_adj[i][coloridos[j]] == -1 && coloracao[coloridos[j]] != -1){
					g_cor[coloracao[coloridos[j]]][coloracao[i]] = 1;
				}
			}
			break;
		}
	}
	
	limpa_memoria_matriz(caminhoT2);
	limpa_memoria_matriz(mT2);
	limpa_memoria_matriz(g_cor);
}

int take2(int n, int el, vector<int> v, int n_vertices, int n_cores, char** mat_adj, int* coloracao) {

	v.push_back(el);
	
	int i, p, l, m, size = v.size();
	
	for(p = 0; p < size - 1; p++){
		if(mat_adj[p][size - 1] == 1) {
			if(v[p] == v[size - 1]){
				return 0;
			}
			
			for(l = 0; l < size; l++){
				for(m = 0; m < size; m++){
					if(mat_adj[l][m] == 1 && v[size - 1] == v[l] && v[m] == v[p])
						return 0;
				}
			}
			
		} else if (mat_adj[p][size - 1] == -1){
			if(v[p] == v[size - 1]){
				return 0;
			}
			
			for(l = 0; l < size; l++){
				for(m = 0; m < size; m++){
					if(mat_adj[l][m] == 1 && v[p] == v[l] && v[m] == v[size - 1])
						return 0;
				}
			}
		}
	}
	
	if(n == n_vertices - 1){
		
		for(i = 0; i < n_vertices; i++)
			coloracao[i] = i;
		
		return 1; 
	}
	

	for(i = 0; i < n_cores; i++) {
		if(take2(n + 1, i, v, n_vertices, n_cores, mat_adj, coloracao) == 1)
			return 1;
	}
	
	return 0;
}

int bfsVertice(char **mat_adj, int* ordem, int* visitado, int n_vertices, int v, int k){
	int i, j;
	stack<int> pilha;
	
	visitado[v] = 1;
	ordem[k++] = v;
	pilha.push(v);
	
    while(!pilha.empty()){
		i = pilha.top();
		pilha.pop();
		
		for(j = 0; j < n_vertices; j++){
			if(mat_adj[i][j] == 1){
				if(visitado[j] == 0){
					visitado[j] = 1;
					ordem[k++] = j;
					pilha.push(j);
				}
			}
		}
	}
	
	return k;
}

void bfs(char **mat_adj, int *ordem, int n_vertices){
	int i, k = 0;
	int *visitado = (int *) malloc (n_vertices * sizeof(int));
	
	for(i = 0; i < n_vertices; i++){
		visitado[i] = 0;
	}
	
	for(i = 0; i < n_vertices; i++){
		if(visitado[i] == 0){
			k = bfsVertice(mat_adj, ordem, visitado, n_vertices, i, k);
		}
	}
	
	free(visitado);
}

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str);
	string tok;
 
	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
 
	return internal;
}

int main(int argc, char **argv){
	int i, j, n_vertices, n_grafos, l, k, n_arestas, n_cores, sim, p, r, primeiro_grafo, ultimo_grafo;
	char** mat_adj;
	char** mat_adj_orientada;
	int* coloracao;
	int* ordem;
	vector<int> v;
	vector<string> sep;
	string name;
	
	sscanf(argv[1], "%d", &n_vertices);
	sscanf(argv[2], "%d", &n_grafos);
	sscanf(argv[3], "%d", &primeiro_grafo);
	sscanf(argv[4], "%d", &ultimo_grafo);
	
	mat_adj = cria_matriz(n_vertices);
	mat_adj_orientada = cria_matriz(n_vertices);
	coloracao = (int *) malloc (n_vertices * sizeof(int));
	ordem = (int *) malloc (n_vertices * sizeof(int));
	
	char cores[n_vertices];
	char entrada[n_grafos][n_vertices][n_vertices];
	
	for(l = 0; l < n_grafos; l++)
		for(i = 0; i < n_vertices; i++)
			for(j = 0; j < n_vertices; j++)
				entrada[l][i][j] = 0;
	
	for(l = 0; l < n_grafos; l++){
		getline(cin, name);
		name.clear();
		
		for(i = 0; i < n_vertices; i++) {
			getline(cin, name);
			sep = split(name, ' ');
			name.clear();
			
			k = sep.size();
			for(j = 1; j < k; j++){
				entrada[l][i][stoi(sep[j]) - 1] = 1;
				entrada[l][stoi(sep[j]) - 1][i] = 1;
			}
			
			sep.clear();
		}
		
		getline(cin, name);
		name.clear();
	}
	
	for(l = primeiro_grafo; l < ultimo_grafo; l++){
		printf("%d\n", l);
		fflush(stdout);
		
		for(i = 0; i < n_vertices; i++)
			for(j = 0; j < n_vertices; j++)
				mat_adj[i][j] = entrada[l][i][j];
		
		n_arestas = n_arestas_matriz(mat_adj, n_vertices, 0);
		
		for(j = 0; j < pow(2, n_arestas); j++){
			
			gerar_orientacao(mat_adj, mat_adj_orientada, n_vertices, j, n_arestas);
			
			for(i = 0; i < n_vertices; i++){
				ordem[i] = i;
				coloracao[i] = -1;
			}
				
			bfs(mat_adj_orientada, ordem, n_vertices);
			
			app(mat_adj_orientada, n_vertices, ordem, coloracao);
			
			for(i = 0; i < n_vertices; i++)
				cores[i] = 0;
				
			for(i = 0; i < n_vertices; i++)
				cores[coloracao[i]]++;
				
			n_cores = 0;
			
			for(i = 0; i < n_vertices; i++)
				if(cores[i] > 0)
					n_cores++;
					
			if(n_cores >= 16){
				for(i = 0; i < n_vertices; i++)
					coloracao[i] = -1;
				
				
				sim = 0;
				
				for(k = 0; k < 15; k++)
					if(take2(0, k, v, n_vertices, 15, mat_adj_orientada, coloracao) == 1) {
						sim = 1;
						break;
					}
	
				v.clear();
				
				if(sim == 0){
					for(p = 0; p < n_vertices; p++){
						for(r = 0; r < n_vertices; r++){
							printf("%2d ", mat_adj_orientada[p][r]);
							fflush(stdout);
						}
						printf("\n");
						fflush(stdout);
					}
				}
			}
				
		}
	}
	
	
	free(ordem);
	free(coloracao);
	limpa_memoria_matriz(mat_adj);
	limpa_memoria_matriz(mat_adj_orientada);
	
	return 0;
}
