#include <dht11.h>
#include <ESP8266WiFi.h>

/************ DHT11 Sensor  ************/
#define DHT11PIN 5
dht11 DHT11;
int deg = 0;

/************ Acces point setup  ************/
const char* ssid = "yourSSID";
const char* password = "yourPassword";
WiFiServer server(80);
String request = "";
bool state = false;     // On board LED state

/************ HTML + CSS + JS  ************/
const char header[]  = {"<!doctype html>\n<html lang=\"en\">\n  <head>\n    <!-- Required meta tags -->\n    <meta charset=\"utf-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n    <!-- CSS only -->\n    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css\" integrity=\"sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z\" crossorigin=\"anonymous\">      \n\n       <title>Weather Station</title>\n  </head>"};
const char navigation[]  = {"  <body class=\"bg-dark\">\n\n    <div class=\"container\">\n        <!--Navigation-->\n        <h1 class=\"text-center text-light p-5\">Weather Station Test Project</h1>\n        <div class=\"row justify-content-center\">\n            <div class=\"col-md-4  m-2 text-center \"><button id=\"index\" class=\"btn btn-info btn-block\">Main Page</button></div>\n            <div class=\"col-md-4 m-2 text-center \"><button id=\"config\" class=\"btn btn-info btn-block\">Configuration</button></div>            \n        <!--/Navigation-->\n        <!--Content-->\n            <div id=\"content\" class=\"container m-5\">"};
const char index_content[]  = {" <div class=\"card-deck\">\n                      <div class=\"card\">                   \n                        <div class=\"card-body p-5 \">\n                          <h5 id=\"tempTitle\" class=\"card-title text-center\">Temperature</h5>\n                          <h5 class=\"card-title text-center my-5\"><i class=\"fa fa-thermometer-three-quarters fa-5x text-danger\"></i></h5>                  \n                          <p class=\"card-text text-center\"><span id=\"temperature\" class=\"h1\">30</span><span id=\"metric\" class=\"h1\">&#176;C</span></p>\n                        </div>\n                      </div>\n                      <div class=\"card\">\n                        <div class=\"card-body p-5\">\n                          <h5 id=\"humTitle\" class=\"card-title text-center\">Humidity</h5>\n                          <h5 class=\"card-title text-center my-5\"><i class=\"fa fa-tint fa-5x text-info\"></i></h5> \n                          <p class=\"card-text text-center\"><span id=\"humidity\" class=\"h1\">30</span><span class=\"h1\">%</span></p>\n                        </div>\n                      </div>\n                    </div>"};
const char config_content[] = {"<div class=\"card-deck\">\n                    <div class=\"card\">                   \n                      <div class=\"card-body p-5\">\n                        <h5 id=\"langTitle\" class=\"card-title text-center\">Language</h5>\n                        <div class=\"row justify-content-center m-4\">\n                          <button id=\"hun\" class=\"btn btn-info btn-block\">Hungarian</button><button id=\"eng\" class=\"btn btn-info btn-block\">English</button>\n                        </div>                    \n                      </div>\n                    </div>\n                    <div class=\"card\">\n                      <div class=\"card-body p-5\">\n                        <h5 id=\"metricTitle\" class=\"card-title text-center\">Metric</h5>\n                        <div class=\"row justify-content-center m-4\">\n                          <button id=\"celsius\"class=\"btn btn-info btn-block\">Celsius</button><button id=\"fahrenheit\" class=\"btn btn-info btn-block\">Fahrenheit</button>\n                        </div>\n                      </div>\n                    </div>\n                  </div>"};
const char footer[] = {"</div>\n        <!--/Content-->    \n        </div>\n        <button id=\"btn\" class=\"btn\">LED</button>\n    </div>\n    <div class=\"footer bg-light fixed-bottom\">\n        <p class=\"text-center m-1\">Weather Station for educational purposes</p>\n    </div>\n  </body>"};
const char script[] = {"<script src=\"https://use.fontawesome.com/86365c243a.js\"></script>\n  <script>\n\n      const content = document.querySelector(\"#content\");\n      let tempData = 0;\n      let humData = 0;\n      let state = \"index\";\n      let metric_state = \"fahrenheit\";\n      let language = false;\n\n    document.querySelector('#btn').addEventListener('click',loadContent);\n    document.querySelector('#config').addEventListener('click',()=>{\n      state = \"config\";\n      loadContent(state);\n    });\n\n    document.querySelector('#index').addEventListener('click',()=>{\n      state = \"index\";\n      loadContent(state);\n      getData(); \n    });\n\n    getData(); \n      setInterval(()=>{\n        if(state == \"index\"){\n          getData();\n        }\n      },2000);\n\n    function getData(){\n        fetch('http://192.168.0.229/getData')\n        .then(res => res.json())\n        .then(data =>{\n          tempData = data.temperature;\n          humData = data.humidity;\n\n            if(metric_state == \"fahrenheit\"){\n              temperature.textContent = tempData * (9/5) +32;\n              document.querySelector('#metric').textContent = \"\\u2109\";\n            }else{\n              document.querySelector('#metric').textContent = \"\\u2103\";\n              document.querySelector('#temperature').textContent = tempData;   \n            }\n      \n            document.querySelector('#humidity').textContent = humData;\n        })\n        .catch(err => console.log(err));     \n    }\n    function loadContent(state){     \n        fetch('http://192.168.0.229/'+state)\n        .then(res => res.text())\n        .then(data => {\n          content.innerHTML = data;\n          if(state == \"config\"){\n            document.querySelector('#celsius').addEventListener('click',()=>{\n            metric_state = \"celsius\";\n            });\n            document.querySelector('#fahrenheit').addEventListener('click',()=>{\n              metric_state = \"fahrenheit\";\n            });\n            document.querySelector('#hun').addEventListener('click',()=>{\n              language = true;\n              setLanguage(language);\n            });\n            document.querySelector('#eng').addEventListener('click',()=>{\n              language = false;\n              setLanguage(language);\n            });\n\n          }\n          setLanguage(language); \n    })\n  }\n    function setLanguage(language){\n        if(language){\n            document.querySelector('#index').textContent = \"Főoldal\";\n            document.querySelector('#config').textContent = \"Beállítások\";\n          if(state == \"index\"){\n            document.querySelector('#tempTitle').textContent = \"Hőmérséklet\";\n            document.querySelector('#humTitle').textContent = \"Páratartalom\";\n          }else{\n            document.querySelector('#langTitle').textContent = \"Nyelv\";\n            document.querySelector('#metricTitle').textContent = \"Mértékegység\";\n            document.querySelector('#hun').textContent = \"Magyar\";\n            document.querySelector('#eng').textContent = \"Angol\";\n          }    \n        }else{\n          document.querySelector('#langTitle').textContent = \"Language\";\n            document.querySelector('#metricTitle').textContent = \"Metric\";\n            document.querySelector('#hun').textContent = \"Hungarian\";\n            document.querySelector('#eng').textContent = \"English\";\n        }\n    }\n  </script>\n</html>"};

void setup() {

  Serial.begin(115200);
  delay(10);
/************ Connect to WiFi network  ************/
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

/************ Start Server  ************/
  server.begin();
  Serial.println("Server started");

/************ Print IP  ************/
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

 /************ On board LED config  ************/ 
  pinMode(2, OUTPUT);
}

 /************ On board LED blink  ************/ 
void toggleLED() {
  digitalWrite(2, state);
  state = !state;
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
/************ Client connected  ************/ 
  request = client.readStringUntil('\r');       //Read client request

/************ Service Normal and Ajax request  ************/ 
  if ( request.indexOf("getData") > 0 )  {      //Provide temperature and humidity data
    digitalWrite(2, state);
    state = !state;
    /************ HTML header  ************/ 
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain; charset=\"utf-8\"");
    client.println("Access-Control-Allow-Origin: \*");
    client.println("Allow: GET, HEAD");
    client.println(""); // do not forget this one
    /************ read sensor data  ************/ 
    int chk = DHT11.read(DHT11PIN);
    float temperature = DHT11.temperature;
    float humidity = DHT11.humidity;
    /************ HTML body, send JSON data  ************/
    client.println("{\"temperature\" : \"" + (String)temperature + "\", \"humidity\" : \"" + (String)humidity + "\"}");
    state = !state;

  } else if (request.indexOf("index") > 0) {        //Service ajax request for loading main content
    /************ HTML header  ************/
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=\"utf-8\"");
    client.println("Access-Control-Allow-Origin: \*");
    client.println("Allow: GET, HEAD");
    client.println(""); // do not forget this one
     /************ HTML body index ************/
    client.println(index_content);

  } else if (request.indexOf("config") > 0) {       //Service ajax request for loading config content
    digitalWrite(2, state);
    state = !state;
    /************ HTML header  ************/
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=\"utf-8\"");
    client.println("Access-Control-Allow-Origin: \*");
    client.println("Allow: GET, HEAD");
    client.println(""); // do not forget this one
    /************ HTML body cofig ************/

    client.println(config_content);
  } else {                                          //Load main page after connection
    /************ HTML header  ************/
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); // do not forget this one
    /************ HTML body cofig ************/
    client.println(header);
    client.println(navigation);
    client.println(index_content);
    client.println(footer);
    client.println(script);
    }

    /************ Client disconnect  ************/
    Serial.println("Client disonnected");
    Serial.println("");
  

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  client.flush();
}