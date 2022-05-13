
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


# emergency = []

# ser = serial.Serial('COM4',115200, timeout=0)
# while True:
#     if ser.in_waiting:
#         datapakke = ser.readline().hex()
#         if len(datapakke) < 130:
#             databuf = datapakke
#             emergency.append(databuf)
#             # print(emergency)
#             datapatched = "".join(emergency)
#             if len(datapatched) == 130:
#                 print(datapatched)
#                 print('reparert data: ' + datapatched)
#                 data = datapatched
#                 # print(bytes.fromhex(datapatched[0:2]).decode("ASCII"))
#                 # print(int(datapatched[2:6],16))
#                 # print(int(datapatched[6:10],16))
#                 # print(len(datapatched))
#                 emergency.clear()
#         else:
#             print(datapakke)
#             data = datapakke
#         print(bytes.fromhex(data[0:2]).decode("ASCII"))
#         print(int(data[2:6],16))
#         print(int(data[6:10],16))
#         print(len(data))


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


global avslutt
avslutt = False
global uart
uart = True
global data

tiddata = []    #Liste for lagring av tid
gyroxdata = []  #Lister for lagring av gyrodata
gyroydata = []  #------------------------------
gyrozdata = []  #------------------------------
gyrordata = []  #------------------------------
gyrosdata = []  #------------------------------
aksxdata = []   #Lister for lagring av gyrodata
aksydata = []   #------------------------------
akszdata = []   #------------------------------
aksrdata = []   #------------------------------
akssdata = []   #------------------------------
komprdata = []  #Lister for lagring av komplimentærfilterdata
kompsdata = []  #--------------------------------------------
emergency = []  #N%*ø*)dliste i tilfellet delt data skulle oppst%*å*)

screen = curses.initscr()   #Variabel for manipulering av terminalvinduet

row = 0     #Initialiserer f%*ø*)rste rad i terminalvindu
col = 0     #Initialiserer f%*ø*)rste kolonne i terminalvindu
screen.addstr(row,col, "Data: ")    #Skriver tekst p%*å*) f%*ø*)rste til 15 rad i terminalvindu
screen.addstr(row+1,col, "Tid passert: ")                
screen.addstr(row+2,col, "Gyroskop rundt x-akse: ")              
screen.addstr(row+3,col, "Gyroskop rundt y-akse: ")              
screen.addstr(row+4,col, "Gyroskop rundt z-akse: ")              
screen.addstr(row+5,col, "Gyroskop rull: ")                 
screen.addstr(row+6,col, "Gyroskop stamp: ")               
screen.addstr(row+7,col, "Akselerometer x-akse: ")         
screen.addstr(row+8,col, "Akselerometer y-akse: ")        
screen.addstr(row+9,col, "Akselerometer z-akse: ")          
screen.addstr(row+10,col, "Akselerometer rull: ")          
screen.addstr(row+11,col, "Akselerometer stamp: ")         
screen.addstr(row+12,col, "Komplementærfilter rull: ")     
screen.addstr(row+13,col, "Komplementærfilter stamp: ")    
screen.addstr(row+15,col, "Insert q to quit the program")  
screen.refresh()    #Oppdaterer terminalvinduet


def my_Serial():    #Lager funksjon for avlesning av data fra USB port
    global uart     #Globale variabler
    global avslutt  #-----------------
    global data     #-----------------
    ser = serial.Serial('COM11',115200, timeout=0)  #%*Å*)pner COM11 for avlesning med bitrate 115kHz
    
    while uart:     #L%*ø*)kke for avlesning 
        if ser.in_waiting:  #Om det er data klar for avlesning i fra USB, 
            datapakke = ser.readline().hex()    #Dekrypter data som heksadesimal verdi og lagre data som datapakke 
            if len(datapakke) < 130:    #Om datapakken er for kort,
                databuf = datapakke     #Sett buffer lik datapakke
                emergency.append(databuf)   #Legg til bufferpakke i n%*ø*)dliste
                datapatched = "".join(emergency)    #Sett sammen oppdelt data
                if len(datapatched) == 130: #Om datapakken n%*å*) har riktig lengde
                    data = datapatched  #Sett data som reparert data
                    emergency.clear()   #T%*ø*)m n%*ø*)dliste
            else:   #Om datapakken har riktig lengde
                data = datapakke    #Sett data som datapakke
            try:    #Om det ikke er noen feilmeldinger som oppst%*å*)r,
                sek = int(data[2:6],16)     #Lagre 3. til 6. dataverdi som sekund
                ms = int(data[7:10],16)     #Lagre 8. og 10. dataverdi som millisekund
                sendetid = int((sek + (ms/1000)) * 100) / 100.0 #Setter sammen tidsdata og lagrer
                g_x = float(int(data[16:20],16)/10) # Lagre 17. til 20 dataverdi som vinkel per sekund rundt x-akse
                g_y = float(int(data[26:30],16)/10) # Lagre 27. til 30 dataverdi som vinkel per sekund rundt y-akse
                g_z = float(int(data[36:40],16)/10) # Lagre 37. til 40 dataverdi som vinkel per sekund rundt z-akse
                g_r = float(int(data[46:50],16)/10) # Lagre 47. til 50 dataverdi som rull fra gyro
                g_s = float(int(data[56:60],16)/10) # Lagre 57. til 60 dataverdi som rull fra stamp
                a_x = float(int(data[66:70],16)/100)    # Lagre 67 til 70 dataverdi som akselerasjon på x-aksen
                a_y = float(int(data[76:80],16)/100)    # Lagre 77 til 80 dataverdi som akselerasjon på y-aksen
                a_z = float(int(data[86:90],16)/100)    # Lagre 87 til 90 dataverdi som akselerasjon på z-aksen
                a_r = float(int(data[96:100],16)/10)    # Lagre 97. til 100 dataverdi som rull fra akselerometer
                a_s = float(int(data[106:110],16)/10)   # Lagre 107. til 110 dataverdi som stamp fra akselerometer
                k_r = float(int(data[116:120],16)/10)   # Lagre 117. til 120 dataverdi som rull fra komplimentærfilter
                k_s = float(int(data[126:130],16)/10)   # Lagre 127. til 130 dataverdi som stamp fra komplimentærfilter


                if bytes.fromhex(data[0:2]).decode("ASCII") == "t":     #Om 1. og 2. verdi i datapakken tilsvarer bokstaven 't' i ASCII-format,       
                    screen.addstr(row+1, 13, "                            ")    #Sett inn blankt i rad 1 fra kolonne 13 av terminalvindu
                    screen.addstr(row+1, 13, str(sendetid) + " s")  #Sett inn tidsverdi i rad 1 fra kolonne 13 terminalvindu
                    tiddata.append(sendetid)        #Legg tidsverdi i liste for lagring
                    if int(data[15:16],16) == 1:    #Repeterer for all data
                        g_x = -g_x    
                    screen.addstr(row+2, 23, "                  ")
                    screen.addstr(row+2, 23, str(g_x) + " dps")
                    gyroxdata.append(g_x)
                    if int(data[25:26],16) == 1:
                        g_y = -g_y    
                    screen.addstr(row+3, 23, "                  ")
                    screen.addstr(row+3, 23, str(g_y) + " dps")
                    gyroydata.append(g_y)                   
                    if int(data[35:36],16) == 1:
                        g_z = -g_z    
                    screen.addstr(row+4, 23, "                  ")
                    screen.addstr(row+4, 23, str(g_z) + " dps")
                    gyrozdata.append(g_z)
                    if int(data[45:46],16) == 1:
                        g_r = -g_r    
                    screen.addstr(row+5, 15, "                  ")
                    screen.addstr(row+5, 15, str(g_r) + " °")
                    gyrordata.append(g_r)
                    if int(data[55:56],16) == 1:
                        g_s = -g_s    
                    screen.addstr(row+6, 16, "                  ")
                    screen.addstr(row+6, 16, str(g_s) + " °")
                    gyrosdata.append(g_s)
                    if int(data[65:66],16) == 1:
                        a_x = -a_x    
                    screen.addstr(row+7, 22, "                  ")
                    screen.addstr(row+7, 22, str(a_x) + " m/s²")
                    aksxdata.append(a_x)
                    if int(data[75:76],16) == 1:
                        a_y = -a_y    
                    screen.addstr(row+8, 22, "                  ")
                    screen.addstr(row+8, 22, str(a_y) + " m/s²")
                    aksydata.append(a_y)                   
                    if int(data[85:86],16) == 1:
                        a_z = -a_z    
                    screen.addstr(row+9, 22, "                  ")
                    screen.addstr(row+9, 22, str(a_z) + " m/s²")
                    akszdata.append(a_z)
                    if int(data[95:96],16) == 1:
                        a_r = -a_r    
                    screen.addstr(row+10, 20, "                  ")
                    screen.addstr(row+10, 20, str(a_r) + " °")
                    aksrdata.append(a_r)
                    if int(data[105:106],16) == 1:
                        a_s = -a_s    
                    screen.addstr(row+11, 21, "                  ")
                    screen.addstr(row+11, 21, str(a_s) + " °")
                    akssdata.append(a_s)
                    if int(data[115:116],16) == 1:
                        k_r = -k_r    
                    screen.addstr(row+12, 25, "                  ")
                    screen.addstr(row+12, 25, str(k_r) + " °")
                    komprdata.append(k_r)
                    if int(data[125:126],16) == 1:
                        k_s = -k_s    
                    screen.addstr(row+13, 26, "                  ")
                    screen.addstr(row+13, 26, str(k_s) + " °")
                    kompsdata.append(k_s)
            except NameError:   #Om det oppst%*å*)r NameError,
                pass    #Hopp til neste instans av l%*ø*)kken
        screen.addstr(1, 0, '') 
        screen.refresh()    #Oppdater terminalvindu




t1 = threading.Thread(target=my_Serial, daemon=True)    #Sett my_Serial funksjonen som daemon (sekund%*æ*)rl%*ø*)kke)
t1.start()  #Start sekund%*æ*)rl%*ø*)kken


while True:     #Hovedl%*ø*)kke
    key = screen.getkey()   #Registrer tastetrykk fra pc
    if key == 'q':      #Om tastetrykket tilsvarer bokstaven 'q'
        uart = False    #Stans sekund%*æ*)rl%*ø*)kken
        matlabdata = [                  #Lag data for avlesning i MATLAB
        't = ' + str(tiddata)+';',      # -----------------------------
        'gx = ' + str(gyroxdata)+';',   # -----------------------------
        'gy = ' + str(gyroydata)+';',   # ----------------------------- 
        'gz = ' + str(gyrozdata)+';',   # -----------------------------
        'gr = ' + str(gyrordata)+';',   # -----------------------------
        'gs = ' + str(gyrosdata)+';',   # -----------------------------
        'ax = ' +str(aksxdata)+';',     # -----------------------------
        'ay = ' +str(aksydata)+';',     # -----------------------------
        'az = ' +str(akszdata)+';',     # -----------------------------
        'ar = ' +str(aksrdata)+';',     # -----------------------------
        'as = ' +str(akssdata)+';',     # -----------------------------   
        'kr = ' +str(komprdata)+';',    # -----------------------------
        'ks = ' +str(kompsdata)+';']    # -----------------------------
        f = open("matlabIMU.txt","w+",encoding='utf-8') #Lag/%*Å*)pne tekstdokument
        f.write('% matlab data :\n\n')  #Skriv matlabdata i tekstdokument
        for x in matlabdata:            # ------------------------------
            f.write(x+'\n')             # ------------------------------
        f.write('\n\n\nTekstfil generert '+ datetime.now().strftime("%d/%m/%Y %H:%M:%S"))   #Skriv tid og dato tekstfil ble oppdatert
        f.close()   #Lukk tekstfil
        
        break   #Hopp ut av l%*ø*)kke