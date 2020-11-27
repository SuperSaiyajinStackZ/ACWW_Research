## Animal Crossing: Wild World Save Detector by SuperSaiyajinStackZ

## English
This tool allows you, to check the region of your Animal Crossing: Wild World Savefile.

This tool does not have a really useful use case for End-Users though.

How to use it? It is simple!!

Just drag and drop your Savefile into the executable.. and it shows you the detected region of your Animal Crossing: Wild World Savefile.


## Compilation
To compile this, you need a C++17 compatible compiler.

After it, run:
```
g++ --std=gnu++17 main.cpp
```

Now you find a `a.exe`, `a` or `a.out`, depending on which Operating System you are compiling.


## Explanation
The first byte is ALWAYS the same on the specific region. And with that.. the second Savcopy would have the same as well. What i do is:

* Check byte 0x0 AND (First byte of second Savcopy) for the gamecode, or whatever that is.

If it matches -> Boom! You got the region. It is unknown though, if other savefiles could randomly have that as well...

| Gamecode | Region   |
| -------- | -------- |
| 0xC5     | Europe   |
| 0x8A     | USA      |
| 0x32     | Japanese |
| 0x32     | Korean   |




## German | Deutsch
Diese Anwendung erlaubt es dir, die Region deines Animal Crossing: Wild World Spielstandes zu checken.

Diese Anwendung ist nicht wirklich nützlich für End-Benutzer.

Wie du dies benutzt? Ganz einfach!!

Ziehe einfach deinen Speicherstand auf die Anwendung.. und es zeigt dir die erkannte Region deines Animal Crossing: Wild World Speicherstandes an.


## Kompilierung
Um dies zu kompilieren brauchst du einen C++17 kompatiblen Compiler.

Danach führe einfach folgendes aus:
```
g++ --std=gnu++17 main.cpp
```

Jetzt findest du eine `a.exe`, `a` oder `a.out`, kommt darauf an auf welches Betriebssystem du das kompilierst.


## Erklärung
Der erste byte ist IMMER das selbe in der speziellen Region. Und damit.. hat die zweite Speicher-Kopie genau das selbe. Was ich ausführe:

* Überprüfe byte 0x0 UND (erster byte der zweiten Speicherkopie) für den Spielcode.. oder was auch immer das ist.

Falls es passt -> Boom! Du hast die Region. Es ist jedoch unbekannt, ob ein anderer Speicherstand dies aus zufall auch besitzt...

| Spielcode | Region   |
| --------- | -------- |
| 0xC5      | Europa   |
| 0x8A      | USA      |
| 0x32      | Japan    |
| 0x32      | Korea    |