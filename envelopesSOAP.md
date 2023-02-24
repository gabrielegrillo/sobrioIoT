# Envelopes 

## Request to import the configuration of the control unit into the db

identificatoreCentralina is the id of the control unit. Localita is the location of the control unit and Umidita, Pressione, Precipitazioni and Direzione_Vento are keys for the sensors.

```xml 
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:mon="monitoraggio.ambientale">
   <soapenv:Header/>
   <soapenv:Body>
      <mon:ConfiguraCentralina>
         <!--Optional:-->
         <mon:stringaJson>{"identificatoreCentralina": "A1234BCD1E", "Localita":"Milano", "Umidita": true, "Pressione": true, "Precipitazioni": true, "Direzione_Vento": true}</mon:stringaJson>
      </mon:ConfiguraCentralina>
   </soapenv:Body>
</soapenv:Envelope>
```

## Response 

just OK string 

## Request with the obtained values from the sensors

```xml 
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:mon="monitoraggio.ambientale">
   <soapenv:Header/>
   <soapenv:Body>
      <mon:MemorizzaRilevazione>
         <!--Optional:-->
         <mon:stringaJson>{"Temperatura": 9, "Pressione": 9, "identificatoreCentralina": "A1234BCD1E" }</mon:stringaJson>
      </mon:MemorizzaRilevazione>
   </soapenv:Body>
</soapenv:Envelope>
```

## Response 

just OK string

## Request of configuration of the central unit by using the id

```xml
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:mon="monitoraggio.ambientale">
   <soapenv:Header/>
   <soapenv:Body>
      <mon:GetConfCentralina>
         <!--Optional:-->
         <mon:identificatoreCentralina>A1234BCD1E</mon:identificatoreCentralina>
      </mon:GetConfCentralina>
   </soapenv:Body>
</soapenv:Envelope>
```

## Response

```xml
<soap11env:Envelope xmlns:soap11env="http://schemas.xmlsoap.org/soap/envelope/" xmlns:tns="monitoraggio.ambientale">
   <soap11env:Body>
      <tns:GetConfCentralinaResponse>
         <tns:GetConfCentralinaResult>{"identificatoreCentralina":"A1234BCD1E","Localita":"Cosenza","Temperatura":True,"Pressione":False,"Umidita":True,"Precipitazioni":False,"Velocita_Vento":True,"Direzione_Vento":True}</tns:GetConfCentralinaResult>
      </tns:GetConfCentralinaResponse>
   </soap11env:Body>
</soap11env:Envelope>
```
