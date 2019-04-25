void getDataFromWebsite(){
  
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  if (DEBUG) Serial.print("connecting to ");
  if (DEBUG) Serial.println(host);

  if (DEBUG) Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);

  if (!client.connect(host, httpsPort)) {
    if (DEBUG) Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    if (DEBUG) Serial.println("certificate matches");
  } else {
    if (DEBUG) Serial.println("certificate doesn't match");
  }

  if (DEBUG) Serial.print("requesting URL: ");
  if (DEBUG) Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  if (DEBUG) Serial.println("request sent");

  //output all data
  //    while (client.connected()) {
  //      Serial.write(client.read());
  //      delay(10);
  //    }
  
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      if (DEBUG) Serial.println("headers received");
      break;
    }
  }

  //delete the first line since there is alway a weird extra line after the headers
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if(line.length() > 2) {
      if (DEBUG) Serial.println("extra weird line removed");
      break;
    }
  }

  //read the actual json
  String line = client.readStringUntil('\r');
  if (line.startsWith("{\"state\":\"success\"")) {
    if (DEBUG) Serial.println("reading json successfull!");
  } else {
    if (DEBUG) Serial.println("reading json failed");
  }
  if (DEBUG) Serial.println("reply was:");
  if (DEBUG) Serial.println(line);
  if (DEBUG) Serial.println("closing connection");

  if (DEBUG) Serial.println("parsing json");
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(line);
  
  // Parameters    
  hue                   = (int)root["hue"];
  sat                   = (int)root["sat"];
  val                   = (int)root["val"];
  power                 = (int)root["power"];
  fadeout               = (int)root["fadeout"];

}
