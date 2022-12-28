Commento le cose che puoi fare per far girare quello che ho fatto finora.

```
root -l
```

Chiaramente puoi caricare manualmente le classi come al solito. Queste sono disponibili:

```
.L custom_classes/pointCC/pointCC.cpp
.L custom_classes/hit/hit.cpp
.L custom_classes/particle/particle.cpp
.L custom_classes/event/event.cpp
.L custom_classes/simulation/simulation.cpp
```

Alternativa figa perché siamo studiati (fa tutte le precedenti in un colpo solo):
```
.x exe/loadMacros.cpp
``` 

Per creare un TTree con la simulazione puoi fare:

```
Simulation * sim = new Simulation("Simulation", "config");
sim->runSimulation(2);
```

Sto vedendo un parser per aggiungere file di configurazione .yaml. Finito di lavorarci ti spiego come puoi divertirti a modificarlo

>>>>>>> Questo commento è invecchiato male 
Conto di fare presto un file per caricare tutto in modo da non impazzire per fare il debug. Ci vorrà un po' di tempo, ma soprattutto tante bestemmie. Dopo il progetto vado a farmi un pellegrinaggio