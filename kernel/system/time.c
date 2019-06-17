#include "system.h"
#include "../include/io.h"
#include "../../libc/include/math/math.h"

// Read a value from the RTC register. The data returned is determined by `type`
char read_rtc_register(uint16_t type)
{
    outb(cmos_address, type);
    return inb(cmos_data);
}

// Return the system time in seconds
int get_system_time()
{
    int second = bcd2bin(read_rtc_register(CMOS_TYPE_SECOND));
    int minute = bcd2bin(read_rtc_register(CMOS_TYPE_MINUTE));
    int hour = bcd2bin(read_rtc_register(CMOS_TYPE_HOUR));
    int day = bcd2bin(read_rtc_register(CMOS_TYPE_DAY));
    int month = bcd2bin(read_rtc_register(CMOS_TYPE_MONTH));
    int year = ADD_YEARS + bcd2bin(read_rtc_register(CMOS_TYPE_YEAR));

    int years_from_epoch = year - UNIX_EPOCH; // Exclude the current year
    int num_leap_years = 0;

    // Calculate the number of leap years between the UNIX epoch and 1 year before now
    for (int i=UNIX_EPOCH; i<year-1; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
            num_leap_years++;
        }
    }

    // Get number of seconds since the UNIX epoch and the end of the last year
    // -108000 (30 hours) seconds for weird time difference
    unsigned int total_seconds_before_current_year = (years_from_epoch * YEAR_SECONDS) + (num_leap_years * (60*60*24));// - 108000;
    unsigned int current_year_seconds;


    // Determine the number of seconds that have gone by in the current year
    switch (month-1) {
        case 1:
            current_year_seconds = JANUARY_SECONDS;
            break;
        case 2:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS;
            break;
        case 3:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS;
            break;
        case 4:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS;
            break;
        case 5:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS + MAY_SECONDS;
            break;
        case 6:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS + MAY_SECONDS + JUNE_SECONDS;
            break;
        case 7:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS + MAY_SECONDS + JUNE_SECONDS
                                 + JULY_SECONDS;
            break;
        case 8:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS + MAY_SECONDS + JUNE_SECONDS
                                 + JULY_SECONDS + AUGUST_SECONDS;
            break;
        case 9:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS + MAY_SECONDS + JUNE_SECONDS
                                 + JULY_SECONDS + AUGUST_SECONDS + SEPTEMBER_SECONDS;
            break;
        case 10:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS + MAY_SECONDS + JUNE_SECONDS
                                 + JULY_SECONDS + AUGUST_SECONDS + SEPTEMBER_SECONDS
                                 + OCTOBER_SECONDS;
            break;
        case 11:
            current_year_seconds = JANUARY_SECONDS + FEBRUARY_SECONDS + MARCH_SECONDS
                                 + APRIL_SECONDS + MAY_SECONDS + JUNE_SECONDS
                                 + JULY_SECONDS + AUGUST_SECONDS + SEPTEMBER_SECONDS
                                 + OCTOBER_SECONDS + NOVEMBER_SECONDS;
            break;
    }

    // The total number of seconds from the UNIX epoch
    unsigned int total_seconds = total_seconds_before_current_year + current_year_seconds;
    total_seconds += ((day-1)*24*60*60) + (hour*60*60) + (minute*60) + second;

    return total_seconds;
}

// Install the PIT timer as IRQ0
void init_timer()
{
    irq_install_handler(0, handle_timer);
}

void handle_timer()
{
    timer_ticks++;

    if (timer_ticks % 18 == 0) {
        // do stuff here when 1 second has past
    }
}

// Return the uptime of the system in seconds
int get_uptime()
{
    return timer_ticks / 18;
}