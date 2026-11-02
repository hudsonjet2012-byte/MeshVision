#include <Arduino.h>
#include <M5Cardputer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

// Edit these, or leave as-is to use the Cardputer's own hotspot
const char* SSID = "YOUR_WIFI";
const char* PASS = "YOUR_PASSWORD";
const bool USE_HOTSPOT = false;  // true = join "Cardputer" / "12345678", browse 192.168.4.1

const int W = 240, H = 135;

WebServer http(80);
WebSocketsServer ws(81);
M5Canvas canvas(&M5Cardputer.Display);

const char PAGE[] PROGMEM = R"(<!doctype html>
<meta name=viewport content="width=device-width,initial-scale=1">
<body style="margin:0;background:#000">
<canvas id=c width=240 height=135 style="width:100%;image-rendering:pixelated"></canvas>
<script>
const c=document.getElementById('c'),x=c.getContext('2d');
const img=x.createImageData(240,135);
const s=new WebSocket('ws://'+location.hostname+':81');
s.binaryType='arraybuffer';
s.onmessage=e=>{
  const v=new DataView(e.data),d=img.data;
  for(let i=0,j=0;i<32400;i++,j+=4){
    const p=v.getUint16(i*2,false);
    d[j]=(p>>11)*255/31;
    d[j+1]=((p>>5)&63)*255/63;
    d[j+2]=(p&31)*255/31;
    d[j+3]=255;
  }
  x.putImageData(img,0,0);
};
</script>)";

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);
  canvas.createSprite(W, H);
  canvas.setTextSize(2);

  if (USE_HOTSPOT) {
    WiFi.softAP("Cardputer", "12345678");
    canvas.println("Join: Cardputer");
    canvas.println(WiFi.softAPIP());
  } else {
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) delay(300);
    canvas.println(WiFi.localIP());
  }
  canvas.pushSprite(0, 0);

  http.on("/", []() { http.send_P(200, "text/html", PAGE); });
  http.begin();
  ws.begin();
  delay(8000);
}

void loop() {
  M5Cardputer.update();
  http.handleClient();
  ws.loop();

  // ---- your drawing goes here (draw to canvas, not Display) ----
  canvas.fillScreen(TFT_BLACK);
  int t = millis() / 10;
  canvas.fillCircle(120 + 80 * sin(t * 0.05), 67 + 40 * cos(t * 0.07), 15, TFT_GREEN);
  // --------------------------------------------------------------

  canvas.pushSprite(0, 0);
  if (ws.connectedClients() > 0)
    ws.broadcastBIN((uint8_t*)canvas.getBuffer(), W * H * 2);
}
