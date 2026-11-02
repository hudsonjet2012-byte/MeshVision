# Cardputer ADV screen mirror

Mirrors the M5Stack Cardputer ADV screen to a phone browser over Wi-Fi.

## Build on GitHub
1. Create a new GitHub repo and upload everything in this folder (keep the folder structure).
2. Open the **Actions** tab and wait for "Build firmware" to finish (or click **Run workflow**).
3. Open the finished run and download the **cardputer-firmware** artifact.

## Flash it
1. Unzip the artifact.
2. Hold **G0** on the Cardputer while switching it on, and plug in USB-C.
3. Go to https://espressif.github.io/esptool-js/ in Chrome or Edge, click Connect, choose the port.
4. Set flash address to `0x0`, choose `cardputer-mirror.bin`, click Program.
5. Power-cycle the Cardputer.

## Use it
Set your Wi-Fi name and password in `src/main.cpp` **before** pushing (or set `USE_HOTSPOT = true`
to have the Cardputer create its own network "Cardputer" / "12345678" and browse to 192.168.4.1).
The Cardputer shows its IP; open `http://<ip>` on your phone (2.4 GHz Wi-Fi only).
