/**
 * @file vector.h
 * @author František Pártl (fpartl@kiv.zcu.cz)
 * @brief Hlavičkový soubor knihovny dynamicky realokovaného pole.
 * @date 2021-11-25
 */

#ifndef _VECTOR_H
#define _VECTOR_H

#include <stddef.h>

/** @brief Násobitel zvětšení pole při realokaci. */
#define VECTOR_SIZE_MULT 2

/** @brief Počáteční velikost vektoru. */
#define VECTOR_INIT_SIZE 4

/**
 * @brief Definice ukazatele na obecnou funkci, která umí uvolnit prvek uložený ve vektoru.
 *        Využívá se ve funkci vector_destroy.
 */
typedef void (*vec_it_dealloc_type)(void **item);

/**
 * @brief Struktura vector_
 * Přestavuje obecné dynamicky alokované pole, do které lze přidávat prvky, manipulovat s nimi a
 * odstraňovat je (včetně jeji uvolnění).
 */
typedef struct vector_ {
    size_t capacity;
    size_t count;
    size_t item_size;
    void* data;
    vec_it_dealloc_type deallocator;
} vector;

/**
 * @brief Alokuje prázdný vektor s kapacitou VECTOR_INIT_SIZE.
 * @param item_size Velikost jednoho prvku vektoru.
 * @return vector* Ukazatel na alokovaný vektor.
 */
vector *vector_create(const size_t item_size, const vec_it_dealloc_type deallocator);

/**
 * @brief Funkce provede inicializaci instance struktury vector, která je dána ukazatelem v.
 *        Datový segment bufferu bude prealokován pro VECTOR_INIT_SIZE prvků o velikosti item_size.
 * 
 * @param item_size Velikost jednoho prvku vektoru.
 * @return int 1, pokud se inicializace podařila, jinak 0.
 */
int vector_init(vector *v, const size_t item_size, const vec_it_dealloc_type deallocator);

/**
 * @brief Uvolní vektor z paměti. V případě platnosti ukazatele (*v)->deallocator je funkce, na kterou ukazatel odkazuje, zavolána
 *        nad všemi prvky vektoru. Interní pole je pak interpretováno jako pole void *.
 * @param v Ukazatel na vektor.
 */
void vector_destroy(vector **v);

/**
 * \brief Vrátí kapacitu vektoru (počet prvků, které do vektoru lze uložit bez realokace).
 * \param v Ukazatel na vektor.
 * \return size_t Kapacita vektoru, daného ukazatelem v.
 */
size_t vector_capacity(const vector *v);

/**
 * @brief Vrátí počet prvků vektoru.
 * @param v Ukazatel na vektor.
 * @return int Počet zapsaných prvků ve vektoru.
 */
size_t vector_count(const vector *v);

/**
 * @brief Říká, zda je zadaný vektor prázdný nebo obsahuje nějaké prvky.
 * @param v Ukazatel na vektor.
 * @return int 1, pokud je vektor prázdný, jinak 0.
 */
int vector_isempty(const vector *v);

/**
 * @brief Realokuje zadaný vektor tak, aby jeho kapacita byla size. Zmenšení vektoru není povoleno.
 * @param v Realokovaný vektor.
 * @param size Nová kapacita vektoru. 
 * @return int 1, pokud se realokace povedla, jinak 0.
 */
int vector_realloc(vector *v, const size_t size);

/**
 * @brief Vrací ukazatel na at-tý prvek vektoru v. V případě neexistence prvku vratí NULL.
 * 
 * @param v Ukazatel na vektor.
 * @param at Index prvku ve vektoru.
 * @return void* Ukazatel na prvek vektoru nebo NULL při neexistenci prvku.
 */
void *vector_at(const vector *v, const size_t at);

/**
 * @brief Přidá prvek do vektoru. V případě chyby není vektor daný ukazatel v změněn.
 * @param v Ukazatel na vektor, kam se prvke přidává.
 * @param item Ukazatel na přidávaný prvek.
 * @return int 1, pokud se přidání prvku povedlo, jinak 0.
 */
int vector_push_back(vector *v, const void *item);

/**
 * @brief Funkce zadanému vektoru odebere vlastnictví udržovaného ukazatele na dynamicky
 *        alokovaný buffer. 
 * 
 * @param v Ukazatel na vektor, nad jehož daty přebíráme kontrolu.
 * @return void* Ukazatel na data získaná z vektoru.
 */
void *vector_giveup(vector *v);

int vector_push(vector* v, const void* item, const size_t index);

vector* vector_init_data (const size_t item_size, const vec_it_dealloc_type deallocator, void** data, int data_count);

#endif