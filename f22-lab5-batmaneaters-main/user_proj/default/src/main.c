#include <stdio.h>
#include <lib642.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define USR_STACK_WORDS 256
#define NUM_THREADS 6
#define NUM_MUTEXES 4
#define CLOCK_FREQUENCY 1000

/** @brief define read buffer length*/
#define READ_LENGTH 128

/** @brief define small step change for flashy led mode*/
#define FREQ_SMALL_STEP 1

/** @brief define large step change for flashy led mode*/
#define FREQ_LARGE_STEP 5

/** @brief define max freq for flashy led mode*/
#define MAX_LED_FREQ 60

/** @brief define max color value for RGB*/
#define MAXIMUM_COLOR 256

/** @brief define symbol for red*/
#define RED 10

/** @brief define symbol for green*/
#define GREEN 11

/** @brief define symbol for blue*/
#define BLUE 12

enum mode_change {
	PARTY_MODE,
	COLOR_TUNER_MODE,
	FLASHY_LED_MODE,
};

/** @struct switch color struct
 *  @brief define struct structure for color tuner and flashy led
 */
typedef struct {
	int color_cycle; /**< track current color cycle */
	int red; /**< track value for red channel */
	int green; /**< track value for green channel */
	int blue; /**< track value for blue channel */
	int step; /**< step for freq change */
	int max_rgb_value; /**< max value within rgb channels */
	int min_rgb_value; /**< min value within rgb channels */
	uint32_t color_increase; /**< indicate color increase for color tuner mode*/
	uint32_t color_decrease; /**< indicate color decrease for color tuner mode */
	uint32_t light_increase; /**< indicate light increase for color tuner mode */
	uint32_t light_decrease; /**< indicate light decrease for color tuner mode */
} switch_color;

/** @brief current frequency for flashy led*/
static uint32_t current_freq;
/** @brief new frequency for flashy led*/
static uint32_t new_freq;

/** @struct define mode state struct
 *  @brief mode state struct
 */
typedef struct {
	enum mode_change mode; /**< track current mode */
} mode_state;

/** @struct mutex struct
 *  @brief define mutex struct for tracking mutex 
 */
typedef struct {
	mutex_t* color_tuner_mutex; /**< define color tuner mutex */
	mutex_t* state_mutex; /**< define state mutex */
	mutex_t* switch_mutex; /**< define switch mode mutex */
	mutex_t* flash_led_mutex; /**< define flash led mutex */
} mutex_struct;

/** @brief struct used for color tuner mode and flashy led mode*/
static switch_color color_tuner;
/** @brief struct used for tracking mode*/
static mode_state get_mode;
/** @brief indicate mode change status*/
static bool mode_change_status;
/** @brief indicate visualizer status*/
static bool get_visualizer_status;

/**
 * @brief function used for finding min value
 * @param X first input for find min
 * @param Y second input for find min
 * @return find min value
**/
uint32_t find_min(uint32_t X, uint32_t Y) {
	if (X < Y) return X;
	else return Y;
}

/**
 * @brief function used for finding max value
 * @param X first input for find max
 * @param Y second input for find max
 * @return find max value
**/
uint32_t find_max(uint32_t X, uint32_t Y) {
	if (X > Y) return X;
	else return Y;
}

/**
 * @brief initialize random variable for color tuner mode
 * @return None
**/
void init_random() {
	color_tuner.red = random_initial();
	color_tuner.green = (color_tuner.red * 31 + 1) % 0xff;
	color_tuner.blue = (color_tuner.green * 31 + 1) % 0xff;
}

/**
 * @brief update max and min rgb value
 * @return None
**/
void update_rgb_max_min() {
	color_tuner.max_rgb_value = find_max(find_max(color_tuner.red, color_tuner.green), color_tuner.blue);
	color_tuner.min_rgb_value = find_min(find_min(color_tuner.red, color_tuner.green), color_tuner.blue);
}

/**
 * @brief init function for color tuner mode
 * @return None
**/
void init_color_tuner() {
	init_random();
	color_tuner.color_increase = 0;
	color_tuner.color_decrease = 0;
	color_tuner.light_increase = 0;
	color_tuner.light_decrease = 0;
	update_rgb_max_min();
	color_tuner.step = color_tuner.max_rgb_value / 10;
	color_tuner.color_cycle = BLUE;
}

/**
 * @brief init function for flashy led mode
 * @return None
**/
void init_flashy_led() {
	stop_flash();
	current_freq = 0;
	new_freq = 1;
}

/**
 * @brief init function for party mode
 * @return None
**/
void init_visualizer() {
	visual_stop();
	pix_set(0, 0, 0);
	get_visualizer_status = false;
}

/**
 * @brief thread function to get button press information
 * @param vargp mutex list for thread function
 * @return None
**/
void thread_button_listen(void* vargp) {
	mutex_struct* mutex_list = (mutex_struct*)vargp;

	while (1) {
		if (button_press()) {
			printf("button pressed\n");
			mutex_lock(mutex_list->switch_mutex);
			mode_change_status = true;
			mutex_unlock(mutex_list->switch_mutex);
			wait_until_next_period();
		}
	}
}

/**
 * @brief calculate change step for color tuner mode
 * @return None
**/
void calculate_step() {
	color_tuner.step = (color_tuner.step / abs(color_tuner.step)) * ceil(((float)color_tuner.max_rgb_value / 10.0));
	return;
}

/**
 * @brief function for updating color cycle
 * @param flag flag for updating color cycle
 * @return None
**/
void update_color_cycle(uint32_t flag) {
	if (flag) {
		if (color_tuner.color_cycle == BLUE) {
			color_tuner.color_cycle = RED;
		} else {
			color_tuner.color_cycle = color_tuner.color_cycle + 1;
		}
	} else {
		if (color_tuner.color_cycle == RED) {
			color_tuner.color_cycle = BLUE;
		} else {
			color_tuner.color_cycle = color_tuner.color_cycle - 1;
		}
	}
	return;
}

/**
 * @brief update color cycle in color tuner mode
 * @param flag flag for getting color cycle
 * @return None
**/
void get_color_cycle(uint32_t flag) {
	calculate_step();

	int* color;
	if (color_tuner.color_cycle == RED) {
		color = &color_tuner.red;
	} else if (color_tuner.color_cycle == GREEN) {
		color = &color_tuner.green;
	} else if (color_tuner.color_cycle == BLUE) {
		color = &color_tuner.blue;
	}

	int step;
	if (flag) {
		step = color_tuner.step;
	} else {
		step = -1 * color_tuner.step;
	}

	int result = *color + step;
	if (result > color_tuner.max_rgb_value) {
		*color = color_tuner.max_rgb_value;
		update_color_cycle(flag);
		color_tuner.step = -1 * color_tuner.step;
		return;

	} else if (result < color_tuner.min_rgb_value) {
		*color = color_tuner.min_rgb_value;
		update_color_cycle(flag);
		color_tuner.step = -1 * color_tuner.step;
		return;
	}

	*color += step;
	
}

/**
 * @brief update mode for mode switch thread
 * @param mutex_list mutex list for mode switch update
 * @return None
**/
void update_mode(mutex_struct* mutex_list) {
	if (mode_change_status) {
		mutex_lock(mutex_list->state_mutex);
		init_visualizer();
		init_color_tuner();
		init_flashy_led();
		get_mode.mode = (get_mode.mode + 1) % 3;
		if (get_mode.mode == PARTY_MODE) {
			printf("switch to party mode\n");
		} else if (get_mode.mode == COLOR_TUNER_MODE) {
			printf("switch to color tuner mode\n");
		} else if (get_mode.mode == FLASHY_LED_MODE) {
			printf("switch to flashy led mode\n");
		}
		mutex_unlock(mutex_list->state_mutex);
		mode_change_status = false;
	}
}

/**
 * @brief thread function for mode switch
 * @param vargp get mutex list from argument
 * @return None
**/
void thread_mode_switch(void* vargp) {
	mutex_struct* mutex_list = (mutex_struct*)vargp;

	while (1) {
		mutex_lock(mutex_list->switch_mutex);
		update_mode(mutex_list);
		mutex_unlock(mutex_list->switch_mutex);
		wait_until_next_period();
	}
}

/**
 * @brief update flashy freq for flashy led mode
 * @param mutex_list mutex list
 * @return None
**/
void update_flashy_freq(mutex_struct* mutex_list) {
	mutex_lock(mutex_list->flash_led_mutex);
	if (current_freq != new_freq) {
		current_freq = new_freq;
		printf("update new freq to %ld\n", new_freq);
		visual_stop();
		stop_flash();
		start_flash(current_freq);
	}
	mutex_unlock(mutex_list->flash_led_mutex);
}

/**
 * @brief check mode status 
 * @param mutex_list mutex list
 * @param mode_flag input mode for check_flag
 * @return None
**/
void check_mode(mutex_struct* mutex_list, int mode_flag) {
	enum mode_change thread_mode;

	while (1) {
		mutex_lock(mutex_list->state_mutex);
		thread_mode = get_mode.mode;
		mutex_unlock(mutex_list->state_mutex);

		if (thread_mode != mode_flag) {
			wait_until_next_period();
		} else {
			break;
		}
	}

	return;
}

/**
 * @brief thread function for flashy led mode
 * @param vargp get mutex list from vargp
 * @return None
**/
void thread_flashy_led_mode(void* vargp) {
	mutex_struct* mutex_list = (mutex_struct*)vargp;

	while (1) {

		check_mode(mutex_list, FLASHY_LED_MODE);

		update_flashy_freq(mutex_list);

		wait_until_next_period();

	}
}

/**
 * @brief thread function for visualizer mode
 * @param vargp get mutex list from vargp
 * @return None
**/
void thread_visualizer(void* vargp) {
	mutex_struct* mutex_list = (mutex_struct*)vargp;

	while (1) {
		check_mode(mutex_list, PARTY_MODE);

		if (!get_visualizer_status) {
			get_visualizer_status = true;
			printf("start visualizer\n");
			stop_flash();
			visual_start();
		}

		wait_until_next_period();
	}
}

/**
 * @brief update rgb value based on factor
 * @param factor calculate factor for updating rgb
 * @param flag indicate different calculating method
 * @return None
**/
void update_rgb(float factor, bool flag) {
	if (flag == 0) {
		color_tuner.red = find_min((uint32_t)(ceil(color_tuner.red * factor)), MAXIMUM_COLOR);
		color_tuner.green = find_min((uint32_t)(ceil(color_tuner.green *factor)), MAXIMUM_COLOR);
		color_tuner.blue = find_min((uint32_t)(ceil(color_tuner.blue * factor)), MAXIMUM_COLOR);
	} else if (flag == 1) {
		color_tuner.red = find_max((uint32_t)(color_tuner.red * factor), 1);
		color_tuner.green = find_max((uint32_t)(color_tuner.green * factor), 1);
		color_tuner.blue = find_max((uint32_t)(color_tuner.blue * factor), 1);
	}
}

/**
 * @brief thread function for color tuner mode
 * @param vargp get mutex list from vargp
 * @return None
**/
void thread_color_tuner(void* vargp) {
	mutex_struct* mutex_list = (mutex_struct*)vargp;

	while (1) {

		check_mode(mutex_list, COLOR_TUNER_MODE);

		bool update_flag = false;

		mutex_lock(mutex_list->color_tuner_mutex);
		if (color_tuner.light_increase) {
			update_rgb(1.2, 0);
			color_tuner.light_increase = 0;
			update_rgb_max_min();

			printf("maximum color value is %d, minimum color value is %d\n", color_tuner.max_rgb_value, color_tuner.min_rgb_value);
			update_flag = true;
		} 
		if (color_tuner.light_decrease) {
			update_rgb(0.8, 1);

			color_tuner.light_decrease = 0;
			update_rgb_max_min();

			printf("maximum color value is %d, minimum color value is %d\n", color_tuner.max_rgb_value, color_tuner.min_rgb_value);
			update_flag = true;
		}
		if (color_tuner.color_increase) {
			get_color_cycle(1);
			color_tuner.color_increase = 0;
			update_flag = true;
		} 
		if (color_tuner.color_decrease) {
			get_color_cycle(0);
			color_tuner.color_decrease = 0;
			update_flag = true;
		}

		uint8_t red = color_tuner.red;
		uint8_t green = color_tuner.green;
		uint8_t blue = color_tuner.blue;
		mutex_unlock(mutex_list->color_tuner_mutex);

		if (update_flag) {
			printf("pix color value red = %d, green = %d, blue = %d\n", red, green, blue);
		}

		pix_set(red, green, blue);

		wait_until_next_period();
	}
}

/**
 * @brief function for checking color and light value
 * @param mutex_list get mutex list 
 * @param flag indicate changing color or light, increase or decrease
 * @return None
**/
void check_color_light(mutex_struct* mutex_list, uint8_t flag) {
	mutex_lock(mutex_list->color_tuner_mutex);
	if (flag == 0) {
		color_tuner.color_increase = 1;
	} else if (flag == 1) {
		color_tuner.color_decrease = 1;
	} else if (flag == 2) {
		color_tuner.light_increase = 1;
	} else if (flag == 3) {
		color_tuner.light_decrease = 1;
	}
	mutex_unlock(mutex_list->color_tuner_mutex);
	return ;
}

/**
 * @brief function for checking freq
 * @param mutex_list get mutex list 
 * @param flag indicate how to change freq for flashy led mode
 * @return None
**/
void check_freq(mutex_struct* mutex_list, uint8_t flag) {
	mutex_lock(mutex_list->flash_led_mutex);
	if (flag == 0) {
		new_freq = find_min(current_freq + FREQ_LARGE_STEP, MAX_LED_FREQ);
	} else if (flag == 1) {
		new_freq = find_max(current_freq - FREQ_LARGE_STEP, 0);
	} else if (flag == 2) {
		new_freq = find_min(current_freq + FREQ_SMALL_STEP, MAX_LED_FREQ);
	} else if (flag == 3) {
		new_freq = find_max(current_freq - FREQ_SMALL_STEP, 0);
	}
	mutex_unlock(mutex_list->flash_led_mutex);
}

/**
 * @brief function for reading text input from serial terminal
 * @param vargp get mutex list from vargp
 * @return None
**/
void thread_text_reader(void* vargp) {
	mutex_struct* mutex_list = (mutex_struct*)vargp;

	char read_buffer[READ_LENGTH];

	int read_bytes = 0;

	while (1) {
		read_bytes = read(0, read_buffer, 1);
		if (read_bytes != 0) {
			mutex_lock(mutex_list->state_mutex);
			enum mode_change thread_mode;
			thread_mode = get_mode.mode;

			for (int i = 0; i < read_bytes; ++i) {
				if (read_buffer[i] == 'm') {
					mutex_lock(mutex_list->switch_mutex);
					mode_change_status = true;
					mutex_unlock(mutex_list->switch_mutex);
				} 
				if (read_buffer[i] == 'w') {
					if (thread_mode == COLOR_TUNER_MODE) {
						check_color_light(mutex_list, 0);
					}
				}
				if (read_buffer[i] == 's') {
					if (thread_mode == COLOR_TUNER_MODE) {
						check_color_light(mutex_list, 1);
					}
				}
				if (read_buffer[i] == 'a') {
					if (thread_mode == COLOR_TUNER_MODE) {
						check_color_light(mutex_list, 2);
					}
				}
				if (read_buffer[i] == 'd') {
					if (thread_mode == COLOR_TUNER_MODE) {
						check_color_light(mutex_list, 3);
					}
				}
				if (read_buffer[i] == 'u') {
					if (thread_mode == FLASHY_LED_MODE) {
						check_freq(mutex_list, 0);
					}
				}
				if (read_buffer[i] == 'j') {
					if (thread_mode == FLASHY_LED_MODE) {
						check_freq(mutex_list, 1);
					}
				}
				if (read_buffer[i] == 'h') {
					if (thread_mode == FLASHY_LED_MODE) {
						check_freq(mutex_list, 2);
					}
				}
				if (read_buffer[i] == 'k') {
					if (thread_mode == FLASHY_LED_MODE) {
						check_freq(mutex_list, 3);
					}
				}
			}
			// reset buffer
			for (int j = 0; j < READ_LENGTH; ++j) {
				read_buffer[j] = 0;
			}
			mutex_unlock(mutex_list->state_mutex);
		}
	}
}

/**
 * @brief function for idle thread
 * @return None
**/
void idle_thread() {
	while (1);
}

/**
 * @brief check if mutex is NULL or not
 * @param get_mutex mutex input for checking
 * @return None
**/
uint8_t check_mutex(mutex_t* get_mutex) {
	if (get_mutex == NULL) {
		return 1;
	}
	return 0;
}

#define UNUSED __attribute__((unused))

int main(UNUSED int argc, UNUSED char* const argv[]) {
	
	ABORT_ON_ERROR(thread_init(NUM_THREADS, USR_STACK_WORDS, &idle_thread, PER_THREAD, NUM_MUTEXES));

	mutex_t* switch_mutex = mutex_init(0);
	mutex_t* state_mutex = mutex_init(0);
	mutex_t* color_tuner_mutex = mutex_init(0);
	mutex_t* flash_led_mutex = mutex_init(0);

	if (check_mutex(switch_mutex) || check_mutex(state_mutex) || check_mutex(color_tuner_mutex) || check_mutex(flash_led_mutex)) {
		return -1;
	}

	mutex_struct* mutex_list = malloc(sizeof(mutex_struct));

	if (mutex_list == NULL) {
		return -1;
	}

	mutex_list->switch_mutex = switch_mutex;
	mutex_list->state_mutex = state_mutex;
	mutex_list->color_tuner_mutex = color_tuner_mutex;
	mutex_list->flash_led_mutex = flash_led_mutex;

	get_mode.mode = PARTY_MODE;
	mode_change_status = false;
	init_visualizer();

	ABORT_ON_ERROR(thread_create(&thread_text_reader,0,1,5,(void*)mutex_list));
	ABORT_ON_ERROR(thread_create(&thread_color_tuner,1,1,5,(void*)mutex_list));
	ABORT_ON_ERROR(thread_create(&thread_flashy_led_mode,2,1,5,(void*)mutex_list));
	ABORT_ON_ERROR(thread_create(&thread_mode_switch,3,1,20,(void*)mutex_list));
	ABORT_ON_ERROR(thread_create(&thread_visualizer,4,1,20,(void*)mutex_list));
	ABORT_ON_ERROR(thread_create(&thread_button_listen,5,1,40,(void*)mutex_list));

	ABORT_ON_ERROR(scheduler_start(CLOCK_FREQUENCY));
	return 0;
}