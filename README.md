# Common interface

Pliki `BackendBase.hpp` i `ToolBase.hpp` zawierają propozycje klas abstrakcyjnych, po których mogłyby dziedziczyć wszystkie toole i backendy.

## Instalacja

Wystarczy wrzucić pliki `BackendBase.hpp` i `ToolBase.hpp` do głównego katalogu swojego projektu.

## Przykłady użycia

#### `ExampleBackend.hpp`

#### `ExampleFrontend.hpp`

Przykład online: [gitpod.io#https://github.com/PO-Project/common-interface](https://www.gitpod.io#https://github.com/PO-Project/common-interface)

Kompilacja przykładu: `g++ main.cpp -o example -lncurses -lpanel`

Na komputerach politechnicznych sugeruję: `g++ main.cpp -o example -lncurses -lpanel -I /usr/include/ncurses`

## ncurses

[Tutorial ncurses najlepszy jaki znalazłem.](http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html) [Jego rozdział o panelach.](http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/panels.html)

Proponuję używać ncurses z panelami:

  * ncurses trzyma globalny stos z panelami określający w jakiej kolejności są rysowane (który jest 'na górze' gdy dwa nachodzą na siebie)
  * Backend trzyma swoje panele na spodzie stosu paneli (np. statusline vima).
  * Backend wywołuje funkcje Tool::setCoordinates(startx, starty, width, height) przed rozpoczęciem wczytywania inputu od usera i po wykryciu zmiany rozmiaru okna, wtedy tool tworzy/zmienia swoje panele, powinny mieścić się w koordynatach podanych w argumentach, inaczej mogą zakrywać to co rysuje backend.
  * Jeśli backend dodaje nowe panele albo chce zmienić kolejność paneli na stosie (np. żeby kursor był w dobrym miejscu przy wpisywaniu komendy w vimie, albo by wyświetlić menu mcedita które może zasłaniać to co rysuje tool podczas wybierania opcji) to zawsze przed wywołaniem następnej funkcji zbindowanej przez toola przenosi swoje panele na spód stosu, nie zmienia kolejności paneli toola
  * Backend wywołuje update\_panels() i doupdate() po każdym wywołaniu zbindowanych funkcji, przed oczekiwaniem na input

// Chyba że napiszemy jakiś ładny, wspólny, obiektowy wrapper do ncurses

## Współpraca

Jestem otwarty na dyskusję o zmianach.


