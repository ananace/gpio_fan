#include "gpio.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PIN_FAN 0

void print_help(const char* argv0);
float read_temperature();

int main(int argc, char** argv)
{
    int fan_pin = 18;

    int interval = 5;
    float min_temp = 45.000;
    float max_temp = 55.000;


    int i;
    for (i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pin") == 0)
            fan_pin = atoi(argv[++i]);
        else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--interval") == 0)
            interval = atoi(argv[++i]);

        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--temp") == 0)
        {
            float temp = atof(argv[++i]);
            min_temp = temp - 10;
            max_temp = temp;
        }
        else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--min") == 0)
            min_temp = atof(argv[++i]);
        else if (strcmp(argv[i], "-M") == 0 || strcmp(argv[i], "--max") == 0)
            max_temp = atof(argv[++i]);

        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_help(argv[0]);
            return 0;
        }
        else
        {
            fprintf(stderr, "Unknown argument %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    fprintf(stdout, "Starting GPIO fan handling on pin %d, for the range %.2fC - %.2fC every %d second(s).\n", fan_pin, min_temp, max_temp, interval);
    fflush(stdout);

    if (gpio_init(0) < 0)
        return -1;

    if (gpio_export(PIN_FAN, fan_pin, GPIO_OUT) < 0)
        return -1;

    for (;;)
    {
        float cur_temp = read_temperature();
        if (cur_temp < 0)
            return -2;

        if (cur_temp >= max_temp)
            gpio_write(PIN_FAN, GPIO_HIGH);
        else if (cur_temp <= min_temp)
            gpio_write(PIN_FAN, GPIO_LOW);

        sleep(interval);
    }

    return 0;
}

float read_temperature()
{
    char data[6] = {};
    FILE* file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to access thermal information: %s\n", strerror(errno));
        return -1;
    }

    if (fread(data, 1, 5, file) <= 0)
    {
        fprintf(stderr, "Failed to read temperature: %s\n", strerror(errno));
        return -1;
    }

    fclose(file);

    return atof(data) / 1000.f;
}

void print_help(const char* argv0)
{
    fprintf(stdout, "Usage: %s [OPTIONS...]\n\n"
            "Args:\n"
            "  -p --pin PIN      Set the GPIO pin to toggle\n"
            "  -n --interval SEC Set the delay between temperature checks (in seconds)\n"
            "  -t --temp TEMP    Set temperature range as in gpio-fan\n"
            "\n"
            "  -m --min MIN      Set turn-off point\n"
            "  -M --max MAX      Set turn-on point\n",
           argv0);
    fflush(stdout);
}
