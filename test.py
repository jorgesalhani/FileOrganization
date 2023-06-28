numeros = input()
lista = numeros.split()
if len(lista) != 0:
    lista = [int(i) for i in lista]
    lista.sort()
    lista.reverse()
    print(lista[-1])
else:
    print('lista vazia')
