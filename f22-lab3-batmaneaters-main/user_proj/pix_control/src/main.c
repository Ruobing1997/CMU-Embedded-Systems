#include <user642.h>
#include <stdio.h>
#define UNUSED __attribute__((unused))


/**
 * @brief main function for pix_control function
 * @param argc unused paramter for main
 * @param argv unused paramter for main
 * @return return function status
**/
int main(UNUSED int argc, UNUSED const char* argv[]) {
    // buffer used to get rrt read data
    char read_buffer[100] = {};
    int read_bytes = 0;
    int current_bytes = 0;
    double maximum = 150.0;
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    pix_set(255, 255, 255);
    while(1) {
        delay_ms(100);
        uint16_t cur_bright = lux_read();
        double temp_bright = (double)cur_bright;
        if (temp_bright <= 0.1) {
            temp_bright = 0.1;
        }
        double temp = temp_bright / maximum;
        if (temp >= 1.0) {
            temp = 1.0;
        } else if (temp < 0.01) {
            temp = 0.01;
        }
        double scale_factor = temp;

        read_bytes = read(0, &read_buffer[current_bytes], 10);
        current_bytes += read_bytes;
        if (read_bytes == 0) {
            pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
            continue;
        }
        if (read_buffer[0] == 'r' && current_bytes == 3) {
            int correct_flag = 1;
            // make sure the command is red
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'r') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'd') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is red
            if (correct_flag == 1) {
                printf("\n***!RED!***\n");
                r = 255;
                g = 0;
                b = 0;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'b' && current_bytes == 4) {
            int correct_flag = 1;
            // make sure the command is blue
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'b') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'l') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'u') {
                    correct_flag = 0;
                    break;
                } else if (j == 3 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is blue
            if (correct_flag == 1) {
                printf("\n***!BLUE!***\n");
                r = 0;
                g = 0;
                b = 255;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'g' && current_bytes == 5) {
            int correct_flag = 1;
            // make sure the command is green
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'g') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'r') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                } else if (j == 3 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                } else if (j == 4 && read_buffer[j] != 'n') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is green
            if (correct_flag == 1) {
                printf("\n***!GREEN!***\n");
                r = 0;
                g = 255;
                b = 0;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'w' && current_bytes == 5) {
            int correct_flag = 1;
            // make sure the command is white
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'w') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'h') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'i') {
                    correct_flag = 0;
                    break;
                } else if (j == 3 && read_buffer[j] != 't') {
                    correct_flag = 0;
                    break;
                } else if (j == 4 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is pink
            if (correct_flag == 1) {
                printf("\n***!WHITE!***\n");
                r = 255;
                g = 255;
                b = 255;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
                
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'y' && current_bytes == 6) {
            int correct_flag = 1;
            // make sure the command is yellow
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'y') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'l') {
                    correct_flag = 0;
                    break;
                } else if (j == 3 && read_buffer[j] != 'l') {
                    correct_flag = 0;
                    break;
                } else if (j == 4 && read_buffer[j] != 'o') {
                    correct_flag = 0;
                    break;
                } else if (j == 5 && read_buffer[j] != 'w') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is yellow
            if (correct_flag == 1) {
                printf("\n***!YELLOW!***\n");
                r = 255;
                g = 255;
                b = 0;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'p' && current_bytes == 6) {
            int correct_flag = 1;
            // make sure the command is purple
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'p') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'u') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'r') {
                    correct_flag = 0;
                    break;
                } else if (j == 3 && read_buffer[j] != 'p') {
                    correct_flag = 0;
                    break;
                } else if (j == 4 && read_buffer[j] != 'l') {
                    correct_flag = 0;
                    break;
                } else if (j == 5 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is purple
            if (correct_flag == 1) {
                printf("\n***!PURPLE!***\n");
                r = 128;
                g = 0;
                b = 128;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'o' && current_bytes == 3) {
            int correct_flag = 1;
            // make sure the command is off
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'o') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'f') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'f') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is off
            if (correct_flag == 1) {
                printf("\n***!TURN IT OFF!***\n");
                r = 0;
                g = 0;
                b = 0;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'e' && current_bytes == 3) {
            int correct_flag = 1;
            // make sure the command is off
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'e') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'n') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'd') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is off
            if (correct_flag == 1) {
                printf("\n***!STOP PROGRAM!***\n");
                r = 0;
                g = 0;
                b = 0;
                pix_set(r * scale_factor, g * scale_factor, b * scale_factor);
                return -1;
            }
            current_bytes = 0;
        } else if (read_buffer[0] == 'c' && current_bytes == 4) {
            int correct_flag = 1;
            // make sure the command is cali
            for (int j = 0; j < current_bytes; ++j) {
                if (j == 0 && read_buffer[j] != 'c') {
                    correct_flag = 0;
                    break;
                } else if (j == 1 && read_buffer[j] != 'a') {
                    correct_flag = 0;
                    break;
                } else if (j == 2 && read_buffer[j] != 'l') {
                    correct_flag = 0;
                    break;
                } else if (j == 3 && read_buffer[j] != 'i') {
                    correct_flag = 0;
                    break;
                }
            }
            // if the command is cali
            if (correct_flag == 1) {
                printf("\n***!CALI!***\n");
                maximum = cur_bright;
                if (maximum == 0) {
                    maximum = 0.01;
                }
            }
            current_bytes = 0;
        } 
    }

    return 0;
}