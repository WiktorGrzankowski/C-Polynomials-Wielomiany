/** @file
        Część pierwsza dużego zadania z IPP

@authors Wiktor Grzankowski <wg429211@mimuw.edu.pl>
@copyright Uniwersytet Warszawski
@date 2021
*/

#include "poly.h"

#include <stdlib.h>

/**
 * Porównuje wykładniki dwóch jednomianów
 * @param[in] a : jednomian
 * @param[in] b : jednomian
 * @return 1 gdy a.exp > b.exp, 0 gdy a.exp == b.exp, -1 gdy a.exp < b.exp
 */
int CompareExpsIncreasingly(const void *a, const void *b) {
    Mono mon1 = *(Mono*) a;
    Mono mon2 = *(Mono*) b;

    if (mon1.exp > mon2.exp)
        return 1;
    if (mon1.exp == mon2.exp)
        return 0;
    return -1;
}

/**
 * Porównuje wykładniki dwóch jednomianów
 * @param[in] a : jednomian
 * @param[in] b : jednomian
 * @return 1 gdy a.exp < b.exp, 0 gdy a.exp == b.exp, -1 gdy a.exp > b.exp
 */
static int CompareExpsDecreasingly(const void *a, const void *b) {
    Mono mon1 = *(Mono*) a;
    Mono mon2 = *(Mono*) b;

    if (mon1.exp > mon2.exp)
        return -1;
    if (mon1.exp == mon2.exp)
        return 0;
    return 1;
}

void PolyDestroy(Poly *p) {
    if (PolyIsCoeff(p))
        return;

    for (size_t i = 0 ; i < p->size ; ++i)
        MonoDestroy(&p->arr[i]);

    free(p->arr);
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff);

    Mono* monoCopy = (Mono*) malloc(p->size * sizeof (Mono));
    if (monoCopy == NULL)
        exit(1);

    for (size_t i = 0 ; i < p->size ; ++i)
        monoCopy[i] = MonoClone(&p->arr[i]);

    return (Poly) {.size = p->size, .arr = monoCopy};
}

/**
 * Sprawdza, czy wielomian jest tożsamościowo równy 0
 * @param[in] p : wielomian
 * @return p == 0
 */
static bool PolyIsZeroHelp(Poly *p) {
    if (PolyIsCoeff(p))
        return PolyIsZero(p);

    for (size_t i = 0 ; i < p->size ; ++i)
        if (!PolyIsZeroHelp(&p->arr[i].p))
            return false;


    return true;
}

/**
 * Zwraca wartość stałej zagnieżdżonej w wielomianie.
 * Wywoływana tylko wtedy, gdy wielomian jest tożsamościowo wielomianem stałym.
 * @param[in] p : wielomian stopnia niedodatniego
 * @return stała wielomianu p
 */
static poly_coeff_t CoeffToSimplifyPoly(Poly *p) {
    while (!PolyIsCoeff(p) && p->size == 1)
        return CoeffToSimplifyPoly(&p->arr[0].p);

    return p->coeff;
}

/**
 * Funkcja pomocnicza do funkcji PolyIsActuallyCoeff.
 * Sprawdza czy podany wielomian może być tożsamościowo stałą.
 * @param[in] p : wielomian
 * @param[in] expOtherThan0 : bool, czy występuje w wielomianie dodatni wykładnik
 * @return czy wielomian p może być tożsamościowo stałą
 */
static bool PolyIsActuallyCoeffHelp(Poly *p, bool *expOtherThan0) {
    if (p->size > 1) {
        if (p->arr == NULL)
            return true;
        return false;
    }

    while (!PolyIsCoeff(p))  {
        if (MonoGetExp(&p->arr[0]) )
            *expOtherThan0 = true;

        return PolyIsActuallyCoeffHelp(&p->arr[0].p, expOtherThan0);
    }

    if (PolyIsCoeff(p))
        return true;

    return false;
}


/**
 * Sprawdza czy podany wielomian jest tożsamościowo stałą.
 * @param[in] p : wielomian @f$p@f$
 * @return czy wielomian jest tożsamościowo stałą
 */
bool PolyIsActuallyCoeff(Poly *p) {
    bool expOtherThan0 = false;

    if (PolyIsActuallyCoeffHelp(p, &expOtherThan0)) {
        if (expOtherThan0)
            return CoeffToSimplifyPoly((Poly*)p) == 0;
        else
            return true;
    }
    return false;

}

static void PolySimplify(Poly *p);

/**
 * Funkcja pomocnicza dla funkcji PolySimplify.
 * Znajduje liczbę wykładników zerowych w tablicy jednomianów wielomianu p.
 * @param[in] p : wielomian @f$p@f$
 * @return liczba zerowych wykładników w tablicy jednomianów wielomianu p
 */
static size_t PolySimplifyFindZeroes(Poly *p) {
    size_t numberOfZeroExps = 0;

    for (size_t i = 0 ; i < p->size ; ++i) {
        PolySimplify(&p->arr[i].p);
        if (PolyIsZeroHelp(&p->arr[i].p)) {
            p->arr[i].exp = -1;
            numberOfZeroExps++;
        }
    }

    return numberOfZeroExps;
}

/**
 * Upraszcza wielomian pozbywając się wielomianów zerowych.
 * @param[in] p : wielomian @f$p@f$
 */
static void PolySimplify(Poly *p) {
    if (PolyIsCoeff(p))
        return;

    size_t numberOfZeroExps = PolySimplifyFindZeroes(p);

    if (numberOfZeroExps == p->size) {
        for (size_t j = 0 ; j < p->size ; ++j)
            MonoDestroy(&p->arr[j]);
        free(p->arr);
        p->size = 0;
        p->arr = NULL;
        p->coeff = 0;
        return;
    }
    if (numberOfZeroExps == 0)
        return;

    // Wszystkie wielomiany zerowe mają wykładnik -1.
    qsort(p->arr, p->size, sizeof(Mono), CompareExpsDecreasingly);

    // Tablica p->arr nie obejmuje wielomianów zerowych.
    size_t newSize = p->size - numberOfZeroExps;

    // Usuwanie wielomianów zerowych.
    for (size_t j = newSize; j < p->size; ++j)
        MonoDestroy(&p->arr[j]);

    p->size = newSize;
}


/**
 * Sprawdza czy wielomian jest tożsamościowo stałą.
 * Jeśli jest, to go upraszcza do postaci stałej.
 * @param[in] p : wielomian stały @f$p@f$
 */
static void PolyMakeCoeffIfPossible(Poly *p) {
    if (!PolyIsCoeff(p) && PolyIsActuallyCoeff(p)) {
        poly_coeff_t tempCoeff = CoeffToSimplifyPoly(p);

        for (size_t i = 0 ; i < p->size ; ++i)
            MonoDestroy(&p->arr[i]);

        free(p->arr);
        p->size = 0;
        p->arr = NULL;
        p->coeff = tempCoeff;
    }
}


/**
 * Tworzy wielomian złożony na podstawie wielomianu stałego.
 * @param[in] p : Wielomian stały
 * @return wielomian składający się z jednego jednomianu o wielomianie stałym
 * p oraz wykładniku 0
 */
static Poly PolyFromCoeffToComplex(const Poly *p) {
    Poly newComplex;
    newComplex.size = 1;
    newComplex.arr = malloc(1 * sizeof (Mono));
    if (newComplex.arr == NULL)
        exit(1);

    newComplex.arr[0].p = PolyFromCoeff(p->coeff);
    newComplex.arr[0].exp = 0;
    return newComplex;
}

/**
 * Dodaje dwa stałe wielomiany.
 * @param[in] p : Wielomian stały.
 * @param[in] q : Wielomian stały.
 * @return p + q
 */
static Poly PolyAddTwoCoeffs(const Poly *p, const Poly *q) {
    return PolyFromCoeff(p->coeff + q->coeff);
}

/**
 * Funkcja pomocnicza dla funkcji PolyAddTwoComplexPolies.
 * Tworzy z jednomianu new głęboką kopię jednomianu old.
 * @param[in] new : Jednomian docelowy.
 * @param[in] old : Jednomian przepisywany.
 * @param[in] indNew : Indeks jednomianu docelowego w tablicy jednomianów.
 * @param[in] indOld : Indeks jednomianu przepisywanego w tablicy jednomianów.
 */
static void MonoReWriteAndIncrementIndexes(Mono* new, Mono* old,
                                    size_t* indNew, size_t* indOld) {
    *new = MonoClone(old);
    (*indNew)++;
    (*indOld)++;
}

/**
 * Przepisuje do tablicy newMonos kopię tablicy jednomianów wielomiany p
 * począwszy od indeksu "j".
 * @param[in] newMonos : Wynikowa tablica jednomianów.
 * @param[in] p : Wielomian, którego reszta tablicy jednomianów
 * jest przepisywana.
 * @param[in] k : Numer indeksu w tablicy newMonos.
 * @param[in] j : Numer indeksu w tablicy jednomianów wielomianu p.
 */
static void MonoReWriteRest(Mono* newMonos, const Poly* p,
                            size_t* k, size_t* j) {
    while (*j < p->size) {
        newMonos[*k] = MonoClone(&p->arr[*j]);
        (*j)++;
        (*k)++;
    }
}

/**
 * Dodaje dwa wielomiany złożone - składające się z tablicy jednomianów
 * @param[in] p : Wielomian złożony.
 * @param[in] q : Wielomian złożony.
 * @return p + q
 */
static Poly PolyAddTwoComplexPolies(const Poly *p, const Poly *q) {
    size_t i = 0, j = 0, k = 0;

    Mono* newMonos = malloc((p->size + q->size) * sizeof (Mono));
    if (newMonos == NULL)
        exit(1);

    while (i < p->size && j < q->size) {
        if (p->arr[i].exp < q->arr[j].exp) {
            MonoReWriteAndIncrementIndexes(&newMonos[k], &p->arr[i], &k, &i);
        } else if (p->arr[i].exp > q->arr[j].exp) {
            MonoReWriteAndIncrementIndexes(&newMonos[k], &q->arr[j], &k, &j);
        } else {
            Poly help = PolyAdd(&p->arr[i].p, &q->arr[j].p);
            if (!PolyIsZero(&help)) {
                newMonos[k].p = help;
                newMonos[k].exp = p->arr[i].exp;
                k++;
            }
            i++;
            j++;
        }
    }

    if (i == p->size && j < q->size)
        MonoReWriteRest(newMonos, q, &k, &j);
    else if (i < p->size && j == q->size)
        MonoReWriteRest(newMonos, p, &k, &i);

    if (k == 0) {
        free(newMonos);
        return PolyFromCoeff(0);
    }
    return (Poly) {.size = k, .arr = newMonos};
}

/**
 * Dodaje wielomian stały oraz wielomian złożony.
 * @param[in] p : Wielomian.
 * @param[in] q : Wielomian.
 * @return p + q
 */
static Poly PolyAddCoeffAndComplexPoly(const Poly *p, const Poly *q) {
    Poly newComplex;
    Poly help;
    if (PolyIsCoeff(p)) {
        if (p->coeff == 0)
            return PolyClone(q);
        newComplex = PolyFromCoeffToComplex(p);
        help = PolyAddTwoComplexPolies(q, &newComplex);
    } else {
        if (q->coeff == 0)
            return PolyClone(p);
        newComplex = PolyFromCoeffToComplex(q);
        help = PolyAddTwoComplexPolies(p, &newComplex);
    }
    PolyDestroy(&newComplex);

    return help;
}

/**
 * Upraszcza wielomian, jeśli ten jest tożsamościowo wielomianem stały,
 * @param[in] p : wielomian
 */
static void PolySimplifyIfCoeff(Poly *p) {
    if (!PolyIsCoeff(p) && p->size == 1 && PolyIsCoeff(&p->arr[0].p) &&
        p->arr[0].exp == 0) {
        poly_coeff_t coeff = p->arr[0].p.coeff;
        Poly help = PolyFromCoeff(coeff);
        PolyDestroy(p);
        *p = help;
    }
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyAddTwoCoeffs(p, q);

    Poly res;

    if (PolyIsCoeff(p) != PolyIsCoeff(q))
        res = PolyAddCoeffAndComplexPoly(p, q);
    else
        res = PolyAddTwoComplexPolies(p, q);

    PolySimplifyIfCoeff(&res);
    return res;
}

/**
 * Funkcja pomocznicza dla funkcji PolyAddMonos.
 * Sumuje listę jednomianów i tworzy z nich wielomian, ale go nie upraszcza.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @param[in] monoCopy : wynikowa tablica jednomianów
 */
static void PolyAddMonosHelp(size_t count, const Mono monos[], Mono* monoCopy) {
    size_t indexInCopy = 0;

    for (size_t i = 0 ; i < count ; ++i) {
        if (i < count - 1 && monos[i].exp == monos[i + 1].exp) {
            Mono temp = monos[i];

            while (i < count - 1 && monos[i].exp == monos[i + 1].exp) {
                Poly tempAnother = PolyAdd(&temp.p, &monos[i + 1].p);
                PolyDestroy(&temp.p);
                temp.p = tempAnother;
                MonoDestroy((Mono*)&monos[i + 1]);
                ++i;
            }
            monoCopy[indexInCopy] = temp;
        } else {
            monoCopy[indexInCopy] = monos[i];
        }
        ++indexInCopy;
    }
}

/**
 * Znajduje liczbę różnych wykładników w tablicy jednomianów.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return liczba różnych wykładników
 */
static size_t PolyAddMonosFindExps(size_t count, const Mono monos[]) {
    size_t numberOfDiffExps = 1;

    for (size_t i = 0 ; i < count - 1 ; ++i)
        if (monos[i].exp != monos[i + 1].exp)
            ++numberOfDiffExps;

    return numberOfDiffExps;
}

/**
 * Funkcja sprawdza czy jednomian jest tożsamościowo zerowy.
 * @param[in] monoCopy : tablica jednomianów
 * @param numberOfDiffExps : liczba różnych wykładników
 */
static void PolyAddMonosCheckForZeroes(Mono* monoCopy,
                                       size_t numberOfDiffExps) {
    if (numberOfDiffExps == 1)
        if (PolyIsZero(&monoCopy[0].p))
            monoCopy[0].exp = 0;

}

/**
 * Znajduje liczbę wielomianów zerowych w tablicy jednomianów.
 * @param[in] monoCopy : tablica jednomianów
 * @param[in] numberOfDiffExps : liczba różnych wykładników - rozmiar
 * @return prawdziwa liczba różnych wykładników,
 * po usunięciu jednomianów zerowych.
 */
static size_t MonoFindZeroes(Mono* monoCopy, size_t numberOfDiffExps) {
    size_t res = numberOfDiffExps;

    for (size_t i = 0 ; i < numberOfDiffExps ; ++i) {
        if (PolyIsZero(&monoCopy[i].p) || (PolyIsActuallyCoeff(&monoCopy[i].p)
            && CoeffToSimplifyPoly(&monoCopy[i].p) == 0)) {
            res--;
        }
    }
    return res;
}

/**
 * Tworzy poprawnie zapisany wielomian na podstawie tablicy jednomianów.
 * Do wynikowej tablicy jednomianów wielomianu p zapisuje jedynie
 * niezerowe jednomiany  tablicy monoCopy.
 * @param[in] monoCopy : tablica jednomianów
 * @param[in] actualSize : rozmiar wynikowej tablicy jednomianów
 * @param[in] numberOfDiffExps : rozmiar wejściowej tablicy jednomianów
 * @return wielomian z tablicy jednomianów
 */
static Poly PolyGetFinalMonoArr(Mono* monoCopy, size_t actualSize,
                                size_t numberOfDiffExps) {
    Mono* resMono;

    if (actualSize == 0) {
        for (size_t i = 0 ; i < numberOfDiffExps ; ++i)
            MonoDestroy(&monoCopy[i]);
        free(monoCopy);

        return PolyFromCoeff(0);
    } else {
        size_t indexInRes = 0;
        resMono = malloc(actualSize * sizeof (Mono));
        if (resMono == NULL)
            exit(1);

        for (size_t i = 0 ; i < numberOfDiffExps ; ++i) {
            if (!PolyIsZero(&monoCopy[i].p) &&
                !(PolyIsActuallyCoeff(&monoCopy[i].p)
                  && CoeffToSimplifyPoly(&monoCopy[i].p) == 0)) {
                resMono[indexInRes] = monoCopy[i];
                indexInRes++;
            } else
                MonoDestroy(&monoCopy[i]);
        }
        free(monoCopy);
        Poly res;
        res.size = actualSize;
        res.arr = resMono;
        if (PolyIsActuallyCoeff(&res)) {
            poly_coeff_t actualCoeff = CoeffToSimplifyPoly(&res);
            PolyDestroy(&res);
            res = PolyFromCoeff(actualCoeff);
        }
        return res;
    }
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (count == 0)
        return PolyFromCoeff(0);

    qsort((Mono*)monos, count, sizeof(Mono), CompareExpsIncreasingly);

    size_t numberOfDiffExps = PolyAddMonosFindExps(count, monos);

    Mono* monoCopy = (Mono*) malloc(numberOfDiffExps * sizeof (Mono));
    if (monoCopy == NULL)
        exit(1);

    PolyAddMonosHelp(count, monos, monoCopy);

    PolyAddMonosCheckForZeroes(monoCopy, numberOfDiffExps);

    size_t actualSize = MonoFindZeroes(monoCopy, numberOfDiffExps);

    Poly res = PolyGetFinalMonoArr(monoCopy, actualSize, numberOfDiffExps);

    if (PolyIsActuallyCoeff(&res)) {
        poly_coeff_t actualCoeff = CoeffToSimplifyPoly(&res);
        PolyDestroy(&res);
        res = PolyFromCoeff(actualCoeff);
    }

    return res;
}

/**
 * Funkcja pomocnicza dla funkcji PolyMul.
 * Mnoży wielomian oraz stałą.
 * @param[in] c : wielomian stały @f$c@f$
 * @param[in] p : wielomian niestały @f$q@f$
 * @return wielomian @f$p * c@f$
 */
static Poly PolyMulDifferentCase(const Poly *c, const Poly *p) {
    Poly res;
    res.size = p->size;
    res.arr = malloc(res.size * sizeof (Mono));
    if (res.arr == NULL)
        exit(1);

    for (size_t i = 0 ; i < p->size ; ++i) {
        res.arr[i].p = PolyMul(c, &p->arr[i].p);
        res.arr[i].exp = p->arr[i].exp;
    }

    Poly final = PolyAddMonos(res.size, res.arr);
    free(res.arr);

    return final;
}

/**
 * Funkcja pomocnicza dla funkcji PolyMul.
 * Mnoży dwa wielomiany.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p * q@f$
 */
static Poly PolyMulHelp(const Poly *p, const Poly *q) {
    Poly temp;
    temp.size = q->size * p->size;
    temp.arr = malloc(temp.size * sizeof (Mono));
    if (temp.arr == NULL)
        exit(1);

    size_t resIndex = 0;

    for (size_t i = 0 ; i < p->size ; ++i) {
        for (size_t j = 0 ; j < q->size ; ++j) {
            temp.arr[resIndex].p = PolyMul(&p->arr[i].p, &q->arr[j].p);
            temp.arr[resIndex].exp = p->arr[i].exp + q->arr[j].exp;
            resIndex++;
        }
    }

    Poly res = PolyAddMonos(temp.size, temp.arr);

    PolySimplify(&res);

    PolyMakeCoeffIfPossible(&res);

    free(temp.arr);

    return res;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff(p->coeff * q->coeff);

    if (PolyIsCoeff(p) != PolyIsCoeff(q)) {
        if (PolyIsCoeff(p))
            return PolyMulDifferentCase(p, q);
        else
            return PolyMulDifferentCase(q, p);
    }

    return PolyMulHelp(p, q);
}

Poly PolyNeg(const Poly *p) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff((-1) * p->coeff);

    Poly negated = PolyClone(p);

    for (size_t i = 0 ; i < negated.size ; ++i) {
        Poly temp = PolyNeg(&negated.arr[i].p);
        PolyDestroy(&negated.arr[i].p);
        negated.arr[i].p = temp;
    }

    return negated;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly negatedQ = PolyNeg(q);

    Poly res = PolyAdd(p, &negatedQ);

    PolyDestroy(&negatedQ);

    return res;
}

/**
 * Funkcja pomocnicza dla funkcji PolyDegBy.
 * Zwraca stopień wielomianu ze względu na zadaną w funkcji PolyDegBy zmienną.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] maxDeg : dotychczasowo największy stopień wielomianu
 * @param[in] recursionsLeft : pozostałe rekursje do wykonania
 * @param[in] maxRecursions : pełna liczba rekursji do wykonania
 * @return stopień wielomianu @p p ze względu na zmienną o indeksie zadanym
 * w funkcji PolyDegBy
 */
static poly_exp_t PolyDegByHelp(const Poly *p, poly_exp_t *maxDeg,
                                size_t *recursionsLeft, size_t maxRecursions) {
    if (PolyIsCoeff(p)) {
        if (PolyIsZero(p))
            return -1;
        return 0;
    }

    for (size_t i = 0 ; i < p->size ; ++i) {
        if (*recursionsLeft == 1) { // ty bylo == 1
            poly_exp_t tempDeg = MonoGetExp(&p->arr[i]);
            if (tempDeg > *maxDeg || tempDeg == -1)
                *maxDeg = tempDeg;
        } else {
            (*recursionsLeft)--;
            PolyDegByHelp(&p->arr[i].p, maxDeg, recursionsLeft, maxRecursions);
        }
        *recursionsLeft = maxRecursions;
    }


    return *maxDeg;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    poly_exp_t maxDeg = 0;

    size_t recursionsLeft = var_idx + 1;

    size_t maxRecursions = var_idx + 1;

    return PolyDegByHelp(p, &maxDeg, &recursionsLeft, maxRecursions);
}

/**
 * Funkcja pomocnicza dla funkcji PolyDeg.
 * Znajduje największy wkładnik stojący przy i-tej zmiennej w wielomianie p.
 * Jeśli taki znajdzie, aktualizue zmienną maxDeg.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] maxDeg : największy dotychczasowy wykładnik w wielomianie p
 * @param[in] i : indeks w tablicy jednomianów wielomianu p
 */
static void PolyDegHelp(const Poly *p, poly_exp_t *maxDeg, size_t i) {
    poly_exp_t tempDeg = MonoGetExp(&p->arr[i]);

    if (PolyIsCoeff(&p->arr[i].p)) {
        if (PolyIsZero(&p->arr[i].p))
            tempDeg = -1;

        if (tempDeg > *maxDeg)
            *maxDeg = tempDeg;
    } else {
        for (size_t j = 0 ; j < p->arr[i].p.size ; ++j) {
            poly_exp_t biggestExpInArr;

            if (PolyIsCoeff(&p->arr[i].p.arr[j].p))
                biggestExpInArr = MonoGetExp(&p->arr[i].p.arr[j]);
            else
                biggestExpInArr = MonoGetExp(&p->arr[i].p.arr[j])
                                  + PolyDeg(&p->arr[i].p.arr[j].p);

            if (biggestExpInArr + tempDeg > *maxDeg)
                *maxDeg = biggestExpInArr + tempDeg;
        }
    }
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsCoeff(p)) {
        if (PolyIsZero(p))
            return -1;

        return 0;
    }

    poly_exp_t maxDeg = -1;

    for (size_t i = 0 ; i < p->size ; ++i)
        PolyDegHelp(p, &maxDeg, i);

    return maxDeg;
}

/**
 * Funkcja pomocnicza dla funkcji PolyAt.
 * Sprawdza czy wielomian p jest tożsamościowo stałą.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] expOtherThan0 : czy występuje w wielomianie wykładnik różny od 0
 * @param[in] numberOfRecursions : liczba pozostałych rekursji do wykonania
 * @return czy wielomian p jest tożsamościowo stałą
 */
static bool PolyAtHelp(Poly *p, bool *expOtherThan0,
                       size_t *numberOfRecursions) {
    if (p->size > 1) {
        if (p->arr == NULL)
            return true;
        return false;
    }

    while (!PolyIsCoeff(p))  {
        if (MonoGetExp(&p->arr[0]) && *numberOfRecursions != 0)
            *expOtherThan0 = true;

        (*numberOfRecursions)++;
        return PolyAtHelp(&p->arr[0].p, expOtherThan0,
                          numberOfRecursions);
    }

    if (PolyIsCoeff(p))
        return true;

    return false;
}

/**
 * Wykonuje potęgowanie.
 * @param[in] x : współczynnik, podstawa potęgowanie
 * @param[in] exponent : wykładnik
 * @return x^exponent
 */
static poly_coeff_t PolyAtFindFactor(poly_coeff_t x, poly_exp_t exponent) {
    if (exponent == 0)
        return 1;

    if (exponent == 1)
        return x;

    if (!(exponent % 2))
        return PolyAtFindFactor(x * x, exponent / 2);

    return x * PolyAtFindFactor(x * x, (exponent - 1) / 2);
}

/**
 * Funkcja pomocnicza dla funkcji PolyAt.
 * Wylicza wartość wielomianu w punkcie @p x i zapisuje ją w wielomianie help.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] x : wartość argumentu @f$x@f$
 * return wielomian @p x
 */
static Poly PolyAtExponenitation(const Poly *p, poly_coeff_t x) {
    Poly res;
    res.size = p->size;
    res.arr = malloc(res.size * sizeof (Mono));
    if (res.arr == NULL)
        exit(1);

    for (size_t i = 0 ; i < p->size ; ++i) {
        poly_coeff_t factor;

        factor = PolyAtFindFactor(x, MonoGetExp(&p->arr[i]));

        Poly factorPoly = PolyFromCoeff(factor);

        res.arr[i].p = PolyMul(&factorPoly, &p->arr[i].p);

        if (PolyIsCoeff(&p->arr[i].p))
            res.arr[i].exp = 0;
        else
            res.arr[i].exp = MonoGetExp(&p->arr[i].p.arr[0]);

        PolyDestroy(&factorPoly);
    }

    return res;
}

/**
 * Sprawdza czy wielomian jest zerem.
 * @param[in] p : wielomian @f$p@f$
 */
static void PolyCheckIfZero(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0 ; i < p->size ; ++i) {
            bool helper = false;
            size_t cos = 0;
            if (!PolyIsCoeff(&p->arr[i].p)
                && PolyAtHelp(&p->arr[i].p, &helper, &cos)) {
                poly_coeff_t tempCoeff = CoeffToSimplifyPoly(&p->arr[i].p);
                PolyDestroy(&(p->arr[i].p));
                p->arr[i].p.arr = NULL;
                p->arr[i].p.coeff = tempCoeff;
            }
        }
    }
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff);

    Poly help = PolyAtExponenitation(p, x);

    qsort(help.arr, help.size, sizeof(Mono), CompareExpsIncreasingly);

    Poly final = PolyAddMonos(help.size, help.arr);

    free(help.arr);

    PolyCheckIfZero(&final);

    return final;
}

/**
 * Porównuje wielomian stały z wielomianem niestałym.
 * @param[in] c : wielomian stały @f$c@f$
 * @param[in] p : wielomian niestały @f$p@f$
 * @return @f$p = c@f$
 */
static bool PolyIsEqDifferentCase(const Poly* c, const Poly* p) {
    if (PolyIsActuallyCoeff((Poly*)p))
        return c->coeff == CoeffToSimplifyPoly((Poly*)p);

    return false;
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) != PolyIsCoeff(q)) {
        if (PolyIsCoeff(p))
            return PolyIsEqDifferentCase(p, q);
        else
            return PolyIsEqDifferentCase(q, p);
    }

    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return p->coeff == q->coeff;

    for (size_t i = 0 ; i < p->size ; ++i) {
        if (p->arr[i].exp != q->arr[i].exp)
            return false;
        if (!PolyIsEq(&p->arr[i].p, &q->arr[i].p))
            return false;
    }

    return true;
}
