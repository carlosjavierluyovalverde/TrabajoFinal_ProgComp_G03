#include "trie.h"

// Constructor de NodoTrie
NodoTrie::NodoTrie() {
    esFinDePalabra = false;
    for (int i = 0; i < ALFABETO; i++) {
        hijos[i] = nullptr;
    }
}

// Constructor de Trie
Trie::Trie() {
    raiz = new NodoTrie();
}

// Destructor de Trie
Trie::~Trie() {
    liberarNodo(raiz);
}

// Función recursiva para liberar nodos
void Trie::liberarNodo(NodoTrie* nodo) {
    if (!nodo) return;
    for (int i = 0; i < ALFABETO; i++) {
        if (nodo->hijos[i]) {
            liberarNodo(nodo->hijos[i]);
        }
    }
    delete nodo;
}

// Insertar cadena en el Trie
void Trie::insertar(const string& clave) {
    NodoTrie* actual = raiz;
    for (unsigned char c : clave) {
        if (!actual->hijos[c]) {
            actual->hijos[c] = new NodoTrie();
        }
        actual = actual->hijos[c];
    }
    actual->esFinDePalabra = true;
}

// Buscar cadena exacta en el Trie
bool Trie::buscar(const string& clave) const {
    NodoTrie* actual = raiz;
    for (unsigned char c : clave) {
        if (!actual->hijos[c]) return false;
        actual = actual->hijos[c];
    }
    return actual->esFinDePalabra;
}

// Calcular la longitud de la subcadena filtrada más larga
size_t Trie::longestSubcadena(const string& clave) const {
    size_t n = clave.size();
    size_t maxLen = 0;
    for (size_t i = 0; i < n; i++) {
        NodoTrie* actual = raiz;
        for (size_t j = i; j < n; j++) {
            unsigned char c = static_cast<unsigned char>(clave[j]);
            if (!actual->hijos[c]) break;
            actual = actual->hijos[c];
            if (actual->esFinDePalabra) {
                size_t lengthFound = (j - i + 1);
                if (lengthFound > maxLen) {
                    maxLen = lengthFound;
                }
            }
        }
    }
    return maxLen;
}
