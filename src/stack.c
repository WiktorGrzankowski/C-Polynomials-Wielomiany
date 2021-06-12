/** @file
        Część druga dużego zadania z IPP - kalkulator

@authors Wiktor Grzankowski <wg429211@mimuw.edu.pl>
@copyright Uniwersytet Warszawski
@date 2021
*/

#include "poly.h"
#include "stack.h"

#include <stdlib.h>
#include <stdio.h>

size_t More(size_t x) {
    if (x == 0)
        return BEGINNING_SIZE;
    return 2 * x;
}

void enlargePolyArr(Stack* stack) {
    stack->size = More(stack->size);
    stack->polyArr = realloc(stack->polyArr, stack->size * sizeof(Stack));
    if (stack->polyArr == NULL)
        exit(1);
}


Stack NewStack() {
    return (Stack) {.polyArr = NULL, .size = 0, .top = -1};
}

void StackPop(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    PolyDestroy(&stack->polyArr[stack->top]);
    (stack->top)--;
}

void StackPush(Stack* stack, Poly* p) {
    (stack->top)++;

    if (stack->top == stack->size)
        enlargePolyArr(stack);

    stack->polyArr[stack->top] = *p;
}

void StackZero(Stack* stack) {
    (stack->top)++;

    if (stack->size == stack->top)
        enlargePolyArr(stack);

    stack->polyArr[stack->top] = PolyFromCoeff(0);
}

void StackIsCoeff(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n",lNum);
        return;
    }

    if (PolyIsCoeff(&stack->polyArr[stack->top]))
        printf("1\n");
    else
        printf("0\n");
}

void StackIsZero(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    if (PolyIsZero(&stack->polyArr[stack->top]))
        printf("1\n");
    else
        printf("0\n");
}

void StackClone(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    if (stack->size == stack->top + 1)
        enlargePolyArr(stack);

    Poly newTopPoly = PolyClone(&stack->polyArr[stack->top]);
    (stack->top)++;
    stack->polyArr[stack->top] = newTopPoly;
}

void StackAdd(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1 || stack->top == 0) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    Poly oldTopPoly = PolyClone(&stack->polyArr[stack->top]);
    PolyDestroy(&stack->polyArr[stack->top]);

    (stack->top)--;

    Poly oldSecondTopPoly = PolyClone(&stack->polyArr[stack->top]);
    PolyDestroy(&stack->polyArr[stack->top]);

    stack->polyArr[stack->top] = PolyAdd(&oldTopPoly, &oldSecondTopPoly);

    PolyDestroy(&oldTopPoly);
    PolyDestroy(&oldSecondTopPoly);
}

void StackMul(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1 || stack->top == 0) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    Poly oldTopPoly = PolyClone(&stack->polyArr[stack->top]);
    PolyDestroy(&stack->polyArr[stack->top]);

    (stack->top)--;

    Poly oldSecondTopPoly = PolyClone(&stack->polyArr[stack->top]);
    PolyDestroy(&stack->polyArr[stack->top]);

    stack->polyArr[stack->top] = PolyMul(&oldTopPoly, &oldSecondTopPoly);

    PolyDestroy(&oldTopPoly);
    PolyDestroy(&oldSecondTopPoly);
}

void StackNeg(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    Poly lastOneNegated = PolyNeg(&stack->polyArr[stack->top]);
    PolyDestroy(&stack->polyArr[stack->top]);

    stack->polyArr[stack->top] = lastOneNegated;
}

void StackSub(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1 || stack->top == 0) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    Poly oldTopPoly = PolyClone(&stack->polyArr[stack->top]);
    PolyDestroy(&stack->polyArr[stack->top]);

    (stack->top)--;

    Poly oldSecondTopPoly = PolyClone(&stack->polyArr[stack->top]);
    PolyDestroy(&stack->polyArr[stack->top]);

    stack->polyArr[stack->top] = PolySub(&oldTopPoly, &oldSecondTopPoly);

    PolyDestroy(&oldTopPoly);
    PolyDestroy(&oldSecondTopPoly);
}

void StackIsEq(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1 || stack->top == 0) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    Poly topPoly = PolyClone(&stack->polyArr[stack->top]);
    StackPop(stack, lNum);

    Poly secondTopPoly = stack->polyArr[stack->top];
    StackPush(stack, &topPoly);

    if (PolyIsEq(&topPoly, &secondTopPoly))
        printf("1\n");
    else
        printf("0\n");
}

void StackDeg(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    printf("%d\n", PolyDeg(&stack->polyArr[stack->top]));
}

void StackDegBy(Stack* stack, size_t idx, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    printf("%d\n", PolyDegBy(&stack->polyArr[stack->top], idx));
}

void StackAt(Stack* stack, long x, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    Poly newTopPoly = PolyAt(&stack->polyArr[stack->top], x);
    PolyDestroy(&stack->polyArr[stack->top]);

    stack->polyArr[stack->top] = newTopPoly;
}


void StackPrintHelp(Poly *p) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
        return;
    }

    qsort(p->arr, p->size, sizeof(Mono), CompareExpsIncreasingly);

    for (size_t i = 0; i < p->size; ++i) {
        printf("(");
        StackPrintHelp(&p->arr[i].p);
        printf(",%d)", p->arr[i].exp);
        if (i < p->size - 1)
            printf("+");
    }
}

void StackPrint(Stack* stack, size_t lNum) {
    if (stack->top == (size_t) -1) {
        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lNum);
        return;
    }

    StackPrintHelp(&stack->polyArr[stack->top]);
    printf("\n");
}

void StackRemoveEverything(Stack* stack) {
    while (stack->top != (size_t) -1)
        StackPop(stack, 0);

    free(stack->polyArr);
}
