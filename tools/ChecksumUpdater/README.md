## Animal Crossing: Wild World Checksum Updater by SuperSaiyajinStackZ

## English
This tool allows you, to update the checksum of your Savefile. Also known as "Rehash" your Savefile.

This tool is useful when..
* You do Save Edits with a hex editor.
* Your Savefile is not being detected on the game. (Sometimes caused by invalid checksums).
* You just want to make sure, everything is alright on checksum side.

How to use it? It is simple!!

Just drag and drop your Savefile into the executable.. and it checks and such for you.

## Compilation
To compile this, you need a C++17 compatible compiler.

After it, run:
```
g++ main.cpp
```

Now you find a `a.exe` or just `a`, depending on which Operating System you are compiling.


## Checksum explained
The game checks, if (everything of the Savecopy except for the checksum) + (The checksum) are 65536. This would be exactly 1 too much for an `uint16_t` aka 2 byte. This causes it at the end, to go back to `0` and it is valid. More about it on the [wiki](https://github.com/SuperSaiyajinStackZ/ACWW_Research/wiki) in the future.




## German | Deutsch
Diese Anwendung erlaubt es dir, deinen Checksum zu aktualisieren. Das ist auch als "Rehash" deinen Speicherstand bekannt.

Diese Anwendung ist hilfreich wenn..
* Du Speicherstand-Modifikationen durchführst.
* Dein Speicherstand vom Spiel nicht erkannt wird. (Passiert manchmal durch ungültige Checksums).
* Du einfach nur sicherstellen willst.. dass von der Checksum Seite alles in Ordnung ist.

Wie du dies benutzt? Ganz einfach!!

Ziehe einfach deinen Speicherstand auf die Anwendung.. und es überprüft und so für dich.


## Kompilierung
Um dies zu kompilieren brauchst du einen C++17 kompatiblen Compiler.

Danach führe einfach folgendes aus:
```
g++ main.cpp
```

Jetzt findest du eine `a.exe` oder einfach nur `a`, kommt darauf an auf welches Betriebssystem du das kompilierst.


## Checksum erklärt
Das Spiel überprüft, ob (Alles von der Speicherkopie außer des Checksums) + (Der Checksum) 65536 ergibt. Das ist genau 1 zu viel für einen `uint16_t` aka 2 byte. Das resultiert, dass dies auf 0 zurückgesetzt wird und dann gültig wird. Mehreres dazu auf der [wiki](https://github.com/SuperSaiyajinStackZ/ACWW_Research/wiki) in der Zukunft.