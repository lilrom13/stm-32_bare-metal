#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_color;

void matrix_init(void);
void test_pixels();
void display_image_loop(rgb_color *image);

#endif // MATRIX_H_