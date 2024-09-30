#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar una regla de la gramática
typedef struct {
    char lhs;  // Lado izquierdo de la producción (no terminal)
    char rhs[3];  // Lado derecho de la producción (dos no terminales o un terminal)
} Rule;

// Función CYK
int CYK(Rule* grammar, int num_rules, char start_symbol, char* string) {
    int n = strlen(string);
    
    // Crear la tabla P (arreglo tridimensional)
    int ***P = (int***) malloc(n * sizeof(int**));
    for (int i = 0; i < n; i++) {
        P[i] = (int**) malloc(n * sizeof(int*));
        for (int j = 0; j < n; j++) {
            P[i][j] = (int*) calloc(26, sizeof(int));  // 26 para las letras mayúsculas A-Z
        }
    }

    // Inicializar la tabla para subcadenas de longitud 1
    for (int i = 0; i < n; i++) {
        for (int r = 0; r < num_rules; r++) {
            if (grammar[r].rhs[1] == '\0' && grammar[r].rhs[0] == string[i]) {
                P[i][i][grammar[r].lhs - 'A'] = 1;
            }
        }
    }

    // Llenar la tabla para subcadenas de longitud mayor a 1
    for (int length = 2; length <= n; length++) {  // Longitud de la subsecuencia
        for (int i = 0; i <= n - length; i++) {  // Posición de inicio de la subsecuencia
            int j = i + length - 1;  // Posición de fin de la subsecuencia
            for (int k = i; k < j; k++) {  // Posición de partición de la subsecuencia
                for (int r = 0; r < num_rules; r++) {
                    if (strlen(grammar[r].rhs) == 2) {
                        char B = grammar[r].rhs[0];
                        char C = grammar[r].rhs[1];
                        if (P[i][k][B - 'A'] && P[k+1][j][C - 'A']) {
                            P[i][j][grammar[r].lhs - 'A'] = 1;
                        }
                    }
                }
            }
        }
    }

    // Verificar si el símbolo inicial puede generar la cadena completa
    int result = P[0][n-1][start_symbol - 'A'];

    // Liberar la memoria de la tabla
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            free(P[i][j]);
        }
        free(P[i]);
    }
    free(P);

    return result;
}

int main() {
    // Definir la gramática en CNF
    Rule grammar[] = {
        {'S', "AB"},
        {'S', "BC"},
        {'A', "BA"},
        {'A', "a"},
        {'B', "CC"},
        {'B', "b"},
        {'C', "AB"},
        {'C', "a"}
    };

    int num_rules = sizeof(grammar) / sizeof(grammar[0]);

    // Definir la cadena a verificar
    char string[] = "baaba";

    // Verificar si la cadena pertenece al lenguaje generado por la gramática
    if (CYK(grammar, num_rules, 'S', string)) {
        printf("La cadena '%s' pertenece al lenguaje.\n", string);
    } else {
        printf("La cadena '%s' NO pertenece al lenguaje.\n", string);
    }

    return 0;
}
