idf_component_register(SRCS "pressure_keepalive_worker.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_timer freertos)
