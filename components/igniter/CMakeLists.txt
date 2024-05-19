idf_component_register(SRCS "igniter.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_timer freertos)