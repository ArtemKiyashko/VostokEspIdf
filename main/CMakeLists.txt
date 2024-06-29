idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES igniter esp_timer pressure_keepalive_worker)