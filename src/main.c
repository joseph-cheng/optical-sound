#include "tp/assert.h"
#include "tp/audio.h"
#include "tp/camera.h"
#include "tp/image.h"
#include "tp/io.h"
#include "tp/log.h"
#include "tp/math.h"
#include "tp/string.h"
#include "tp/time.h"
#include <emmintrin.h>

int main(void) {
  tp_allocator allocator = tp_allocator_create(
      tp_allocator_virtual_memory_alloc(1024 * 1024 * 1024));
  /*
  tp_audio audio = tp_audio_create( &allocator,
  tp_string_from_string_constant("optical-sound"), 1, 44100);

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
  tp_audio audio = tp_audio_create(
      &allocator, tp_string_from_string_constant("optical-sound"), 1, 44100);
  TP_ASSERT(camera.pixel_format == TP_PIXEL_FORMAT_YUYV);

  while (true) {
    tp_allocator_wipe(&allocator);
    tp_image frame = tp_camera_capture_frame(&allocator, camera);
    f64 start = tp_time_get_time_f64();
    TP_ASSERT(frame.rows >= 735);
    TP_ASSERT(frame.cols == 960);
    TP_ASSERT(frame.format == TP_PIXEL_FORMAT_YUYV);

    tp_slice_i16 samples;
    // i16 samples_buf[735 * 2] = {0};
    i16 samples_buf[735] = {0};
    samples.data = samples_buf;
    samples.count = sizeof(samples_buf) / sizeof(samples_buf[0]);
    for (usize ii = 0; ii < 735; ii++) {

      usize total_luminance = 0;

      /*
      const u8 *bytes = (const u8 *)&frame.data.yuyv.data[ii * frame.cols];
      usize total_bytes = frame.cols * sizeof(yuyv);

      __m128i sum = _mm_setzero_si128();
      __m128i mask = _mm_set1_epi16((short)0x00ff);

      for (usize jj = 0; jj + 16 <= total_bytes; jj += 16) {
        __m128i v = _mm_loadu_si128((const __m128i *)(bytes + jj));
        __m128i luminance_bytes = _mm_and_si128(v, mask);
        sum = _mm_add_epi16(sum, luminance_bytes);
      }

      u16 partial[8];
      _mm_storeu_si128((__m128i *)partial, sum);

      for (int kk = 0; kk < 8; kk++) {
        total_luminance += partial[kk];
      }
      */

      for (usize col = 0; col < frame.cols; col++) {
        yuyv px = frame.data.yuyv.data[ii * frame.cols + col];
        total_luminance += px.y1 + px.y2;
      }

      f32 average_luminance = (f32)total_luminance / frame.cols / 2.0f;
      i16 sample = ((average_luminance / 255.0) - 0.5) * 2.0 * TP_I16_MAX;
      samples_buf[ii] = sample;
      /*
      samples_buf[ii * 2] = sample;
      if (ii > 0) {
        samples_buf[ii * 2 - 1] =
            (samples_buf[ii * 2 - 2] + samples_buf[ii * 2]) / 2;
      }
      */
    }
    // samples_buf[samples.count - 1] = samples_buf[samples.count - 2];
    tp_audio_write(&allocator, audio, samples);
    f64 end = tp_time_get_time_f64();
    tp_log_f32(end - start);
  }
  tp_camera_destroy(camera);

  return 0;
}
