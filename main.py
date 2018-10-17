from flask import Flask, request, render_template
from flask_socketio import SocketIO, send, emit

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

@app.route('/id/<id_card>')
def card(id_card):
    socketio.emit('s2c',id_card, broadcast=True)
    print("Card ID : ",id_card)
    return id_card

@app.route('/card')
def page():
    return render_template('card.html')