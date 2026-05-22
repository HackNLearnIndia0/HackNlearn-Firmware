# HackNlearn India — Flipper Zero Custom Firmware

```
██╗  ██╗ █████╗  ██████╗██╗  ██╗███╗   ██╗██╗     ███████╗ █████╗ ██████╗ ███╗   ██╗
██║  ██║██╔══██╗██╔════╝██║ ██╔╝████╗  ██║██║     ██╔════╝██╔══██╗██╔══██╗████╗  ██║
███████║███████║██║     █████╔╝ ██╔██╗ ██║██║     █████╗  ███████║██████╔╝██╔██╗ ██║
██╔══██║██╔══██║██║     ██╔═██╗ ██║╚██╗██║██║     ██╔══╝  ██╔══██║██╔══██╗██║╚██╗██║
██║  ██║██║  ██║╚██████╗██║  ██╗██║ ╚████║███████╗███████╗██║  ██║██║  ██║██║ ╚████║
╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝
                         INDIA  —  Flipper Zero Custom Firmware
```

> **Made with ❤️ in India | Educational Use Only**

---

## 🐬 About

**HackNlearn India Firmware** is a custom Flipper Zero firmware built for the Indian hacking and security research community. Focused on **RF research, NFC tools, IR blasting, and developer utilities** — designed strictly for **educational purposes**.

---

## ✨ Features

### 📡 Sub-GHz — Enhanced

| Feature | Details |
|---------|---------|
| **TX/RX Max Power** | CC1101 PA table unlocked — `0xC0 x8` max power |
| **Region Unlock** | All frequencies enabled — 300MHz to 928MHz |
| **13 Custom Presets** | Car / Gate / Doorbell / ISM / SmartHome / FM / Narrow |
| **Replay Bypass** | Rolling code protocols (dynamic) transmittable |
| **Jamming Detection** | RSSI threshold monitoring — detects interference |
| **India Protocols** | Godrej India + Anchor India (433MHz OOK 24bit) |
| **Global Frequencies** | 20+ frequencies across all CC1101 bands |
| **Hopper Frequencies** | 315 / 433.92 / 868.35 / 915 MHz |

#### Custom Presets Available
```
TXRX_Car_433       — Car remote 433 MHz
TXRX_Car_315       — Car remote 315 MHz
TXRX_Gate_433      — Gate / Garage 433 MHz
TXRX_Gate_315      — Gate / Garage 315 MHz
TXRX_Weather_433   — Weather station 433 MHz
TXRX_Doorbell_433  — Doorbell 433 MHz
TXRX_Doorbell_868  — Doorbell 868 MHz
TXRX_ISM_433       — Industrial ISM 433 MHz
TXRX_ISM_868       — Industrial ISM 868 MHz
TXRX_ISM_915       — Industrial ISM 915 MHz
TXRX_SmartHome_868 — Smart home 868 MHz
TXRX_NarrowBand    — Noise rejection preset
TXRX_MaxPower      — Maximum power all bands
```

---

### 🔴 Infrared — Enhanced

| Feature | Details |
|---------|---------|
| **DMA Buffer 2x** | 200 → 400 — longer signals supported |
| **GPIO Max Speed** | VeryHigh speed for cleaner signal |
| **Duty Cycle 50%** | Maximum IR LED power |
| **3x Repeat** | Signal sent 3 times for stronger effective range |
| **Frequency Limits Removed** | More protocol support |

---

### 💳 NFC / RFID

| Feature | Details |
|---------|---------|
| Read / Write / Emulate | MIFARE Classic, Ultralight, DESFire |
| EMV Parser | Indian debit/credit card data parsing |
| ISO14443A/B | Full protocol support |
| RFID 125kHz | EM4100, HID, Indala, and more |

---

### ⌨️ BadUSB

| Feature | Details |
|---------|---------|
| HID Payloads | Ducky Script support |
| Keyboard Layouts | Multi-language support |
| Network Scanner | Built-in network scan payload |
| Sample Scripts | whoami, net_scan included |

---

### 🔌 GPIO

| Feature | Details |
|---------|---------|
| Full GPIO Control | All pins accessible |
| UART / SPI / I2C | Interface support |
| 5V Output | External device power |
| Logic Analyzer | Signal debugging |

---

### 🔵 Bluetooth LE

| Feature | Details |
|---------|---------|
| BLE Scanner | Nearby device enumeration |
| BadKB | Bluetooth HID attacks |
| Always On Option | Background scanning |

---

### 🌐 Supported Protocols — 54 Total

#### India Specific
```
Godrej India    — Indian lock systems 433MHz
Anchor India    — Indian lock systems 433MHz
```

#### Gate / Garage
```
KeeLoq, CAME, FAAC, Nice Flo, Hormann, BFT,
Somfy Telis/Keytis, Scher Khan, DoorHan,
Ansonic, Clemsa, Dickert MAHS, Gangqi, Hollarm,
Kinggates Stylo 4K, Marantec, Alutech AT-4N
```

#### Car / Remote
```
KIA, Star Line, Princeton, Mastercode,
Hay21, Revers RB2, Roger, Feron
```

#### Smart Home / RF
```
Intertechno V3, Legrand, Power Smart,
Holtek, Holtek HT12X, SMC5326, Dooya,
Honeywell WDB, Linear, Linear Delta3
```

#### Security
```
SecPlus V1/V2, Megacode, MagellanMagellan,
Phoenix V2, Doitrand, Bett, CAME Twee,
CAME Atomo, Nero Radio, Nero Sketch, IDO
```

#### RAW
```
RAW, Bin RAW — capture any signal
```

---

## 🚀 Getting Started

### Build from Source

```bash
# Clone the repo
git clone https://github.com/HackNlearnindia1/HackNlearn-Firmware.git
cd HackNlearn-Firmware

# Build
./fbt updater_package          # Linux/Mac
fbt.cmd updater_package        # Windows

# Flash via USB
./fbt flash_usb
```

### Flash via qFlipper

1. Download [qFlipper](https://flipperzero.one/update)
2. Connect Flipper Zero via USB
3. Click **Install from file**
4. Select `dist/f7-D/f7-update-local/update.fuf`

### SD Card Setup

Copy `setting_user` to your SD card:
```
SD Card/subghz/assets/setting_user
```

---

## 📁 Project Structure

```
HackNlearn-Firmware/
├── targets/f7/furi_hal/
│   ├── furi_hal_subghz.c      ← SubGHz TX/RX power unlock
│   └── furi_hal_infrared.c    ← IR signal boost
├── applications/main/subghz/
│   ├── helpers/subghz_txrx.c  ← Replay bypass + jamming detection
│   └── resources/subghz/assets/setting_user  ← Global frequencies
├── lib/subghz/protocols/
│   ├── godrej_india.c/.h      ← India: Godrej protocol
│   ├── anchor_india.c/.h      ← India: Anchor protocol
│   ├── protocol_items.c/.h    ← All 54 protocols registered
└── lib/infrared/signal/
    └── infrared_signal.c      ← IR 50% duty cycle + 3x repeat
```

---

## 🔧 HackNlearn Changes Summary

### `furi_hal_subghz.c`
- CC1101 PA table = `0xC0 x8` (max power) on boot
- Region lock bypass — TX always enabled
- Max power after every antenna switch

### `subghz_txrx.c`
- Replay bypass — `check_type` ignored, all protocols transmittable
- Jamming detection — RSSI > -50dBm triggers warning
- Repeat count = 10 for all signals

### `furi_hal_infrared.c`
- DMA buffer: 200 → 400
- GPIO speed: High → VeryHigh
- Frequency limits removed

### `infrared_signal.c`
- Duty cycle forced to 50%
- Send count: 1 → 3x
- Max duty cycle on transmit

### `setting_user`
- `ignore_default_tx_region: true`
- 20 global frequencies
- 13 custom TX/RX presets

---

## ⚠️ Legal Disclaimer

This firmware is intended **strictly for educational and research purposes**.

- Get **explicit permission** before testing on any device
- Follow all **local laws and regulations** (India IT Act 2000)
- Use only on devices **you own or have permission to test**
- RF transmission may be regulated in your region

**HackNlearn India team is not responsible for any misuse.**

---

## 🤝 Contributing

```bash
git checkout -b feature/YourFeature
git commit -m 'feat: Add YourFeature'
git push origin feature/YourFeature
# Open a Pull Request
```

---

## 📬 Contact

- **GitHub:** [@HackNlearnindia1](https://github.com/HackNlearnindia1)
- **YouTube:** [HackNlearn India](https://www.youtube.com/@HackNLearnIndia)
- **Live Demo:** [hacknlearnindia1.github.io](https://hacknlearnindia1.github.io/HackNlearn-Flipper/)

---

## 📜 License

GPL-3.0 License — see [LICENSE](LICENSE) for details.

---

<div align="center">

**HackNlearn India** — *Seekho. Hack Karo. Grow Karo.*

⭐ Agar pasand aaya toh star do bhai!

**Build:** `fbt.cmd updater_package` ✅ | **Device:** Flipper Zero f7

</div>
