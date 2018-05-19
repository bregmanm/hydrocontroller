/*
  Web Server


 */

#include <SPI.h>
#include <Ethernet.h>

#define BAUD_RATE 9600
#define INPUT_BUFFER_LENGTH 200
#define END_OF_UNIT '\n'

// RS485 params
const uint8_t     PIN_direction_TX_RX = 10;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192, 168, 0, 110);

char inputBuffer[INPUT_BUFFER_LENGTH];

enum state_t {
	method,
	path,
	protocol,
	body
};

void switchTX_HI() {
  digitalWrite(PIN_direction_TX_RX, HIGH); // transmit mode
  delay(1);
}

void switchTX_LOW() {
  digitalWrite(PIN_direction_TX_RX, LOW); // receive mode
  delay(1);
}

void transmitString(String s) {
  switchTX_HI();
  for (int i = 0; i < s.length(); i++) {
    Serial.print(s.charAt(i);
    delay(1);
  }
  Serial.print(END_OF_UNIT);
  delay(1);
  switchTX_LOW();
}

void processRequest(String requestMethod, String requestPath, String requestProtocol, String requestBody) {
          // test: send to hydrocontroller a symbol after "/" in the path and return all response
          // example: /+, /-
          String response = "No POST method";
          if ("POST".equals(requestMethod)) {
            size_t bytesReceived;
            transmitString(requestPath.substring(1));
            bytesReceived  = Serial.readBytesUntil(END_OF_UNIT, inputBuffer, INPUT_BUFFER_LENGTH);
            if (!bytesReceived) {
              response = "No responce from hydrocontroller";
            } else {
              inputBuffer[bytesReceived] = '\0';
              response = Sring(inputBuffer);
            }
          }
            

          // send a standard http response header
          client.println(requestProtocol + " 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("Response from controller is: ");
          client.print(response);
          client.println("<br>");
          client.println("</html>");
}

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Set RS485 to receive mode
  pinMode(PIN_direction_TX_RX,   OUTPUT); // D10 is output pin
  digitalWrite(PIN_direction_TX_RX, LOW); // Receive mode


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  // Serial.print("server is at ");
  // Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    state_t state = method; // start read method
    String httpMethod = string();
    String httpPath = String();
    String httpProtocol = String();
    String httpBody = String();
    // Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLineIsBlank) {
            processRequest(httpMethod, httpPath, httpProtocol, httpBody);
            break; // end of request
          } else { // start of new line in request
            currentLineIsBlank = true;
          }
        } else { // line is not blank
          currentLineIsBlank = false;
        }
        switch (state) {
          case method:
            if (isSpace(c)) {
              if (httpMethod.length() == 0) { // space before method
                break; // ignore
              } else { // space after method
                state = path;
              }
            } else {
              httpMethod += c;
            }
            break;
          case path:
            if (isSpace(c)) {
              if (httpPath.length() == 0) { // space before path
                break; // ignore
              } else { // space after path
                state = protocol;
              }
            } else {
              httpPath += c;
            }
            break;
          case protocol:
            if (isWhitespace(c)) {
              if (httpProtocol.length() == 0) { // space before protocol
                break; // ignore
              } else { // space after protocol
                state = body;
              }
            } else {
              httpProtocol += c;
            }
            break;
          case body:
            if (isWhitespace(c)) {
              if (httpBody.length() == 0) { // space before body
                break; // ignore
            } else {
              httpProtocol += c;
            }
            break;
        }
        // Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    // Serial.println("client disconnected");
  }
}
