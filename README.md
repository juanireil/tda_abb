# Trabajo Practico Algoritmos y Programacion 2
# TDA ABB
## Explicacion del Trabajo:
### Implementacion:
Esta implementación se trata de un árbol de busqueda binaria. En este caso, se nos brindaron las estructuras a usar y las primitivas a implementar. Estas primitivas nos permiten crear un arbol, insertar, borrar, buscar, recorrer y destruir un arbol. Además se implementaron funciones para saber si el arbol esta vacio, obtener su raiz y un iterador interno para recorrer el arbol de diversas maneras.


### Compilación y ejecución:
Para simplificar la compilación y ejecución del programa opté por armar un makefile. En este, dividí en dos casos diferentes, uno en el que se quiera ejecutar usando las minipruebas y el otro en el que se desee ejecutar usando mis pruebas.

Para el caso de las minipruebas se debe ejecutar primero el comando:

- _make minipruebas_

Posteriormente se ejecutara el comando:

- _make valgrind_minipruebas_

Con estos dos comandos ya se habrá compilado y ejecutado mostrando asi lo esperado por pantalla. En el caso de querer ejecutar mis pruebas las lineas de comando son las mismas reemplazando minipruebas por pruebas. En este caso seria:

- _make pruebas_

- _make valgrind_pruebas_

Además, en el make esta la opcion de realizar un debugging usando gdb si se reemplaza valgrind por debug en la linea de comando. Por ultimo si se desea borrar todo lo generado en las compilaciones anteriores simplemente se ejecuta el comando

- _make clean_

En los casos en que se compila, la linea de compilacion es:

- _gcc pruebas.c abb.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0_

(Aclaración: Los nombres a poner son los de los archivos a compilar si se desean las minipruebas se pone en lugar de pruebas.c)

Para la ejecución con valgrind la linea de comando que se ejecuta es:

- _valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

### Consideraciones tomadas:

- En todas las funciones es tomado como un error que se reciba un arbol NULL, en caso de recibirlo se devuelve lo correspondiente para indicar un error.

- Un arbol NULL es considerado como vacío.

- En las operaciones de borrado si el arbol está vacio se devuelve lo correspondiendte a un error.

- Si el arbol no posee comparador  se devuelve error.

- El arbol puede funcionar perfectamente sin un destructor siempre y cuando el elemento no requiera ningun cuidado particular para su eliminación.

- El usuario podra insertar elementos NULL si así lo desea, pero deberá tener un comparador que lo sepa manejar correctamente.

- En las eliminaciones se reemĺazará por el predecesor inorden.

- En caso de tener un elemento repetido la implementacion lo soportará sin inconvenientes, siempre devolverá o borrará el primero de ellos que encuentre.

- La implementacion soporta elementos iguales (misma clave), pero no que esten en la misma direccion de memoria.

-  En los recorridos, se debe recibir un vector con una capacidad para alguna cantidad de elementos y esta capacidad, sino se tomará como que no se recorrió ninguno.

- El iterador interno, en caso de recivir una funcion NULL, o un recorrido invalido devolverá error.

- Los archivos abb.h y pa2mm.h fueron brindados, no son de elaboración propia.
## Explicacion de conceptos teoricos:
### Arbol Binario y ABB:

Un arbol binario es un tipo de dato abstracto el cual puede estar vacio o poseer un nodo raiz. Este nodo raíz contiene 2 arboles binarios como nodos hijos, uno a la izquierda y el otro a la derecha. Un ABB (Arbol de Busqueda Binaria) es un tipo de arbol binaria el cuál posee la particularidad de estar ordenado. En este siempre el hijo izquierdo será menor al padre y el hijo derecho será mayor al padre siendo esta la diferencia con un Arbol Binario común.

Sus operaciones basicas constan de crear, destruir, vacio, insertar, eliminar, buscar y recorrer. Además como es evidente el hecho de estar ordenado llevá a reducir la complijadad de las operaciones de eliminacion y busqueda.

### Funcion de destrucción:

En esta implementación, en nuestra estructura se posee un campo llamado destructor. Este sería la función de destrucción. Esto posibilita que el usuario pueda ingresar cualquier tipo de informacion en su nodo que insertará al arbol si armó e ingresó el destructor correcto.

Si no se poseé dicha función unicamente se podríán ingresar elementos que no requieran ningun tratamiento específico para su eliminación. Por ejemplo no se podrían insertar elementos del heap ya que no habría forma de hacer el free necesario para estos. Así se puede ver que el principal motivo de esto es la flexibilidad que le brinda a la implementacion.

### Complejidad de las operaciones:
#### Arbol crear:
En esta operación, el proceso es siempre el mismo, se verifica estar recibiendo lo necesario, se reserva la memoria y se inicializan los valores. Por esto la complejidad de esta como no varía el proceso según elementos ni nada será:
- O(1)
#### Arbol_insertar, borrar y buscar:
En estas operaciones, se recorre el arbol busando o la posicion a insertar o un elemento en específico. Ahora, al saber que el arbol es de busqueda binaria, sabemos que el elemento de la izquierda es menor y el de la derecha es mayor. Por esto, con cada vez que avanzamos reducimo n/2 elementos hasta llegar a la posición deseada. Esto se podría traducir a _1 = N/2^x_. Al resolver esto obtenemos _x = log(n)_. Por esto la compleidad será:
- log(n)

Aclaración: En caso de que el arbol deforme a lista la coplejidad se vuelve de _O(n)_ ya que necesitará recorrer todos los elementos y no puede descartar.


#### Arbol_raiz:
En esta operación, el proceso es siempre el mismo, se verifica estar recibiendo lo necesario y se devuelve la raíz. Por esto la complejidad de esta como no varía el proceso según elementos ni nada será:
- O(1)
#### Arbol_vacio:
En esta operación, el proceso es siempre el mismo, se verifica estar recibiendo lo necesario, se reserva la memoria y se verifica tener raiz. Por esto la complejidad de esta como no varía el proceso según elementos ni nada será:
- O(1)
#### Funciones recorrido del arbol:
En estas operaciones el porceso es entrar por la raiz, avanzar al siguiente, luego al siguiente y asi sucesivamente pero siempre en un mismo orden establecida segun el recorrido. Cada vez que se ingresa a un elemento, eso posee una complejidad fija _O(1)_. Ahora este proceso, se va a hacer n veces por los n elementos. Entónces la complejidad de estas opeaciones será:
- O(n)


#### Arbol_destruir:
Esta operación al igual que las de los recorridos deve recorrer todos los nodos eliminanadolos 1 por 1. Entónces, hará n veces una operación de complejidad _O(1)_. Por esto su complejidad será:
- O(n)

#### Abb_con_cada_elemento:
En esta operación el funcionamiento es igual que los recorridos ya que se recorre en arbol con un recorrido indicado. La diferencia radica en que se aplica una función específica en cada uno pero esto no afectará a la complejidad. La complejidad de cada nodo será _O(1)_ y se recorren n nodos. Entónces la complejidad de estas opeaciones será:
- O(n)
