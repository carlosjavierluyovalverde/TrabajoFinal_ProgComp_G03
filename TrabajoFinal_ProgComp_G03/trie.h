#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <string>
using namespace std;

static const int ALFABETO = 256;

struct NodoTrie {
    bool esFinDePalabra;
    NodoTrie* hijos[ALFABETO];
    NodoTrie();
};

class Trie {
private:
    NodoTrie* raiz;
    void liberarNodo(NodoTrie* nodo);

public:
    Trie();
    ~Trie();
    void insertar(const string& clave);
    bool buscar(const string& clave) const;
    size_t longestSubcadena(const string& clave) const;
};

#endif // TRIE_H_INCLUDED
#pragma once
