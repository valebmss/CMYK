# Función CYK para verificar si una cadena es generada por una gramática en CNF
def CYK(grammar, start_symbol, string):
    n = len(string)
    # Crear la tabla P
    P = [[set() for _ in range(n)] for _ in range(n)]
    
    # Inicializar la tabla para subcadenas de longitud 1
    for i in range(n):
        for lhs, rhs in grammar:
            if rhs == string[i]:
                P[i][i].add(lhs)
    
    # Llenar la tabla para subcadenas de longitud mayor
    for length in range(2, n+1):  # Longitud de la subsecuencia
        for i in range(n-length+1):  # Posición de inicio de la subsecuencia
            j = i + length - 1  # Posición de fin de la subsecuencia
            for k in range(i, j):  # Posición de partición de la subsecuencia
                for lhs, rhs in grammar:
                    if len(rhs) == 2:
                        B, C = rhs
                        if B in P[i][k] and C in P[k+1][j]:
                            P[i][j].add(lhs)
    
    # Verificar si el símbolo inicial puede generar la cadena completa
    return start_symbol in P[0][n-1]

# Ejemplo de gramática en forma normal de Chomsky
grammar = [
    ('S', 'AB'),
    ('S', 'BC'),
    ('A', 'BA'),
    ('A', 'a'),
    ('B', 'CC'),
    ('B', 'b'),
    ('C', 'AB'),
    ('C', 'a')
]

# Verificar si la cadena pertenece al lenguaje generado por la gramática
cadena = "baaba"
if CYK(grammar, 'S', cadena):
    print(f"La cadena '{cadena}' pertenece al lenguaje.")
else:
    print(f"La cadena '{cadena}' NO pertenece al lenguaje.")
