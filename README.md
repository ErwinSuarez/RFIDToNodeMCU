# RFIDToNodeMCU
Connection of NodeMCU to RFID-RC522
This is to connect RFID-RC522 to NodeMCU with the Use of Arduino IDE
Arduino IDE is used to display the response value from server
Raspberry Pi3 is being used as a web server.

The way this work is MFRC522 reads the RFID tag, 
NodeMCU collects this data and sends this data through a web server
The web server then recieves this data and records this into servers 
database. The server should have responded to clients request and
returns a boolean value of true or false depending on the RFID tag

NOTE: THIS IS STILL IN DEVELOPMENT. goal is index.php should be used
to respond to clients request with data being passed still need to 
html script to recieve servers reposnse
