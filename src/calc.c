/** @file
        Część druga dużego zadania z IPP - kalkulator

@authors Wiktor Grzankowski <wg429211@mimuw.edu.pl>
@copyright Uniwersytet Warszawski
@date 2021
*/

#include "calcDefinitions.h"

/**
 * Sprawdza, czy wiersz wejścia może zawierać komendę.
 * @param[in] firstChar : pierwszy znak wiersza.
 * @return Czy znak jest litera?
 */
static bool LineMayBeACommand(char firstChar) {
    return isalpha(firstChar);
}

/**
 * Sprawdza jaki rodzaj komendy IS zawiera wiersz. Jeśli zawiera poprawną
 * komendę, to wywłouje funkcję do jej wykonania.
 * Jeśli zawiera nielegalną komendę, wypisuje na standardowe wyjście
 * diagnostyczne informacje o błędzie wraz z numerem wiersza,
 * w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy, czy zawiera poprawne
 * polecenie IS i jeśli tak, to jakie.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackIsIfCorrectCommand(Stack* stack, char* line,
                             size_t lineSize, size_t lNum) {
    if (strcmp("IS_ZERO\n", line) == 0 ||
        (strcmp("IS_ZERO", line) == 0 && lineSize == 7)) {
        StackIsZero(stack, lNum);

    } else if (strcmp("IS_COEFF\n", line) == 0 ||
                (strcmp("IS_COEFF", line) == 0 && lineSize == 8)) {
        StackIsCoeff(stack, lNum);

    } else if (strcmp("IS_EQ\n", line) == 0 ||
                (strcmp("IS_EQ", line) == 0 && lineSize == 5)) {
        StackIsEq(stack, lNum);

    } else {
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
    }
}

/**
 * Sprawdza, czy wiersz zaczyna się od komendy AT.
 * @param[in] line : Analizowany wiersz.
 * @param[in] lineSize : Rozmiar wiersza.
 * @return Czy wiersz zaczyna się komendą AT?
 */
static bool LineBeginsWithAt(char* line, size_t lineSize) {
    if (lineSize < 2 || line[0] != 'A' || line[1] != 'T')
        return false;

    if (lineSize == 2)
        return true;

    return isspace(line[2]);
}

/**
 * Sprawdza, czy zmienna wartość x przy komendzie AT jest na pewno błędna.
 * @param[in] line : Analizowany wiersz.
 * @param[in] lineSize : Rozmiar wiersza.
 * @return Czy wartość komendy AT jest na pewno błędna?
 */
static bool AtValueIsDefinetlyWrong(char* line, size_t lineSize) {
    return lineSize == 3 || (!isdigit(line[3]) && line[3] != MINUS) ||
            line[2] != SPACE;
}

/**
 * Sprawdza, czy komenda AT x jest poprawna. Jeśli jest, wywołuje funkcję
 * do jej wykonania. W przeciwnym wypadku wypisuje na standardowe wyjście
 * diagnostyczne o błędzie wraz z numerem wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia AT x.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackAtIfCorrectCommand(Stack* stack, char* line,
                             size_t lineSize, size_t lNum) {
    if (LineBeginsWithAt(line, lineSize)) {
        if (AtValueIsDefinetlyWrong(line, lineSize)) {
            fprintf(stderr, "ERROR %zu AT WRONG VALUE\n", lNum);
            return;
        }

        char* endPTR = NULL;
        long atNumber = strtol(line + 3, &endPTR, 10);

        if (errno || (*endPTR != NEWLINE && *endPTR))
            fprintf(stderr, "ERROR %zu AT WRONG VALUE\n", lNum);
        else
            StackAt(stack, atNumber, lNum);
    } else {
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
    }
}

/**
 * Sprawdza, czy wiersz zawiera polecenie ADD lub czy zawiera potencjalnie
 * poprawne polecenie AT. Jeśli zawiera ADD, to wywołuje funkcję do wykonania
 * komendy ADD. Jeśli nie, to wywołuje funkcje do sprawdzenia poprawności 
 * komendy AT.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy typ zawieranego polecenia.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackAddOrAtIfCorrectCommand(Stack* stack, char* line,
                                  size_t lineSize, size_t lNum) {
    if (strcmp("ADD\n", line) == 0 ||
        (strcmp("ADD", line) == 0 && lineSize == 3))
        StackAdd(stack, lNum);
    else
        StackAtIfCorrectCommand(stack, line, lineSize, lNum);
}

/**
 * Sprawdza, czy wiersz zawiera poprawne polecenie ZERO. Jeśli tak, wywołuje
 * funkcje do wykonania komendy ADD. W przeciwnym wypadku wypisuje na
 * standardowe wyjście diagnostyczne informacje o błędzie wraz z numerem
 * wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia ZERO.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackZeroIfCorrectCommand(Stack* stack, char* line,
                               size_t lineSize, size_t lNum) {
    if (strcmp("ZERO\n", line) == 0 ||
        (strcmp("ZERO", line) == 0 && lineSize == 4))
        StackZero(stack);
    else
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
}

/**
 * Sprawdza, czy wiersz zawiera poprawne polecenie CLONE. Jeśli tak, wywołuje
 * funkcje do wykonania komendy CLONE. W przeciwnym wypadku wypisuje na
 * standardowe wyjście diagnostyczne informacje o błędzie wraz z numerem
 * wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia CLONE.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackCloneIfCorrectCommand(Stack* stack, char* line,
                                size_t lineSize, size_t lNum) {
    if (strcmp("CLONE\n", line) == 0 ||
        (strcmp("CLONE", line) == 0 && lineSize == 5))
        StackClone(stack, lNum);
    else
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
}

/**
 * Sprawdza, czy wiersz zawiera poprawne polecenie MUL. Jeśli tak, wywołuje
 * funkcje do wykonania komendy MUL. W przeciwnym wypadku wypisuje na
 * standardowe wyjście diagnostyczne informacje o błędzie wraz z numerem
 * wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia MUL.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackMulIfCorrectCommand(Stack* stack, char* line,
                              size_t lineSize, size_t lNum) {
    if (strcmp("MUL\n", line) == 0 ||
        (strcmp("MUL", line) == 0 && lineSize == 3))
        StackMul(stack, lNum);
    else
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
}

/**
 * Sprawdza, czy wiersz zawiera poprawne polecenie NEG. Jeśli tak, wywołuje
 * funkcje do wykonania komendy NEG. W przeciwnym wypadku wypisuje na
 * standardowe wyjście diagnostyczne informacje o błędzie wraz z numerem
 * wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia NEG.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackNegIfCorrectCommand(Stack* stack, char* line,
                              size_t lineSize, size_t lNum) {
    if (strcmp("NEG\n", line) == 0 ||
        (strcmp("NEG", line) == 0 && lineSize == 3))
        StackNeg(stack, lNum);
    else
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
}

/**
 * Sprawdza, czy wiersz zawiera poprawne polecenie SUB. Jeśli tak, wywołuje
 * funkcje do wykonania komendy SUB. W przeciwnym wypadku wypisuje na
 * standardowe wyjście diagnostyczne informacje o błędzie wraz z numerem
 * wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia SUB.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackSubIfCorrectCommand(Stack* stack, char* line,
                              size_t lineSize, size_t lNum) {
    if (strcmp("SUB\n", line) == 0 ||
        (strcmp("SUB", line) == 0 && lineSize == 3))
        StackSub(stack, lNum);
    else
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
}

/**
 * Sprawdza, czy wiersz zaczyna się od polecenia DEG_BY.
 * @param[in] line : Analizowany wiersz.
 * @param[in] lineSize : Rozmiar wiersza.
 * @return Czy wiersz zaczyna się od DEG_BY?
 */
static bool LineBeginsWithDegBy(char* line, size_t lineSize) {
    if (lineSize < 6 || line[0] != 'D' || line[1] != 'E' ||
        line[2] != 'G' || line[3] != '_' || line[4] != 'B' || line[5] != 'Y')
        return false;

    if (lineSize == 6)
        return true;

    return isspace(line[6]);
}

/**
 * Sprawdza, czy zmienna polecenia Deg by jest na pewno błędna.
 * @param[in] line : Analizowany wiersz.
 * @param[in] lineSize : Rozmiar wiersza.
 * @return Czy zmienna polecenia Deg by jest na pewno błędna?
 */
static bool DegByVariableIsDefinetlyWrong(char* line, size_t lineSize) {
    return lineSize < 8 || line[6] != SPACE || !isdigit(line[7]);
}

/**
 * Sprawdza, czy wiersz zawiera poprawne polecenie DEG BY. Jeśli tak, wywołuje
 * funkcje do wykonania komendy DEG BY. W przeciwnym wypadku wypisuje na
 * standardowe wyjście diagnostyczne informacje o błędzie wraz z numerem
 * wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia DEG BY.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackDegByIfCorrectCommand(Stack* stack, char* line,
                                size_t lineSize, size_t lNum) {
    if (LineBeginsWithDegBy(line, lineSize)) {
        if (DegByVariableIsDefinetlyWrong(line, lineSize)) {
            fprintf(stderr, "ERROR %zu DEG BY WRONG VARIABLE\n", lNum);
            return;
        }

        char* endPTR = NULL;
        errno = 0;
        size_t byNumber = strtoull(line + 7, &endPTR, 10);

        if (errno || (*endPTR != NEWLINE && *endPTR))
            fprintf(stderr, "ERROR %zu DEG BY WRONG VARIABLE\n", lNum);
        else
            StackDegBy(stack, byNumber, lNum);

    } else
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
}

/**
 * Sprawdza, czy wiersz zawiera polecenie DEG lub czy zawiera potencjalnie
 * poprawne polecenie DEG BY. Jeśli zawiera DEG, to wywołuje funkcję
 * do wykonania komendy ADD. Jeśli nie, to wywołuje funkcje do
 * sprawdzenia poprawności komendy DEG BY.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia DEG BY.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackDegOrDegByIfCorrectCommand(Stack* stack, char* line,
                                     size_t lineSize, size_t lNum) {
    if (strcmp("DEG\n", line) == 0 ||
        (strcmp("DEG", line) == 0 && lineSize == 3))
        StackDeg(stack, lNum);
    else
        StackDegByIfCorrectCommand(stack, line, lineSize, lNum);
}

/**
 * Sprawdza, czy wiersz zawiera polecenie PRINT lub czy zawiera polecenie POP.
 * Jeśli zawiera jedno z nich, wywołuje funkcje doń wywołania. W przeciwnym
 * wypadku wypisuje na standardowe wyjście diagnostyczne o błędzie wraz
 * z numerem wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia DEG BY.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void StackPrintOrPopIfCorrectCommand(Stack* stack, char* line,
                                     size_t lineSize, size_t lNum) {
    if (strcmp("PRINT\n", line) == 0 ||
        (strcmp("PRINT", line) == 0 && lineSize == 5))
        StackPrint(stack, lNum);

    else if (strcmp("POP\n", line) == 0 ||
            (strcmp("POP", line) == 0 && lineSize == 3))
        StackPop(stack, lNum);

    else
        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
}

/**
 * Analizuje komendę znajdującą się w wierszu. Na podstawie pierwszego znaku
 * wiersza sprawdza poprawność konkretnych komend. Gdy pierwszy znak nie może
 * być początkiem żadnej z legalnych komend, wypisuje na standardowe wyjście
 * diagnostyczne informacje o błędzie wraz z numerem wiersza,
 * w którym jest błąd.
 * @param[in] stack : Stos, na którym wykonujemy operacje.
 * @param[in] line : Wiersz, w którym sprawdzamy poprawność polecenia DEG BY.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void AnalyzeCommand(Stack* stack, char* line, size_t lineSize, size_t lNum) {
    switch (line[0]) {
        case ZERO:
            StackZeroIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case IS_SOMETHING:
            StackIsIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case CLONE:
            StackCloneIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case ADD_OR_AT:
            StackAddOrAtIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case MUL:
            StackMulIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case NEG:
            StackNegIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case SUB:
            StackSubIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case DEG_SOMETHING:
            StackDegOrDegByIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        case PRINT_OR_POP:
            StackPrintOrPopIfCorrectCommand(stack, line, lineSize, lNum);
            break;

        default:
            fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lNum);
            break;
    }
}

/**
 * Zlicza liczbę znaków, z których składa się liczba w zaczynająca się od
 * indeksu "i" w wierszu.
 * @param[in] line : Wiersz, w którym zliczana jest liczba znaków w liczbie
 * zaczynającej się od indeksu "i".
 * @param[in] i : Indeks, od którego rozpoczyna się zliczanie znaków w liczbie.
 * @return pierwszy indeks w wierszu nienależący do badanej liczby
 */
static size_t countDigits(char* line, size_t i) {
    size_t newI = i;

    while (isdigit(line[newI]) || (newI == i && line[newI] == '-'))
        newI++;

    return newI;
}

/**
 * Sprawdza, czy pod indeksem "i" znajduje się niepoprawnie zapisany
 * nawias otwierający.
 * @param[in] line : Wiersz, w którym sprawdzana jest poprawność nawiasu.
 * @param[in] i : Indeks, pod którym jest nawias otwierający.
 * @return Czy nawias otwierający stoi w złym miejscu?
 */
static bool IncorrectOpeningBracket(char* line, size_t i) {
    return !(i == 0 || line[i - 1] == OPENING_BRACKET || line[i - 1] == PLUS);
}

/**
 * Sprawdza, czy pod indeksem "i" znajduje się niepoprawnie zapisany przecinek.
 * @param[in] line : Wiersz, w którym sprawdzana jest poprawność przecinka.
 * @param[in] i : Indeks, pod którym jest przecinek.
 * @return Czy przecinek stoi w złym miejscu?
 */
static bool IncorrectComa(char* line, size_t i) {
    return !(i > 0 && (line[i - 1] == CLOSING_BRACKET
                        || isdigit(line[ i - 1])));
}

/**
 * Sprawdza, czy pod indeksem "i" znajduje się niepoprawnie zapisany
 * nawias zamykający.
 * @param[in] line : Wiersz, w którym sprawdzana jest poprawność nawiasu.
 * @param[in] i : Indeks, pod którym jest nawias zamykający.
 * @return Czy nawias zamykający stoi w złym miejscu?
 */
static bool IncorrectClosingBracket(char* line, size_t i) {
    return !(i > 0 && isdigit(line[i - 1]));
}

/**
 * Sprawdza, czy pod indeksem "i" znajduje się niepoprawnie zapisany
 * znak minus.
 * @param[in] line : Wiersz, w którym sprawdzana jest poprawność minusa.
 * @param[in] i : Indeks, pod którym jest minus.
 * @return Czy minus stoi w złym miejscu?
 */
static bool IncorrectMinus(char* line, size_t* i) {
    if (*i > 0 && line[*i - 1] != OPENING_BRACKET)
        return true;

    size_t newI = countDigits(line, *i);
    char* endPTR = NULL;
    errno = 0;

    /*poly_coeff_t coeff = */strtol(line + *i, &endPTR, 10);
    if (errno || *endPTR != line[newI])
        return true;

    *i = newI - 1;

    return false;
}

/**
 * 
 * Sprawdza, czy pod indeksem "i" znajduje się niepoprawnie zapisany
 * znak plus.
 * @param[in] line : Wiersz, w którym sprawdzana jest poprawność plusa.
 * @param[in] i : Indeks, pod którym jest plus.
 * @param[in] lineSize : Rozmiar wiersza.
 * @return Czy plus stoi w złym miejscu?
 */
static bool IncorrectPlus(char* line, size_t lineSize, size_t i) {
    return i == 0 || i == lineSize - 1 ||
            line[i - 1] != CLOSING_BRACKET || line[i + 1] != OPENING_BRACKET;
}

/**
 * Sprawdza, czy pod indeksem "i" znajduje się poprawnie zapisana liczba.
 * @param[in] line : Wiersz, w którym sprawdzana jest poprawność cyfra.
 * @param[in] i : Indeks, pod którym jest cyfra.
 * @return Czy cyfra stoi w złym miejscu?
 */
static bool IllegalDigit(char* line, size_t *i) {
    if (*i > 0 && line[*i - 1] != OPENING_BRACKET && line[*i - 1] != COMMA &&
        !isdigit(line[*i - 1]) && line[*i - 1] != MINUS)
        return true;

    size_t newI = countDigits(line, *i);
    char* endPTR = NULL;
    errno = 0;

    if (*i > 0 && line[*i - 1] == COMMA) {
        poly_exp_t exp = strtol(line + *i, &endPTR, 10);
        if (errno || exp < 0 || *endPTR != line[newI])
            return true;
    } else {
        strtol(line + *i, &endPTR, 10);
        if (errno || *endPTR != line[newI])
            return true;
    }

    *i = newI - 1;

    return false;
}

/**
 * Sprawdza, czy wielomian w wierszu jest poprawny.
 * @param[in] line : Wiersz, w który sprawdzana jest poprawność zapisanego
 * wielomianu.
 * @param[in] lineSize : Rozmiar wiersza.
 * @return Czy wielomian w wierszu jest poprawny?
 */
static bool PolyIsCorrect(char* line, size_t lineSize) {
    size_t openingBracketCount = 0;
    size_t closeBracketCount = 0;
    size_t commaCount = 0;
    bool illegalChar = false;

    for (size_t i = 0; i < lineSize - 1 && !illegalChar; ++i) {
        if (line[i] == OPENING_BRACKET) {
            openingBracketCount++;
            illegalChar =  IncorrectOpeningBracket(line, i);

        } else if (line[i] == COMMA) {
            commaCount++;
            illegalChar = IncorrectComa(line, i);

        } else if (line[i] == CLOSING_BRACKET) {
            closeBracketCount++;
            illegalChar = IncorrectClosingBracket(line, i);

        } else if (line[i] == MINUS) {
            illegalChar = IncorrectMinus(line, &i);

        } else if (line[i] == PLUS) {
            illegalChar = IncorrectPlus(line, lineSize, i);

        } else if (isdigit(line[i])) {
            illegalChar = IllegalDigit(line, &i);

        } else {
            return false;
        }
    }
    return openingBracketCount == closeBracketCount
            && openingBracketCount == commaCount && !illegalChar;
}

/**
 * Sprawdza, czy wielomian jest prosty - to znaczy czy jest stałą,
 * czy jest złożony - składa się z tablicy jednomianów.
 * Funkcja wywoływana jest przy parsowaniu wielomianu.
 * @param[in] c : Znak w wierszu, który jest sprawdzany.
 * @return Czy następny parsowany wielomian będzie stałą?
 */
static bool PolyIsSimple(char c) {
    return c != '(';
}

static Poly ReadPoly(char* line, size_t lineSize, size_t* i);
static Poly ReadPolyComplicated(char* line, size_t lineSize, size_t* i);
static Poly ReadPolySimple(char* line, size_t* i);

/**
 * Wczytuje wykładnik jednomianu i parsuje go.
 * @param[in] line : Wiersz, w którym wczytujemy wykładnik jednomianu.
 * @param[in] i : Indeks, na którym jest przecinek poprzedzający wykładnik.
 * @return wykładnik analizowanego jednomianu w wierszu.
 */
static poly_exp_t ReadMonoExp(char* line, size_t* i) {
    (*i)++;

    size_t newI = countDigits(line, *i);
    char* endPTR = NULL;

    poly_exp_t exp = strtol(line + *i, &endPTR, 10);
    *i = newI;

    return exp;
}

/**
 * Wczytuje jednomian w wierszu i odpowiednio go parsuje.
 * @param[in] line : Wiersz, w którym wczytywany jest jednomian.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] i : Indeks, pod którym w wierszu odbywa się kolejny
 * krok parsowania.
 * @return kolejny jednomian zapisany w wierszu
 */
static Mono ReadMono(char* line, size_t lineSize, size_t* i) {
    Poly thisPoly;
    poly_exp_t thisExp = 0;

    while (line[*i] != ')') {
        if (line[*i] == '(') {
            thisPoly = ReadPolyComplicated(line, lineSize, i);

        } else if (isdigit(line[*i] ) || line[*i] == '-') {
            thisPoly = ReadPolySimple(line, i);

        } else if (line[*i] == ',') {
            thisExp = ReadMonoExp(line, i); // Po przecinku jest wykładnik.
        }
    }

    (*i)++;

    return MonoFromPoly(&thisPoly, thisExp);
}

/**
 * Wczytuje kolejny wielomian w wierszu składający się z tablicy jednomianów.
 * @param[in] line : Wiersz, w który wczytywany jest wielomian.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] i : Indeks, pod którym w wierszu odbywa się kolejny
 * krok parsowania.
 * @return kolejny wielomian złożony w wierszu
 */
static Poly ReadPolyComplicated(char* line, size_t lineSize, size_t* i) {
    Mono* monos = NULL;
    size_t indInMonos = 0;
    size_t monosSize = 0;
    Poly p;

    while (line[*i] != ')') {
        if (line[*i] == '(') {
            if (indInMonos == monosSize) {
                monosSize = More(monosSize);
                monos = realloc(monos, monosSize * sizeof (Mono));
                if (monos == NULL)
                    exit(1);
            }
            (*i)++;
            monos[indInMonos] = ReadMono(line, lineSize, i);

        } else if (line[*i] == '+') {
            indInMonos++;
            (*i)++;

        } else {
            p = PolyAddMonos(indInMonos + 1, monos);
            free(monos);
            break;
        }
    }
    return p;
}

/**
 * Wczytuje kolejny wielomian w wierszu, który jest stałą.
 * @param[in] line : Wiersz, w którym wczytywany jest wielomian.
 * @param[in] i : Indeks wiersza, pod który odbywa się kolejny
 * krok parsowania.
 * @return kolejny wielomian stały
 */
static Poly ReadPolySimple(char* line, size_t* i) {
    size_t newI = countDigits(line, *i);
    char* endPTR = NULL;

    poly_coeff_t coeff = strtol(line + *i, &endPTR, 10);

    *i = newI;

    return PolyFromCoeff(coeff);
}

/**
 * Wczytuje kolejny wielomian w wierszu. Sprawdza jego typ i do dalszego
 * wczytania wywołuje odpowiednią funkcję.
 * @param[in] line : Wiersz, w którym wczytywany jest wielomian.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] i : Indeks, w wierszu pod którym odbywa się kolejny
 * krok parsowania.
 * @return kolejny wielomian
 */
static Poly ReadPoly(char* line, size_t lineSize, size_t* i) {
    if (PolyIsSimple(line[*i]))
        return ReadPolySimple(line, i);
    else
        return ReadPolyComplicated(line, lineSize, i);
}

/**
 * Sprawdza, czy wielomian w wierszu jest poprawny. Jeśli tak, wywołuje
 * odpowiednie funkcje do wczytania go i wrzucenia na stos. W przeciwnym
 * wypadku wypisuje na standardowe wyjście diagnostyczne informacje o błędzie
 * wraz z numerem wiersza, w którym jest błąd.
 * @param[in] stack : Stos, na który wykonywane są operacje.
 * @param[in] line : Wiersz, który jest analizowany.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, z którego polecenie jest analizowane.
 */
static void AnalyzePoly(Stack* stack, char* line, size_t lineSize, size_t lNum) {
    if (PolyIsCorrect(line, lineSize)) {
        size_t i = 0;
        Poly p = ReadPoly(line, lineSize, &i);
        StackPush(stack, &p);
    }
    else
        fprintf(stderr, "ERROR %zu WRONG POLY\n", lNum);
}

/**
 * Sprawdza, czy wiersz należy pominąć.
 * @param[in] firstChar : Pierwszy znak w wierszu.
 * @return Czy wiersz należy pominąć?
 */
static bool LineIsNotToBeIgnored(char firstChar) {
    return firstChar != COMMENT && firstChar != NEWLINE;
}

/**
 * Analizuje pojedynczy wiersz. Sprawdza, czy należy ją zignorować. Jeśli nie,
 * to sprawdza, czy w wierszu może być komenda czy wielomian i wywołuje
 * odpowiednie funkcje doń wczytania.
 * @param[in] stack : Stos, na który wykonywane są operacje.
 * @param[in] line : Wiersz, który jest analizowany.
 * @param[in] lineSize : Rozmiar wiersza.
 * @param[in] lNum : Numer wiersza, który jest analizowany.
 */
static void AnalyzeOneLine(Stack* stack, char* line, size_t lineSize, size_t lNum) {
    if (LineIsNotToBeIgnored(line[0])) {
        if (LineMayBeACommand(line[0]))
            AnalyzeCommand(stack, line, lineSize, lNum);
        else
            AnalyzePoly(stack, line, lineSize, lNum);
    }
}

/**
 * Wczytuje i analizuje wszystkie wiersze wejścia, wywołując do tego
 * odpowiednie funkcje
 * @param[in] stack : Stos, na którym wykonywane są operacje.
 */
static void AnalyzeAllLines(Stack* stack) {
    char* line = NULL;
    size_t lineSize = 0;
    size_t lNum = 0;
    errno = 0;
    while ((lineSize = getline(&line, &lineSize, stdin)) != (size_t) -1) {
        if (errno == ENOMEM)
            exit(1);
        lNum++;
        AnalyzeOneLine(stack, line, lineSize, lNum);
        errno = 0;
    }
    free(line);
}

/**
 * Funkcja wykonująca program kalkulatora wielomianów.
 * Tworzy stos wielomianów oraz wykonuje cały program przy pomocy
 * odpwienich funkcji. Ostatecznie zwalnia pamięć po stosie.
 */
static void Calculate() {
    Stack stack = NewStack();
    AnalyzeAllLines(&stack);
    StackRemoveEverything(&stack);
}

/**
 * Funkcja główna programu calc.c
 * Wywołuje funkcję Calculate() do wykonania programu
 * @return 0 jeśli program przebiegł prawidłowo
 */
int main() {
    Calculate();
    return 0;
}
