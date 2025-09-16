#include "raylib.h"
#include "tp/audio.h"
#include "tp/io.h"
#include "tp/log.h"
#include "tp/math.h"
#include "tp/string.h"

#define WIDTH 600
#define HEIGHT 600

int main() {
  tp_allocator allocator = tp_allocator_create(
      tp_allocator_virtual_memory_alloc(1024 * 1024 * 1024));
  tp_audio audio = tp_audio_create(
      &allocator, tp_string_from_string_constant("optical-sound"), 1, 44100);

  tp_slice_i16 samples;
  i16 samples_buf[44100] = {0};
  samples.data = samples_buf;
  samples.count = sizeof(samples_buf) / sizeof(samples_buf[0]);
  for (usize ii = 0; ii < samples.count; ii++) {
    samples.data[ii] = tp_math_sin_f32(ii / (2 * 3.14159265)) * 16000;
  }
  while (true) {
    for (usize ii = 0; ii < 44100; ii++) {
      tp_audio_write(&allocator, audio, samples);
    }
  }

  return 0;
}
