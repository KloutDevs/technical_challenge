# IT4W Technical Challenge

Este proyecto implementa un grafo dirigido ponderado con múltiples funcionalidades, incluyendo la detección y eliminación de ciclos negativos y la búsqueda de caminos más cortos teniendo en cuenta múltiples objetivos (costo y tiempo de entrega).

## Decisiones de Diseño y Estrategias de Optimización

### 1. Diseño de Estructura de Datos
- **Grafo dirigido ponderado**: Se utiliza un grafo dirigido representado mediante una lista de adyacencia con nodos que tienen costos y tiempos asociados. Esto permite almacenar eficientemente las conexiones y atributos del grafo.
  
- **Estructuras auxiliares**: Se definen estructuras como `Edge` para representar aristas y `Label` para gestionar las etiquetas en la estrategia multiobjetivo.

- **Listas de adyacencia**: Almacenar el grafo en listas de adyacencia en lugar de matrices de adyacencia reduce significativamente el consumo de memoria, especialmente en grafos dispersos.

### 2. Optimización de Algoritmos
- **Eliminación de ciclos negativos**: Se utiliza una modificación del algoritmo Bellman-Ford para detectar y eliminar ciclos negativos. Esta técnica asegura que los caminos calculados sean viables en términos de costo y tiempo.

- **Uso de `deque`**: Para mejorar el rendimiento del algoritmo, utilicé `deque` (doble cola) en conjunto con las estrategias SLF (Smallest Label First) y LLL (Large Label Last). SLF prioriza el procesamiento de los nodos con la menor distancia actual, mientras que LLL retrasa la expansión de los nodos con etiquetas grandes. Esto ayuda a que las actualizaciones de distancia sean más eficientes.

- **Optimización de caminos múltiples**: El algoritmo emplea un enfoque basado en etiquetas y una cola de prioridad con ordenamiento parcial para encontrar caminos eficientes bajo dos criterios (costo y tiempo). Esto permite la comparación simultánea y eliminación de soluciones sub-óptimas.


## Documentación del Diseño del Algoritmo y Decisiones de Optimización

### Algoritmo de Detección y Eliminación de Ciclos Negativos
Hice uso de una modificación del algoritmo Bellman-Ford llamada SPFA (Single-Source Shortest Path First) ya que las instrucciones era poder detectar ciclos negativos y eliminarlos, junto con esto debía trabajar con grafos muy grandes, por lo cual entre muchos algoritmos como Dijkstra, Johnson, o D'Esopo-Pape, etcétera, decidí utilizar el algoritmo SPFA con mejoras para conseguir el mejor rendimiento sin perder la eficacia del algoritmo.

El método `removeNegativeCycles` implementa una variante del algoritmo Bellman-Ford, los pasos son los siguientes:
1. Inicializar distancias y predecesores.
2. Relajar aristas múltiples veces y detectar ciclos negativos si el número de actualizaciones excede un límite.
3. Marcar nodos y aristas en ciclos negativos para su posterior eliminación.
4. Remover los nodos y aristas involucradas en ciclos negativos, asegurándose que el grafo final no contenga dichos ciclos.
5. **Optimización de rendimiento**:
    - Utilización de `deque` para gestionar eficientemente los nodos.
    - Implementación de estrategias SLF (Smallest Label First) y LLL (Large Label Last) para optimizar el ordenamiento de los nodos en la cola, priorizando los de menor costo y retrasando los de costo mayor. Esto mejora la eficiencia de las actualizaciones de las distancias.

### Algoritmo de Caminos Múltiples Multi-Objetivo
El método `multiObjectiveShortestPath` calcula rutas óptimas considerando múltiples objetivos (costo y tiempo):
1. Se inicializa un conjunto de soluciones por nodo y una cola de prioridad para gestionar las etiquetas.
2. Se evalúan todos los nodos utilizando la cola de prioridad, expandiendo y comparando soluciones para evitar las sub-óptimas.
3. Se mantienen las mejores soluciones en términos de costo y tiempo para cada nodo, y se eliminan las dominadas.
4. Al final del proceso, se presentan rutas óptimas ordenadas para un número máximo de nodos especificado por el usuario.

### Testeo y Validación
Implementé diversas pruebas unitarias usando `gtest` para comprobar:
- La correcta inicialización del grafo y adición de aristas (`Constructor`, `AddEdge`).
- La lectura y manejo de archivos (`FromFileValid`, `FromFileInvalid`, `FromFileNonexistent`).
- La eliminación de ciclos negativos (`RemoveNegativeCycles`).
- La ejecución del algoritmo multi-objetivo (`MultiObjectiveShortestPath`, `DisconnectedGraph`, `LargeGraph`).
- El manejo de casos de entrada inválidos (`InvalidInputs`).

Este enfoque asegura la robustez del código y permite detectar y corregir errores de manera eficiente.

Para realizar las pruebas, utilicé un servidor con las siguientes especificaciones:

- **CPU**: Intel Xeon Scalable - 4 núcleos
- **RAM**: 32 GB

Este servidor está alojado en Google Cloud Platform (GCP) y cuenta con un procesador optimizado para tareas de alto rendimiento en entornos de servidor.

#### Pruebas realizadas con Grafos

|  Prueba | Descripción  | Resultado  | Ejecución |
| ------------ | ------------ | ------------ | ---------- |
| Grafo simple con tiempos| Grafo con una variación entre más costoso y menor tiempo, el algoritmo debe mostrar ambos resultados especificando cuál es más costoso y cuál tiene un tiempo reducido  | Pasa  | ./GraphMain 1 0 5 |
| Grafo con ciclo nomal | El algoritmo debe validar que el grafo no tiene ningún ciclo negativo ya que la suma de los pesos es 1 en la prueba  | Pasa  | ./GraphMain 2 0 5 |
| Grafo con un solo ciclo negativo  | Grafo de 10 nodos con un ciclo negativo del nodo 0 al 4, el algoritmo debe eliminar los nodos de 0 al 4 pero permitiendo ver las rutas del 5 al 9  | Pasa  | ./GraphMain 3 0 5   - ./GraphMain 3 5 10
| Grafo de gran escala | El algoritmo debe poder procesar un grafo de 100 mil nodos y 1 millón de aristas para comprobar el rendimiento del mismo  | Pasa  | ./GraphMain 6 9584 10
| Grafo sin diferencia de tiempo | El grafo no debe contar con diferencias en tiempos de entrega en ninguna ruta, permitiendo encontrar la mejor ruta de una manera tradicional sin contar el tiempo  | Pasa  | ./GraphMain 2 0 5

## Instrucciones de Ejecución

Clonar el proyecto:

```sh
git clone https://github.com/KloutDevs/technical_challenge.git
cd technical_challenge
```

Necesitarás tener instalado `g++` y `cmake`.
Para compilar y ejecutar el proyecto:

```sh
cmake .
make
cd build
./GraphMain Opción Nodo_Origen MáximoResultados
```

Para ejecutar las pruebas necesitarás tener instalado `gtest`:
Ejecutar las pruebas:

```sh
./GraphTest
```