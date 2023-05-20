import math                         #Uvozimo modul z matematičnimi funkcijami
import serial                       #Uvozimo modul, ki nam omogoča branje serijskega vmesnika na računalniku
import matplotlib.pyplot as plt     #Modul, ki nam prikaže podatke

ser = serial.Serial('COM3', 9600)      #Initilaiziramo komunikacijskiu kanal na com portu

i = 0                   #Neka spremenljivka
coordinates = []        #Pripraimo prostor za shranjevanje kordinat
X = []                  #Pripravimo prostor za sharanjevanje kordinat z osjo
Y = []                  #Pripravimo prostor za sharanjevanje kordinat z osjo
Z = []                  #Pripravimo prostor za sharanjevanje kordinat z osjo

obrat = 200             #Definiram velikost obrata na motorju


def plotting(x, y, z):                  #Funkcija ki vzame podatke ter sestavi 3d model
    fig = plt.figure()                  #Pripravimo vrst 3d prikazovalnika
    ax = plt.axes(projection="3d")      #Povemo da bomo uporabili 

    ax.plot3D(x, y, z)                  #Povemo na kero os plottamo katere podatke
    ax.set_xlabel('X Axes')             #Poimenujemo osi
    ax.set_ylabel('Y Axes')             #Poimenujemo osi
    ax.set_zlabel('Z Axes')             #Poimenujemo osi

    plt.show()                          #prikažemo graf


while (i < 1 * obrat):      #Ko je i manjše kot število obratov 
    try:                    # Če je vse vredu poizkusimo 

        bingo = str(ser.readline())                             # preberemo vrstico, ki je bla poslano na sersijki vhod

        board = bingo.split(':')                                #Ločimo vrstice glede na dvopičja
        measured_height = board[1].split(',')                   #Loči besedilo glede na vejice
        measured_height = int(measured_height[0])               #Iz številke, ki je zapisana kot beseda jo spremenimo v številka
        measured_spin = board[2].split(',')                     #Loči besedilo glede na vejice
        measured_spin = measured_spin[0]                        #Iz številke, ki je zapisana kot beseda jo spremenimo v številka
        measured_distance = board[3].split(",")                 #Loči besedilo glede na vejice
        measured_distance = measured_distance[0]                #iz številke, ki je zapisana kot beseda jo spremenimo v številka

        calculated_angle = int(measured_spin) * (360 / 2048)                        #Izračunamo kot za katerega se motor zavrti
        calculated_distance = 10 - int(measured_distance)                          #Pretvori v pravilne enote!!!!!
        calculated_width = math.sin(calculated_angle) * calculated_distance       #izračunamo x kordinante

        X.append(calculated_distance)       #Dodamo x kordinato v prej narejeno tabelo
        Y.append(calculated_width)          #Dodamo y kordinato v prej narejeno tabelo
        Z.append(measured_height)           #Dodamo z kordinato v prej narejeno tabelo

        coordinates.extend([calculated_distance, calculated_width, measured_height])        #Premaknemo kordinate v spremewnljivko kordinta

        print(coordinates)               #Izpišemo kordinate

        i = i + 1                       #povečamo i (i predstavlja število meritev, ki jih bo senzor naredil)
        print(i)                        #Izpišemo na kateri meritvi smo


    except:                         #Ko prekinemo zanko oz. zanki zmanjka podatkov
        plotting(X, Y, Z)           #Aktiviramo funkcijo za prikazovanje pik v 3d kordinatnem objektu 
        break                       #Prekinemo in končamo



