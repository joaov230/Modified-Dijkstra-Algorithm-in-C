#### Se quiser ler esse README em português, [clique aqui](/READMEPORT.md)

# Modified Dijkstra Algorithm in C

A project for my Data Structure subject
It's basically an implementation in C of the Dijkstra algorithm, but with some other rules:

- It finds the shortest **possible** path from the point A to point C, passing through the point B (brute force by combining the weights of each path)
- By "possible" i mean: the car has 150L of gasoline and some cities have gas station and others don't

**You can find the project's definition [clicking here](/Project-Definition/Trabalho%20Pr%C3%A1tico.pdf)**

## General Information

- The graph functions are [defined here](/mainProject/grafo.h) and [implemented here](/mainProject/grafo.c)
- The vertex list functions are [defined here](/mainProject/lista_v.h) and [implemented here](/mainProject/lista_v.c)
- The main function is [implemented here](/mainProject/main.c)
- The map that the program uses is [defined here](/mainProject/Mapa.txt)
- If you want to create another map, just change the .txt file following the rules below:

```
<name of the city (one letter)>,<1 if it has a gas station, 0 if it doesn't>
<name of the city (one letter)>,<1 if it has a gas station, 0 if it doesn't>
---
<city 1>, <city 2>, <distance between those 2 cities>
<city 1>, <city 2>, <distance between those 2 cities>
```

- You can enter how many city vertexes you want
- Those three characters `---` are neccessary to inform the program that all the cities where created, then the connections between the cities are going to be defined

## Compile and run

1.  To compile you must have GCC installed, then go to [mainProject folder](/mainProject/), run the following code on the terminal:
    `gcc -o main main.c grafo.c lista_v.c` (Not sure if it works on windows, but it works if you use the bash terminal)
2.  Run main.exe
    **How it works:**
3.  The first input is the inital vertex of the path
4.  The second input is the final vertex of the path
5.  The third input is the middle vertex of the path
6.  The output will show:

- If there's any path (and show the path and where and how much the car must refill it's gas)
- If there's no possible path at all
- If there's a path but no gas to get at the end point
- If the cities you entered doesn't exist in the map
