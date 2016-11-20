#ifndef PERIMETER_RECEIVER_H
#define PERIMETER_RECEIVER_H

#include <stdint.h>

#define PR_SENDER_RATE 9615

// #define PR_SAMPLE_RATE 9615
// #define PR_SAMPLE_RATE 19231
#define PR_SAMPLE_RATE 38462

#define PR_SUBSAMPLES (PR_SAMPLE_RATE / PR_SENDER_RATE)
#define PR_INPUT_BUFFER_SIZE (sizeof(sigcode) * 2 * PR_SUBSAMPLES)

const signed char sigcode[] = { 1,0,-1, 0,1,-1,1,-1, 0,1,-1,1,0,-1, 0,1,-1, 0,1,-1, 0,1,0,-1 };

class PerimeterReceiver
{
public:
    PerimeterReceiver();

    void calculate(const int16_t* input_array);
    void setOffset(uint16_t offset);
    void calculateOffset(const int16_t* input_array);
    void setInvertSignal(bool is_invert);

    int16_t  getMagnitude() const;
    int16_t  getSmoothMagnitude() const;
    bool     isInside() const;
    bool     isSenderEnable() const;
    uint16_t getIntersections() const;
    bool     isInvertSignal() const;

private:
    int32_t correlationFilter(  const int16_t* input_array,
                                uint16_t input_array_size,
                                const int8_t*  sigcode,
                                int16_t  sigcode_size,
                                int8_t   n_subsamples);

private:
    int16_t  offset_;
    int16_t  magnitude_;
    int16_t  smooth_magnitude_;
    bool     is_inside_;
    bool     is_sender_enable_;
    uint16_t intersections_;
    bool     is_invert_signal_;

};

#endif
