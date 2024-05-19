The Modified BFS - Permutation Algorithm:
1. Making a graph with starting city as the first node (root)
2. Using permutation to connect starting graph to next city
3. After the permutation tree is completed, connect each brach to the starting node (end node)
4. Use BFS algorithm to find the shortest distance (By checking each node distance from the permutation layer by layer)