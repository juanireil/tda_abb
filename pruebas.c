#include  "abb.h"
#include "pa2mm.h"


static const int NO_SE_PUDO_INSERTAR = -1, SE_PUDO_INSERTAR = 0;
static const int SE_PUDO_BORRAR = 0, NO_SE_PUDO_BORRAR = -1;
static const bool ARBOL_VACIO = true, ARBOL_CON_ELEMENTOS = false;
const size_t ARBOL_SIN_ELEMENTOS = 0;
const size_t NINGUN_ELEMENTO_RECORRIDO = 0;
#define ABB_RECORRER_INORDEN   0


typedef struct cosa{
    int clave;
    char contenido[10];
}cosa;

void destruir_cosa(cosa* c){
    free(c);
}

int comparar_cosas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;

    if(((cosa*)elemento1)->clave>((cosa*)elemento2)->clave)
        return 1;
    if(((cosa*)elemento1)->clave<((cosa*)elemento2)->clave)
        return -1;
    return 0;
}

void destructor_de_cosas(void* elemento){
    if(!elemento)
        return;
    destruir_cosa((cosa*)elemento);
}

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
        c->clave = clave;
    return c;
}

bool mostrar_elemento(void* elemento, void* extra){
    extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
    if(elemento)
        printf("%i ", ((cosa*)elemento)->clave);
    return false;
}

void probar_arbol_crear(){
    abb_t* arbol;
    pa2m_afirmar(arbol_crear(NULL, destructor_de_cosas) == NULL, "No puedo crear un arbol sin comparador");

    pa2m_afirmar((arbol=arbol_crear(comparar_cosas, destructor_de_cosas)) != NULL, "Puedo crear un arbol");
    pa2m_afirmar((*arbol).nodo_raiz == NULL, "Arbol creado nodo raiz en NULL");
    pa2m_afirmar((*arbol).comparador == comparar_cosas, "Arbol creado comparador correcto");
    pa2m_afirmar((*arbol).destructor == destructor_de_cosas, "Arbol creado destructor correcto");
    
    arbol_destruir(arbol);

}

void probar_arbol_insertar(){
    abb_t* arbol;
    cosa* cosa_1;
    cosa* cosa_2;
    cosa* cosa_3;
    cosa* cosa_4;
    
    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    cosa_1 = crear_cosa(3);
    pa2m_afirmar(arbol_insertar(NULL, cosa_1) == NO_SE_PUDO_INSERTAR, "Arbol invalido no se puede insertar");
    pa2m_afirmar(arbol_insertar(arbol, cosa_1) == SE_PUDO_INSERTAR, "Arbol vacio se puede insertar");
    pa2m_afirmar(((*arbol).nodo_raiz->elemento) == cosa_1, "Se inserto el elemento correctamente");
    pa2m_afirmar(((*arbol).nodo_raiz->izquierda) == NULL, "Izquierda apunta a NULL");
    pa2m_afirmar(((*arbol).nodo_raiz->derecha )== NULL, "Derecha apunta a NULL");

    cosa_2 = crear_cosa(2);
    pa2m_afirmar(arbol_insertar(arbol, cosa_2) == SE_PUDO_INSERTAR, "Arbol con un elemento se puede insertar a la izquierda");
    pa2m_afirmar(((*arbol).nodo_raiz->izquierda->elemento) == cosa_2, "Izquierda apunta a correctamente al nuevo elemento");
    pa2m_afirmar(((*arbol).nodo_raiz->derecha) == NULL, "Derecha apunta a NULL");

    cosa_3 = crear_cosa(8);
    pa2m_afirmar(arbol_insertar(arbol, cosa_3) == SE_PUDO_INSERTAR, "Arbol con dos elementos se puede insertar a la derecha");
    pa2m_afirmar(((*arbol).nodo_raiz->derecha->elemento) == cosa_3, "Derecha apunta al nuevo elemento");

    cosa_4 = crear_cosa(3);
    pa2m_afirmar(arbol_insertar(arbol, cosa_4) == SE_PUDO_INSERTAR, "Arbol con tres elementos se inserta elemento repetido");
    pa2m_afirmar(((*arbol).nodo_raiz->izquierda->derecha->elemento) == cosa_4, "Elemento repetido ubicado correctamente a la izquierda");

    arbol_destruir(arbol);
}

void probar_arbol_borrar(){
    abb_t* arbol;

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    cosa* cosa_1;
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6;
    cosa* cosa_7;
    cosa* cosa_8;
    cosa* cosa_9;

    pa2m_afirmar(arbol_borrar(NULL, cosa_3) == NO_SE_PUDO_BORRAR, "Arbol invalido no se puede borrar");
    pa2m_afirmar(arbol_borrar(arbol, cosa_3) == NO_SE_PUDO_BORRAR, "Arbol vacio no se puede borrar");
    pa2m_afirmar(arbol_borrar(arbol, NULL) == NO_SE_PUDO_BORRAR, "Elemento invalido no se puede borrar");

    arbol_insertar(arbol, cosa_3);
    pa2m_afirmar(arbol_borrar(arbol, cosa_3) == SE_PUDO_BORRAR, "Arbol con un elemento se pudo borrar");
    pa2m_afirmar((*arbol).nodo_raiz == NULL, "Arbol con un elemento ahora esta vacio");

    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_2);
    pa2m_afirmar(arbol_borrar(arbol, cosa_4) == SE_PUDO_BORRAR, "Arbol con varios elementos borro raiz con 2 hijos");
    pa2m_afirmar((*arbol).nodo_raiz->elemento == cosa_2, "Nueva raiz es la correcta");
    pa2m_afirmar((*arbol).nodo_raiz->izquierda == NULL, "Hijo izquierdo es NULL");
    pa2m_afirmar((*arbol).nodo_raiz->derecha->elemento == cosa_5, "Hijo izquierdo es NULL");
    pa2m_afirmar(arbol_borrar(arbol, cosa_2) == SE_PUDO_BORRAR, "Raiz con un hijo borra correctamente");
    pa2m_afirmar((*arbol).nodo_raiz->elemento == cosa_5, "Nueva raiz es la correcta");

    arbol_destruir(arbol);

    cosa_1 = crear_cosa(1);
    cosa_2 = crear_cosa(2);
    cosa_3 = crear_cosa(3);
    cosa_4 = crear_cosa(4);
    cosa_5 = crear_cosa(5);
    cosa_6 = crear_cosa(6);
    cosa_7 = crear_cosa(7);
    cosa_8 = crear_cosa(8);
    cosa_9 = crear_cosa(9);

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_3);
    arbol_insertar(arbol, cosa_8);
    arbol_insertar(arbol, cosa_2);
    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_7);
    arbol_insertar(arbol, cosa_6);
    arbol_insertar(arbol, cosa_9);
    arbol_insertar(arbol, cosa_1);

    pa2m_afirmar(arbol_borrar(arbol, cosa_8) == SE_PUDO_BORRAR, "Arbol con varios elementos borro hijo de la derecha");
    pa2m_afirmar((*arbol).nodo_raiz->derecha->elemento == cosa_7, "Nuevo hijo derecho es el correcto");

    cosa* cosa_aux = crear_cosa(28);
    pa2m_afirmar(arbol_borrar(arbol, cosa_aux) == NO_SE_PUDO_BORRAR, "Elemento no esta en el arbol no se puede borrar");
    
    destructor_de_cosas(cosa_aux);
    arbol_destruir(arbol);
}

void probar_buscar_elemento(){
    abb_t* arbol;

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    pa2m_afirmar(arbol_buscar(NULL, NULL) == NULL, "Arbol no existe, no se puede encontrar el elemento");
    pa2m_afirmar(arbol_buscar(arbol, NULL) == NULL, "Arbol vacio, no se puede encontrar el elemento");
    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);

    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_3);
    arbol_insertar(arbol, cosa_8);
    arbol_insertar(arbol, cosa_2);
    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_7);
    arbol_insertar(arbol, cosa_6);
    arbol_insertar(arbol, cosa_9);

    pa2m_afirmar(arbol_buscar(arbol, cosa_8) == cosa_8, "Arbol busca un elemento, lo encuentra y lo devuelve");
    pa2m_afirmar(arbol_buscar(arbol, cosa_1) == NULL, "Arbol busca un elemento, no lo encuentra devuelve NULL");

    destructor_de_cosas(cosa_1);
    arbol_destruir(arbol);
}

void probar_arbol_raiz(){
    abb_t* arbol;
    cosa* cosa_3 = crear_cosa(3);

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    pa2m_afirmar(arbol_raiz(NULL) == NULL, "Arbol no existe, no hay elemento en la raiz");
    pa2m_afirmar(arbol_raiz(arbol) == NULL, "Arbol vacio, no hay elemento en la raiz");

    arbol_insertar(arbol, cosa_3);
    pa2m_afirmar(arbol_raiz(arbol) == cosa_3, "Elemento en la raiz es el correcto");

    arbol_destruir(arbol);
}

void probar_arbol_vacio(){
    abb_t* arbol;
    cosa* cosa_3 = crear_cosa(3);

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    pa2m_afirmar(arbol_vacio(NULL) == ARBOL_VACIO, "Arbol no existe, esta vacio");
    pa2m_afirmar(arbol_vacio(arbol) == ARBOL_VACIO, "Arbol vacio, esta vacio");

    arbol_insertar(arbol, cosa_3);
    pa2m_afirmar(arbol_vacio(arbol) == ARBOL_CON_ELEMENTOS, "Arbol no esta vacio");

    arbol_destruir(arbol);
}

void probar_arbol_recorrido_inorden(){
    abb_t* arbol = NULL;
    cosa* elementos[10];
    cosa* vector[5];

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    pa2m_afirmar(arbol_recorrido_inorden(NULL, (void**) elementos, 5) == ARBOL_SIN_ELEMENTOS, "Arbol no existe, no se puede recorrer");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**) elementos, 5) == ARBOL_SIN_ELEMENTOS, "Arbol vacio, no se puede recorrer");

    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);

    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_3);
    arbol_insertar(arbol, cosa_8);
    arbol_insertar(arbol, cosa_2);
    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_7);
    arbol_insertar(arbol, cosa_6);
    arbol_insertar(arbol, cosa_9);
    arbol_insertar(arbol, cosa_1);

    pa2m_afirmar(arbol_recorrido_inorden(arbol, NULL, 5) == ARBOL_SIN_ELEMENTOS, "Vector no existe no se pueden cargar los elementos");
    pa2m_afirmar(arbol_recorrido_inorden(NULL, (void**) elementos, 0) == ARBOL_SIN_ELEMENTOS, "Vector de tamanio nulo no se pueden cargar elementos");
    
    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**) elementos, 10) == 9, "Recibe parametros validos, recorre todo el arbol y devuelve correctamente");
    
    pa2m_afirmar(elementos[0] == cosa_1, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[1] == cosa_2, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[2] == cosa_3, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[3] == cosa_4, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[4] == cosa_5, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[5] == cosa_6, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[6] == cosa_7, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[7] == cosa_8, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[8] == cosa_9, "Elemento en posicion correcta del vector");

    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**) vector, 5) == 5, "Arbol con mas elementos que la capacidad, devuelve la capacidad");
    pa2m_afirmar(vector[0] == cosa_1, "Elemento en posicion correcta del vector");
    pa2m_afirmar(vector[1] == cosa_2, "Elemento en posicion correcta del vector");
    pa2m_afirmar(vector[2] == cosa_3, "Elemento en posicion correcta del vector");
    pa2m_afirmar(vector[3] == cosa_4, "Elemento en posicion correcta del vector");
    pa2m_afirmar(vector[4] == cosa_5, "Elemento en posicion correcta del vector");

    arbol_destruir(arbol);
}

void probar_arbol_recorrido_preorden(){
    abb_t* arbol = NULL;
    cosa* elementos[10];
    cosa* vector[5];

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    pa2m_afirmar(arbol_recorrido_preorden(NULL, (void**) elementos, 5) == ARBOL_SIN_ELEMENTOS, "Arbol no existe, no se puede recorrer");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**) elementos, 5) == ARBOL_SIN_ELEMENTOS, "Arbol vacio, no se puede recorrer");

    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);

    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_3);
    arbol_insertar(arbol, cosa_8);
    arbol_insertar(arbol, cosa_2);
    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_7);
    arbol_insertar(arbol, cosa_6);
    arbol_insertar(arbol, cosa_9);
    arbol_insertar(arbol, cosa_1);

    pa2m_afirmar(arbol_recorrido_preorden(arbol, NULL, 5) == ARBOL_SIN_ELEMENTOS, "Vector no existe no se pueden cargar los elementos");
    pa2m_afirmar(arbol_recorrido_preorden(NULL, (void**) elementos, 0) == ARBOL_SIN_ELEMENTOS, "Vector de tamanio nulo no se pueden cargar elementos");
    
    pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**) elementos, 10) == 9, "Recibe parametros validos, recorre todo el arbol y devuelve correctamente");
    
    pa2m_afirmar(elementos[0] == cosa_5, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[1] == cosa_3, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[2] == cosa_2, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[3] == cosa_1, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[4] == cosa_4, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[5] == cosa_8, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[6] == cosa_7, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[7] == cosa_6, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[8] == cosa_9, "Elemento en posicion correcta del vector");

    pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**) vector, 5) == 5, "Arbol con mas elementos que la capacidad, devuelve la capacidad");
    pa2m_afirmar(elementos[0] == cosa_5, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[1] == cosa_3, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[2] == cosa_2, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[3] == cosa_1, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[4] == cosa_4, "Elemento en posicion correcta del vector");

    arbol_destruir(arbol);
}

void probar_arbol_recorrido_postorden(){
    abb_t* arbol = NULL;
    cosa* elementos[10];
    cosa* vector[5];

    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
    pa2m_afirmar(arbol_recorrido_postorden(NULL, (void**)elementos, 5) == ARBOL_SIN_ELEMENTOS, "Arbol no existe, no se puede recorrer");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)elementos, 5) == ARBOL_SIN_ELEMENTOS, "Arbol vacio, no se puede recorrer");

    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);

    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_3);
    arbol_insertar(arbol, cosa_8);
    arbol_insertar(arbol, cosa_2);
    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_7);
    arbol_insertar(arbol, cosa_6);
    arbol_insertar(arbol, cosa_9);
    arbol_insertar(arbol, cosa_1);

    pa2m_afirmar(arbol_recorrido_postorden(arbol, NULL, 5) == ARBOL_SIN_ELEMENTOS, "Vector no existe no se pueden cargar los elementos");
    pa2m_afirmar(arbol_recorrido_postorden(NULL, (void**)elementos, 0) == ARBOL_SIN_ELEMENTOS, "Vector de tamanio nulo no se pueden cargar elementos");
    
    pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)elementos, 10) == 9, "Recibe parametros validos, recorre todo el arbol y devuelve correctamente");
    
    pa2m_afirmar(elementos[0] == cosa_1, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[1] == cosa_2, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[2] == cosa_4, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[3] == cosa_3, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[4] == cosa_6, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[5] == cosa_7, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[6] == cosa_9, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[7] == cosa_8, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[8] == cosa_5, "Elemento en posicion correcta del vector");

    pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)vector, 5) == 5, "Arbol con mas elementos que la capacidad, devuelve la capacidad");
    pa2m_afirmar(elementos[0] == cosa_1, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[1] == cosa_2, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[2] == cosa_4, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[3] == cosa_3, "Elemento en posicion correcta del vector");
    pa2m_afirmar(elementos[4] == cosa_6, "Elemento en posicion correcta del vector");

    arbol_destruir(arbol);
}

void probar_abb_con_cada_elemento(){
    abb_t* arbol = NULL;
    void* extra = NULL;
    
    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    pa2m_afirmar(abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, mostrar_elemento, extra) == NINGUN_ELEMENTO_RECORRIDO, "Arbol NULL no se puede recorrer");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_elemento, extra) == NINGUN_ELEMENTO_RECORRIDO, "Arbol vacio no se puede recorrer");

    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);

    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_3);
    arbol_insertar(arbol, cosa_8);
    arbol_insertar(arbol, cosa_2);
    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_7);
    arbol_insertar(arbol, cosa_6);
    arbol_insertar(arbol, cosa_9);
    arbol_insertar(arbol, cosa_1);
    
    pa2m_afirmar(abb_con_cada_elemento(arbol, -1, mostrar_elemento, extra) == NINGUN_ELEMENTO_RECORRIDO, "Recorrido invalido no se puede recorrer");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, NULL, extra) == NINGUN_ELEMENTO_RECORRIDO, "Funcion NULL no se puede recorrer");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_elemento, extra) == 9, "Arbol se puede recorrer, recorrido inorden correctamente(Esperado 1,2,3,4,5,6,7,8,9)");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_elemento, extra) == 9, "Arbol se puede recorrer, recorrido postorden correctamente(Esperado 1,2,4,3,6,7,9,8,5)");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_elemento, extra) == 9, "Arbol se puede recorrer, recorrido preorden correctamente(Esperado 5,3,2,1,4,8,7,6,9)");

    arbol_destruir(arbol);
}

void probar_funcionamiento_abb(){
    abb_t* arbol = NULL;
    cosa* elementos[10];
    
    arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);
    cosa* cosa_aux = crear_cosa(8);

    arbol_insertar(arbol, cosa_5);
    arbol_insertar(arbol, cosa_3);
    arbol_insertar(arbol, cosa_8);
    arbol_insertar(arbol, cosa_2);
    arbol_insertar(arbol, cosa_4);
    arbol_insertar(arbol, cosa_7);
    arbol_insertar(arbol, cosa_6);
    arbol_insertar(arbol, cosa_9);
    pa2m_afirmar(arbol_insertar(arbol, cosa_1) == SE_PUDO_INSERTAR, "Se insertaron varios elementos");
    pa2m_afirmar(arbol_borrar(arbol, cosa_5) == SE_PUDO_BORRAR, "Raiz borrada correctamente");
    pa2m_afirmar(arbol_raiz(arbol) == cosa_4, "Nueva raiz es la correcta");
    pa2m_afirmar((*arbol).nodo_raiz->izquierda->elemento == cosa_3, "Hijo izquierdo es el correcto");
    pa2m_afirmar((*arbol).nodo_raiz->derecha->elemento == cosa_8, "Hijo derecho es el correcto");
    
    arbol_borrar(arbol, cosa_8);
    arbol_borrar(arbol, cosa_2);
    arbol_borrar(arbol, cosa_4);
    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)elementos, 10) == 5, "Varios elementos eliminados, quedaron 5 y se recorrieron");
    pa2m_afirmar(elementos[0] == cosa_1, "Primer elemento del vector es el correcto");
    pa2m_afirmar(elementos[1] == cosa_3, "Segundo elemento del vector es el correcto");
    pa2m_afirmar(elementos[2] == cosa_6, "Tercer elemento del vector es el correcto");
    pa2m_afirmar(elementos[3] == cosa_7, "Cuarto elemento del vector es el correcto");
    pa2m_afirmar(elementos[4] == cosa_9, "Quinto elemento del vector es el correcto");

    pa2m_afirmar(arbol_buscar(arbol, cosa_aux) == NULL, "Elemento eliminado, no lo encontrara");
    pa2m_afirmar(arbol_buscar(arbol, cosa_6) == cosa_6, "Elemento encontrado correctamente");

    pa2m_afirmar(arbol_vacio(arbol) == ARBOL_CON_ELEMENTOS, "Arbol no esta vacio");

    destructor_de_cosas(cosa_aux);
    arbol_destruir(arbol);
    
}

int main(){

    pa2m_nuevo_grupo("PRUEBAS CREACION DE ARBOL");
    probar_arbol_crear();
    pa2m_nuevo_grupo("PRUEBAS INSERCION DE ARBOL");
    probar_arbol_insertar();
    pa2m_nuevo_grupo("PRUEBAS BORRADO EN ARBOL");
    probar_arbol_borrar();
    pa2m_nuevo_grupo("PRUEBAS BUSQUEDA EN ARBOL");
    probar_buscar_elemento();
    pa2m_nuevo_grupo("PRUEBAS OTRAS FUNCIONALIDADES");
    probar_arbol_raiz();
    probar_arbol_vacio();
    pa2m_nuevo_grupo("PRUEBAS RECORRIDO INORDEN DE ARBOL");
    probar_arbol_recorrido_inorden();
    pa2m_nuevo_grupo("PRUEBAS RECORRIDO PREORDEN DE ARBOL");
    probar_arbol_recorrido_preorden();
    pa2m_nuevo_grupo("PRUEBAS RECORRIDO POSTORDEN DE ARBOL");
    probar_arbol_recorrido_postorden();
    pa2m_nuevo_grupo("PRUEBAS ITERADOR INTERNO");
    probar_abb_con_cada_elemento();
    pa2m_nuevo_grupo("PRUEBAS FUNCIONAMIENTO GLOBAL ABB");
    probar_funcionamiento_abb();
    pa2m_mostrar_reporte();
    return 0;
}
