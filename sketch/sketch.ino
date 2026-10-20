#include <Wire.h>
#include <SPI.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// ---------------- PINS (NORVI XN-CPU-ESPS3) ----------------
#define SDA_PIN        8
#define SCL_PIN        9
#define MISO_PIN       13
#define MOSI_PIN       11
#define SCLK_PIN       12
#define ETH_CS         1
#define DSP_CS         45
#define SD_CS          42
#define PORT_RST       38

// ---------------- LOVYANGFX SETUP ----------------
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance;

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = 12;
      cfg.pin_mosi = 11;
      cfg.pin_miso = 13;
      cfg.pin_dc = 46;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = 45;
      cfg.pin_rst = 47;
      cfg.pin_busy = -1;
      cfg.panel_width = 240;
      cfg.panel_height = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel_instance.config(cfg);
    }
    setPanel(&_panel_instance);
  }
};

LGFX tft;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- NORVI XN: display test ---");

  // Wake up the I/O expanders (same as reference sketch)
  pinMode(PORT_RST, OUTPUT);
  digitalWrite(PORT_RST, HIGH);
  delay(100);

  Wire.begin(SDA_PIN, SCL_PIN);
  SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN);
  delay(100);

  // Deselect other SPI slaves sharing the bus with the display
  pinMode(ETH_CS, OUTPUT); digitalWrite(ETH_CS, HIGH);
  pinMode(SD_CS, OUTPUT);  digitalWrite(SD_CS, HIGH);
  pinMode(DSP_CS, OUTPUT); digitalWrite(DSP_CS, HIGH);

  tft.init();
  // No setRotation() call: default rotation 0 (portrait 240x320),
  // same orientation as the reference sketch.
  tft.fillScreen(TFT_BLACK);

  tft.setFont(&fonts::FreeSansBold12pt7b);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextDatum(middle_center);
  tft.drawString("norvi", tft.width() / 2, tft.height() / 2 - 15);
  tft.drawString("controllers", tft.width() / 2, tft.height() / 2 + 15);

  Serial.println("Displayed: norvi controllers");
}

void loop() {
  delay(1000);
}
