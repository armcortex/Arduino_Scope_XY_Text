// Debugger tools ref: https://www.codeproject.com/Articles/5160447/Creating-and-Debugging-Arduino-Programs-in-Visua-2
// #include <app_api.h>
// #include <avr_debugger.h>
// #include <avr8-stub.h>

// Reference from: https://github.com/GadgetReboot/Arduino/blob/master/Uno/Oscilloscope_XY_PWM_Tutorial/scope_XY_Test.ino

#define TEXT1            ("YEAH>///<")
#define TEXT2            ("HELL")

#define X_PIN             (6)
#define Y_PIN             (5)
#define CHAR_CNT          (30)
#define CHAR_PTS_MAX      (12)
#define WORD_CNT          (10)
#define BIG_SYMBOL        (27)
#define SMALL_SYMBOL      (28)
#define SLASH_SYMBOL      (29)   

#define TRACE_DELAY	      (5000)  // trace delay in uS. making this longer will
                                  // result in a straighter drawing, but slower
                                  // refresh rate. making it too short will result
                                  // in an angular blob.

typedef unsigned char   u8;

typedef enum ROW_e {
    ROW_0 = 0,
    ROW_1 = 1
}ROW_t;

typedef struct _pts_t {
    u8 x;
    u8 y;
} pts_t;

typedef struct _word_t{
    pts_t s[CHAR_PTS_MAX];
    u8 len;
} word_t;

const word_t dict[CHAR_CNT] = {
    // 0. None
    {
        .s = {{0, 0}},
        .len = 1,
    },

    // 1. A
    {
        .s = {{0, 0}, {0, 3}, {2, 3}, {2, 0}, {2, 2}, {0, 2}, {0, 0}}, 
        .len = 7,
    },

    // 2. B
    {
        .s = {{0, 0}, {2, 0}, {2, 3}, {0, 3}, {1, 3}, {1, 0}, {1, 2}, {2, 2}, {2, 0}, {0, 0}},
        .len = 10,
    },

    // 3. C
    {
        .s = {{0, 0}, {2, 0}, {0, 0}, {0, 3}, {2, 3}, {0, 3}, {0, 0}},
        .len = 7,
    },

    // 4. D
    {
        .s = {{0, 0}, {2, 0}, {2, 3}, {0, 3}, {1, 3}, {1, 0}, {0, 0}},
        .len = 7,
    },

    // 5. E
    {
        .s = {{0, 0}, {2, 0}, {0, 0}, {0, 2}, {2 ,2}, {0, 2}, {0, 3}, {2, 3}, {0, 3}, {0, 0}},
        .len = 10,
    },

    // 6. F
    {
        .s = {{0, 0}, {0, 3}, {2, 3}, {0, 3}, {0, 2}, {2, 2}, {0, 2}, {0, 0}},
        .len = 8,
    },

    // 7. G
    {
        .s = {{0, 0}, {2, 0}, {2, 1}, {1, 1}, {2, 1}, {2, 0}, {0, 0}, {0, 3}, {2, 3}, {0, 3}, {0, 0}},
        .len = 11,
    },

    // 8. H
    {
        .s = {{0, 0}, {0, 3}, {0, 2}, {2, 2}, {2, 3}, {2, 0}, {2, 2}, {0, 2}, {0, 0}},
        .len = 9,
    },

    // 9. I
    {
        .s = {{0, 0}, {2, 0}, {1, 0}, {1, 3}, {0, 3}, {2, 3}, {1, 3}, {1, 0}, {0, 0}},
        .len = 9,    
    },

    // 10. J
    {
        .s = {{0, 0}, {0, 1}, {0, 0}, {2, 0}, {2, 3}, {2, 0}, {0, 0}},
        .len = 7,    
    },

    // 11. K
    {
        .s = {{0, 0}, {0, 3}, {0, 1}, {2, 3}, {0, 1}, {2, 0}, {0, 1}, {0, 0}},
        .len = 8,    
    },

    // 12. L
    {
        .s = {{0, 0}, {2, 0}, {0, 0}, {0, 3}, {0, 0}},
        .len = 5,    
    },

    // 13. M
    {
        .s = {{0, 0}, {0, 3}, {1, 2}, {2, 3}, {2, 0}, {2, 3}, {1, 2}, {0, 3}, {0, 0}},
        .len = 9,
    },

    // 14. N
    {
        .s = {{0, 0}, {0, 3}, {2, 0}, {2, 3}, {2, 0}, {0, 3}, {0, 0}},
        .len = 7,    
    },

    // 15. O
    {
        .s = {{0, 0}, {2, 0}, {2, 3}, {0, 3}, {0, 0}},
        .len = 5,
    },

    // 16. P
    {
        .s = {{0, 0}, {0, 3}, {2, 3}, {2, 2}, {0, 2}, {2, 2}, {2, 3}, {0, 3}, {0, 0}},
        .len = 9,
    },

    // 17. Q
    {
        .s = {{0, 0}, {0, 3}, {2, 3}, {2, 0}, {1, 1}, {2, 0}, {0, 0}},
        .len = 7,
    },

    // 18. R
    {
        .s = {{0, 0}, {0, 3}, {2, 3}, {2, 2}, {0, 2}, {1, 2}, {2, 0}, {1, 2}, {0, 2}, {0, 0}},
        .len = 10,
    },

    // 19. S
    {
        .s = {{0, 0}, {2, 0}, {2, 2}, {0, 2}, {0, 3}, {2, 3}, {0, 3}, {0, 2}, {2, 2}, {2, 0}, {0, 0}},
        .len = 11,
    },

    // 20. T
    {
        .s = {{0, 0}, {1, 0}, {1, 3}, {0, 3}, {2, 3}, {1, 3}, {1, 0}, {0, 0}},
        .len = 8,
    },

    // 21. U
    {
        .s = {{0, 0}, {0, 3}, {0, 0}, {2, 0}, {2, 3}, {2, 0}, {0, 0}},
        .len = 7,
    },

    // 22. V
    {
        .s = {{0, 0}, {1, 0}, {0, 3}, {1, 0}, {2, 3}, {1, 0}, {0, 0}},
        .len = 7,
    },

    // 23. W
    {
        .s = {{0, 0}, {0, 3}, {0, 0}, {1, 1}, {2, 0}, {2, 3}, {2, 0}, {1, 1}, {0, 0}},
        .len = 9,
    },

    // 24. X
    {
        .s = {{0, 0}, {2, 3}, {1, 2}, {0, 3}, {2, 0}, {1, 2}, {0, 0}},
        .len = 7,
    },

    // 25. Y
    {
        .s = {{0, 0}, {1, 0}, {1, 2}, {0, 3}, {1, 2}, {2, 3}, {1, 2}, {1, 0}, {0, 0}},
        .len = 9    
    },

    // 26. Z
    {
        .s = {{0, 0}, {2, 0}, {0, 0}, {2, 3}, {0, 3}, {2, 3}, {0, 0}},
        .len = 7,    
    },
    
    // 27. >
    {
        .s = {{0, 0}, {2, 1}, {0, 3}, {2, 1}, {0, 0}},
        .len = 5,
    },

    // 28. <
    {
        .s = {{0, 0}, {2, 0}, {0, 1}, {2, 3}, {0, 1}, {2, 0}, {0, 0}},
        .len = 7,
    },

    // 29. /
    {
        .s = {{0, 0}, {2, 3}, {0, 0}},
        .len = 3,
    }
};


typedef struct _sentence   sentence_t;

struct _sentence
{
    const char *s;
    const pts_t offset;
    const pts_t ratio;
    const pts_t org_size;
    ROW_t row_pos;
    u8 len;
    u8 idxs[WORD_CNT];
    word_t words[WORD_CNT+2];
    void (*init)(sentence_t *h);
    void (*copy)(sentence_t *h);
    void (*resize)(sentence_t *h);
    void (*reposition)(sentence_t *h);
    void (*newline)(sentence_t *h);
};

void str2idxs(const char *s, u8 *dst_idxs, u8 len)
{
    for (u8 i=0; i<len; i++)
    {
        if (isalpha(s[i]))
        {
            if (isupper(s[i]))
            {
                dst_idxs[i] = s[i] - 'A' + 1;
            }
            else
            {
                dst_idxs[i] = s[i] - 'a' + 1;
            }
        }
        else
        {
            if ('>' == s[i])
            {
                dst_idxs[i] = BIG_SYMBOL;
            }
            else if ('<' == s[i])
            {
                dst_idxs[i] = SMALL_SYMBOL;
            }
            else if ('/' == s[i])
            {
                dst_idxs[i] = SLASH_SYMBOL;
            }
            else
            {
                dst_idxs[i] = 0;
            }
        }
    }
}

void init(sentence_t *h)
{
    h->len = strlen(h->s);
    str2idxs(h->s, h->idxs, h->len);
    h->copy(h);
}

void copy(sentence_t *h)
{
    for (u8 c=0; c<h->len; c++)
    {
        u8 idx = h->idxs[c];
        u8 p_len = dict[idx].len;
        for (u8 p=0; p<p_len; p++)
        {
            h->words[c].s[p].x = dict[idx].s[p].x;
            h->words[c].s[p].y = dict[idx].s[p].y;
            h->words[c].len    = dict[idx].len;
        }
    } 
}

void resize(sentence_t *h)
{
    for (u8 c=0; c<h->len; c++)
    {
        u8 p_max = h->words[c].len;
        for (u8 p=0; p<p_max; p++)
        {
            h->words[c].s[p].x *= h->ratio.x;
            h->words[c].s[p].y *= h->ratio.y;
        }
    }
}

void reposition(sentence_t *h)
{
    u8 x_offset = (h->org_size.x * h->ratio.x) + h->offset.x;
    u8 y_offset = (h->org_size.y * h->ratio.y) + h->offset.y;

    for (u8 c=0; c<h->len; c++)
    {
        u8 p_max = h->words[c].len;
        for (u8 p=0; p<p_max; p++)
        {
            h->words[c].s[p].x += (c * x_offset);

            if (ROW_1 == h->row_pos)
            {
                h->words[c].s[p].y += y_offset;
            }
        }
    }
}

void newline(sentence_t *h)
{
    u8 c = ++(h->len) - 1;

    if (ROW_0 == h->row_pos)
    {
        h->words[c].s[0].x = 0;
        h->words[c].s[0].y = 0;
        h->words[c].len    = 1;
    }
    else
    {
        h->words[c].s[0].x = 0;
        h->words[c].s[0].y = (h->org_size.y * h->ratio.y) + h->offset.y;

        h->words[c].s[1].x = 0;
        h->words[c].s[1].y = 0;
        h->words[c].len    = 2;
    }
}

void show_one_word(word_t *words, u8 c, u8 ratio)
{
    u8 p_len = words[c].len;

    for (u8 p=0; p<p_len; p++)
    {
        u8 x = words[c].s[p].x * ratio;
        u8 y = words[c].s[p].y * ratio;

        analogWrite(X_PIN, x);
        analogWrite(Y_PIN, y);
        delayMicroseconds(TRACE_DELAY);   // wait TRACE_DELAY microseconds
    }
}

void show_sentence(sentence_t *s)
{
    for (u8 c=0; c<s->len; c++)
    {
        show_one_word(s->words, c, 1);
    }
}

void loop_show_all_words()
{
    for (u8 c=0; c<CHAR_CNT; c++)
    {
        for (u8 i=0; i<10; i++)
        {
            show_one_word((word_t*)dict, c, 20);
        }
    }
}

void setup()
{
    // debug_init();
    pinMode(X_PIN, OUTPUT);
    pinMode(Y_PIN, OUTPUT);

    // The following sets the PWM clock to maximum on the Arduino(no CPU clock division)
    // DO NOT CHANGE THESE UNLESS YOU KNOW WHAT YOU ARE DOING!
    TCCR0A = (	1 << COM0A1 | 0 << COM0A0 |		// clear OC0A on compare match (hi-lo PWM)
                1 << COM0B1 | 0 << COM0B0 |		// clear OC0B on compare match (hi-lo PWM)
                1 << WGM01  | 1 << WGM00);		// set PWM lines at 0xFF

    TCCR0B = (	0 << FOC0A | 0 << FOC0B |		// no force compare match
                0 << WGM02 |			        // set PWM lines at 0xFF
                0 << CS02	 | 0 << CS01 |		// use system clock (no divider)
                1 << CS00 );

    TIMSK0 = (	0 << OCIE0B | 0 << TOIE0 |
                0 << OCIE0A );
}


void loop()
{
    loop_show_all_words();

    sentence_t s1 = 
    {
        .s          = TEXT1,
        .offset     = {8, 10},
        .ratio      = {8, 10},
        .org_size   = {2, 3},
        .row_pos    = ROW_0,
        .len        = 0,
        .idxs       = {0},
        .words      = {0},
        .init       = init,
        .copy       = copy,
        .resize     = resize,
        .reposition = reposition,
        .newline    = newline,
    };

    s1.init(&s1);
    s1.resize(&s1);
    s1.reposition(&s1);
    s1.newline(&s1);

    sentence_t s2 = 
    {
        .s          = TEXT2,
        .offset     = {8, 10},
        .ratio      = {8, 10},
        .org_size   = {2, 3},
        .row_pos    = ROW_1,
        .len        = 0,
        .idxs       = {0},
        .words      = {0},
        .init       = init,
        .copy       = copy,
        .resize     = resize,
        .reposition = reposition,
        .newline    = newline,
    };

    s2.init(&s2);
    s2.resize(&s2);
    s2.reposition(&s2);
    s2.newline(&s2);

    while(1)
    {
        show_sentence(&s1);
        show_sentence(&s2);
    }

    // while(1)
    // {
    //     loop_show_all_words();
    // }

    // while(1)
    // {
    //     show_one_word((word_t*)dict, SLASH_SYMBOL, 20);
    // }
}

