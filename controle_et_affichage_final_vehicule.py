import math
import tkinter as tk
import paho.mqtt.client as mqtt

x = 0
y = 0
angle = 0
distance = 3.249
Premier_Tournant = ""
Compteur_Tournant = 0
Compteur_Tournant_Gauche = 0
Compteur_Tournant_Droit = 0
previous_positions = []

window = tk.Tk()
window.title("Position de la CASHMOBILE")

canvas = tk.Canvas(window)
canvas.pack(fill=tk.BOTH, expand=True)

echelle = 1.0


def on_message(client, userdata, msg):
    global x, y, angle, echelle, Compteur_Tournant, Compteur_Tournant_Gauche, Compteur_Tournant_Droit, Premier_Tournant

    message = msg.payload.decode()
    print(message)
    if message == "forward":
        if(Compteur_Tournant_Droit > 3):
            angle -= 1.8*Compteur_Tournant_Droit
        Compteur_Tournant_Gauche = 0
        Compteur_Tournant_Droit = 0
        x += distance * math.cos(math.radians(angle))
        y += distance * math.sin(math.radians(angle))
        previous_positions.append((x, y))

    elif message == "left":
        Compteur_Tournant_Gauche += 1
        angle += 7.8

    elif message == "right":
        Compteur_Tournant_Droit += 1

    elif message == "back":
        Compteur_Tournant_Gauche = 0
        Compteur_Tournant_Droit = 0
        x -= distance * math.cos(math.radians(angle))
        y -= distance * math.sin(math.radians(angle))
        previous_positions.append((x, y))

    if Compteur_Tournant_Gauche == 125:
        client.publish(topic, "rbtforward")
        client.publish(topic, "rbtforward")
        client.publish(topic, "rbtforward")
        Compteur_Tournant_Gauche = 0

    if Compteur_Tournant_Droit == 140:  
        client.publish(topic, "rbtforward")
        client.publish(topic, "rbtforward")
        client.publish(topic, "rbtforward")
        Compteur_Tournant_Droit = 0
        
    canvas.delete("all")

def affichage_trajet(x, y):
    px = x * 10 + window.winfo_width() / 2
    py = y * 10 + window.winfo_height() / 2

    canvas.create_line(px - 5, py, px + 5, py, fill="red")
    canvas.create_line(px, py - 5, px, py + 5, fill="red")

def affichage_ligne(x1, y1, x2, y2):
    px1 = x1 * 10 + window.winfo_width() / 2
    py1 = y1 * 10 + window.winfo_height() / 2
    px2 = x2 * 10 + window.winfo_width() / 2
    py2 = y2 * 10 + window.winfo_height() / 2

    canvas.create_line(px1, py1, px2, py2, fill="black")

    for i in range(len(previous_positions) - 1):
        affichage_ligne(
            previous_positions[i][0] * echelle,
            previous_positions[i][1] * echelle,
            previous_positions[i + 1][0] * echelle,
            previous_positions[i + 1][1] * echelle,
        )
        affichage_trajet(previous_positions[i][0] * echelle,
                   previous_positions[i][1] * echelle)

    affichage_trajet(x * echelle, y * echelle)


def mise_a_jour_canvas():
    canvas.delete("all")
    for i in range(len(previous_positions) - 1):
        affichage_ligne(
            previous_positions[i][0] * echelle,
            previous_positions[i][1] * echelle,
            previous_positions[i + 1][0] * echelle,
            previous_positions[i + 1][1] * echelle,
        )
        affichage_trajet(previous_positions[i][0] * echelle,
                   previous_positions[i][1] * echelle)
    affichage_trajet(x * echelle, y * echelle)


def GO():
    client.publish(topic, "start")

def zoom():
    global echelle
    echelle *= 1.1
    mise_a_jour_canvas()

def dezoom():
    global echelle
    echelle *= 0.9
    mise_a_jour_canvas()

client = mqtt.Client()

client.on_message = on_message
broker_address = "10.1.4.89"
client.connect(broker_address)
topic = "CASH"
client.subscribe(topic)

start_button = tk.Button(window, text="GO !", command=GO, width=20, height=5)
start_button.pack(pady=30)
zoom_frame = tk.Frame(window)
zoom_frame.pack(pady=10)
zoom_in_button = tk.Button(zoom_frame, text="Zoom", command=zoom, width=10)
zoom_in_button.pack(side=tk.LEFT)
zoom_out_button = tk.Button(zoom_frame, text="Dézoom", command=dezoom, width=10)
zoom_out_button.pack(side=tk.LEFT)

client.loop_start()
window.mainloop()
