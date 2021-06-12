/** @file
        Część druga dużego zadania z IPP - kalkulator

@authors Wiktor Grzankowski <wg429211@mimuw.edu.pl>
@copyright Uniwersytet Warszawski
@date 2021
*/

#ifndef STACK_H
#define STACK_H

#define BEGINNING_SIZE 8 ///< początkowy rozmiar tablicy

/**
 * Struktura przechowująca stos wielomianów.
 */
typedef struct Stack {
    Poly* polyArr; ///< tablica wielomianów
    size_t top; ///< indeks wierzchu stosu
    size_t size; ///< zaalokowany rozmiar stosu
} Stack;

/**
 * Zwiększa podaną liczbę
 * @param[in] x : Liczba do powiększenia.
 * @return Jeśli x == 0, to 16 w przeciwnym wypadku 2 * x
 */
size_t More(size_t x);

/**
 * Powiększa tablicę wielomianów w strukturze stosu.
 * @param[in] stack : Stos do powiększenia.
 */
void enlargePolyArr(Stack* stack);

/**
 * Tworzy nowy stos.
 * @return pusty stos
 */
Stack NewStack();

/**
 * Zdejmuje wielomian z wierzchu stosu.
 * @param[in] stack : Stos, z którego zdejmowany jest wielomian.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackPop(Stack* stack, size_t lNum);

/**
 * Wrzuca wielomian na wierzch stosu.
 * @param[in] stack : Stos, na który wrzucany jest wielomian.
 * @param[in] p : Wielomian wrzucany na wierzch stosu.
 */
void StackPush(Stack* stack, Poly* p);

/**
 * Wstawia na wierzch stosu wielomian zerowy.
 * @param[in] stack : Stos, na który wrzucany jest wielomian zerowy.
 */
void StackZero(Stack* stack);

/**
 * Sprawdza, czy na wierzchu stosu znajduje się wielomian stały.
 * Wypisuje na standardowe wyjście 1 jeśli tak bądź 0, jeśli nie.
 * @param[in] stack : Stos, którego wierzch jest sprawdzany.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackIsCoeff(Stack* stack, size_t lNum);

/**
 * Sprawdza, czy na wierzchu stosu znajduje się wielomian zerowy.
 * Wypisuje na standardowe wyjście 1 jeśli tak bądź 0, jeśli nie.
 * @param[in] stack : Stos, którego wierzch jest sprawdzany.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackIsZero(Stack* stack, size_t lNum);

/**
 * Klonuje wielomian na wierzchu stosu i wrzuca go na wierzch.
 * @param[in] stack : Stos, którego wierzch jest klonowany.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackClone(Stack* stack, size_t lNum);

/**
 * Dodaje dwa wielomiany na wierzchu stosu, usuwa je, a ich sumę
 * wrzuca na nowy wierzch stosu.
 * @param[in] stack : Stos, na którym wykonywana jest operacja
 * dodania wielomianów.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackAdd(Stack* stack, size_t lNum);

/**
 * Mnoży dwa wielomiany na wierzchu stosu, usuwa je, a ich iloczyn
 * wrzuca na nowy wierzch stosu.
 * @param[in] stack : Stos, na którym wykonywana jest operacja
 * mnożenia wielomianów.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackMul(Stack* stack, size_t lNum);

/**
 * Neguje wielomian na wierzchu stosu.
 * @param[in] stack : Stos, na wierzchu którego negowany jest wielomian.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackNeg(Stack* stack, size_t lNum);

/**
 * Odejmuje dwa wielomiany na wierzchu stosu, usuwa je, a ich różnicę
 * wrzuca na nowy wierzch stosu.
 * @param[in] stack : Stos, na którym wykonywana jest operacje odejmowania
 * wielomianów.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackSub(Stack* stack, size_t lNum);

/**
 * Sprawdza, czy dwa wielomiany na wierzchu stosu są sobie równe.
 * Wypisuje 1 jeśli tak bądź 0, jeśli nie.
 * @param[in] stack : Stos, na wierzchu którego porównu
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackIsEq(Stack* stack, size_t lNum);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu
 * (−1 dla wielomianu tożsamościowo równego zeru).
 * @param[in] stack : Stos, na którego wierzchowym wielomianie sprawdzany
 * jest stopień.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackDeg(Stack* stack, size_t lNum);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną
 * o numerze idx (−1 dla wielomianu tożsamościowo równego zeru).
 * @param[in] stack : Stos, na którego wierzchowym wielomianie sprawdzany
 * jest stopień ze względu na zadaną zmienną.
 * @param[in] idx : Zmienna, względem której sprawdzany jest stopień wielomianu.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackDegBy(Stack* stack, size_t idx, size_t lNum);

/**
 * Wylicza wartość wielomianu w punkcie x, usuwa wielomian z wierzchołka
 * i wstawia na stos wynik operacji.
 * @param[in] stack : Stos, na którego wielomianie wierzchowym wykonywana
 * jest operacja.
 * @param[in] x : Punkt, w którym obliczana jest wartość wielomianu.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackAt(Stack* stack, long x, size_t lNum);

/**
 * Funkcja pomocnicza dla funkcji StackPrint.
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu,
 * @param[in] p : Wielomian, który jest wypisywany.
 */
void StackPrintHelp(Poly *p);

/**
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu.
 * Wykorzystuje do tego funkcję pomocniczą StackPrintHelp.
 * @param[in] stack : Stos, którego wierzchowy wielomian jest wypisywany.
 * @param[in] lNum : Numer linii, z której polecenie jest wykonywane.
 */
void StackPrint(Stack* stack, size_t lNum);

/**
 * Opróżnia stos po wykonaniu programu.
 * @param[in] stack : Stos do opróżnienia.
 */
void StackRemoveEverything(Stack* stack);

int CompareExpsIncreasingly(const void *a, const void *b);

#endif /* STACK_H */

