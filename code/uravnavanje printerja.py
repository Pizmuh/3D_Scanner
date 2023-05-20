import math #Uvozimo knjižnice, ki vsebujejo matematične funkcije
import serial #Uvozimo knjižnico ki python programu omogoča branje serijskega porta
import matplotlib.pyplot as plt

ser = serial.Serial('COM12', 115200)
sum = 0
i = 0
coordinates = []
X = []
Y = []
Z = []

obrat = 200

def plotting(x, y, z):
    fig = plt.figure()
    ax = plt.axes(projection="3d")

    ax.plot3D(x, y, z)
    ax.set_xlabel('X Axes')
    ax.set_ylabel('Y Axes')
    ax.set_zlabel('Z Axes')

    plt.show()



while True:
    try:

        bingo = str(ser.readline())

        board = bingo.strip()
        v1 = board.split(',')
        distance = v1[1]

        distance = int(distance)
        sum = sum + distance


        if (i % 10 == 0):
            avrage = sum/10
            print(avrage)
            sum = 0

    except:
        continue

    i = i + 1






