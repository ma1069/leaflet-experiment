# Progetto Ambiente

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

Nella cartella <_device_> e' presente il codice da caricare sul device per l'acquisizione

## Trasmissione dei dati 

TBD - Libreria Arduino da scrivere per il modulo ESP 

## Stoccaggio e trattamento dei dati

Sotto <_server_> e' presente il progetto Python Flask che implementa i seguenti endpoints:

### Data upload

  Endpoint utilizzato dal device per inviare i dati rilevati dai propri sensori.
  La strategia di questo endpoint consiste nell'inserire direttamente in url l'identificativo del dispositivo, in modo che anche direttamente dai log delle chiamate sia possibile distinguere i vari dispositivi, mentre come parametro PUT va inserita la stringa con tutti i valori dei sensori. Il formato di questa stringa non e' ancora definitivo.

  * **URL:** 
    /v0/data
  * **Method:**

    `PUT`

  * **URL Params:** 

    **Required**
    `device=[alphanumeric]`

  * **Data Params (URL encoded)**

    **Required**
    `values=[alphanumeric]`

  * **Success Response:**

    * **Code:** 200 <br />
      **Content:** `{ status: 'ok' }`

  * **Error Response:**

    * **Code:** 400 <br />
      **Content:** `{ status: 'fail', reason: 'Invalid values' }`    
    
  * **Sample Call:**

    ```
    import requests
    r = requests.put('https://your_url_here/v0/data?device=A001', { 'values': 'P25:1929.3P10:992' })
    ```

### Data fetch (latest)

  Endpoint utilizzato dal web client per ottenere l'ultimo valore inviato dai vari dispositivi

  * **URL:** 
    /v0/data/last
  * **Method:**

    `GET`

  * **Success Response:**

    * **Code:** 200 <br />
      **Content:** 
   
    ``` 
    {
	"status": "ok",
	"devices": [
	{
		"id": "A001",
		"meta": {
			"nome": "I.I.S. G.Cena",
			"desc": "Descrizione <b>generica</b>",
			"lat": 45.467,
			"lon": 7.876
		},
		"values": {
			"pm2": 15.2,
			"pm10": 92.2
		}
	},
	...
	]
    }
    ```
    
  * **Notes:**

    Tutto l'oggetto "meta" puo' essere vuoto, se il dispositivo non e' ancora stato geolocalizzato

### Aggiornamento metadati

  Endpoint utilizzato dal client web per associate metadati ad un dispositivo.
  Tutti i parametri sono opzionali, in modo che questa API possa essere richiamata per aggiornare anche solo un valore.

  * **URL:** 
    /v0/device
  * **Method:**

    `POST`

  * **URL Params:** 

    **Required**
    `id=[alphanumeric]`

  * **Data Params (URL encoded)**

    **Optional**
    `nome=[alphanumeric]`
    `desc=[alphanumeric]`
    `lat=[numeric]`
    `lon=[numeric]`

  * **Success Response:**

    * **Code:** 200 <br />
      **Content:** `{ status: 'ok' }`

  * **Error Response:**

    * **Code:** 400 <br />
      **Content:** `{ status: 'fail', reason: 'Invalid values' }`    
    
  * **Sample Call:**

    ```
    import requests
    r = requests.post('https://your_url_here/v0/device?id=A001', { nome='I.I.S. G.Cena', desc='nuova descrizione' })
    ```

### Ping

  Endpoint utilizzato dai dispositivi per testare la connessione

  * **URL:** 
    /v0/ping
  * **Method:**

    `GET`

  * **Success Response:**

    * **Code:** 200 <br />
      **Content:** 
      `{ status: 'ok', message: 'pong' }`


Tutti i dati sono memorizzati su di un db MySQL, la cui struttura e' ancora in via di definizione.

## Fruizione dei dati (WEB)

Sotto <_web_> e' presente il web client statico in grado di comunicare con il server e di mostrare i dati geolocalizzati tramite OpenStreetMap

# Nice to have per il futuro

- Meccanismo di sicurezza  per autenticare e validare i dati ricevuti dai vari dispositivi
- Adozione di shield LoRa per trasmettere i valori dei sensori a distanza, riducendo la necessita' di connettivita' internet tramite reti wifi
- Aggiunta di funzioni di storage per permettere la trasmissione dei valori su reti intermittenti senza perdita di informazioni storiche
- Potenziamento del server di stoccaggio tramite motori di database scalabili (e.g. sistemi open NoSQL)
- Aggiunta di un'interfaccia web protetta da autenticazione per l'aggiunta di metadati ai vari flussi di informazioni (e.g. aggiungere i dati della scuola che sta trasmettendo i valori dell'inquinamento, geolocalizzarla etc. direttamente da web)
