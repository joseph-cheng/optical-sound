#include "raylib.h"
#include "tp/assert.h"
#include "tp/audio.h"
#include "tp/camera.h"
#include "tp/image.h"
#include "tp/io.h"
#include "tp/log.h"
#include "tp/math.h"
#include "tp/string.h"

#define WIDTH 600
#define HEIGHT 600

int main(void) {
  tp_allocator allocator = tp_allocator_create(
      tp_allocator_virtual_memory_alloc(1024 * 1024 * 1024));
  /*
  tp_audio audio = tp_audio_create(
      &allocator, tp_string_from_string_constant("optical-sound"), 1, 44100);

  tp_image image = tp_image_from_bmp(
      &allocator, tp_string_from_string_constant("image.bmp"));

  TP_ASSERT(image.rows == 735);

  tp_slice_i16 samples;
  i16 samples_buf[735] = {0};
  samples.data = samples_buf;
  samples.count = sizeof(samples_buf) / sizeof(samples_buf[0]);
  for (usize ii = 0; ii < samples.count; ii++) {

    usize total_luminance = 0;
    for (usize col = 0; col < image.cols; col++) {
      switch (image.format) {
      case TP_PIXEL_FORMAT_GREY: {
        total_luminance += image.data.grey.data[ii * image.cols + col];
        break;
      }
      case TP_PIXEL_FORMAT_RGB: {
        rgb px = image.data.rgb.data[ii * image.cols + col];
        total_luminance += (px.r + px.g + px.b) / 3;
        break;
      }
      }
    }

    f32 average_luminance = (f32)total_luminance / image.cols;
    i16 sample = ((average_luminance / 255.0) - 0.5) * 2.0 * TP_I16_MAX;
    samples_buf[ii] = sample;
  }

  tp_audio_write_wav(&allocator, samples,
                     tp_string_from_string_constant("out.wav"), 1, 44100);

  while (true) {
    tp_audio_write(&allocator, audio, samples);
  }
  */

  tp_camera camera = tp_camera_create(
      &allocator, tp_string_from_string_constant("/dev/video0"));

  tp_image frame = tp_camera_capture_frame(&allocator, camera);
  tp_image_to_bmp(&allocator, frame,
                  tp_string_from_string_constant("frame.bmp"));
  tp_allocator_free(&allocator, (void **)&frame.data);
  tp_camera_destroy(camera);

  return 0;
}
