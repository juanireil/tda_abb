#include "abb.h"
#include <stdio.h>
#include <stdlib.h>

static const int NO_SE_PUDO_INSERTAR = -1, SE_PUDO_INSERTAR = 0;
static const bool ARBOL_VACIO = true, ARBOL_CON_ELEMENTOS = false;
const int NO_SE_PUDO_ELIMINAR = -1, SE_PUDO_ELIMINAR = 0;
#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    abb_t* arbol = NULL;

    if(!comparador){
        return NULL;
    }
    arbol = malloc(sizeof(abb_t));
    if(!arbol){
        return arbol;
    }
    (*arbol).nodo_raiz = NULL;
    (*arbol).comparador = comparador;
    (*arbol).destructor = destructor;
    return arbol;
}
nodo_abb_t* crear_nodo(void* elemento){
    nodo_abb_t* nodo;

    nodo = malloc(sizeof(nodo_abb_t));
    if(!nodo){
        return nodo;
    }
    (*nodo).elemento = elemento;
    (*nodo).izquierda = NULL;
    (*nodo).derecha = NULL;
    return nodo;
}
void insertar (nodo_abb_t* nodo, nodo_abb_t* nodo_a_insertar, abb_comparador comparador){
    
    if(comparador((*nodo).elemento, (*nodo_a_insertar).elemento) >= 0){
        if(!(*nodo).izquierda){
            (*nodo).izquierda = nodo_a_insertar;
            return;
        }
        insertar((*nodo).izquierda, nodo_a_insertar, comparador);
    }
    else{
        if(!(*nodo).derecha){
            (*nodo).derecha = nodo_a_insertar;
            return;
        }
        insertar((*nodo).derecha, nodo_a_insertar, comparador);
    } 
}

int arbol_insertar(abb_t* arbol, void* elemento){
    nodo_abb_t* nodo;
    nodo_abb_t* nodo_a_insertar;
    nodo_a_insertar = crear_nodo(elemento);

    if(!arbol || !nodo_a_insertar){
        free(nodo_a_insertar);
        return NO_SE_PUDO_INSERTAR;
    }
    nodo = (*arbol).nodo_raiz;
    if (!(*arbol).nodo_raiz){
        (*arbol).nodo_raiz = nodo_a_insertar;
        return SE_PUDO_INSERTAR;
    }
    insertar(nodo, nodo_a_insertar, (*arbol).comparador);
    return SE_PUDO_INSERTAR; 
}

nodo_abb_t* buscar_predecesor_inorden(nodo_abb_t* nodo){
    nodo_abb_t* nodo_aux = NULL;
    if(!(*nodo).derecha){
        return(nodo);
    }
    nodo_aux = buscar_predecesor_inorden((*nodo).derecha);
    return nodo_aux;
} 

void borrar_puntero_a_predecesor(nodo_abb_t* nodo){
    if(!(*nodo).derecha->derecha){
        (*nodo).derecha = NULL;
        return;
    }
    borrar_puntero_a_predecesor((*nodo).derecha);
}

nodo_abb_t* abb_borrar(nodo_abb_t* nodo, abb_comparador comparador, abb_liberar_elemento destructor, void* elemento, bool* esta_el_elemento){
    nodo_abb_t* nodo_aux = NULL;

    if(comparador((*nodo).elemento, elemento) == 0){
        *esta_el_elemento = true;
        if(!(*nodo).derecha && !(*nodo).izquierda){
            if(destructor){
                destructor((*nodo).elemento);
            }
            free(nodo);
            return NULL;
        }
        if((*nodo).derecha && (*nodo).izquierda){
            nodo_aux = buscar_predecesor_inorden((*nodo).izquierda);
            if(comparador((*nodo).izquierda->elemento, (*nodo_aux).elemento) < 0){
                (*nodo_aux).izquierda = (*nodo).izquierda;
                borrar_puntero_a_predecesor((*nodo).izquierda);
            }
            if(destructor){
                destructor((*nodo).elemento);
            }
            (*nodo_aux).derecha = (*nodo).derecha;
            free(nodo);
            return nodo_aux;
        }
        if((*nodo).derecha){
            nodo_aux = (*nodo).derecha;
            if(destructor){
                destructor((*nodo).elemento);
            }
            free(nodo);
            return nodo_aux;
        }
        nodo_aux = (*nodo).izquierda;
        if(destructor){
            destructor((*nodo).elemento);
        }
        free(nodo);
        return nodo_aux;
    }
    if(comparador((*nodo).elemento, elemento) > 0){
        if((*nodo).izquierda){
            (*nodo).izquierda = abb_borrar((*nodo).izquierda, comparador, destructor, elemento, esta_el_elemento);
        }
    }
    else if((*nodo).derecha){
        (*nodo).derecha = abb_borrar((*nodo).derecha, comparador, destructor, elemento, esta_el_elemento);
    }

    return nodo;
}

int arbol_borrar(abb_t* arbol, void* elemento){
    bool esta_el_elemento = false;
    if(!arbol || !(*arbol).comparador || !(*arbol).nodo_raiz){
        return NO_SE_PUDO_ELIMINAR;
    }
    (*arbol).nodo_raiz = abb_borrar((*arbol).nodo_raiz, (*arbol).comparador, (*arbol).destructor, elemento, &esta_el_elemento);
    if(esta_el_elemento){
        return SE_PUDO_ELIMINAR;
    }
    return NO_SE_PUDO_ELIMINAR;
    
}

void* buscar_elemento(nodo_abb_t* nodo, void* elemento_buscado, abb_comparador comparador){
    void* elemento = NULL;
    if(comparador((*nodo).elemento, elemento_buscado) == 0){
        return (*nodo).elemento;
    }
    else if(comparador((*nodo).elemento, elemento_buscado) > 0){
        if((*nodo).izquierda){
            elemento = buscar_elemento((*nodo).izquierda, elemento_buscado, comparador);
        }
    }
    else if ((*nodo).derecha){
        elemento = buscar_elemento((*nodo).derecha, elemento_buscado, comparador);
    }
    return elemento;
}

void* arbol_buscar(abb_t* arbol, void* elemento){
    if(!arbol || !(*arbol).nodo_raiz){
        return NULL;
    }
    return buscar_elemento((*arbol).nodo_raiz, elemento, (*arbol).comparador);

}
void* arbol_raiz(abb_t* arbol){
    if(!arbol || !(*arbol).nodo_raiz){
        return NULL;
    }
    return (*arbol).nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
    if(!arbol || !(*arbol).nodo_raiz){
        return ARBOL_VACIO;
    }
    return ARBOL_CON_ELEMENTOS;
}

void recorrido_inorden(nodo_abb_t* nodo, void** vector_elementos, size_t tamanio_array, size_t* elementos_insertados){
    if(!(*nodo).izquierda && !(*nodo).derecha && (*elementos_insertados) < tamanio_array){
        vector_elementos[*elementos_insertados] = (*nodo).elemento;
        (*elementos_insertados) ++;
        return;
    }
    if((*nodo).izquierda){
        recorrido_inorden((*nodo).izquierda, vector_elementos, tamanio_array, elementos_insertados);
    }
    if(*elementos_insertados < tamanio_array){
        vector_elementos[*elementos_insertados] = (*nodo).elemento;
        (*elementos_insertados) ++;
    }
    if((*nodo).derecha){
        recorrido_inorden((*nodo).derecha, vector_elementos, tamanio_array, elementos_insertados);
    }
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){
    size_t elementos_insertados = 0;
    if(!arbol || !(*arbol).nodo_raiz || !array || tamanio_array == 0){
        return elementos_insertados;
    }
    recorrido_inorden((*arbol).nodo_raiz, array, tamanio_array, &elementos_insertados);
    return elementos_insertados;
}

void recorrido_preorden(nodo_abb_t* nodo, void** vector_elementos, size_t tamanio_array, size_t* elementos_insertados){
    if(*elementos_insertados < tamanio_array){
        vector_elementos[*elementos_insertados] = (*nodo).elemento;
        (*elementos_insertados) ++;
    }
    if((*nodo).izquierda){
        recorrido_preorden((*nodo).izquierda, vector_elementos, tamanio_array, elementos_insertados);
    }
    if((*nodo).derecha){
        recorrido_preorden((*nodo).derecha, vector_elementos, tamanio_array, elementos_insertados);
    }   
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){
    size_t elementos_insertados = 0;
    if(!arbol || !(*arbol).nodo_raiz || !array || tamanio_array == 0){
        return elementos_insertados;
    }
    recorrido_preorden((*arbol).nodo_raiz, array, tamanio_array, &elementos_insertados);
    return elementos_insertados;
}

void recorrido_postorden(nodo_abb_t* nodo, void** vector_elementos, size_t tamanio_array, size_t* elementos_insertados){
    if(!(*nodo).izquierda && !(*nodo).derecha && (*elementos_insertados) < tamanio_array){
        vector_elementos[*elementos_insertados] = (*nodo).elemento;
        (*elementos_insertados) ++;
        return;
    }
    if((*nodo).izquierda){
        recorrido_postorden((*nodo).izquierda, vector_elementos, tamanio_array, elementos_insertados);
    }
    if((*nodo).derecha){
        recorrido_postorden((*nodo).derecha, vector_elementos, tamanio_array, elementos_insertados);
    }
    if(*elementos_insertados < tamanio_array){
        vector_elementos[*elementos_insertados] = (*nodo).elemento;
        (*elementos_insertados) ++;
    }   
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
    size_t elementos_insertados = 0;
    if(!arbol || !(*arbol).nodo_raiz || !array || tamanio_array == 0){
        return elementos_insertados;
    }
    recorrido_postorden((*arbol).nodo_raiz, array, tamanio_array, &elementos_insertados);
    return elementos_insertados;
}

void eliminar_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    if(!(*nodo).izquierda && !(*nodo).derecha){
        if(destructor){
            destructor((*nodo).elemento);
        }
        free(nodo);
        return;
    }
    if((*nodo).izquierda){
        eliminar_nodo((*nodo).izquierda, destructor);
    }
    if((*nodo).derecha){
        eliminar_nodo((*nodo).derecha, destructor);
    }
    if(destructor){
        destructor((*nodo).elemento);
    }    
    free(nodo);
}

void arbol_destruir(abb_t* arbol){
    if(arbol){
        if((*arbol).nodo_raiz && (*arbol).comparador){
        eliminar_nodo((*arbol).nodo_raiz, (*arbol).destructor);
    }
    free(arbol);
    }
    
}

void recorrer_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* elementos_recorridos, bool* no_continua_recorriendo){ 
    if(!(*nodo).izquierda && !(*nodo).derecha){
        (*elementos_recorridos)++;
        *no_continua_recorriendo = funcion((*nodo).elemento, extra);
        return;
    }
    if((*nodo).izquierda && !(*no_continua_recorriendo)){
        recorrer_inorden((*nodo).izquierda, funcion, extra, elementos_recorridos,no_continua_recorriendo);
    }
    if(!(*no_continua_recorriendo)){
        (*elementos_recorridos) ++;
        *no_continua_recorriendo = funcion((*nodo).elemento, extra);
    }
    if((*nodo).derecha && !(*no_continua_recorriendo)){
        recorrer_inorden((*nodo).derecha, funcion, extra, elementos_recorridos,no_continua_recorriendo);
    }
}

void recorrer_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* elementos_recorridos, bool* no_continua_recorriendo){ 
    if(!(*nodo).izquierda && !(*nodo).derecha){
        (*elementos_recorridos)++;
        *no_continua_recorriendo = funcion((*nodo).elemento, extra);
        return;
    }
    if((*nodo).izquierda && !(*no_continua_recorriendo)){
        recorrer_postorden((*nodo).izquierda, funcion, extra, elementos_recorridos,no_continua_recorriendo);
    }
    if((*nodo).derecha && !(*no_continua_recorriendo)){
        recorrer_postorden((*nodo).derecha, funcion, extra, elementos_recorridos,no_continua_recorriendo);
    }

    if(!(*no_continua_recorriendo)){
        (*elementos_recorridos) ++;
        *no_continua_recorriendo = funcion((*nodo).elemento, extra);
    }
}

void recorrer_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* elementos_recorridos, bool* no_continua_recorriendo){ 

    if(!(*no_continua_recorriendo)){
        (*elementos_recorridos) ++;
        *no_continua_recorriendo = funcion((*nodo).elemento, extra);
    }
    if((*nodo).izquierda && !(*no_continua_recorriendo)){
        recorrer_preorden((*nodo).izquierda, funcion, extra, elementos_recorridos,no_continua_recorriendo);
    }
    if((*nodo).derecha && !(*no_continua_recorriendo)){
        recorrer_preorden((*nodo).derecha, funcion, extra, elementos_recorridos, no_continua_recorriendo);
    } 
}
size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
    bool no_continua_recorriendo = false;
    size_t elementos_recorridos = 0;
    if(!arbol || !funcion || !(*arbol).nodo_raiz){
        return elementos_recorridos;
    }
    if(recorrido == ABB_RECORRER_INORDEN){
        recorrer_inorden((*arbol).nodo_raiz, funcion, extra, &elementos_recorridos, &no_continua_recorriendo);
    }
    if(recorrido == ABB_RECORRER_POSTORDEN){
        recorrer_postorden((*arbol).nodo_raiz, funcion, extra, &elementos_recorridos, &no_continua_recorriendo);
    }
    if(recorrido == ABB_RECORRER_PREORDEN){
        recorrer_preorden((*arbol).nodo_raiz, funcion, extra, &elementos_recorridos, &no_continua_recorriendo);
    }
    return elementos_recorridos;
}