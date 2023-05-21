#include "PicoBuzzer.h"

SongNote::SongNote()
{
    note = NOTE_A4;
    len_us = 100 * 1000;
}
SongNote::SongNote(uint note_freq)
{
    note = note_freq;
};
SongNote::SongNote(uint note_freq, u64_t len_ms)
{
    note = note_freq;
    len_us = len_ms * 1000;
};

PicoBuzzer::PicoBuzzer()
{
    _volume = 128;
    _tone_callbackID = (int32_t)0;
    buzzerData.ptr = this;
};

PicoBuzzer::PicoBuzzer(uint8_t GPIO) : PicoBuzzer()
{
    _GPIO = GPIO;
};

PicoBuzzer::PicoBuzzer(uint8_t GPIO, alarm_pool_t *alarm_pool) : PicoBuzzer(GPIO)
{
    assign_alarm_pool(alarm_pool);
};
int PicoBuzzer::assign_GPIO(uint8_t GPIO)
{
    _GPIO = GPIO;
    return 0;
};
uint8_t PicoBuzzer::get_GPIO()
{
    return _GPIO;
};
int PicoBuzzer::assign_alarm_pool(alarm_pool_t *alarm_pool)
{
    _alarm_pool = alarm_pool;
    return 0;
};

int PicoBuzzer::set_volume(uint8_t newVolume)
{
    _volume = newVolume;
    return _volume;
};

int PicoBuzzer::set_volume(float newVolume)
{
    float multi = 1;
    if (newVolume > 1)
    {
        multi = 1;
    }
    else if (newVolume < 0)
    {
        multi = 0;
    }
    else
    {
        multi = newVolume;
    };
    return set_volume((uint8_t)(255 * multi));
};

uint8_t PicoBuzzer::get_volume()
{
    return _volume;
}

float PicoBuzzer::get_volume_pc()
{
    return (_volume / 255);
}

u64_t PicoBuzzer::play(SongNote SongNote)
{
    analogWriteFreq(SongNote.note);
    Serial1.println(_volume);
    analogWrite(_GPIO, 128);
    return 0;
};
u64_t PicoBuzzer::play(std::vector<SongNote> song)
{

    if (_tone_callbackID < 1)
    {
        buzzerData.ptr = this;
        buzzerData.current = song.begin();
        buzzerData.end = song.end();
        this->current = song.begin();
        this->end = song.end();
        _tone_callbackID = alarm_pool_add_alarm_in_ms(_alarm_pool, 0, tone_callback, &this->buzzerData, true);

        // dataStruct = toneData(BUZZER_GPIO, happy_song);

        // auto itter = dataStruct.current;
        // auto deref = *itter;
        // dataStruct.current = happy_song.begin();
        // dataStruct.end = happy_song.end();
        // tone_callbackID = alarm_pool_add_alarm_in_us(alarm_pool_primary, 0, tone_callback, &dataStruct, true);
    };
    // analogWriteFreq(SongNote.note);
    // analogWrite(_GPIO, _volume);
    return 0;
};
u64_t PicoBuzzer::play()
{

    int64_t return_us = 0;
    if (buzzerData.current != buzzerData.end)
    {
        analogWriteFreq(buzzerData.current->note);
        analogWrite(_GPIO, 128);
        return_us = buzzerData.current->len_us;
    }
    else
    {
        analogWrite(_GPIO, 0);
        alarm_pool_cancel_alarm(_alarm_pool, _tone_callbackID);
        _tone_callbackID = 0;
    }

    buzzerData.current++;
    return return_us;
};

int64_t PicoBuzzer::tone_callback(alarm_id_t id, void *user_data)
{
    int64_t time = ((BuzzerCallbackData *)user_data)->ptr->play();
    return (time);
};
