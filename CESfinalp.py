import spotipy, spotipy.util
from signal import pause
import socket
UDP_IP = "192.168.1.172" # The IP that is printed in the serial monitor from the ESP32
SHARED_UDP_PORT = 4210
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
sock.connect((UDP_IP, SHARED_UDP_PORT))
# Credentials
client_id = "dcb1989bb07046c6b6415db05c378309"
client_secret = "696e193110a044eb8ad572756ba9afa9"
username = "n7y40usd3a4f85rxptavdxl91"


def login_to_spotify():
	print("Logging in to Spotify...")

	token = spotipy.util.prompt_for_user_token(
		username = username,
		scope = "user-read-currently-playing",
		redirect_uri = "http://localhost:8888/callback",
		client_id = client_id,
		client_secret = client_secret
	)

	spotify = spotipy.Spotify(auth = token)

	user = spotify.current_user()
	print("Logged in as " + user["id"])

	return spotify

def get_song_name():
	current_track = spotifyy.current_user_playing_track()
	stuff = current_track['item']
	title_of_song = stuff['name']
	print(title_of_song)
	sock.send(title_of_song.encode())



spotifyy = login_to_spotify()
def loop():
	get_song_name()
	while True:
		data = sock.recv(2048)
		print(data)

if __name__ == "__main__":
	sock.send('Hello ESP32'.encode())
	loop()
