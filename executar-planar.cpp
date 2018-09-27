#include <bits/stdc++.h>

using namespace std;

int main (int argc, char *argv[]){
	int n_threads, n_grafos, n_vertices, i, k;
	char mensagem[200];
	
	sscanf(argv[1], "%d", &n_vertices);
	sscanf(argv[2], "%d", &n_threads);
	sscanf(argv[3], "%d", &n_grafos);
	
	sprintf(mensagem, "gcc planarread.c -o planarread");
	system(mensagem);
	
	sprintf(mensagem, "g++ planar.cpp -o planar");
	system(mensagem);
	
	sprintf(mensagem, "./planarread < quadrangulations_20.pl > entrada.txt");
	system(mensagem);
	
	k = n_grafos / n_threads;
	
	for(i = 0; i < n_threads - 1; i++){
		
		sprintf(mensagem, "./planar %d %d %d %d < entrada.txt > testePlanar%dv%d.out &", n_vertices, n_grafos, i * k, (i + 1) * k, n_vertices, i);
		system(mensagem);
		
	}
		
	sprintf(mensagem, "./planar %d %d %d %d < entrada.txt > testePlanar%dv%d.out &", n_vertices, n_grafos, i * k, ((i + 1) * k) + (n_grafos % n_threads), n_vertices, i);
	system(mensagem);
	
	
	return 0;
}


