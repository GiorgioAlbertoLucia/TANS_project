# README solo per Roby

<img src="charmander.gif" width="500" height="500">


## Come leggere questo README in maniera decente

Premi ctrl + shift + V (se non funziona è colpa di Windows)

## Come caricare le cose su root senza impazzire

Chiaramente puoi caricare manualmente le classi come al solito. È un po' da pezzenti. Per questo motivo il tuo compagnetto ti fa le cose fighe.

### Caricare tutte le classi di Giogio e Yaml
```
.x exe/loadMacros.cpp
``` 

### Caricare tutte le classi di Giogio e Yaml e RECOSTRUCTION
```
.x exe/loadMacrosRoby.cpp
``` 

## Come creare un TTree con la simulazione

Per creare un TTree con la simulazione puoi fare:

```
Simulation * sim = new Simulation("Simulation", "config");
sim->runSimulation(200);
```

Chiaramente al posto del 200 puoi mettere il numero che vuoi. In realtà quelli che ti lasciavo finora avevano quel valore uguale a 2. Se ne fai di più grossi per verifica o altro ricordati di eliminarli prima di pushare (altrimenti o github si lamenta perché sono pesanti o presumo ci metta una vita). Quando ci vediamo/discordiamo mettiamo a posto questa cosa.
