# README solo per Roby

<img src="charmander.gif" width="500" height="500">


## Come leggere questo README in maniera decente

Premi ctrl + shift + V (se non funziona è colpa di Windows)

## Come caricare le cose su root senza impazzire

Chiaramente puoi caricare manualmente le classi come al solito. È un po' da pezzenti. Per questo motivo il tuo compagnetto ti fa le cose fighe.

### Caricare tutte le classi di RoGio e Yaml
```
.x exe/loadMacros.cpp
``` 

## Come far girare tutto senza impazzire
```
.x exe/runSoftware.cpp
```

### Vabbé GioGio, ma sto modello 3D rompe le palle ogni volta che faccio girare
Uff, fai così allora:
```
.x exe/runNo3D.cpp
```

## Come creare un TTree con la simulazione

Per creare un TTree con la simulazione puoi fare:

```
Simulation * sim = new Simulation("Simulation", "config/configFile.txt");
sim->runSimulation(10000);
```

## ABBIAMO LE RETTE ROBYYYY

Se vuoi, adesso puoi giocare con i cilindri e con le rette delle traiettorie delle particelle. All'interno del file data/recordSimulation.txt ci sono informazioni di un evento della simulazione (guarda come l'ho fatto scrivere bene). 

La macro test/geomDisplay.cpp genera i detector e le traiettorie simulate (poi ci aggiungeremo quelle ricostruite).
La pensata è parecchio utile, perché mi ha fatto capire che sono stato un animale a fare il multiple scattering. Quindi aspetta a instagrammare.
Se vuoi farlo girare, comunque, 

```
.L yaml/Yaml.cpp+
.L test/geomDisplay.cpp+
geomDisplay();
```
