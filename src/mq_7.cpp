#include "mq_7.h"
#include "pin.h"


// --- Tabel Konversi ADC ke PPM (tidak berubah) ---
const int adc_table[] = {70, 72, 75, 77, 78, 90, 100, 105, 110, 115, 120, 145, 170, 190, 215, 235, 250, 290, 330, 370, 400};
const int ppm_table[] = {0 , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 45, 50, 60, 70, 80, 90, 100};
const int table_size = sizeof(adc_table) / sizeof(adc_table[0]);

// --- Variabel Global (dapat diakses dari file lain) ---
float ppm_mq7 = 0;
int adc_mq7 = 0;

// --- Variabel Internal untuk State Machine (Non-Blocking) ---
// Konfigurasi pembacaan
const long reading_interval = 5000;      // Mulai siklus pembacaan baru setiap 5 detik
const long sample_interval = 33;         // Interval antar sampel (sekitar 30 sampel/detik)
const int num_samples_per_cycle = 150;   // Total sampel per siklus (30 sampel/detik * 5 detik)

// Variabel state
static unsigned long last_reading_start_time = 0;
static unsigned long last_sample_time = 0;
static long total_adc_for_cycle = 0;
static int sample_count = 0;
static bool reading_in_progress = false;


void setup_mq7() {
  pinMode(MQ_PIN, INPUT);
  // Memberi nilai awal pada last_reading_start_time agar pembacaan pertama tidak langsung dimulai
  last_reading_start_time = millis(); 
}

float convertADCtoPPM(int adc) {
  if (adc <= adc_table[0]) return ppm_table[0];
  if (adc >= adc_table[table_size - 1]) return ppm_table[table_size - 1];
  for (int i = 1; i < table_size; i++) {
    if (adc <= adc_table[i]) {
      int x0 = adc_table[i - 1];
      int x1 = adc_table[i];
      int y0 = ppm_table[i - 1];
      int y1 = ppm_table[i];
      return y0 + (float)(adc - x0) * (y1 - y0) / (x1 - x0);
    }
  }
  return -1; // Seharusnya tidak pernah terjadi
}

void loop_mq7() {
  unsigned long current_millis = millis();

  if (!reading_in_progress && (current_millis - last_reading_start_time >= reading_interval)) {
    reading_in_progress = true;
    sample_count = 0;
    total_adc_for_cycle = 0;
    last_sample_time = current_millis; // Mulai sampling sekarang
    last_reading_start_time = current_millis;
  }

  // 2. Jika siklus pembacaan sedang berjalan, ambil SAMPEL secara berkala
  if (reading_in_progress) {
    // Cek apakah sudah waktunya mengambil sampel berikutnya
    if (current_millis - last_sample_time >= sample_interval) {
      last_sample_time = current_millis;

      total_adc_for_cycle += analogRead(MQ_PIN);
      sample_count++;

      // 3. Cek apakah semua sampel dalam siklus ini sudah terkumpul
      if (sample_count >= num_samples_per_cycle) {
        // Hitung rata-rata ADC
        float final_avg = (float)total_adc_for_cycle / num_samples_per_cycle;
        adc_mq7 = (int)final_avg;
        
        // Konversi ke PPM
        ppm_mq7 = convertADCtoPPM(adc_mq7);

        // Selesai, reset state untuk menunggu siklus berikutnya
        reading_in_progress = false; 
      }
    }
  }
}
