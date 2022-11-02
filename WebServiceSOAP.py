import logging
import json
import pyodbc
from spyne.decorator import srpc
from spyne.service import ServiceBase
from spyne.util.simple import wsgi_soap_application
import numpy as np

class ServizioMonitoraggioCentraline(ServiceBase):


    @srpc(str,_returns=str)
    def MemorizzaRilevazione(stringaJson): # Store the data logged, with control unit, in the db
        server = '(localdb)\\MSSQLLocalDB'
        database = 'Dati_Raccolti'
        username = 'sa'
        password = 'abacus'
        cnxn = pyodbc.connect(
            'DRIVER={ODBC Driver 17 for SQL Server};SERVER=' + server + ';DATABASE=' + database + ';UID=' + username + ';PWD=' + password)
        stringaJson = json.loads(stringaJson)
        cursor = cnxn.cursor()
        count = cursor.execute(costruisciRichiestaInsertSql(stringaJson, "Rilevazioni")).rowcount
        cnxn.commit()
        if count>0:
            return 'OK!'
        else:
            return 'Error!'


    @srpc(str,_returns=str)
    def ConfiguraCentralina(stringaJson): # Response with the config of the control unit
        server = '(localdb)\\MSSQLLocalDB'
        database = 'Dati_Raccolti'
        username = 'sa'
        password = 'abacus'
        cnxn = pyodbc.connect(
            'DRIVER={ODBC Driver 17 for SQL Server};SERVER=' + server + ';DATABASE=' + database + ';UID=' + username + ';PWD=' + password)
        stringaJson = json.loads(stringaJson)
        cursor = cnxn.cursor()
        count = cursor.execute(costruisciRichiestaInsertSql(stringaJson, "Centraline")).rowcount
        cnxn.commit()
        return 'OK!'

    @srpc(str, _returns=str)
    def GetConfCentralina(identificatoreCentralina): # Gets the config from the db
        server = '(localdb)\\MSSQLLocalDB'
        database = 'Dati_Raccolti'
        username = 'sa'
        password = 'abacus'
        cnxn = pyodbc.connect(
            'DRIVER={ODBC Driver 17 for SQL Server};SERVER=' + server + ';DATABASE=' + database + ';UID=' + username + ';PWD=' + password)
        cursor = cnxn.cursor()
        cursor.execute("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'Centraline'")
        nomiColonne = np.asarray(cursor.fetchall())
        c2 = cnxn.cursor()
        c2.execute("SELECT * FROM Centraline WHERE identificatoreCentralina='A1234BCD1E'") # TODO: Adjust this query
        valori = np.transpose(np.asarray(c2.fetchall()))
        stringaJson = "{"
        for i in range(0, len(nomiColonne)):
            nomeCampo = nomiColonne[i][0]
            valoreCampo = valori[i][0]
            if isinstance(valoreCampo, str) and (valoreCampo != "True") and (valoreCampo != "False"):
                valoreCampo = '"' + valoreCampo + '"'
            stringaJson += '"' + nomeCampo + '"' + ":" + valoreCampo + ","
        stringaJson = stringaJson[0:len(stringaJson) - 1] + "}"
        return stringaJson



# Make the insert query
def costruisciRichiestaInsertSql(stringaJson,tabella):
        sqlInsert = "INSERT INTO " + tabella + "("
        for k in stringaJson:
            sqlInsert += k + ','
        sqlInsert = sqlInsert[0:len(sqlInsert) - 1]
        sqlInsert += ") VALUES("
        for k in stringaJson.values():
            sqlInsert += "'" + str(k) + "',"
        sqlInsert = sqlInsert[0:len(sqlInsert) - 1]
        sqlInsert += ");"
        return sqlInsert


if __name__=='__main__':
    from wsgiref.simple_server import make_server

    logging.basicConfig(level=logging.DEBUG)
    logging.getLogger('spyne.protocol.xml').setLevel(logging.DEBUG)

    logging.info("listening to http://127.0.0.1:7789")
    logging.info("wsdl is at: http://localhost:7789/?wsdl")

    wsgi_app = wsgi_soap_application([ServizioMonitoraggioCentraline], 'monitoraggio.ambientale')
    server = make_server('127.0.0.1', 7789, wsgi_app)
    server.serve_forever()