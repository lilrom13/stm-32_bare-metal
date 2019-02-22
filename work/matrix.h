#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_color;

void matrix_init(void);
void pulse_SCK(void);
void pulse_LAT(void);
void deactivate_rows(void);
void activate_row(int);
void send_byte(uint8_t, int);
void mat_set_row(int, const rgb_color *);
void init_bank0();l

#endif // MATRIX_H_