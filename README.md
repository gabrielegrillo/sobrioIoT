# sobrioIoT
Webservice project reliased during the last months before the Esame di Stato (Final exams). 

## How does it work?
Each control unit must be registered before it can start its detection activity, or its configuration must be stored in the database.

An environmental technician interacts with a special web page in which he indicates the types of sensors with which the control unit is equipped and a unique identifier that will be suggested by the page. This web page will use the **MemorizzaConfigurazione** method made available by the webservice.

Once the control unit is enabled, it will start transmitting the detected environmental parameters at regular intervals. The data is encoded in a SOAP envelope so that it can be processed by the webservice, invoking the **MemorizzaRilevazione** method.

## Tecnologies used
In this project we used Python for the SOAP Webservice with Spyne framework (including WSGI) and Adruino for coding the main sketch of each control unit.
Each message between control units and the webserver is decoded in JSON.

## Special Thanks
Thanks to the teacher Paolo Mangraviti for leading and contributing in this project <3
