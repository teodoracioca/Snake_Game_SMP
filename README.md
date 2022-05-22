# Snake_Game_SMP
Proiect SMP

Proiectul consta in implementarea jocului Snake cu ajutorul micrcontrollerul ATmega328p. Sarpele va fi prezent pe o matrice 8x8, iar scpul lui este sa se hraneasca pentru a creste in lungime. Acesta a fost implementat in limbajul de programare C++.

### Elemente componente:

* Micrcontrollerul ATmega328p 
* Modul Matrice LED max7219
* Modul Joystick PS2

# Functionare
Pentru a putea misca sarpele avem nevoie sa ne plimbam pe cele doua axe: x si y. Cu ajutorul joysick-ului putem sa vedem daca mergem destul de in stanga sau in dreapta pentru a face deplasarile. Mai mult, calculam si timpul delta care reprezinta diferenta de timp dintre cadrul anterior care a aparut pe matrice si cadrul actual. De fiecare data cand ne deplasam, actualizam matricea.
*	Prima conditie: daca vrem sa mergem in stanga (sa ne apropiem de valoarea 0 si sa ne deplasam pe coloane) si ma aflu pe o coloana
*	A doua canditie: daca vrem sa mergem in dreapta (sa ne apropriem de valoarea maxima 1023 pe coloane) si ma aflu pe o coloana
*	A treia conditie: daca vrem sa mergem in jos
*	A patra conditie: daca vrem sa mergem in sus

In functia Actualizare():
Definim un nou cap al sarpelui care va fi de fapt capul acestuia.
Verificam daca acesta iese din matrice: iesire dreapta – 8 si iesire stanga - -1, atunci acesta va reveni pe coloana 0 sau 8 de pe acelasi rand
Acelasi principiu il vom folosi si pe axa y.
Pentru a verifica daca sarpele se loveste de el vedem daca acesta ajunge cu capul pe o parte a corpului. Daca da, facem o pauza de o secunda si resetam matricea: ii dam hranei si sarpelui alte pozitii.
Pentru a verifica daca sarpele s-a hranit vedem daca acesta ajunge pe pozitia acestuia. Daca da, acesta isi va creste lungimea si ii vom schimba pozitia marului, aceasta fiind aleasa random. Dupa atasam corpului sarpelui noul cap, urmand actualizarea matricei din nou cu noua lungime a sarpelui si noua pozitia a marului. Pentru a actualiza, matricea shiftam la dreapta numarul coloanei cu numarul liniei adica aprindem led-urile de pe matrice.

# Bibliografie

[Led Matrix](https://docs.arduino.cc/built-in-examples/display/RowColumnScanning)
[Joy Stick](https://lastminuteengineers.com/joystick-interfacing-arduino-processing/)
[Delta Time](https://forum.arduino.cc/t/calculating-delta-time/490002)

