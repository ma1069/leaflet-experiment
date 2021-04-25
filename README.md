# Progetto InquIno

Il progetto si propone di realizzare un sistema open per la condivisione di dati geolocalizzati relativi all'inquinamento ambientale.

Questo progetto nasce con lo scopo di gettare le basi per realizzare un sistema centralizzato per il monitoraggio dello stato di inquinamento del territorio, basi che possono poi essere riproposte alle scuole della zona che possono contribuire attraverso nuovi progetti, per la realizzazione nuove centraline per il monitoraggio o per il potenziamento di quanto costruito.

# Struttura

Nella sua prima iterazione, il progetto si compone da quattro moduli essenziali: 

 - Modulo di acquisizione dei dati
 - Modulo di trasmissione dei dati
 - Server per il trattamento dei dati
 - Interfaccia web per la fruizione di tali dati

I quattro moduli sono studiati per essere sviluppati in modo del tutto indipendente, con interfacce di comunicazione preconcordate. In questo modo sara' possibile sostituire un solo modulo per volta per effettuarne eventuali miglioramenti/potenziamenti.

## Acquisizione dei dati

L'acquisizione dei dati viene effettuata attraverso un dispositivo Arduino, munito di sensori adeguati e programmato attraverso il codice di Luigi (ancora da far pervenire e da includere nel repository). I dati recuperati da questo software verranno inviati al modulo successivo tramite la relativa interfaccia, descritta nella prossima sezione

## Trasmissione dei dati

Per poter trasmettere i valori dei sensori al server si presuppone di avere a disposizione una connessione WiFi, a cui la scheda Arduino potra' collegarsi tramite un modulo wifi connesso tramite porta seriale. Verra' sviluppata una libreria apposita per l'utilizzo di questo modulo, che si proporra' di ricevere i valori dei sensori dal sistema di acquisizione per inviarli al server centrale tramite una chiamata alle API REST disponibili sul server

## Stoccaggio e trattamento dei dati

E' necessario sviluppare un server centrale per poter raccogliere tutti i dati trasmessi dai vari dispositivi e poi metterli a disposizione dai software di fruizione (in questo momento, solamente l'interfaccia web, ma in futuro i software potrebbero essere molteplici). Indicativamente, le API da sviluppare dovranno occuparsi delle seguenti funzioni:
  - API per il salvataggio dei dati (endpoint che andra' condiviso con il modulo di trasimssione dei dati) 
  - API per la geolocalizzazione a l'aggiunta di informazioni ad ogni flusso di dati 
  - API per la ricezione dei dati aggregati, utilizzata per aggiornare regolarmente l'interfaccia web
  - API per la consultazione storica dei dati

## Interfaccia web per la fruizione dei dati 

Per poter fruire i dati ci si prepone di utilizzare un'interfaccia web in grado di scaricare i dati dal server centrale e mostrarli, possibilimente abbinati ad una mappa del territorio. Questo e' il modulo che ha il maggior numero di possibili iterazioni: e' possibile partire da una semplice cartina con indicati i valori di inquinamento posizionati geograficamente nelle posizioni piu' corrette per arrivare a veri e propri strumenti di data warehouse per l'analisi andamentale dei valori registrati. Ci sono molti strumenti di analisi open collegabili a questa interfaccia, che andranno valutati in futuro.

# Implementazione 

## Acquisizione dei dati

TBD - Soluzione Arduino preesistente ancora da reperire

## Trasmissione dei dati 

TBD - Libreria Arduino da scrivere per il modulo ESP 

## Stoccaggio e trattamento dei dati

TBD - Set di endpoint Python Flask in grado di salvare i dati su di un database SQLite

## Fruizione dei dati (WEB)

TBD - Al momento dovrebbe essere sufficiente una pagina statica HTML, che aggiorna una mappa OpenStreetMap tramite del codice JavaScript

# Nice to have per il futuro

- Meccanismo di sicurezza  per autenticare e validare i dati ricevuti dai vari dispositivi
- Adozione di shield LoRa per trasmettere i valori dei sensori a distanza, riducendo la necessita' di connettivita' internet tramite reti wifi
- Aggiunta di funzioni di storage per permettere la trasmissione dei valori su reti intermittenti senza perdita di informazioni storiche
- Potenziamento del server di stoccaggio tramite motori di database scalabili (e.g. sistemi open NoSQL)
