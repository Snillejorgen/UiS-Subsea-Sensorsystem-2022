from base64 import encode
from datetime import datetime
from encodings import utf_8
from datetime import date
from glob import glob
from logging import exception
from ntpath import join
from time import sleep
import serial
import serial.tools.list_ports 
import curses
import threading
import signal
import sys
import numpy as np
import struct

global avslutt
avslutt = False
global uart
uart = True
global data
minutt = 0

# ports = serial.tools.list_ports.comports()

# portList = []

# for onePort in ports:
#     portList.append(str(onePort))
#     print(str(onePort))

# val = input("select Port: COM")

# for x in range(0,len(portList)):
#     if portList[x].startswith("COM" + str(val)):
#         portVar = "COM" + str(val)
#         print(portList[x])

tempdata = []   #Liste for lagring av temperaturdata
emergency = []  #Nødliste i tilfellet delt data skulle oppstå
tiddata = []    #Liste for lagring av tid passert

screen = curses.initscr()   #Variabel for manipulering av terminalvinduet

row = 0     #Initialiserer første rad i terminalvindu
col = 0     #Initialiserer første rad i terminalvindu
screen.addstr(row+1,col, "Temperatur: 1 °C")    #Skriver tekst på første rad i terminalvindu
screen.addstr(row+2,col, "Tid gått: 5 minutt")
screen.addstr(row+4,col, "Insert q to quit the program")    #Skriver tekst på andre rad i terminalvindu
screen.refresh()    #Oppdaterer terminalvinduet


def my_Serial():    #Lager funksjon for avlesning av data fra USB port
    global uart     #Globale variabler
    global avslutt  # --------------
    global data     # --------------
    global minutt   # --------------
    ser = serial.Serial('COM3',115200, timeout=0)   #Åpner COM4 for avlesning med bitrate 115kHz
    
    while uart:     #Løkke for avlesning 
        if ser.in_waiting:  #Om det er data klar for avlesning i fra USB, 
            datapakke = ser.readline().hex()    #Dekrypter data som heksadesimal verdi og lagre data som datapakke 
            if len(datapakke) < 6:  #Om datapakken er for kort,
                databuf = datapakke #Sett buffer lik datapakke
                emergency.append(databuf)   #Legg til bufferpakke i nødliste
                datapatched = "".join(emergency)    #Sett sammen oppdelt data
                if len(datapatched) == 6:   #Om datapakken nå har riktig lengde
                    data = datapatched  #Sett data som reparert data
                    emergency.clear()   #Tøm nødliste
            else:   #Om datapakken har riktig lengde
                data = datapakke # Sett data som datapakke
            try: #Om det ikke er noen feilmeldinger som oppstår,
                temp = int(data[4:6],16)    #Lagre 5. og 6. dataverdi som temperatur
                if bytes.fromhex(data[0:2]).decode("ASCII") == "c": #Om 1. og 2. verdi i datapakken tilsvarer bokstaven 'c' i ASCII-format,
                    if int(data[2:4],16) == 1:  #og om 3. og 4. verdi i datapakken gir 1,
                        temp = -temp    #Snu polariteten på temperaturvariabelen
                    screen.addstr(row+1, 12, "                  ")  #Sett inn blankt i rad 1 fra kolonne 12 av terminalvindu
                    screen.addstr(row+1, 12, str(temp) + " °C")     #Sett inn temperaturverdi i rad 1 fra kolonne 12 terminalvindu
                    tempdata.append(temp)   #Legg temperaturverdi i liste for lagring
            except NameError:   #Om det oppstår NameError,
                pass            #Hopp til neste instans av løkken
            minutt += 1
            screen.addstr(row+2, 10, "                  ") 
            screen.addstr(row+2, 10, str(minutt - 1) + ' minutt')     
            tiddata.append(minutt - 1)   #Legg tidsverdi i liste for lagring

        screen.addstr(1, 0, '') 
        screen.refresh()        #Oppdater terminalvindu




t1 = threading.Thread(target=my_Serial, daemon=True)    #Sett my_Serial funksjonen som daemon (sekundærløkke)
t1.start()  #Start sekundærløkken


while True: #Hovedløkke
    key = screen.getkey()   #Registrer tastetrykk fra pc
    if key == 'q':  #Om tastetrykket tilsvarer bokstaven 'q'
        uart = False    #Stans sekundærløkken
        matlabdata = [                  #Lag data for avlesning i MATLAB
        'temp = ' +str(tempdata)+';',   # -----------------------------
        't = ' +str(tiddata)+';']       # -----------------------------
        f = open("matlabTEMP.txt","w+",encoding='utf-8')    #Lag/Åpne tekstdokument
        f.write('% matlab data :\n\n')  #Skriv matlabdata i tekstdokument
        for x in matlabdata:            # ------------------------------
            f.write(x+'\n')             # ------------------------------
        f.write('\n\n\nTekstfil generert '+ datetime.now().strftime("%d/%m/%Y %H:%M:%S"))   #Skriv tid og dato tekstfil ble oppdatert
        f.close()       #Lukk tekstfil
        
        break   #Hopp ut av løkke