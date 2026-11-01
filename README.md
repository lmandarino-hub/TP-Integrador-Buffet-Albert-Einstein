# TP-Integrador-Buffet-Albert-Einstein

## Grupo J:

Mandarino Luca Lautaro Ivan lmandarino-hub / normalizacion.cpp (ventas y stock)
---
More Sebastián Alejandro sebasmore22 / cierre.cpp
---
Christian Alejandro Aragon Correa AragonChristian20 / ventas.cpp
---
Longo Atamañuk Sebastian Valentin Langur213 / normalizacion.cpp (Mozos y comandas)
---
Manuel Enrique Guevara Isturiz Manueh05xd / resumen.cpp
---

# Pasos a seguir para correr el codigo:

1- Correr "Normalizacion.cpp" una sola vez y el resto de las veces iniciar desde "ventas.cpp"
---
2- A la hora de que el codigo pregunte si quiere cargar una nueva venta para afirmar, poner solo "s"  
---
3- Cuando corra el "resumen.cpp" colocar el nombre del archivo poniendo el .dat (va a ser el archivo que se cree en el momento)
---
4- Las contraseñas se encuentran en el codigo "normalizacion.cpp". Respetar las posiciones en las que estan puestas las contraseñas con el id, 1-5
---
5- Orden de compilado: normalizacion.cpp, ventas.cpp, cierre.cpp, resumen.cpp
---

# Lo que corre cada archivo:

normalizacion.cpp: Este archivo genera los mozos con su propio id, la comision total que le corresponde a cada uno, ordena los mozos, realiza los stocks y los actualiza cuando se quita algun elemento, y separa las ventas que se realicen.
---
ventas.cpp: Este archivo/codigo busca la clave de acceso del mozo con su cifra, valida esa clave cifrada, busca el producto con una clave, actualiza el stock (esta funcion se realizo para que en el momento en el que un mozo cargue 2 veces el mismo producto se actualice en ese preciso momento y no tenga que esperar que termine el programa), ordena las ventas, muestra las ventas.
---
cierre.cpp: Este archivo junta las planillas diarias generadas por ventas.cpp para transformarlas en una sola planilla semanal ordenada por id_mozo para que luego resumen.cpp pueda leer ese archivo semanal.
---
resumen.cpp: Este archivo lee el archivo generado en cierre.cpp, suma todas las ganancias de cada mozo, muestra el total de los productos vendidos por semana, lo vendido por cada mozo y la comision de cada mozo.
---