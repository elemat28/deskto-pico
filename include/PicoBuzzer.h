#ifndef PICOBUZZ_H
#define PICOBUZZ_H
#include <Arduino.h>
#include <stdlib.h>
#include <vector>
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208

#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415

#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831

#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661

#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
struct SongNote
{
    uint note;
    u64_t len_us;
    SongNote();
    SongNote(uint note_freq);
    SongNote(uint note_freq, u64_t len_ms);
};
class PicoBuzzer
{
public:
    PicoBuzzer();
    PicoBuzzer(uint8_t GPIO);
    PicoBuzzer(uint8_t GPIO, alarm_pool_t *alarm_pool);
    int assign_GPIO(uint8_t GPIO);
    uint8_t get_GPIO();
    int assign_alarm_pool(alarm_pool_t *alarm_pool);
    int set_volume(float newVolume);
    int set_volume(uint8_t newVolume);
    uint8_t get_volume();
    float get_volume_pc();
    u64_t play(SongNote SongNote);
    u64_t play(std::vector<SongNote> song);
    u64_t play();
    static int64_t tone_callback(alarm_id_t id, void *user_data);

protected:
    struct BuzzerCallbackData
    {
        PicoBuzzer *ptr;
        std::vector<SongNote>::iterator current;
        std::vector<SongNote>::iterator end;
    };

private:
    uint8_t _GPIO;
    uint8_t _volume;
    BuzzerCallbackData buzzerData;
    alarm_pool_t *_alarm_pool;
    int32_t _tone_callbackID;
    SongNote _currentNote;
    std::vector<SongNote>::iterator current;
    std::vector<SongNote>::iterator end;
};

#endif