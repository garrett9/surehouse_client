#Makefile for the surehouse-client library.
SRC=surehouse_client_init.c surehouse_client_debug_send.c surehouse_client_debug_recieve.c surehouse_client_init_headers.c surehouse_client_authenticate.c surehouse_client_ignore_ssl_warning.c surehouse_client_query.c surehouse_client_custom.c surehouse_client_recent.c surehouse_client_close.c \
query_params_init.c query_params_set_from.c query_params_set_to.c query_params_to_json.c query_params_clean.c \
error.c rtrim.c free_pointer.c xmemcpy.c write_memory_callback.c

LIB=../lib/libsurehouse_client.a
OBJ=$(SRC:.c=.o)
RM=rm -f

all: $(OBJ)

lib: all
	ar rc $(LIB) $(OBJ)
	
clean:
	-$(RM) *~
	-$(RM) *.o
	-$(RM) \#*
	-$(RM) *.swp
	-$(RM) *.core